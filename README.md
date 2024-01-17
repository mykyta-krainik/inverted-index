# Inverted index with parallel processing

## Description

This repository contains an implementation of an inverted index with parallel processing

Besides, it contains a traditional implementation of a thread pool, document parser, and a 
simple search engine server.

You can find all of them in the corresponding directories.

There are also unit tests for each of them in the `google_tests` directory.

## Prerequisites

Before you begin, ensure you have met the following requirements:
- **C++ Compiler**: Ensure you have CLang and the std library installed. Clang >= 14
- **CMake**: Make sure CMake is installed on your system. CMake >= 3.27

## Installation

To install <ProjectName>, follow these steps:

```bash
git clone https://github.com/mykyta-krainik/inverted-index.git
cd inverted-index
```

## Building the Program

To build the program, run the following commands in the root directory of the project:

```bash
mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=clang++ ..
make
```

This will compile the program using CLang as the compiler.

**DON'T FORGET TO SPECIFY YOUR OWN COMPILER IF IT DIFFERS FROM THAT LISTED ABOVE**

## Running the Program

After building the program, you can run it with:

```bash
./main
```