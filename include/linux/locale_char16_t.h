/////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Файл locale_char16_t.h  - объявление новый фасет ctype<char16_t>
//

#pragma once

#include <locale>
#include <vector>
#include <Tstring.h>
#include <langinfo.h>

//namespace tlib
//{
//      namespace inner_impl
//      {
//            template <class CharT>
//            class ctype : public std::ctype<CharT>
//            {
//                  static_assert(true, "implementation of this class is missing");
//            };
//
//            template <>
//            class ctype<char16_t> : public std::__ctype_abstract_base<char16_t>
//            {
//                  std::__c_locale _loc;
//
//                  template <class Facet>
//                  struct deletable_ctype : Facet
//                  {
//                        template <class... Args>
//                        deletable_ctype(Args&&... args) : Facet(std::forward<Args>(args)...)
//                        {
//                        }
//                        // note: создается неявный public деструктор
//                  };
//
//                  deletable_ctype<std::ctype<wchar_t>> ctype_wchar;
//
//              public:
//                  using char_type = char16_t;
//                  using base = std::__ctype_abstract_base<char16_t>;
//
//                  static std::locale::id id;
//
//
//                  template <class LR, class... Args>
//                  ctype(LR loc, Args&&... args) : base(std::forward<Args>(args)...),
//                                                  ctype_wchar(loc, std::forward<Args>(args)...)
//                  {
//                        using L = std::remove_cv_t<std::remove_reference_t<LR>>;
//                        if constexpr (std::is_same_v<L, std::__c_locale>)
//                        {
//                              _loc = loc;
//                        }
//                  }
//
//              protected:
//                  virtual ~ctype(){};
//
//                  virtual bool do_is(mask m, char16_t c) const
//                  {
//                        return ctype_wchar.is(m, c);
//                  };
//
//                  virtual const char16_t* do_is(const char16_t* __lo, const char16_t* __hi, mask* __vec) const
//                  {
//                        std::wstring tmp = tlib::u16str_wstr(std::u16string(__lo, __hi));
//                        ctype_wchar.is(tmp.data(), tmp.data() + tmp.size(), __vec);
//                        return __hi;
//                  };
//
//                  virtual const char16_t* do_scan_is(mask __m, const char16_t* __lo, const char16_t* __hi) const
//                  {
//                        while (__lo < __hi && !this->do_is(__m, *__lo))
//                              ++__lo;
//                        return __lo;
//                  };
//
//                  virtual const char16_t* do_scan_not(mask __m, const char16_t* __lo, const char16_t* __hi) const
//                  {
//                        while (__lo < __hi && this->do_is(__m, *__lo) != 0)
//                              ++__lo;
//                        return __lo;
//                  };
//
//                  virtual char16_t do_toupper(char16_t __c) const
//                  {
//                        return (char16_t)ctype_wchar.toupper(__c);
//                  };
//
//                  virtual const char16_t* do_toupper(char16_t* __lo, const char16_t* __hi) const
//                  {
//                        while (__lo < __hi)
//                        {
//                              *__lo = (char16_t)ctype_wchar.toupper(*__lo);
//                              ++__lo;
//                        }
//                        return __hi;
//                  };
//
//                  virtual char16_t do_tolower(char16_t __c) const
//                  {
//                        return (char16_t)ctype_wchar.tolower(__c);
//                  };
//
//                  virtual const char16_t* do_tolower(char16_t* __lo, const char16_t* __hi) const
//                  {
//                        while (__lo < __hi)
//                        {
//                              *__lo = (char16_t)ctype_wchar.tolower(*__lo);
//                              ++__lo;
//                        }
//                        return __hi;
//                  };
//
//
//                  virtual char16_t do_widen(char __c) const
//                  {
//                        return (char16_t)ctype_wchar.widen(__c);
//                  }
//
//                  virtual const char* do_widen(const char* __lo, const char* __hi, char16_t* __dest) const
//                  {
//                        while (__lo < __hi)
//                        {
//                              *__dest = do_widen(*__lo);
//                              ++__lo;
//                              ++__dest;
//                        }
//                        return __hi;
//                  }
//
//                  virtual char do_narrow(char16_t __c, char def) const
//                  {
//                        return ctype_wchar.narrow(__c, def);
//                  }
//
//                  virtual const char16_t* do_narrow(const char16_t* __lo, const char16_t* __hi, char __dfault, char* __dest) const
//                  {
//                        while (__lo < __hi)
//                        {
//                              *__dest = do_narrow(*__lo, __dfault);
//                              ++__lo;
//                              ++__dest;
//                        }
//                        return __hi;
//                  }
//
//              private:
//                  ctype(const ctype&);
//                  void operator=(const ctype&);
//            };
//
//      }
//}
//








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

