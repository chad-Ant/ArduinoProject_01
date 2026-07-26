#pragma once

// Compile the PRODUCTION OBD2 library, not a copy. BuildAndUpload.cmd puts
// Car_Dashboard/{include,src} on the include path (-I) so these <...> includes
// resolve — same convention as tests/GPSTesting and tests/CommTesting. This keeps
// the test exercising the shipping implementation instead of a drifting snapshot.
#include <OBD2Functions.h>
#include <TimerFunctions.h>
