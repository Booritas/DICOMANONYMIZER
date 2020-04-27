## DICOMANONYMIZER
[](images/web.png)

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
## C++ API reference
The WebAssembly exposes the following functons.
### anonymizeFile
The functiion reads DICOM data from an input byte array, removes confidential tags and saves the data to output byte array. 

Parameters:
- input (const void*): input byte array with content of a DICOM file.
- inputLength (size_t): size of the input data in bytes.
- outputData (void*): a buffer for the output data after anomyzation.
- outputLength (size_t): length of the buffer for output data.
- tracingLevel (int): integet 0,1,2 that defines a tracing level (output to console);
- - 0: No information.
- - 1: Function calls only.
- - 2: Detailed information.  

File wrap.js contains a wrapper function (anonymizeFile) for calling of the function from javascript.
### printTags
The function reads DICOM data from an input byte array and prints DICOM tags it to the WEB page. Location where the information is printed defined by a HTIML tag - container on the WEB page.

Parameters:
- input (const void*): input byte array with content of a DICOM file.
- inputLength (size_t): size of the input data in bytes.
- parentHtmlTag (const char*): id of the parent HTML element which should show the information.

File wrap.js contains a wrapper function (printTags) for calling of the function from javascript.

## Javascript API
### anonymizeFile
A javascript wrapper function for calling of WebAssembly function anonymizeFile.

Parameters:
- input: Uint8Array with content of a DICOM file.
- output: a buffer allocated in the WebAssembly module heap for anonymized data.
- bufferLen: size of the output buffer in bytes.
- tracingLevel:  integet 0,1,2 that defines a tracing level (output to console);

### printTags
A javascript wrapper function for calling of WebAssembly function printTags.

Parameters:
- input (Uint8Array): an array with content of a DICOM file.
- parentHtmlTag (string): id of HTML element where the information will be printed.

### sendData
The functon sends content of DICOM file to a orthanc server (endpoint and credentials currently hardcoded).

Parameters:
- dicomData (UInt8Array): array with content of a DICOM file.

## Javascript auxillary functions.
The following functions facilitate displaying of information on th WEB page and are called form the WebAssembly. The functions are located in the file library.js and compiled in the WebAssembly module.
### addTagsJS
Auxillary function that prints DICOM tag information to the WEB page.

Parameters:
- parentHtmlTagNameUTF8 (utf8 string): id of an HTML element used to display the information.
- s (utf8 string): DICOM tag information
- confidential (boolean): true if the tag is confidential.
### showErrorMessageJS
Auxillary function to display error messages on the WEB page.

Parameters:
- errorMessageUTF8 (utf8 string): error message.

## Used software
- DCMTK library
- Repository Parsing DICOM using WebAssembly (https://github.com/jodogne/wasm-dicom-parser)
- Emscripten framework
- cmake

