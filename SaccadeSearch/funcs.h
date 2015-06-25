#include "Conandata.h"
#pragma once

ConanData* ReadConanFile(TCHAR* fname, CEdit* log);
bool FileExists(CString strFilename);