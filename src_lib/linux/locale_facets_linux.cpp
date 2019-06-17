
#include <linux/locale_glibcxx.h>
CTYPE_DEFINE_PURE(char16_t)


#include <linux/locale_facets_linux.h>
#include <locale>
#include <cstring>
#include <langinfo.h>
#include <Tstring.h>


#include <locale>
#include <ctime>
#include <cwchar>
#include <stdlib.h>
#include <langinfo.h>


//using namespace tlib;

//--------------------------
namespace std
{
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
                        _M_data->_M_atoms_out[__i] = static_cast<char16_t>(__num_base::_S_atoms_out[__i]);

                  for (size_t __j = 0; __j < __num_base::_S_iend; ++__j)
                        _M_data->_M_atoms_in[__j] = static_cast<char16_t>(__num_base::_S_atoms_in[__j]);
            }
            else
            {
                  // Named locale.
                  // NB: In the GNU model wchar_t is always 32 bit wide.
                  union {
                        char* __s;
                        wchar_t __w;
                  } __u;
                  __u.__s = nl_langinfo_l(_NL_NUMERIC_DECIMAL_POINT_WC, __cloc);
                  _M_data->_M_decimal_point = (char16_t)__u.__w;
                  __u.__s = nl_langinfo_l(_NL_NUMERIC_THOUSANDS_SEP_WC, __cloc);
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
                        string __src = nl_langinfo_l(GROUPING, __cloc);
                        if (__src.size())
                        {
                              __try
                              {
                                    char* __dst = new char[__src.size() + 1];
                                    __src.copy(__dst, __src.size());
                                    __dst[__src.size()] = '\0';
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
                        _M_data->_M_grouping_size = __src.size();
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

      //------------------------------------------------------

      template <>
      void __timepunct<char16_t>::
          _M_put(char16_t* __s, size_t __maxlen, const char16_t* __format, const tm* __tm) const throw()
      {
            wstring tmp(__maxlen, L'\0');
            const size_t __len = wcsftime_l(tmp.data(), __maxlen, tlib::u16str_wstr(__format).c_str(), __tm, _M_c_locale_timepunct);
            // Make sure __s is null terminated.
            if (__len == 0)
                  tmp[0] = L'\0';
            tlib::wstr_u16str(tmp).copy(__s, __len);
            __s[__len] = u'\0';
      }

#define CHAR16_LANGINFO(M, FMT)                                    \
      holder = tlib::u8str_u16str(nl_langinfo_l(FMT, (locale_t)__cloc)); \
      langstring = new char16_t[holder.size() + 1];                \
      holder.copy(langstring, holder.size());                      \
      langstring[holder.size()] = u'\0';                           \
      _M_data->M = langstring;

      template <>
      void __timepunct<char16_t>::_M_initialize_timepunct(__c_locale __cloc)
      {
            if (!_M_data)
                  _M_data = new __timepunct_cache<char16_t>;
            if (!__cloc)
            {
                  // "C" locale
                  _M_c_locale_timepunct = _S_get_c_locale();
                  _M_data->_M_date_format = u"%m/%d/%y";
                  _M_data->_M_date_era_format = u"%m/%d/%y";
                  _M_data->_M_time_format = u"%H:%M:%S";
                  _M_data->_M_time_era_format = u"%H:%M:%S";
                  _M_data->_M_date_time_format = u"";
                  _M_data->_M_date_time_era_format = u"";
                  _M_data->_M_am = u"AM";
                  _M_data->_M_pm = u"PM";
                  _M_data->_M_am_pm_format = u"";
                  // Day names, starting with "C"'s Sunday.
                  _M_data->_M_day1 = u"Sunday";
                  _M_data->_M_day2 = u"Monday";
                  _M_data->_M_day3 = u"Tuesday";
                  _M_data->_M_day4 = u"Wednesday";
                  _M_data->_M_day5 = u"Thursday";
                  _M_data->_M_day6 = u"Friday";
                  _M_data->_M_day7 = u"Saturday";
                  // Abbreviated day names, starting with "C"'s Sun.
                  _M_data->_M_aday1 = u"Sun";
                  _M_data->_M_aday2 = u"Mon";
                  _M_data->_M_aday3 = u"Tue";
                  _M_data->_M_aday4 = u"Wed";
                  _M_data->_M_aday5 = u"Thu";
                  _M_data->_M_aday6 = u"Fri";
                  _M_data->_M_aday7 = u"Sat";
                  // Month names, starting with "C"'s January.
                  _M_data->_M_month01 = u"January";
                  _M_data->_M_month02 = u"February";
                  _M_data->_M_month03 = u"March";
                  _M_data->_M_month04 = u"April";
                  _M_data->_M_month05 = u"May";
                  _M_data->_M_month06 = u"June";
                  _M_data->_M_month07 = u"July";
                  _M_data->_M_month08 = u"August";
                  _M_data->_M_month09 = u"September";
                  _M_data->_M_month10 = u"October";
                  _M_data->_M_month11 = u"November";
                  _M_data->_M_month12 = u"December";
                  // Abbreviated month names, starting with "C"'s Jan.
                  _M_data->_M_amonth01 = u"Jan";
                  _M_data->_M_amonth02 = u"Feb";
                  _M_data->_M_amonth03 = u"Mar";
                  _M_data->_M_amonth04 = u"Apr";
                  _M_data->_M_amonth05 = u"May";
                  _M_data->_M_amonth06 = u"Jun";
                  _M_data->_M_amonth07 = u"Jul";
                  _M_data->_M_amonth08 = u"Aug";
                  _M_data->_M_amonth09 = u"Sep";
                  _M_data->_M_amonth10 = u"Oct";
                  _M_data->_M_amonth11 = u"Nov";
                  _M_data->_M_amonth12 = u"Dec";
            }
            else
            {
                  char16_t* langstring = 0;
                  u16string holder;
                  _M_c_locale_timepunct = _S_clone_c_locale(__cloc);
                  CHAR16_LANGINFO(_M_date_format, D_FMT)
                  CHAR16_LANGINFO(_M_date_era_format, ERA_D_FMT)
                  CHAR16_LANGINFO(_M_time_format, T_FMT)
                  CHAR16_LANGINFO(_M_time_era_format, ERA_T_FMT)
                  CHAR16_LANGINFO(_M_date_time_format, D_T_FMT)
                  CHAR16_LANGINFO(_M_date_time_era_format, ERA_D_T_FMT)
                  CHAR16_LANGINFO(_M_am, AM_STR)
                  CHAR16_LANGINFO(_M_pm, PM_STR)
                  CHAR16_LANGINFO(_M_am_pm_format, T_FMT_AMPM)
                  // Day names, starting with "C"'s Sunday.
                  CHAR16_LANGINFO(_M_day1, DAY_1)
                  CHAR16_LANGINFO(_M_day2, DAY_2)
                  CHAR16_LANGINFO(_M_day3, DAY_3)
                  CHAR16_LANGINFO(_M_day4, DAY_4)
                  CHAR16_LANGINFO(_M_day5, DAY_5)
                  CHAR16_LANGINFO(_M_day6, DAY_6)
                  CHAR16_LANGINFO(_M_day7, DAY_7)
                  // Abbreviated day names, starting with "C"'s Sun.
                  CHAR16_LANGINFO(_M_aday1, ABDAY_1)
                  CHAR16_LANGINFO(_M_aday2, ABDAY_2)
                  CHAR16_LANGINFO(_M_aday3, ABDAY_3)
                  CHAR16_LANGINFO(_M_aday4, ABDAY_4)
                  CHAR16_LANGINFO(_M_aday5, ABDAY_5)
                  CHAR16_LANGINFO(_M_aday6, ABDAY_6)
                  CHAR16_LANGINFO(_M_aday7, ABDAY_7)
                  // Month names, starting with "C"'s January.
                  CHAR16_LANGINFO(_M_month01, MON_1)
                  CHAR16_LANGINFO(_M_month02, MON_2)
                  CHAR16_LANGINFO(_M_month03, MON_3)
                  CHAR16_LANGINFO(_M_month04, MON_4)
                  CHAR16_LANGINFO(_M_month05, MON_5)
                  CHAR16_LANGINFO(_M_month06, MON_6)
                  CHAR16_LANGINFO(_M_month07, MON_7)
                  CHAR16_LANGINFO(_M_month08, MON_8)
                  CHAR16_LANGINFO(_M_month09, MON_9)
                  CHAR16_LANGINFO(_M_month10, MON_10)
                  CHAR16_LANGINFO(_M_month11, MON_11)
                  CHAR16_LANGINFO(_M_month12, MON_12)
                  // Abbreviated month names, starting with "C"'s Jan.
                  CHAR16_LANGINFO(_M_amonth01, ABMON_1)
                  CHAR16_LANGINFO(_M_amonth02, ABMON_2)
                  CHAR16_LANGINFO(_M_amonth03, ABMON_3)
                  CHAR16_LANGINFO(_M_amonth04, ABMON_4)
                  CHAR16_LANGINFO(_M_amonth05, ABMON_5)
                  CHAR16_LANGINFO(_M_amonth06, ABMON_6)
                  CHAR16_LANGINFO(_M_amonth07, ABMON_7)
                  CHAR16_LANGINFO(_M_amonth08, ABMON_8)
                  CHAR16_LANGINFO(_M_amonth09, ABMON_9)
                  CHAR16_LANGINFO(_M_amonth10, ABMON_10)
                  CHAR16_LANGINFO(_M_amonth11, ABMON_11)
                  CHAR16_LANGINFO(_M_amonth12, ABMON_12)
            }
      }
      template <>
      __timepunct<char16_t>::~__timepunct()
      {
            if (_M_c_locale_timepunct != _S_get_c_locale())
            {
                  delete[] _M_data->_M_date_format;
                  delete[] _M_data->_M_date_era_format;
                  delete[] _M_data->_M_time_format;
                  delete[] _M_data->_M_time_era_format;
                  delete[] _M_data->_M_date_time_format;
                  delete[] _M_data->_M_date_time_era_format;
                  delete[] _M_data->_M_am;
                  delete[] _M_data->_M_pm;
                  delete[] _M_data->_M_am_pm_format;
                  delete[] _M_data->_M_day1;
                  delete[] _M_data->_M_day2;
                  delete[] _M_data->_M_day3;
                  delete[] _M_data->_M_day4;
                  delete[] _M_data->_M_day5;
                  delete[] _M_data->_M_day6;
                  delete[] _M_data->_M_day7;
                  delete[] _M_data->_M_aday1;
                  delete[] _M_data->_M_aday2;
                  delete[] _M_data->_M_aday3;
                  delete[] _M_data->_M_aday4;
                  delete[] _M_data->_M_aday5;
                  delete[] _M_data->_M_aday6;
                  delete[] _M_data->_M_aday7;
                  delete[] _M_data->_M_month01;
                  delete[] _M_data->_M_month02;
                  delete[] _M_data->_M_month03;
                  delete[] _M_data->_M_month04;
                  delete[] _M_data->_M_month05;
                  delete[] _M_data->_M_month06;
                  delete[] _M_data->_M_month07;
                  delete[] _M_data->_M_month08;
                  delete[] _M_data->_M_month09;
                  delete[] _M_data->_M_month10;
                  delete[] _M_data->_M_month11;
                  delete[] _M_data->_M_month12;
                  delete[] _M_data->_M_amonth01;
                  delete[] _M_data->_M_amonth02;
                  delete[] _M_data->_M_amonth03;
                  delete[] _M_data->_M_amonth04;
                  delete[] _M_data->_M_amonth05;
                  delete[] _M_data->_M_amonth06;
                  delete[] _M_data->_M_amonth07;
                  delete[] _M_data->_M_amonth08;
                  delete[] _M_data->_M_amonth09;
                  delete[] _M_data->_M_amonth10;
                  delete[] _M_data->_M_amonth11;
                  delete[] _M_data->_M_amonth12;
            }
            delete _M_data;
      }
      ///-------------------------------------------------------------------

      template <>
      void moneypunct<char16_t, true>::_M_initialize_moneypunct(__c_locale __cloc, const char*)
      {
            if (!_M_data)
                  _M_data = new __moneypunct_cache<char16_t, true>;
            if (!__cloc)
            {
                  // "C" locale
                  _M_data->_M_decimal_point = u'.';
                  _M_data->_M_thousands_sep = u',';
                  _M_data->_M_grouping = "";
                  _M_data->_M_grouping_size = 0;
                  _M_data->_M_use_grouping = false;
                  _M_data->_M_curr_symbol = u"";
                  _M_data->_M_curr_symbol_size = 0;
                  _M_data->_M_positive_sign = u"";
                  _M_data->_M_positive_sign_size = 0;
                  _M_data->_M_negative_sign = u"";
                  _M_data->_M_negative_sign_size = 0;
                  _M_data->_M_frac_digits = 0;
                  _M_data->_M_pos_format = money_base::_S_default_pattern;
                  _M_data->_M_neg_format = money_base::_S_default_pattern;
                  // Use ctype::widen code without the facet...
                  for (size_t __i = 0; __i < money_base::_S_end; ++__i)
                        _M_data->_M_atoms[__i] =
                            static_cast<char16_t>(money_base::_S_atoms[__i]);
            }
            else
            {
                  // Named locale.
                  __c_locale __old = uselocale(__cloc);

                  union {
                        char* __s;
                        char16_t __w;
                  } __u;
                  __u.__s = nl_langinfo_l(_NL_MONETARY_DECIMAL_POINT_WC, __cloc);
                  _M_data->_M_decimal_point = __u.__w;
                  __u.__s = nl_langinfo_l(_NL_MONETARY_THOUSANDS_SEP_WC, __cloc);
                  _M_data->_M_thousands_sep = __u.__w;
                  // Check for NULL, which implies no fractional digits.
                  if (_M_data->_M_decimal_point == u'\0')
                  {
                        // Like in "C" locale.
                        _M_data->_M_frac_digits = 0;
                        _M_data->_M_decimal_point = u'.';
                  }
                  else
                        _M_data->_M_frac_digits = *(nl_langinfo_l(__INT_FRAC_DIGITS, __cloc));

                  string __cgroup = nl_langinfo_l(__MON_GROUPING, __cloc);
                  string __cpossign = nl_langinfo_l(__POSITIVE_SIGN, __cloc);
                  string __cnegsign = nl_langinfo_l(__NEGATIVE_SIGN, __cloc);
                  string __ccurr = nl_langinfo_l(__INT_CURR_SYMBOL, __cloc);

                  char* __group = 0;
                  char16_t* __wcs_ps = 0;
                  char16_t* __wcs_ns = 0;

                  const char __nposn = *(nl_langinfo_l(__INT_N_SIGN_POSN, __cloc));
                  __try
                  {
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
                              if (__cgroup.size())
                              {
                                    __group = new char[__cgroup.size() + 1];
                                    __cgroup.copy(__group, __cgroup.size() + 1);
                                    _M_data->_M_grouping = __group;
                              }
                              else
                              {
                                    _M_data->_M_grouping = "";
                                    _M_data->_M_use_grouping = false;
                              }
                              _M_data->_M_grouping_size = __cgroup.size();
                        }

                        if (__cpossign.size())
                        {
                              u16string tmp = tlib::u8str_u16str(__cpossign);
                              __wcs_ps = new char16_t[tmp.size() + 1];
                              tmp.copy(__wcs_ps, tmp.size() + 1);
                              _M_data->_M_positive_sign = __wcs_ps;
                              _M_data->_M_positive_sign_size = tmp.size();
                        }
                        else
                        {
                              _M_data->_M_positive_sign = u"";
                              _M_data->_M_positive_sign_size = 0;
                        }

                        if (!__nposn)
                              _M_data->_M_negative_sign = u"()";
                        else if (__cnegsign.size())
                        {
                              u16string tmp = tlib::u8str_u16str(__cnegsign);
                              __wcs_ns = new char16_t[tmp.size() + 1];
                              tmp.copy(__wcs_ns, tmp.size() + 1);
                              _M_data->_M_negative_sign = __wcs_ns;
                              _M_data->_M_negative_sign_size = tmp.size();
                        }
                        else
                        {
                              _M_data->_M_negative_sign = u"";
                              _M_data->_M_negative_sign_size = 0;
                        }

                        // _Intl == true.
                        if (__ccurr.size())
                        {
                              u16string tmp = tlib::u8str_u16str(__ccurr);
                              char16_t* __wcs = new char16_t[tmp.size() + 1];
                              tmp.copy(__wcs, tmp.size() + 1);
                              _M_data->_M_curr_symbol = __wcs;
                              _M_data->_M_curr_symbol_size = tmp.size();
                        }
                        else
                        {
                              _M_data->_M_curr_symbol = u"";
                              _M_data->_M_curr_symbol_size = 0;
                        }
                  }
                  __catch(...)
                  {
                        delete _M_data;
                        _M_data = 0;
                        delete[] __group;
                        delete[] __wcs_ps;
                        delete[] __wcs_ns;
                        uselocale(__old);

                        __throw_exception_again;
                  }

                  char __pprecedes = *(nl_langinfo_l(__INT_P_CS_PRECEDES, __cloc));
                  char __pspace = *(nl_langinfo_l(__INT_P_SEP_BY_SPACE, __cloc));
                  char __pposn = *(nl_langinfo_l(__INT_P_SIGN_POSN, __cloc));
                  _M_data->_M_pos_format = _S_construct_pattern(__pprecedes, __pspace, __pposn);

                  char __nprecedes = *(nl_langinfo_l(__INT_N_CS_PRECEDES, __cloc));
                  char __nspace = *(nl_langinfo_l(__INT_N_SEP_BY_SPACE, __cloc));
                  _M_data->_M_neg_format = _S_construct_pattern(__nprecedes, __nspace, __nposn);

                  uselocale(__old);
            }
      }

      template <>
      void moneypunct<char16_t, false>::_M_initialize_moneypunct(__c_locale __cloc, const char*)
      {
            if (!_M_data)
                  _M_data = new __moneypunct_cache<char16_t, false>;
            if (!__cloc)
            {
                  // "C" locale
                  _M_data->_M_decimal_point = u'.';
                  _M_data->_M_thousands_sep = u',';
                  _M_data->_M_grouping = "";
                  _M_data->_M_grouping_size = 0;
                  _M_data->_M_use_grouping = false;
                  _M_data->_M_curr_symbol = u"";
                  _M_data->_M_curr_symbol_size = 0;
                  _M_data->_M_positive_sign = u"";
                  _M_data->_M_positive_sign_size = 0;
                  _M_data->_M_negative_sign = u"";
                  _M_data->_M_negative_sign_size = 0;
                  _M_data->_M_frac_digits = 0;
                  _M_data->_M_pos_format = money_base::_S_default_pattern;
                  _M_data->_M_neg_format = money_base::_S_default_pattern;
                  // Use ctype::widen code without the facet...
                  for (size_t __i = 0; __i < money_base::_S_end; ++__i)
                        _M_data->_M_atoms[__i] =
                            static_cast<char16_t>(money_base::_S_atoms[__i]);
            }
            else
            {
                  // Named locale.
                  __c_locale __old = uselocale(__cloc);

                  union {
                        char* __s;
                        char16_t __w;
                  } __u;
                  __u.__s = nl_langinfo_l(_NL_MONETARY_DECIMAL_POINT_WC, __cloc);
                  _M_data->_M_decimal_point = __u.__w;
                  __u.__s = nl_langinfo_l(_NL_MONETARY_THOUSANDS_SEP_WC, __cloc);
                  _M_data->_M_thousands_sep = __u.__w;
                  // Check for NULL, which implies no fractional digits.
                  if (_M_data->_M_decimal_point == u'\0')
                  {
                        // Like in "C" locale.
                        _M_data->_M_frac_digits = 0;
                        _M_data->_M_decimal_point = u'.';
                  }
                  else
                        _M_data->_M_frac_digits = *(nl_langinfo_l(__INT_FRAC_DIGITS, __cloc));

                  string __cgroup = nl_langinfo_l(__MON_GROUPING, __cloc);
                  string __cpossign = nl_langinfo_l(__POSITIVE_SIGN, __cloc);
                  string __cnegsign = nl_langinfo_l(__NEGATIVE_SIGN, __cloc);
                  string __ccurr = nl_langinfo_l(__CURRENCY_SYMBOL, __cloc);

                  char* __group = 0;
                  char16_t* __wcs_ps = 0;
                  char16_t* __wcs_ns = 0;

                  const char __nposn = *(nl_langinfo_l(__INT_N_SIGN_POSN, __cloc));
                  __try
                  {
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
                              if (__cgroup.size())
                              {
                                    __group = new char[__cgroup.size() + 1];
                                    __cgroup.copy(__group, __cgroup.size() + 1);
                                    _M_data->_M_grouping = __group;
                              }
                              else
                              {
                                    _M_data->_M_grouping = "";
                                    _M_data->_M_use_grouping = false;
                              }
                              _M_data->_M_grouping_size = __cgroup.size();
                        }

                        if (__cpossign.size())
                        {
                              u16string tmp = tlib::u8str_u16str(__cpossign);
                              __wcs_ps = new char16_t[tmp.size() + 1];
                              tmp.copy(__wcs_ps, tmp.size() + 1);
                              _M_data->_M_positive_sign = __wcs_ps;
                              _M_data->_M_positive_sign_size = tmp.size();
                        }
                        else
                        {
                              _M_data->_M_positive_sign = u"";
                              _M_data->_M_positive_sign_size = 0;
                        }

                        if (!__nposn)
                              _M_data->_M_negative_sign = u"()";
                        else if (__cnegsign.size())
                        {
                              u16string tmp = tlib::u8str_u16str(__cnegsign);
                              __wcs_ns = new char16_t[tmp.size() + 1];
                              tmp.copy(__wcs_ns, tmp.size() + 1);
                              _M_data->_M_negative_sign = __wcs_ns;
                              _M_data->_M_negative_sign_size = tmp.size();
                        }
                        else
                        {
                              _M_data->_M_negative_sign = u"";
                              _M_data->_M_negative_sign_size = 0;
                        }

                        // _Intl == true.
                        if (__ccurr.size())
                        {
                              u16string tmp = tlib::u8str_u16str(__ccurr);
                              char16_t* __wcs = new char16_t[tmp.size() + 1];
                              tmp.copy(__wcs, tmp.size() + 1);
                              _M_data->_M_curr_symbol = __wcs;
                              _M_data->_M_curr_symbol_size = tmp.size();
                        }
                        else
                        {
                              _M_data->_M_curr_symbol = u"";
                              _M_data->_M_curr_symbol_size = 0;
                        }
                  }
                  __catch(...)
                  {
                        delete _M_data;
                        _M_data = 0;
                        delete[] __group;
                        delete[] __wcs_ps;
                        delete[] __wcs_ns;
                        uselocale(__old);

                        __throw_exception_again;
                  }

                  char __pprecedes = *(nl_langinfo_l(__P_CS_PRECEDES, __cloc));
                  char __pspace = *(nl_langinfo_l(__P_SEP_BY_SPACE, __cloc));
                  char __pposn = *(nl_langinfo_l(__P_SIGN_POSN, __cloc));
                  _M_data->_M_pos_format = _S_construct_pattern(__pprecedes, __pspace,
                      __pposn);
                  char __nprecedes = *(nl_langinfo_l(__N_CS_PRECEDES, __cloc));
                  char __nspace = *(nl_langinfo_l(__N_SEP_BY_SPACE, __cloc));
                  _M_data->_M_neg_format = _S_construct_pattern(__nprecedes, __nspace,
                      __nposn);

                  uselocale(__old);
            }
      }
      template <>
      moneypunct<char16_t, true>::~moneypunct()
      {
            if (_M_data->_M_grouping_size)
                  delete[] _M_data->_M_grouping;
            if (_M_data->_M_positive_sign_size)
                  delete[] _M_data->_M_positive_sign;
            if (_M_data->_M_negative_sign_size && u16string(_M_data->_M_negative_sign) != u"()")
                  delete[] _M_data->_M_negative_sign;
            if (_M_data->_M_curr_symbol_size)
                  delete[] _M_data->_M_curr_symbol;
            delete _M_data;
      }
      template <>
      moneypunct<char16_t, false>::~moneypunct()
      {
            if (_M_data->_M_grouping_size)
                  delete[] _M_data->_M_grouping;
            if (_M_data->_M_positive_sign_size)
                  delete[] _M_data->_M_positive_sign;
            if (_M_data->_M_negative_sign_size && u16string(_M_data->_M_negative_sign) != u"()")
                  delete[] _M_data->_M_negative_sign;
            if (_M_data->_M_curr_symbol_size)
                  delete[] _M_data->_M_curr_symbol;
            delete _M_data;
      }
      ///////////////
}


