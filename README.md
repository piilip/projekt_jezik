## Build
### Ubuntu 22.04

#### Prerequisites

LLVM 17:
``sudo apt install llvm-17``
or:
```
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 17
```

```
sudo apt install build-essential
sudo apt-get install libcurl4-openssl-dev
sudo apt install -y libzstd-dev
sudo apt-get install zlib1g-dev
```
#### Building
Standard cmake build.

Clone the repository
`git clone https://github.com/piilip/projekt_jezik`

place yourself in the repository directory
`cd projekt_jezik`

```
mkdir build
cd build
cmake .. 
make
```

Now you have executable moj in the build folder, which you can use like`./moj <input_file>`
If you want to test some of the examples: `./moj ../example/<example_file>`

By default we are also creating a .syn syntax file and two .ll  files (LLVM IR, unoptimized and optimized). If you want to disable that you can call with `DUMP=0 ./moj ../example/<example_file>`
### Windows
I recommend using WSL and following the instructions for Ubuntu 22.04, as building it on Windows requires obtaining the llvm-config file by compiling the llvm-project from source, at least the llvm part of it, which can take a lot of memory and time.

