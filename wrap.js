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
  

function anonymizeFile(dicom)
{
    const done = Module.ccall(
        'anonymizeFile',    // Name of the C++ function
        'number',           // Return value (a Boolean indicating success)
        [                   // The list of arguments
            'array',    
            'number'
        ],
        [                   // The value of the arguments
            new Uint8Array(dicom),
            dicom.byteLength
        ]
    );
    return done;
}

function parseFile(dicom)
{
    const done = Module.ccall(
        'parseFile',    // Name of the C++ function
        'number',           // Return value (a Boolean indicating success)
        [                   // The list of arguments
            'array',    
            'number'
        ],
        [                   // The value of the arguments
            new Uint8Array(dicom),
            dicom.byteLength
        ]
    );
    return done;
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
            // Call the C++ function "ParseDicom()"
            if (!anonymizeFile(dicom))
            {
                // The C++ function has failed (it has returned "false")
                alert('Sorry, unable to parse to DICOM file');
            }
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
  