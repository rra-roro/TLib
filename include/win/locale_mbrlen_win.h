#include <cwchar>

namespace tlib
{
      int mbrlen_l(const char* s, size_t n, std::mbstate_t* state, const tlib::locale& loc);
}
