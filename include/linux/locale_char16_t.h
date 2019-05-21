/////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Файл locale_char16_t.h  - объявление новый фасет ctype<char16_t>
//

#pragma once

#ifndef TLOCALE_CHAR16_T_LINUX
#define TLOCALE_CHAR16_T_LINUX

#include <locale>
#include <vector>


/**@brief ctype facet modification for unicode cyrillic (basic russian
 * alphabet and cyrillic extensions) symbols
 *
 * See ctype facet decl*/
template <>
class std::ctype<char16_t> : public std::__ctype_abstract_base<char16_t>
{
  protected:
      std::locale m_locale;
      const std::ctype<wchar_t>& ctype_wchar;


  public:
      /// The facet id for ctype<char16_t>
      static std::locale::id id;

      explicit ctype(size_t r = 0) : ctype(std::locale(), r){};

      explicit ctype(const std::locale& loc, size_t r = 0) : std::__ctype_abstract_base<char16_t>(r),
                                                             m_locale(loc),
                                                             ctype_wchar(std::use_facet<std::ctype<wchar_t>>(loc)){};

      ~ctype(){};

  protected:
      virtual bool do_is(mask m, char16_t c) const
      {
            return ctype_wchar.is(m, c);
      };

      virtual const char16_t* do_is(const char16_t* __lo, const char16_t* __hi, mask* __vec) const
      {
            vector<wchar_t> tmp(__lo, __hi);
            ctype_wchar.is(&tmp[0], &tmp[tmp.size()], __vec);
            return __hi;
      };

      virtual const char16_t* do_scan_is(mask __m, const char16_t* __lo, const char16_t* __hi) const
      {
            while (__lo < __hi && !this->do_is(__m, *__lo))
                  ++__lo;
            return __lo;
      };

      virtual const char16_t* do_scan_not(mask __m, const char16_t* __lo, const char16_t* __hi) const
      {
            while (__lo < __hi && this->do_is(__m, *__lo) != 0)
                  ++__lo;
            return __lo;
      };

      virtual char16_t do_toupper(char16_t __c) const
      {
            return (char16_t)ctype_wchar.toupper(__c);
      };

      virtual const char16_t* do_toupper(char16_t* __lo, const char16_t* __hi) const
      {
            while (__lo < __hi)
            {
                  *__lo = (char16_t)ctype_wchar.toupper(*__lo);
                  ++__lo;
            }
            return __hi;
      };

      virtual char16_t do_tolower(char16_t __c) const
      {
            return (char16_t)ctype_wchar.tolower(__c);
      };

      virtual const char16_t* do_tolower(char16_t* __lo, const char16_t* __hi) const
      {
            while (__lo < __hi)
            {
                  *__lo = (char16_t)ctype_wchar.tolower(*__lo);
                  ++__lo;
            }
            return __hi;
      };


      virtual char16_t do_widen(char __c) const
      {
            return (char16_t)ctype_wchar.widen(__c);
      }

      virtual const char* do_widen(const char* __lo, const char* __hi, char16_t* __dest) const
      {
            while (__lo < __hi)
            {
                  *__dest = do_widen(*__lo);
                  ++__lo;
                  ++__dest;
            }
            return __hi;
      }

      virtual char do_narrow(char16_t __c, char def) const
      {
            return ctype_wchar.narrow(__c, def);
      }

      virtual const char16_t* do_narrow(const char16_t* __lo, const char16_t* __hi, char __dfault, char* __dest) const
      {
            while (__lo < __hi)
            {
                  *__dest = do_narrow(*__lo, __dfault);
                  ++__lo;
                  ++__dest;
            }
            return __hi;
      }

  private:
      ctype(const ctype&);
      void operator=(const ctype&);
};


#endif // TLOCALE_CHAR16_T_LINUX_
