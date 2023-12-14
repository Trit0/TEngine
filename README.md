# TEngine

```bash
conan install . --output-folder=include --build=missing    
cmake ./cmake-build-debug -DCMAKE_TOOLCHAIN_FILE=include/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
```

`cmake --preset conan-release`