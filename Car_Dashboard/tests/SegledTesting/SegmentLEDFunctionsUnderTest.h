#pragma once

// Compile the PRODUCTION SegmentLED library, not a copy. BuildAndUpload.cmd puts
// Car_Dashboard/{include,src} on the include path (-I) so these <...> includes
// resolve — same convention as tests/GPSTesting, tests/CommTesting, tests/OBD2Testing.
#include <SegmentLEDFunctions.h>
