#ifndef TREGEX
#define TREGEX

#include <regex>
#include <Tstring.h>

#ifdef _WIN32

// CLASS regex_traits<char16_t>
template <>
class std::regex_traits<char16_t>
    : public std::_Regex_traits<char16_t>
{ // specialization for char16_t
  public:
      int value(char16_t _Ch, int _Base) const
      { // map character value to numeric value
            if ((_Base != 8 && u'0' <= _Ch && _Ch <= u'9') || (_Base == 8 && u'0' <= _Ch && _Ch <= u'7'))
                  return (_Ch - u'0');
            else if (_Base != 16)
                  ;
            else if (u'a' <= _Ch && _Ch <= u'f')
                  return (_Ch - u'a' + 10);
            else if (u'A' <= _Ch && _Ch <= u'F')
                  return (_Ch - u'A' + 10);
            return (-1);
      }
};

template <>
inline const char16_t *std::_Cl_names::_Get<char16_t>() const
{ // get wide pointer
      return ((char16_t *)_Wide);
};

extern template std::locale::id std::collate<char16_t>::id;

#endif

namespace tlib
{
      using uregex = std::basic_regex<char16_t>;
      using ucmatch = std::match_results<const char16_t *>;
      using usmatch = std::match_results<std::u16string::const_iterator>;

      using ucregex_token_iterator = std::regex_token_iterator<const char16_t *>;
      using usregex_token_iterator = std::regex_token_iterator<std::u16string::const_iterator>;
      using ucregex_iterator = std::regex_iterator<const char16_t *>;
      using usregex_iterator = std::regex_iterator<std::u16string::const_iterator>;

      using tregex = std::basic_regex<TCHAR>;
      using tcmatch = std::match_results<const TCHAR *>;
      using tsmatch = std::match_results<tlib::tstring::const_iterator>;

      using tcregex_token_iterator = std::regex_token_iterator<const TCHAR *>;
      using tsregex_token_iterator = std::regex_token_iterator<tlib::tstring::const_iterator>;
      using tcregex_iterator = std::regex_iterator<const TCHAR *>;
      using tsregex_iterator = std::regex_iterator<tlib::tstring::const_iterator>;
}


#if !(defined(__linux__) || defined(_LIB))

#define _LIB_NAME "tlib_char16"

#include <win/select_lib.h>

#undef _LIB_NAME

#endif // !(defined(__linux__) || defined(_LIB))


#endif // TREGEX
