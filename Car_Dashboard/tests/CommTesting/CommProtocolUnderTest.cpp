// Build with BuildAndUpload.cmd, which adds Car_Dashboard/{include,src} to the
// include path (-I) so this <...> include resolves — same convention as
// tests/GPSTesting. A plain `arduino-cli compile` without those -I flags cannot
// find CommProtocol.cpp; use the .cmd.
#include <CommProtocol.cpp>
