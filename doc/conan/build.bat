mkdir build
cd build
conan install ..
cmake .. -G "Visual Studio 16"
cmake --build . --config Release