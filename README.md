# deepbrainchain
Artificial Intelligence Computing Platform Driven By BlockChain

If you want to add a machine to the DBC network to participate in mining to obtain DBC revenue, go straight to the “ https://github.com/DeepBrainChain/DBC-DOC” and ignore the rest
如果你想要添加机器到DBC网络中参与挖矿获得DBC收益，直接查看 https://github.com/DeepBrainChain/DBC-DOC


# build dbc in linux os with dbc compile container
Suppose download dbc source code locates into ~/deepbrainchain folder.
```
    $ cd ~
    $ git clone https://github.com/DeepBrainChain/DBC-AIComputingNet.git
    $ git checkout dev
    
    $ docker pull dbctraining/dbc_compile:v3
    
    $ docker run -it --rm -v ~/deepbrainchain:/home/deepbrainchain dbctraining/dbc_compile:v3 /bin/bash
    
    # cd /home/deepbrainchain/make
    # ./clean.sh; ./build.sh
    
    # cd /home/deepbrainchain/deployment
    # bash ./package.sh
    # ls ./package
```


# build dbc in mac osx
   
prerequisite

* xcode with command line tool support
* boost 1.6x, for example, brew install boost    

   
```
    $ cd ~
    $ git clone https://github.com/DeepBrainChain/DBC-AIComputingNet.git
    $ git checkout dev
    
    $ cd ~/deepbrainchain/make
    $ ./clean.sh; ./build.sh
    $ ls ~/deepbrainchain/output/dbc 
```

-------------------------------------------------------------------
    
