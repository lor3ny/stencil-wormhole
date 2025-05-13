# Compilation Temaplate for Tenstorrent Cards

*The project is compiled with the following tt-metal commit [91e61c006a9c1900ec90aee8e0898f69574bae04](https://github.com/tenstorrent/tt-metal/commit/53d4192a5bb07df8ae8da5f2ff8ca60967d9118b)*

This project is built to give a general template to build TT-Metalium applications using [tt-metal](https://github.com/tenstorrent/tt-metal) repo as an external API.

## Compiling and running

### Requirements

- `TT-Metal` is required to use TT-Metalium, before compiling your project you need to compile tt-metal [(instructions over here)](https://github.com/tenstorrent/tt-metal/blob/main/INSTALLING.md)
- In my tt-metal version, the API is compiled using clang++-17. To compile your project you need to use the same compiler of tt-metal, in the case of this repo clang++-17.

### TT-Metal Building

Follow the link before to have the most updated version, but in general you need to follow these steps:

0. Clone tt-metal
```sh
git clone tt-metal/repo/link
cd tt-metal
```

2. Set environment variables before building 
```sh
export TT_METAL_HOME=/path/to/tt-metal
export ARCH_NAME=wormhole_b0 # IN THEIR LINK ARE REPORTED THE LIST OF ALL THE AVAILABLE CARDS
export PYTHONPATH=/path/to/tt-metal
```

3. Build tt-metal
```sh
./build_metal.sh
```

Now tt-metal API can be accessed be an external project.

### Build your project

1. Clone this repository
```sh
git clone https://github.com/lor3ny/stecil_wormhole
cd stecil_wormhole
```

The template is inside the folder ```tt-metal_compilation_template``` 

2. Run cmake
```sh
cmake -S . -B build -DCMAKE_CXX_COMPILER=clang++-17 -DCMAKE_C_COMPILER=clang-17
# My tt-metal version is compiled with clang++-17, so I'm using the same compiler also to compile my project
```

3. Run make
```sh
make -C build -j(procs)
```

4. Run the program
```sh
# note: You MUST do it in the parent directory or else TT-Metal wont recognise the kernels
./build/name_of_your_application
```
