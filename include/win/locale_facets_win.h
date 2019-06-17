//////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Файл locale_char16_t.h  - объявление новый фасет ctype<char16_t>
//
#pragma once

#include <locale>
#include <Tlocale_config.h>

namespace tlib
{
      namespace inner_impl
      {

            template <class CharT>
            class ctype : public std::ctype<CharT>
            {
                  static_assert(true, "implementation of this class is missing");
            };

            template <>
            class ctype<char16_t> : public std::ctype<char16_t>

            {
                  template <class Facet>
                  struct deletable_ctype : Facet
                  {
                        template <class... Args>
                        deletable_ctype(Args &&... args) : Facet(std::forward<Args>(args)...)
                        {
                        }
                        // note: создается неявный public деструктор
                  };

                  deletable_ctype<std::ctype<wchar_t>> ctype_wchar;

              public:
                  using char_type = char16_t;
                  using base = std::ctype<char16_t>;

                  template <class... Args>
                  ctype(Args &&... args) : base(std::forward<Args>(args)...),
                                           ctype_wchar(std::forward<Args>(args)...){};

              protected:
                  virtual ~ctype() noexcept {};

                  virtual bool do_is(mask m, char16_t c) const
                  {
                        return ctype_wchar.is(m, c);
                  };

                  virtual const char16_t *do_is(const char16_t *__lo, const char16_t *__hi, mask *__vec) const
                  {
                        ctype_wchar.is((const wchar_t *)__lo, (const wchar_t *)__hi, __vec);
                        return __hi;
                  };

                  virtual const char16_t *do_scan_is(mask __m, const char16_t *__lo, const char16_t *__hi) const
                  {
                        return (const char16_t *)ctype_wchar.scan_is(__m, (const wchar_t *)__lo, (const wchar_t *)__hi);
                  };

                  virtual const char16_t *do_scan_not(mask __m, const char16_t *__lo, const char16_t *__hi) const
                  {
                        return (const char16_t *)ctype_wchar.scan_not(__m, (const wchar_t *)__lo, (const wchar_t *)__hi);
                  };

                  virtual char16_t do_toupper(char16_t __c) const
                  {
                        return ctype_wchar.toupper(__c);
                  };

                  virtual const char16_t *do_toupper(char16_t *__lo, const char16_t *__hi) const
                  {
                        return (const char16_t *)ctype_wchar.toupper((wchar_t *)__lo, (const wchar_t *)__hi);
                  };

                  virtual char16_t do_tolower(char16_t __c) const
                  {
                        return ctype_wchar.tolower(__c);
                  };

                  virtual const char16_t *do_tolower(char16_t *__lo, const char16_t *__hi) const
                  {
                        return (const char16_t *)ctype_wchar.tolower((wchar_t *)__lo, (const wchar_t *)__hi);
                  };

                  virtual char16_t do_widen(char __c) const
                  {
                        return ctype_wchar.widen(__c);
                  };

                  virtual const char *do_widen(const char *__lo, const char *__hi, char16_t *__dest) const
                  {
                        return ctype_wchar.widen(__lo, __hi, (wchar_t *)__dest);
                  };

                  virtual char do_narrow(char16_t __c, char def) const
                  {
                        return ctype_wchar.narrow(__c, def);
                  };

                  virtual const char16_t *do_narrow(const char16_t *__lo, const char16_t *__hi, char __dfault, char *__dest) const
                  {
                        return (const char16_t *)ctype_wchar.narrow((const wchar_t *)__lo, (const wchar_t *)__hi, __dfault, __dest);
                  };
            };

            // --------------------------------------------------------------------------
            template <class CharT>
            class numpunct : public std::numpunct<CharT>
            {
                  static_assert(true, "implementation of this class is missing");
            };

            template <>
            class numpunct<char16_t> : public std::numpunct<char16_t>
            {
              public:
                  using char_type = char16_t;
                  using string_type = std::basic_string<char_type>;
                  using base = std::numpunct<char_type>;

                  template <class... Args>
                  numpunct(Args &&... args) : base(std::forward<Args>(args)...)
                  {
                        init();
                  }

              protected:
                  virtual char_type do_decimal_point() const
                  {
                        return l_cfg_numbers.decimal_point;
                  }

                  virtual char_type do_thousands_sep() const
                  {
                        return l_cfg_numbers.thousands_sep;
                  }

                  virtual std::string do_grouping() const
                  {
                        return l_cfg_numbers.grouping;
                  }

                  virtual string_type do_truename() const
                  {
                        return TemplateTypeOfStr("true", char_type);
                  }

                  virtual string_type do_falsename() const
                  {
                        return TemplateTypeOfStr("false", char_type);
                  }

                  ~numpunct(){};

              private:
                  locale_cfg_numbers<char_type> l_cfg_numbers;

                  void init()
                  {
                        const lconv *ptr_lconv = localeconv();
                        l_cfg_numbers.decimal_point = (char_type)*ptr_lconv->_W_decimal_point;
                        l_cfg_numbers.thousands_sep = (char_type)*ptr_lconv->_W_thousands_sep;
                        l_cfg_numbers.grouping = ptr_lconv->grouping;
                  }
            };

            // --------------------------------------------------------------------------

            template <class CharT, bool _Intl = false>
            class moneypunct : public std::moneypunct<CharT, _Intl>
            {
              public:
                  using char_type = CharT;
                  using string_type = std::basic_string<char_type>;
                  using base = std::moneypunct<char_type>;

                  template <class... Args>
                  moneypunct(Args &&... args) : base(std::forward<Args>(args)...)
                  {
                        init();
                  }

              protected:
                  virtual char_type do_decimal_point() const
                  {
                        return l_cfg_moneys.decimal_point;
                  }

                  virtual char_type do_thousands_sep() const
                  {
                        return l_cfg_moneys.thousands_sep;
                  }

                  virtual std::string do_grouping() const
                  {
                        return l_cfg_moneys.grouping;
                  }

                  virtual string_type do_curr_symbol() const
                  {
                        return l_cfg_moneys.curr_symbol;
                  }

                  virtual string_type do_positive_sign() const
                  {
                        return l_cfg_moneys.positive_sign;
                  }

                  virtual string_type do_negative_sign() const
                  {
                        return l_cfg_moneys.negative_sign;
                  }

                  virtual int do_frac_digits() const
                  {
                        return l_cfg_moneys.frac_digits;
                  }

                  virtual std::money_base::pattern do_pos_format() const
                  {
                        using mb = std::money_base;
                        return { mb::sign, mb::value, mb::space, mb::symbol };
                  }

                  virtual std::money_base::pattern do_neg_format() const
                  {
                        using mb = std::money_base;
                        return { mb::sign, mb::value, mb::space, mb::symbol };
                  }

                  ~moneypunct(){};

              private:
                  locale_cfg_moneys<char_type> l_cfg_moneys;

                  void init()
                  {
                        const lconv *ptr_lconv = localeconv();
                        if constexpr (std::is_same_v<char_type, char>)
                        {
                              l_cfg_moneys.decimal_point = *ptr_lconv->mon_decimal_point;
                              l_cfg_moneys.thousands_sep = *ptr_lconv->mon_thousands_sep;
                              l_cfg_moneys.grouping = ptr_lconv->mon_grouping;
                              l_cfg_moneys.curr_symbol = (*ptr_lconv->_W_currency_symbol == 0x20bd) ? TemplateTypeOfStr("p.", char_type) : (*ptr_lconv->_W_currency_symbol == 0x20ac) ? TemplateTypeOfStr("euro", char_type) : ptr_lconv->currency_symbol;
                              l_cfg_moneys.positive_sign = ptr_lconv->positive_sign;
                              l_cfg_moneys.negative_sign = ptr_lconv->negative_sign;
                              l_cfg_moneys.frac_digits = ptr_lconv->frac_digits;
                        }
                        else
                        {
                              l_cfg_moneys.decimal_point = (char_type)*ptr_lconv->_W_mon_decimal_point;
                              l_cfg_moneys.thousands_sep = (char_type)*ptr_lconv->_W_mon_thousands_sep;
                              l_cfg_moneys.grouping = ptr_lconv->mon_grouping;
                              l_cfg_moneys.curr_symbol = (*ptr_lconv->_W_currency_symbol == 0x20bd) ? TemplateTypeOfStr("p.", char_type) : (*ptr_lconv->_W_currency_symbol == 0x20ac) ? TemplateTypeOfStr("euro", char_type) : (char_type *)ptr_lconv->_W_currency_symbol;
                              l_cfg_moneys.positive_sign = (char_type *)ptr_lconv->_W_positive_sign;
                              l_cfg_moneys.negative_sign = (char_type *)ptr_lconv->_W_negative_sign;
                              l_cfg_moneys.frac_digits = ptr_lconv->frac_digits;
                        }
                  }
            };

            // ----------------------------------------------------------------------------------------------
            template <class CharT, class OutputIt = std::ostreambuf_iterator<CharT>>
            class time_put : public std::time_put<CharT, OutputIt>
            {
                  static_assert(true, "implementation of this class is missing");
            };

            ////////////////////////////////////////////////////////
            template <>
            class time_put<char16_t> : public std::time_put<char16_t>
            {
              public:
                  using char_type = char16_t;
                  using iter_type = std::ostreambuf_iterator<char_type>;
                  using string_type = std::basic_string<char_type>;
                  using base = std::time_put<char_type>;

                  template <class LR, class... Args>
                  time_put(LR loc, Args &&... args) : base(loc, std::forward<Args>(args)...)
                  {
                        using L = std::remove_cv_t<std::remove_reference_t<LR>>;
                        if constexpr (std::is_same_v<L, std::_Locinfo>)
                        {
                              _Tnames = loc._W_Gettnames();
                        }
                  }

              protected:
                  virtual iter_type do_put(iter_type out, std::ios_base &, char_type, const tm *ptm, char format, char modifier = '\0') const
                  {
                        string_type _Fmt = TemplateTypeOfStr("!%x\0", char_type); // '!' for nonzero count, null for modifier
                        size_t _Count, _Num;
                        string_type _Str;

                        if (modifier == '\0')
                              _Fmt[2] = (char_type)format;
                        else
                        { // add both modifier and specifier
                              _Fmt[2] = (char_type)modifier;
                              _Fmt[3] = (char_type)format;
                        }

                        for (_Num = 16;; _Num *= 2)
                        { // convert into ever larger string buffer until success
                              _Str.append(_Num, TemplateTypeOfCh('\0', char_type));
                              if (0 < (_Count = _Wcsftime((wchar_t *)&*_Str.begin(), _Str.size(), (const wchar_t *)_Fmt.c_str(), ptm, _Tnames._Getptr())))
                              {
                                    break;
                              }
                        }

                        return copy(&_Str[1], &_Str[_Count], out);
                  }

                  virtual ~time_put(){};

              private:
                  std::_Locinfo::_Timevec _Tnames; // locale-specific stuff for _Wcsftime
            };

      }
}
