#!/bin/bash

echo
echo "==================================="
echo "package start: $version $type"

base_dir=../../..
tool_dir=$base_dir/tool
test_dir=$base_dir/test
deployment_dir=$base_dir/deployment
ipfs_repo_src_dir=$deployment_dir/ipfs_repo

dbc_repo_dir=./dbc_repo
ipfs_repo_dir=./ipfs_repo
mining_repo_dir=./mining_repo

os_name=`uname -a | awk '{print $1}'`
if [ $os_name == 'Linux' ];then
    os_name=linux
    ipfs_pkg_tgz=go-ipfs_v0.4.15_linux-amd64.tar.gz
elif [ $os_name == 'Darwin' ];then
    os_name=macos
    ipfs_pkg_tgz=go-ipfs_v0.4.15_darwin-amd64.tar.gz
fi

# get default version from src file: version.h

remove_leading_zero()
{
    v=$1
    v=$(echo "$v" |  sed 's/^0*//')

    if [ -z "$v" ]; then
        v="0"
    fi
    echo $v
}

get_version()
{
    version_file=$base_dir/src/core/common/version.h

    ver=$(grep "#define CORE_VERSION" $version_file  | awk '{print $3}')

    v1="${ver:2:2}"
    v1=$(remove_leading_zero $v1)


    v2="${ver:4:2}"
    v2=$(remove_leading_zero $v2)

    v3="${ver:6:2}"
    v3=$(remove_leading_zero $v3)

    v4="${ver:8:2}"
    v4=$(remove_leading_zero $v4)

    echo "$v1.$v2.$v3.$v4"

}


help()
{
    echo "synopsis: bash ./package.sh <version> <type>"
    echo "type: client or mining"
    echo "pls input version and type, e.g. bash ./package.sh 0.3.0.1 client"
    exit
}

# dbc install pkg: dbc_repo.tar.gz
dbc_package()
{
    echo "step: dbc program package"

    mkdir $dbc_repo_dir
    mkdir $dbc_repo_dir/tool

    # dbc exe and default configure
    cp ../../../output/dbc      $dbc_repo_dir/

    # strip
#    if [ $os_name == 'linux' ]; then
#        strip --strip-all $dbc_repo_dir/dbc
#    fi

    cp -R ../../../conf         $dbc_repo_dir/

    if [ $type == 'client' ];then
        echo "    substep: rm container.conf for client"
        rm -f $dbc_repo_dir/conf/container.conf
    fi

    # dbc related tool
    cp $tool_dir/dbc-ctl            $dbc_repo_dir/
    cp $tool_dir/add_dbc_user.sh    $dbc_repo_dir/tool/
    cp $tool_dir/p                  $dbc_repo_dir/tool/
    cp $tool_dir/rm_containers.sh   $dbc_repo_dir/tool/
    cp $tool_dir/plog               $dbc_repo_dir/tool/
    cp $tool_dir/ipfs-ctl           $dbc_repo_dir/tool/
    cp $tool_dir/upload.sh          $dbc_repo_dir/tool/

    chmod +x $dbc_repo_dir/dbc
    chmod +x $dbc_repo_dir/dbc-ctl
    chmod +x $dbc_repo_dir/tool/*

    # test task conf
#    if [ $type == 'client' ];then
#        mkdir ./$dbc_repo_dir/test
#        cp $test_dir/caffe2/mnist/task.conf $dbc_repo_dir/test/task_caffe2_mnist.conf
#    fi

    # dbc ai-training container related files: only for mining node
    if [ $os_name == 'linux' -a $type == 'mining' ];then

        echo "    substep: container package"

        cp -r $deployment_dir/container $dbc_repo_dir/

        # add ipfs install pkg
        cp $ipfs_repo_src_dir/swarm.key         $dbc_repo_dir/container/
        cp $ipfs_repo_src_dir/install_ipfs.sh   $dbc_repo_dir/container/
        cp $ipfs_repo_src_dir/$ipfs_pkg_tgz     $dbc_repo_dir/container/
        cp $tool_dir/upload.sh                  $dbc_repo_dir/container/
        cp $tool_dir/upload.py                  $dbc_repo_dir/container/

    fi
}


ipfs_package()
{
    echo "step: ipfs package"

    mkdir $ipfs_repo_dir

    # 1) swarm key
    cp $ipfs_repo_src_dir/swarm.key             $ipfs_repo_dir/

    # 2) install infs sh
    cp $ipfs_repo_src_dir/install_ipfs.sh       $ipfs_repo_dir/

    # 3) ipfs tar gz
    cp $ipfs_repo_src_dir/$ipfs_pkg_tgz             $ipfs_repo_dir/
}

mining_package()
{
    echo "step: mining package"

    mkdir $mining_repo_dir

    cp $tool_dir/mining_install.sh  $mining_repo_dir/
    cp $tool_dir/docker_gpg.key     $mining_repo_dir/

    chmod +x $mining_repo_dir/*.sh
}


if [ $# -lt 2 ]; then
    echo "need more arguments"
    help
    exit 1
fi

version=$1
type=$2

if [ -z "$version" ]; then
    version=$(get_version)
    echo "verson: $version"
    exit 0
fi

if [ "$type" != "client" -a "$type" != "mining" ]; then
    echo "unknown type"
    help
fi


tar_ball=dbc-$os_name-$type-$version.tar.gz
rm -f ./package/$tar_ball
rm -rf ./package/$version
mkdir -p  ./package/$version
cd ./package/$version
pwd


dbc_package

if [ $type == 'client' ];then
    ipfs_package
fi

if [ $type == 'mining' ]; then
    mining_package
fi


# package
cd ../
tar -czvf $tar_ball $version

rm -rf ./$version


echo "package complete: $version $type"
echo "==================================="
echo