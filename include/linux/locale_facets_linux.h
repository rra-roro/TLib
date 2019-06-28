/////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Файл locale_char16_t.h  - объявление новый фасет ctype<char16_t>
//

#pragma once

#include <locale>
#include <vector>
#include <Tstring.h>
#include <langinfo.h>
#include <string_view>
#include <algorithm>
#include <wctype.h>

namespace tlib
{
      namespace inner_impl
      {
            template <class CharT>
            class ctype : public std::ctype<CharT>
            {
                  static_assert(true, "implementation of this class is missing");
            };


            /**@brief ctype facet modification for unicode cyrillic (basic russian
             * alphabet and cyrillic extensions) symbols
             *
             * See ctype facet decl*/
            template <>
            class ctype<char16_t> : public std::ctype<char16_t>
            {
                  template <class Facet>
                  struct deletable_ctype : Facet
                  {
                        template <class... Args>
                        deletable_ctype(Args&&... args) : Facet(std::forward<Args>(args)...)
                        {
                        }
                        // note: создается неявный public деструктор
                  };

                  deletable_ctype<std::ctype<wchar_t>> ctype_wchar;

              public:
                  using char_type = char16_t;
                  using base = std::ctype<char16_t>;

                  template <class LR, class... Args>
                  ctype(LR loc, Args&&... args) : base(std::forward<Args>(args)...),
                                                  ctype_wchar(loc, std::forward<Args>(args)...)
                  {
                  }

              protected:
                  virtual ~ctype(){};

                  virtual bool do_is(mask m, char16_t c) const
                  {
                        return ctype_wchar.is(m, c);
                  };

                  virtual const char16_t* do_is(const char16_t* __lo, const char16_t* __hi, mask* __vec) const
                  {
                        std::vector<wchar_t> tmp(__lo, __hi);
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

            template <class CharT>
            class ctype_byname : public ctype<CharT>
            {
              public:
                  ctype_byname(std::string_view sw) : ctype<CharT>(newlocale(LC_ALL_MASK, sw.data(), (locale_t)0)){};
            };


            //---------------------------------------------------
            template <class CharT>
            class numpunct : public std::numpunct<CharT>
            {
              public:
                  using char_type = CharT;
                  using string_type = std::basic_string<char_type>;
                  using base = std::numpunct<char_type>;

                  template <class LR, class... Args>
                  numpunct(LR loc, Args&&... args) : base(loc, std::forward<Args>(args)...)
                  {
                        using L = std::remove_cv_t<std::remove_reference_t<LR>>;
                        if constexpr (std::is_same_v<L, std::__c_locale>)
                        {
                              __cloc = loc;

                              if (__cloc)
                              {
                                    auto __old = uselocale(__cloc);

                                    if constexpr (std::is_same_v<CharT, char>)
                                    {
                                          unsigned char decimal_point_tmp = nl_langinfo_l(DECIMAL_POINT, __cloc)[0];
                                          unsigned char thousands_sep_tmp = nl_langinfo_l(THOUSANDS_SEP, __cloc)[0];

                                          if (thousands_sep_tmp != 0xE2)
                                                thousands_sep = thousands_sep_tmp;
                                          else
                                                thousands_sep = ' ';

                                          if (decimal_point_tmp != 0xE2)
                                                decimal_point = decimal_point_tmp;                                    
                                    }
                                    else if constexpr (!std::is_same_v<CharT, char>)
                                    {
                                          union {
                                                char* __s;
                                                char_type __w;
                                          } __u;
                                          __u.__s = nl_langinfo_l(_NL_MONETARY_DECIMAL_POINT_WC, __cloc);
                                          decimal_point = __u.__w;
                                          __u.__s = nl_langinfo_l(_NL_MONETARY_THOUSANDS_SEP_WC, __cloc);
                                          thousands_sep = __u.__w;

                                          if (thousands_sep == 0x202f) // неразрывный пробел в UTF-8 - 3 байта
                                                thousands_sep = TemplateTypeOfCh('\xa0', CharT);   // неразрывный пробел ASCII  - 1 байт
                                    }

                                    uselocale(__old);
                              }
                        }
                  }

              protected:
                  virtual char_type
                  do_decimal_point() const
                  {
                        return decimal_point;
                  }

                  virtual char_type
                  do_thousands_sep() const
                  {
                        return thousands_sep;
                  }

                  virtual ~numpunct(){};

              private:
                  std::__c_locale __cloc;
                  char_type decimal_point = TemplateTypeOfCh('.', CharT);
                  char_type thousands_sep = TemplateTypeOfCh(',', CharT);
            };

            template <class CharT>
            class numpunct_byname : public numpunct<CharT>
            {
              public:
                  numpunct_byname(std::string_view sw) : numpunct<CharT>(newlocale(LC_ALL_MASK, sw.data(), (locale_t)0)){};
            };

            //---------------------------------------------------
            template <class CharT, bool _Intl = false>
            class moneypunct : public std::moneypunct<CharT, _Intl>
            {
              public:
                  using char_type = CharT;
                  using string_type = std::basic_string<char_type>;
                  using base = std::moneypunct<CharT, _Intl>;

                  template <class LR, class... Args>
                  explicit moneypunct(LR __cloc, Args&&... args) : base(__cloc, std::forward<Args>(args)...)
                  {
                        using L = std::remove_cv_t<std::remove_reference_t<LR>>;
                        if constexpr (std::is_same_v<L, std::__c_locale>)
                        {
                              if (__cloc)
                              {
                                    init(__cloc);
                              }
                        }
                  }

              protected:
                  virtual char_type
                  do_decimal_point() const
                  {
                        return m_decimal_point;
                  }

                  virtual char_type
                  do_thousands_sep() const
                  {
                        return m_thousands_sep;
                  }

                  virtual string_type
                  do_curr_symbol() const
                  {
                        return m_curr_symbol;
                  }

                  virtual ~moneypunct(){};

              private:
                  std::__c_locale __cloc;
                  char_type m_decimal_point = '.';
                  char_type m_thousands_sep = ',';
                  string_type m_curr_symbol = TemplateTypeOfStr("", CharT);

                  void init(std::__c_locale __cloc)
                  {
                        auto __old = uselocale(__cloc);

                        if constexpr (std::is_same_v<CharT, char>)
                        {
                              unsigned char decimal_point_tmp = nl_langinfo_l(__MON_DECIMAL_POINT, __cloc)[0];
                              unsigned char thousands_sep_tmp = nl_langinfo_l(__MON_THOUSANDS_SEP, __cloc)[0];

                              if (thousands_sep_tmp != 0xE2)
                                    m_thousands_sep = thousands_sep_tmp;
                              else
                                    m_thousands_sep = ' ';

                              std::string curr_symbol_tmp = nl_langinfo_l(__CURRENCY_SYMBOL, __cloc);
                              std::string curr_symbol_int_tmp = nl_langinfo_l(__INT_CURR_SYMBOL, __cloc);

                              if (decimal_point_tmp != 0xE2)
                                    m_decimal_point = decimal_point_tmp;

                              if (_Intl == false)
                                    m_curr_symbol = curr_symbol_tmp;
                              else
                                    m_curr_symbol = curr_symbol_int_tmp;

                              if (m_curr_symbol.find(" ") != npos)
                              {
                                    m_curr_symbol.erase(std::remove(m_curr_symbol.begin(), m_curr_symbol.end(), ' '),
                                        m_curr_symbol.end());
                              }
                        }
                        else
                        {
                              auto to_templateStr = [&](std::string str) {
                                    string_type tmp;

                                    std::string_view encoding = nl_langinfo_l(CODESET, (locale_t)__cloc);
                                    if (encoding.find("utf") != npos || encoding.find("UTF") != npos)
                                    {
                                          tmp = cstr_templateStr<CharT>(str);
                                    }
                                    else
                                    {
                                          mbstate_t __state = {};
                                          wchar_t* __wcs = new wchar_t[str.size() + 1];
                                          const char* __ccurr = str.data();
                                          size_t ret = mbsrtowcs(__wcs, &__ccurr, str.size() + 1, &__state);
                                          __wcs[ret] = L'\0';
                                          tmp = wstr_templateStr<CharT>(__wcs);
                                          delete[] __wcs;
                                    }

                                    return tmp;
                              };

                              union {
                                    char* __s;
                                    char_type __w;
                              } __u;
                              __u.__s = nl_langinfo_l(_NL_MONETARY_DECIMAL_POINT_WC, __cloc);
                              m_decimal_point = __u.__w;
                              __u.__s = nl_langinfo_l(_NL_MONETARY_THOUSANDS_SEP_WC, __cloc);
                              m_thousands_sep = __u.__w;

                              if (m_thousands_sep == 0x202f) // неразрывный пробел в UTF-8 - 3 байта
                                     m_thousands_sep = TemplateTypeOfCh('\xa0', CharT);  // неразрывный пробел ASCII  - 1 байт

                              if (_Intl == false)
                              {
                                    std::string tmp = nl_langinfo_l(__CURRENCY_SYMBOL, __cloc);
                                    m_curr_symbol = to_templateStr(tmp);
                              }
                              else
                              {
                                    std::string tmp = nl_langinfo_l(__INT_CURR_SYMBOL, __cloc);
                                    m_curr_symbol = to_templateStr(tmp);
                              }


                              if (m_curr_symbol.find(TemplateTypeOfStr(" ", CharT)) != npos)
                              {
                                    m_curr_symbol.erase(std::remove(m_curr_symbol.begin(), m_curr_symbol.end(), TemplateTypeOfCh(' ', CharT)),
                                        m_curr_symbol.end());
                              }                              
                        };

                        uselocale(__old);
                  }
            };

            template <class CharT, bool _Intl = false>
            class moneypunct_byname : public moneypunct<CharT, _Intl>
            {
              public:
                  moneypunct_byname(std::string_view sw) : moneypunct<CharT, _Intl>(newlocale(LC_ALL_MASK, sw.data(), (locale_t)0), sw.data()){};
            };

      }
}
