#!/bin/bash


if [ $# -lt 1 ]; then
    echo "bash install_ipfs.sh [go-ipfs.tgz]"
    [[ "$0" = "$BASH_SOURCE" ]] && exit 1 || return 1
fi


workspace=`pwd`
sleep_time=5s
wait_ipfs_init_time=10s
wait_ipfs_daemon_time=30s
ipfs_tgz=$1

# env IPFS_PATH, as ipfs product doc required. by default ~/.ipfs
ipfs_path=$IPFS_PATH
if [ -z "$ipfs_path" ]; then
    ipfs_path=~/.ipfs
fi

rm -rf $ipfs_path
mkdir $ipfs_path

# install ipfs
echo "install ipfs"
cp -f $ipfs_tgz /tmp
cd /tmp
tar xvzf $ipfs_tgz
cd /tmp/go-ipfs

user=`whoami`
if [ "$user" != "root" ]; then
    sudo ./install.sh
else
    ./install.sh
fi

cd $workspace
rm -rf /tmp/go-ipfs

#init ipfs
echo "begin to init ipfs"
ipfs init &
sleep $wait_ipfs_init_time

exit_code=$?

if [ $exit_code -ne 0 ]; then
        echo "init ipfs failed"
        exit
fi

echo "end to init ipfs"
sleep $sleep_time


cp ./swarm.key $ipfs_path/

#start ipfs
#echo "======================================================="
echo "begin to configure ipfs"
ipfs daemon --enable-gc &
sleep $wait_ipfs_daemon_time

exit_code=$?
#echo -n "start ipfs exitcode:"
#echo $exit_code

if [ $exit_code -ne 0 ]; then
        echo "configure ipfs failed"
        exit
fi

#add ipfs bootstrap node
ipfs bootstrap rm --all

#ipfs bootstrap add /ip4/114.116.19.45/tcp/4001/ipfs/QmPEDDvtGBzLWWrx2qpUfetFEFpaZFMCH9jgws5FwS8n1H
ipfs bootstrap add /ip4/49.51.49.192/tcp/4001/ipfs/QmRVgowTGwm2FYhAciCgA5AHqFLWG4AvkFxv9bQcVB7m8c
ipfs bootstrap add /ip4/49.51.49.145/tcp/4001/ipfs/QmPgyhBk3s4aC4648aCXXGigxqyR5zKnzXtteSkx8HT6K3
ipfs bootstrap add /ip4/122.112.243.44/tcp/4001/ipfs/QmPC1D9HWpyP7e9bEYJYbRov3q2LJ35fy5QnH19nb52kd5



echo "ipfs configure complete"
sleep $sleep_time