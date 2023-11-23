# TEngine

```bash
conan install . --output-folder=libs --build=missing    
cmake ./cmake-build-debug -DCMAKE_TOOLCHAIN_FILE=libs/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
```