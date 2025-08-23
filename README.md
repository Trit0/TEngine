# TEngine

Should now be good by runnig the root CMake. Might need to reload it one or two times.

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

if needed install conan dependencies
```bash
cd TEngine
conan install . --output-folder=conan_deps --build=missing    
```

On Mac you can use Homebrew to install Conan, otherwise [Conan Installation](https://docs.conan.io/2/installation.html)