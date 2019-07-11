#include <Tlocale.h>
#include <win/locale_mbrlen_win.h> 
#include <cwchar>
#include <locale.h>
#include <stdlib.h>

namespace tlib
{
      int mbrlen_l(const char* s, size_t n, std::mbstate_t* state, const tlib::locale& loc)
      {
            _locale_t c_loc = _create_locale(LC_ALL, loc.name().c_str());
            return _mblen_l(s, n, c_loc);
      }
}
