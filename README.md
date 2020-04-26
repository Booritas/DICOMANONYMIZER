## DICOMANONYMIZER
WebAssembly based DICOM file anonymization. The software strips confidential tags from a DICOM file.
## Prerequisites
- Emscripten SDK
- cmake
## To build a WebAssembly 
- Edit build.sh file and define correct path to the Emscripten toolkit file. It is specified at the beginnin of the file in variable TOOL_CHAIN_PATH.
- Run script build.sh
```
cd DICOMANONYMIZER
./build.sh
```
- Distribution files are copied in dist subfolder of the repository root directory.
## WebAssembly distribution files.
- index.html: HTML file for the WEB page.
- library.js: Javascript functions that will be called from the WebAssembly.
- style.cs: style sheet for the WEB page.
- wrap.js: Javascript that contains function called from the HTML code, include javascript wrappers for the WebAssembly.
- DICOMAnonymizer.js: Plumbing functions for the WebAssembly.
- DICOMAnonymizer.wasm: The WebAsembly file.
## To build a c++ execution file
```
cd DICOMANONYMIZER
mkdir build
cmake ..
cmake --build .
```
## Repository structure
- Folder dcmtk-3.6.0: source code of dcmtk library
- Folder config_wa: files with configuration of dcmtk for WebAssembly
- Folder config_cpp: files with configuration of dcmtk for Linux c++ compilation
- Folder dist: location of WebAssembly distribution files.
- Folder build: location of temporay build files.
- anonymize.cpp: c++ code for function exported for WebAssembly.
- stripDataset.h, stripDataset.cpp: functions for anonymization of a dcmtk dataset.
- utils.h, utils.cpp: functions for saving/loading of dcmtk instances to/from buffer
- library.js: functions to be called from the WebAssembly to put information to the WEB page.
- wrap.js: javascript functions for communications with WebAssembly and functions sending DICOM information to a server.
- index.html: HTML code for the WEB page.
- style.css: style sheet for the WEB page.
- CMakeLists.txt: cmake file for building of the WebAssembly or c++ executable.
- build.sh: script for building of the WebAssembly.
- main.cpp: main function for c++ executable (for testing reason).
## Used software
- DCMTK library
- Repository Parsing DICOM using WebAssembly (https://github.com/jodogne/wasm-dicom-parser)
- Emscripten framework
- cmake
## Live Demo

http://slideio.com/dicom/
