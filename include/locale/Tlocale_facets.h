﻿#pragma once
#include <locale>
#include <Tlocale_config.h>

namespace tlib
{
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
