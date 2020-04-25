//
// Created by stas on 21.04.20.
//

#ifndef DICOMANONYMIZER_UTILS_H
#define DICOMANONYMIZER_UTILS_H
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcistrmb.h>
#include <dcmtk/dcmdata/dcmetinf.h>
#include <dcmtk/ofstd/ofcond.h>

#include <memory>
std::shared_ptr<DcmFileFormat> loadFromMemoryBuffer(const void* buffer, size_t size);
OFCondition saveToMemoryBuffer(std::shared_ptr<DcmFileFormat> dicom, void* buffer, size_t size, size_t& written);

#endif //DICOMANONYMIZER_UTILS_H
