#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcistrmb.h>
#include <dcmtk/dcmdata/dcmetinf.h>
#include <memory>

void stripPrivateTags(std::shared_ptr<DcmFileFormat> file);
bool isConfidential(const DcmTagKey& key);
