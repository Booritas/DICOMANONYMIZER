var ENTITY_MAP = {
    '&': '&amp;',
    '<': '&lt;',
    '>': '&gt;',
    '"': '&quot;',
    "'": '&#39;',
    '/': '&#x2F;',
    '`': '&#x60;',
    '=': '&#x3D;'
  };
  
  function EscapeHtml (string) {
    // http://stackoverflow.com/a/12034334/881731
    return String(string).replace(/[&<>"'`=\/]/g, function(s) {
      return ENTITY_MAP[s];
    });
  }
  

function anonymizeFile(input, output, bufferLen)
{
    const done = Module.ccall(
        'anonymizeFile',    // Name of the C++ function
        'number',           // Return value (a Boolean indicating success)
        [                   // The list of arguments
            'array',    
            'number',
            'number',
            'number'
        ],
        [                   // The value of the arguments
            input,
            input.byteLength,
            output,
            bufferLen 
        ]
    );
    return done;
}
function updateProtocol(text)
{
   document.getElementById("protocol").innerHTML += text + "<br/>";
}

function updateRequestResult(text)
{
    document.getElementById("response").innerHTML = text;
}

function sendData(dicomData)
{
    var url = "http://localhost/orthanc/instances";
    updateProtocol("Sending data. Data size:" + dicomData.byteLength);
    var oReq = new XMLHttpRequest();
    oReq.withCredentials = true;
    oReq.open("POST", url, false, "orthanc", "orthanc");
    oReq.onload = function(e){
        console.log(oReq.response)
        updateProtocol("Response code:" + oReq.status);
        updateRequestResult(oReq.response);
    }
    oReq.send(dicomData);
    updateProtocol("Data is sent");

}
// This event is triggered when the user uploads a DICOM file
document.getElementById('upload').addEventListener('submit', function(e) 
{
    // Check whether a file has actually been selected
    var fileInput = document.getElementById('dicom');
    if('files' in fileInput && fileInput.files.length == 1)
    {
        // Create a reader to receive the file selected by the HTML form
        // https://developer.mozilla.org/fr/docs/Web/API/FileReader/onload
        var reader = new FileReader();
        reader.onload = function(event) 
        {
            var dicom = this.result;
            console.log("Received " + dicom.byteLength + " bytes.")
            var input = new Uint8Array(dicom);
            const bytesPerElement = Module.HEAPU8.BYTES_PER_ELEMENT;
            const newBufferLen = dicom.byteLength*2;
            var outputBuffer = Module._malloc(newBufferLen*bytesPerElement);
            //Module.HEAPU8.set(output, outputBuffer/bytesPerElement);
            var newLen = anonymizeFile(input, outputBuffer, newBufferLen);
            if (newLen<=0)
            {
                alert('Sorry, unable to process the DICOM file');
            }
            else
            {
                var output = new Uint8Array(Module.HEAPU8.buffer, outputBuffer, newLen);
                console.log("Sending " + newLen + " bytes");
                sendData(output);
            }
            Module._free(outputBuffer);
        };
        // Instruct JavaScript to load the file as an ArrayBuffer
        reader.readAsArrayBuffer(fileInput.files[0]);

    }
    else
    {
        alert('Please select one DICOM file');
    }
    // Prevent the actual uploading of the form
    e.preventDefault();
});
  