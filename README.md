# Building SGM
## For Ubuntu 18.04
```
sudo apt-get install -y \  
        autoconf \  
        automake \  
        cmake \  
        g++ \  
        git \  
        libssl-dev \  
        libtool \  
        make \  
        pkg-config \  
        python3 \  
        python3-jinja2
```

```
sudo apt-get install -y \  
        libboost-chrono-dev \  
        libboost-context-dev \  
        libboost-coroutine-dev \  
        libboost-date-time-dev \  
        libboost-filesystem-dev \  
        libboost-iostreams-dev \  
        libboost-locale-dev \  
        libboost-program-options-dev \  
        libboost-serialization-dev \  
        libboost-signals-dev \  
        libboost-system-dev \  
        libboost-test-dev \  
        libboost-thread-dev  
```

```
sudo apt-get install -y \  
        doxygen \  
        libncurses5-dev \  
        libreadline-dev \  
        perl  
```

```
git clone https://github.com/tncsource/tnc-mainnet
git submodule update --init --recursive
mkdir build
cd build
-> add chmod 777 build folder
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc) sgmd
make -j$(nproc) cli_wallet
```

# H/W requirements.
```
HDD free space 54G or more
CPU dual core or more
RAM 4 GB or more
OS
Linux: Ubuntu 16.04 or later
```

# How to run the Node Program
## Linux
1. Move to the directory where the sgmd file is located, execute the node program with the following command
   > ./sgmd

# Setting up Node program 
Modify the node_data_dir/config.ini file created after the first node execution.
After modifying config.ini, you must restart node program.

## Frequently used settings in config.ini Description
```
. p2p-endpoint: ip, port settings for this node
. seed-node: ip, port setting of node to receive block data (multiple setting possible)
. rpc-endpoint: settings for ip or port to be used when the wallet or API communicates with this node through rpc.
. public-api: settings for api to be used on this node
. enable-plugin: settings for plugin to be used on this node
. bobserver: Setting up the BP or BO on this node (multiple setting possible)
. private-key: Block creation key for BP and BO that were setup in bobserver (in the same order as the order of bobserver during multiple setting)
```

# How to run wallet program
## Linux
1. For ubuntu 16.0.4
Execute the following command to connect the readline package version.
```
sudo ln -s /lib/x86_64-linux-gnu/libreadline.so.7/lib/x86_64-linux-gnu/libreadline.so.6
```
2. Move to the directory where the cli_wallet file is located and run the wallet program executing the following command
```
> ./cli_wallet
```

# Using wallet
You must set the password to be used for this wallet when running the wallet program for first time or when the command prompt is marked as new.
** If you lose this password, you will not be able to use the information stored in the existing wallet, so do not lose it. **

```
      set_password [wallet password]
      new >>> set_password
```

## Unlock wallet
If command prompt is locked, other commands can not be used.
     
```
      unlock [wallet password]  
      locked >>> unlock
```

## Key registration
When executing commands such as create / update etc, the active private key of BP / BO is needed, so the key is included in the wallet.

```
      import_key [private key]
      unlocked >>> import_key 5JCvj3Dx9D3BE9FYRH7Apj6Emnxmvks4aQAj5ZKGjJX6pveRfjB
```

## Create an account
```
      create_account [creator account id] [account id to create] [json meta data (empty string possible)] true  
      unlocked >>> create_account chainmaker test001 "" true
```  
## Account information inquiry
```
      get_account [account id]
      unlocked >>> get_account test001
```  

## coin transfer
```
      transfer [sender account id] [recipient account id] [amount sent] [note] true   
      unlocked >>> transfer test001 test002 "100.00000000 SGM" "" true
```


