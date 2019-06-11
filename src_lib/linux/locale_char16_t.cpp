
#include <linux/locale_char16_t.h>
#include <locale>
#include <cstring>
#include <langinfo.h>

using namespace std;

//--------------------------
extern "C" __typeof(nl_langinfo_l) __nl_langinfo_l;

template <>
void numpunct<char16_t>::_M_initialize_numpunct(__c_locale __cloc)
{
      if (!_M_data)
            _M_data = new __numpunct_cache<char16_t>;
      if (!__cloc)
      {
            // "C" locale
            _M_data->_M_grouping = "";
            _M_data->_M_grouping_size = 0;
            _M_data->_M_use_grouping = false;
            _M_data->_M_decimal_point = u'.';
            _M_data->_M_thousands_sep = u',';
            // Use ctype::widen code without the facet...
            for (size_t __i = 0; __i < __num_base::_S_oend; ++__i)
                  _M_data->_M_atoms_out[__i] =
                      static_cast<char16_t>(__num_base::_S_atoms_out[__i]);
            for (size_t __j = 0; __j < __num_base::_S_iend; ++__j)
                  _M_data->_M_atoms_in[__j] =
                      static_cast<char16_t>(__num_base::_S_atoms_in[__j]);
      }
      else
      {
            // Named locale.
            // NB: In the GNU model wchar_t is always 32 bit wide.
            union {
                  char* __s;
                  wchar_t __w;
            } __u;
            __u.__s = __nl_langinfo_l(_NL_NUMERIC_DECIMAL_POINT_WC, __cloc);
            _M_data->_M_decimal_point = (char16_t)__u.__w;
            __u.__s = __nl_langinfo_l(_NL_NUMERIC_THOUSANDS_SEP_WC, __cloc);
            _M_data->_M_thousands_sep = (char16_t)__u.__w;
            // Check for NULL, which implies no grouping.
            if (_M_data->_M_thousands_sep == u'\0')
            {
                  // Like in "C" locale.
                  _M_data->_M_grouping = "";
                  _M_data->_M_grouping_size = 0;
                  _M_data->_M_use_grouping = false;
                  _M_data->_M_thousands_sep = u',';
            }
            else
            {
                  const char* __src = __nl_langinfo_l(GROUPING, __cloc);
                  const size_t __len = strlen(__src);
                  if (__len)
                  {
                        __try
                        {
                              char* __dst = new char[__len + 1];
                              memcpy(__dst, __src, __len + 1);
                              _M_data->_M_grouping = __dst;
                        }
                        __catch(...)
                        {
                              delete _M_data;
                              _M_data = 0;
                              __throw_exception_again;
                        }
                  }
                  else
                  {
                        _M_data->_M_grouping = "";
                        _M_data->_M_use_grouping = false;
                  }
                  _M_data->_M_grouping_size = __len;
            }
      }
      // NB: There is no way to extact this info from posix locales.
      // _M_truename = __nl_langinfo_l(YESSTR, __cloc);
      _M_data->_M_truename = u"true";
      _M_data->_M_truename_size = 4;
      // _M_falsename = __nl_langinfo_l(NOSTR, __cloc);
      _M_data->_M_falsename = u"false";
      _M_data->_M_falsename_size = 5;
}

template <>
numpunct<char16_t>::~numpunct()
{
      if (_M_data->_M_grouping_size)
            delete[] _M_data->_M_grouping;
      delete _M_data;
}
//--------------------------

//std::locale::id std::ctype<char16_t>::id{};
