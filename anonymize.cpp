#include <emscripten/emscripten.h>
#include <iostream>
#include "stripDataset.h"


extern "C" void AddTag(bool isHeader, const char* tag, bool red);

// Use DCMTK to parse a memory buffer containing a DICOM file. This
// code comes from Orthanc.
static std::shared_ptr<DcmFileFormat> LoadFromMemoryBuffer(const void* buffer,
                                           size_t size)
{
    DcmInputBufferStream is;
    if (size > 0)
    {
        is.setBuffer(buffer, size);
    }
    is.setEos();

    std::shared_ptr<DcmFileFormat> result(new DcmFileFormat);

    result->transferInit();
    if (!result->read(is).good())
    {
        result.reset();
    }
    else
    {
        result->loadAllDataIntoMemory();
        result->transferEnd();
    }
    return result;
}

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
extern "C" bool EMSCRIPTEN_KEEPALIVE anonymizeFile(const void* data, size_t length)
{
    bool ok = false;
    std::cout << "---------anonymizeFile Begin" << std::endl;
    if(data && length)
    {
        std::shared_ptr<DcmFileFormat> dicom(LoadFromMemoryBuffer(data, length));
        if(dicom)
        {
            std::cout << "DICOM file successfuly loaded" << std::endl;
            printTags(dicom, true);
            stripPrivateTags(dicom);
            printTags(dicom, false);
            ok = true;
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
    return ok;
}
