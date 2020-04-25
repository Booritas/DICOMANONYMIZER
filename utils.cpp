//
// Created by stas on 21.04.20.
//
#include "utils.h"
#include <dcmtk/dcmdata/dcostrmb.h>


// Use DCMTK to parse a memory buffer containing a DICOM file. This
// code comes from Orthanc.
std::shared_ptr<DcmFileFormat> loadFromMemoryBuffer(const void* buffer, size_t size)
{
    DcmInputBufferStream is;
    if (size > 0)
    {
        is.setBuffer(buffer, size);
    }
    is.setEos();

    std::shared_ptr<DcmFileFormat> dicom(new DcmFileFormat);

    dicom->transferInit();
    OFCondition result = dicom->read(is);
    if (!result.good())
    {
        dicom.reset();
    }
    else
    {
        dicom->loadAllDataIntoMemory();
        dicom->transferEnd();
    }
    return dicom;
}

OFCondition saveToMemoryBuffer(std::shared_ptr<DcmFileFormat> dicom, void* buffer, size_t size, size_t& written)
{
    DcmOutputBufferStream out(buffer, size);

    dicom->transferInit();
    OFCondition res = dicom->write(out, EXS_Unknown, EET_UndefinedLength, nullptr,
                                   EGL_recalcGL);
    dicom->transferEnd();

    if(res.good())
    {
        void *bf(nullptr);
        offile_off_t writtens(0);
        out.flushBuffer(bf,writtens);
        written = (size_t)writtens;
    }

    return res;
}
