#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#include <iostream>
#include <vector>
#include "stripDataset.h"
#include "utils.h"
#include <dcmtk/dcmdata/dcostrmb.h>

extern "C" void AddTag(bool isHeader, const char* tag, bool red);


// Loop over the tags contained in a DICOM dataset, convert them as
// strings, and fill the HTML page with these strings.
static void sendTagsToJavascript(DcmItem* item, bool isHeader)
{
    if (item != NULL)
    {
        for (unsigned long i = 0; i < item->card(); i++)
        {
            DcmElement* element = item->getElement(i);
            if (element != NULL)
            {
                char buf[64];
                sprintf(buf, "(%04x,%04x) ", element->getGTag(), element->getETag());

                // Copy the tag to ensure const-correctness
                DcmTag tag(element->getTag());
                bool red = isConfidential(tag);  
                std::string name = std::string(buf) + std::string(tag.getTagName()) + ": ";
                
                char* value = NULL;
                if (element != NULL)
                {
                    if (element->isaString() &&
                        element->getString(value).good() &&
                        value != NULL)
                    {
                        std::string tag = name + std::string(value);
                        AddTag(isHeader, tag.c_str(), red);
                    }
                    else
                    {
                        std::string tag = name + "Not a string";
                        AddTag(isHeader, tag.c_str(), red);
                    }
                }
            }
        }
    }
}

// Shows tag values on the WEB page
static void printTags(std::shared_ptr<DcmFileFormat> dicom, bool left)
{
    sendTagsToJavascript(dicom->getMetaInfo(), left);
    sendTagsToJavascript(dicom->getDataset(), left);
}

// Performes anonimyzation of a DICOM file
extern "C" size_t EMSCRIPTEN_KEEPALIVE anonymizeFile(
    const void* inputData, size_t inputLength,
    void* outputData, size_t outputLength)
{
    // std::cout << "Copy " << inputLength << " bytest to the output buffer" << std::endl;
    // std::memcpy(outputData, inputData, inputLength);
    // return inputLength;
    size_t written = 0;
    std::cout << "---------anonymizeFile Begin. Received bytes: " << inputLength << std::endl;
    std::cout << "Output buffer size: " << outputLength << std::endl;
    if(inputData && inputLength)
    {
        std::shared_ptr<DcmFileFormat> dicom(loadFromMemoryBuffer(inputData, inputLength));
        if(dicom)
        {
            std::cout << "DICOM file successfuly loaded" << std::endl;
            printTags(dicom, true);
            stripPrivateTags(dicom);
            printTags(dicom, false);
            OFCondition res = saveToMemoryBuffer(dicom, outputData, outputLength, written);
            if(res.bad())
            {
                written = 0;
                std::cout << "Failed to save dataset" << std::endl;
                std::cout << "Error code:" << res.code() << ". Message: " << res.text() << std::endl;
            }

        }
        else
        {
            std::cout << "Failed to load DICOM file" << std::endl;
        }
    }
    else
    {
        std::cout << "Invalid data received" << std::endl;
    }
    std::cout << "---------anonymizeFile End" << std::endl;
    return written;
}
#endif