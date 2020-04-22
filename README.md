## DICOMANONYMIZER
WebAssembly based DICOM file anonymization. The software strips confidential tags from a DICOM file.
## Prerequisites
- Emscripten SDK
- cmake
## To build 
```
cd DICOMANONYMIZER
mkdir build
cmake .. -DCMAKE_TOOLCHAIN_FILE=~/projects/emscripten/cmake/Modules/Platform/Emscripten.cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=.
cmake --build .
cmake --install .
```

Distribution files are copied in dist subfolder of the repository root directory.

## Used software
- DCMTK library
- Repository Parsing DICOM using WebAssembly (https://github.com/jodogne/wasm-dicom-parser)
- Emscripten framework
- cmake
## Live Demo

http://slideio.com/dicom/
