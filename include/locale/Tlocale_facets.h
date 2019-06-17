#pragma once
#include <locale>
#include <tuple>
#include <Tlocale_config.h>
#include <type_traits>

#ifdef __linux__
#include <langinfo.h>
#endif

namespace tlib
{
      template <class CharT>
      class numpunct : public std::numpunct<CharT>
      {
            static_assert(true, "implementation of this class is missing");
      };

#ifdef _WIN32

      template <>
      class numpunct<char16_t> : public std::numpunct<char16_t>
      {
        public:
            using char_type = char16_t;
            using string_type = std::basic_string<char_type>;
            using base = std::numpunct<char_type>;

            template <class... Args>
            numpunct(Args&&... args) : base(std::forward<Args>(args)...)
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
                  const lconv* ptr_lconv = localeconv();
                  l_cfg_numbers.decimal_point = (char_type)*ptr_lconv->_W_decimal_point;
                  l_cfg_numbers.thousands_sep = (char_type)*ptr_lconv->_W_thousands_sep;
                  l_cfg_numbers.grouping = ptr_lconv->grouping;
            }
      };

       template <class CharT, bool _Intl = false>
      class moneypunct : public std::moneypunct<CharT, _Intl>
      {
        public:
            using char_type = CharT;
            using string_type = std::basic_string<char_type>;
            using base = std::moneypunct<char_type>;

            template <class... Args>
            moneypunct(Args&&... args) : base(std::forward<Args>(args)...)
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
                  const lconv* ptr_lconv = localeconv();
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
                        l_cfg_moneys.curr_symbol = (*ptr_lconv->_W_currency_symbol == 0x20bd) ? TemplateTypeOfStr("p.", char_type) : (*ptr_lconv->_W_currency_symbol == 0x20ac) ? TemplateTypeOfStr("euro", char_type) : (char_type*)ptr_lconv->_W_currency_symbol;
                        l_cfg_moneys.positive_sign = (char_type*)ptr_lconv->_W_positive_sign;
                        l_cfg_moneys.negative_sign = (char_type*)ptr_lconv->_W_negative_sign;
                        l_cfg_moneys.frac_digits = ptr_lconv->frac_digits;
                  }
            }
      };


      ////////////////////////////////////////////////////////
      template <class CharT, class OutputIt = std::ostreambuf_iterator<CharT>>
      class time_put : public std::time_put<CharT, OutputIt>
      {
        public:
            using char_type = CharT;
            using iter_type = std::ostreambuf_iterator<CharT>;
            using string_type = std::basic_string<char_type>;
            using base = std::time_put<CharT, OutputIt>;

            template <class LR, class... Args>
            time_put(LR loc, Args&&... args) : base(loc, std::forward<Args>(args)...)
            {
                  using L = std::remove_cv_t<std::remove_reference_t<LR>>;
                  if constexpr (std::is_same_v<L, std::_Locinfo>)
                  {
                        _Tnames = loc._W_Gettnames();
                  }
            }

        protected:
            virtual iter_type do_put(iter_type out, std::ios_base&, char_type, const tm* ptm, char format, char modifier = '\0') const
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
                        if (0 < (_Count = _Wcsftime((wchar_t*)&*_Str.begin(), _Str.size(), (const wchar_t*)_Fmt.c_str(), ptm, _Tnames._Getptr())))
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

#elif __linux__
 
      template <>
      class numpunct<char> : public std::numpunct<char>
      {
        public:
            using char_type = char;
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
                              unsigned char decimal_point_tmp = nl_langinfo_l(DECIMAL_POINT, __cloc)[0];
                              unsigned char thousands_sep_tmp = nl_langinfo_l(THOUSANDS_SEP, __cloc)[0];

                              if (thousands_sep_tmp != 0xE2)
                                    thousands_sep = thousands_sep_tmp;
                              else
                                    thousands_sep = ' ';

                              if (decimal_point_tmp != 0xE2)
                                    decimal_point = decimal_point_tmp;
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
            char_type decimal_point = '.';
            char_type thousands_sep = ',';

      };

      //---------------------------------------------------
      template <class CharT, bool _Intl = false>
      class moneypunct : public std::moneypunct<CharT, _Intl>
      {
            static_assert(true, "implementation of this class is missing");
      };

      template <>
      class moneypunct<char> : public std::moneypunct<char>
      {
        public:
            using char_type = char;
            using string_type = std::basic_string<char_type>;
            using base = std::moneypunct<char_type>;

            template <class LR, class... Args>
            moneypunct(LR loc, Args&&... args) : base(loc, std::forward<Args>(args)...)
            {
                  using L = std::remove_cv_t<std::remove_reference_t<LR>>;
                  if constexpr (std::is_same_v<L, std::__c_locale>)
                  {
                        __cloc = loc;

                        if (__cloc)
                        {
                              unsigned char decimal_point_tmp = nl_langinfo_l(__MON_DECIMAL_POINT, __cloc)[0];
                              unsigned char thousands_sep_tmp = nl_langinfo_l(__MON_THOUSANDS_SEP, __cloc)[0];

                              if (thousands_sep_tmp != 0xE2)
                                    thousands_sep = thousands_sep_tmp;
                              else
                                    thousands_sep = ' ';

                              if (decimal_point_tmp != 0xE2)
                                    decimal_point = decimal_point_tmp;
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

            virtual ~moneypunct(){};

        private:
            std::__c_locale __cloc;
            char_type decimal_point = '.';
            char_type thousands_sep = ',';
      };
#endif

      //////////////////////////////////////////////////////////
      template <class CharT>
      class numpunct_by_cfg : public std::numpunct<CharT>
      {
        public:
            using char_type = CharT;
            using string_type = std::basic_string<char_type>;
            using base = std::numpunct<char_type>;

            template <class... Args>
            numpunct_by_cfg(Args&&... args) : base(std::forward<Args>(args)...)
            {
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
                  return l_cfg_numbers.truename;
            }

            virtual string_type do_falsename() const
            {
                  return l_cfg_numbers.falsename;
            }

            ~numpunct_by_cfg(){};

        private:
            locale_cfg_numbers<char_type> l_cfg_numbers;
      };

      template <class CharT>
      class moneypunct_by_cfg : public std::moneypunct<CharT>
      {
        public:
            using char_type = CharT;
            using string_type = std::basic_string<char_type>;
            using base = std::moneypunct<char_type>;

            template <class... Args>
            moneypunct_by_cfg(Args&&... args) : base(std::forward<Args>(args)...)
            {
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
                  return l_cfg_moneys.pos_format;
            }

            virtual std::money_base::pattern do_neg_format() const
            {
                  return l_cfg_moneys.neg_format;
            }

            ~moneypunct_by_cfg(){};

        private:
            locale_cfg_moneys<char_type> l_cfg_moneys;
      };

}
