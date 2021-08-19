# neat-libtorch

C++ NEAT implementation using LibTorch.

This project is a C++ translation of [github(ddehueck/pytorch-neat)](https://github.com/ddehueck/pytorch-neat).

## Documentation

### LibTorch
- [PyTorch C++ API Docs](https://pytorch.org/cppdocs/)
- [Using the PyTorch C++ frontend](https://pytorch.org/tutorials/advanced/cpp_frontend.html)
- [Index](https://pytorch.org/cppdocs/genindex.html)
- [Tensor indexing](https://pytorch.org/cppdocs/notes/tensor_indexing.html)
- [Loading a PyTorch model in C++](https://pytorch.org/tutorials/advanced/cpp_export.html)
- [How to move data between PyTorch and libtorch](https://github.com/pytorch/pytorch/issues/20356)

### Additional info
- [Loading a torchscript model in C++](https://pytorch.org/tutorials/advanced/cpp_export.html#depending-on-libtorch-and-building-the-application) (_optional_)
- [Follow this post to build only libtorch](https://discuss.pytorch.org/t/compile-libtorch-c-api-from-source/81624/2) (_optional_)
- [libtorch (C++-only) -> Building libtorch](https://github.com/pytorch/pytorch/blob/master/docs/libtorch.rst) (_optional_)

## Requirements

- **CMake**: Download from [here](https://cmake.org/download/)
- **ruby**: [Install following this guide](https://www.ruby-lang.org/en/documentation/installation/)
- **rake**: [rake repo](https://github.com/ruby/rake)
- **LibTorch**: Download from [pytorch site](https://pytorch.org/get-started/locally/), extract and copy in `/opt` (after copy, libs must be placed on `/opt/libtorch/lib`)

## Build

    rake

Then execute the following command to test that compilation works fine:

    build/system-info


