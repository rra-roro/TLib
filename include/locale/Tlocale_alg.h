#pragma once

#include <string_view>
#include <string>
#include <sstream>
#include <functional>
#include <algorithm>
#include <locale>
#include <locale/Tlocale_class.h>
#include <Ttype.h>


namespace tlib
{

      //////////////////////////////////////////////////////////////////////////////////
      // Функции проверки из STL принадлежности символов группе
      // Ф-ии одинаково хорошо работают и c Ansi и с Unicode

      // isalnum () возвращает, true, если его параметр - или буква алфавита или цифра.
      //            Если символ не является алфавитно-цифровым, возвращается false.
      // template<class _Elem> inline bool isalnum(_Elem _Ch, const locale& _Loc);
      using std::isalnum;

      // isalpha () возвращает, true, если ch - буква алфавита; иначе возвращается false.
      //            То, что составляет букву алфавита, может измениться в разных языках.
      //            Для английского языка это диапазон верхних и нижних символов от A до Z.
      // template<class _Elem> inline bool isalpha(_Elem _Ch, const locale& _Loc)
      using std::isalpha;

      // iscntrl () возвращает, true, если ch находится в диапазон между 0 и 0x1F или равен 0x7F (DEL);
      //            иначе возвращается false.
      // template<class _Elem> inline bool iscntrl(_Elem _Ch, const locale& _Loc)
      using std::iscntrl;

      // isdigit () возвращает, true, если ch - цифра, то есть, 0 до 9. Иначе возвращается false.
      // template<class _Elem> inline bool isdigit(_Elem _Ch, const locale& _Loc)
      using std::isdigit;

      // isgraph () возвращает, true, если ch - какой-нибудь печатаемый символ исключая пробел;
      //            иначе возвращается false. Печатаемые символы для английского языка обычно находятся в
      //            диапазоне от 0x21 до 0x7E.
      // template<class _Elem> inline bool isgraph(_Elem _Ch, const locale& _Loc)
      using std::isgraph;

      // islower () возвращает, true, если ch - строчная буква; иначе возвращается false.
      // template<class _Elem> inline bool islower(_Elem _Ch, const locale& _Loc)
      using std::islower;

      // isprint () возвращает, true, если ch - печатаемый символ, включая пробел(пространство);
      //            иначе возвращается false. Печатаемые символы часто находятся в диапазоне от 0x20 до 0x7E.
      // template<class _Elem> inline bool isprint(_Elem _Ch, const locale& _Loc)
      using std::isprint;

      // ispunct () возвращает, true, если ch - символ пунктуации; иначе возвращается false.
      //            Термин "пунктуация", как определено этой функцией, включает все литеры,
      //            которые не являются ни алфавитно-цифровыми символами, ни пробелом.
      // template<class _Elem> inline bool ispunct(_Elem _Ch, const locale& _Loc)
      using std::ispunct;

      // isspace () возвращает, true, если ch - или пробел, или горизонтальная табуляция, или
      //            вертикальная табуляция, прогон страницы, символ возврата каретки, или символ новой строки;
      //            иначе возвращается false.
      // template<class _Elem> inline bool isspace(_Elem _Ch, const locale& _Loc)
      using std::isspace;

      // isupper () возвращает, true, если ch - символ верхнего регистра; иначе возвращается false.
      // template<class _Elem> inline bool isupper(_Elem _Ch, const locale& _Loc)
      using std::isupper;

      // isxdigit () возвращает, true, если ch - шестнадцатеричная цифра; иначе возвращается false.
      //             Шестнадцатеричная цифра будет в одном из этих диапазонов: A-F, a-f, или 0-9.
      // template<class _Elem> inline bool isxdigit(_Elem _Ch, const locale& _Loc)
      using std::isxdigit;

      //////////////////////////////////////////////////////////////////////////////////
      // Функции проверки является ли символ слэшем.
      //		Аргумент имеет тип wchar_t.
      //		Если передать char, то ф-ия отработает, как следует. Так как символ слеша
      //		успешно конвертируется из типа  char в  wchar_t

      inline bool isslashr(wchar_t _Ch)
      { // проверяет является ли символ правым слешем "/"
            return (_Ch == L'/') ? true : false;
      }

      inline bool isslashl(wchar_t _Ch)
      { // проверяет является ли символ левым слешем "\"
            return (_Ch == L'\\') ? true : false;
      }

      inline bool isslash(wchar_t _Ch)
      { // проверяет является ли символ слешем
            return (isslashl(_Ch) || isslashr(_Ch)) ? true : false;
      }


      //////////////////////////////////////////////////////////////////////////////////
      // Функции из STL смены регистра символа

      // convert character to lower case, locale specific
      // template<class _Elem> inline _Elem tolower(_Elem _Ch, const locale& _Loc)
      using std::tolower;

      // convert character to upper case, locale specific
      // template<class _Elem> inline _Elem toupper(_Elem _Ch, const locale& _Loc);
      using std::toupper;

      //////////////////////////////////////////////////////////////////////////////////
      //    Функция для конвертации симолов строки  в нижний регистр
      //
      //    string get_lower_str(const string& str, const locale& Locale = tlib::locale::get_locale_program());
      //
      //    void to_lower(string& str, const locale& Locale = tlib::locale::get_locale_program());
      //

      template <class T,
                class _Elem = get_underlying_char_t<T>,
                class string_t = std::basic_string<_Elem>,
                class string_view_t = std::basic_string_view<_Elem>,
                class tl = tlib::locale
      >
      inline string_t get_lower_str(T&& str_arg, const tl& loc = tl::get_locale_program())
      {
            string_view_t str(std::forward<T>(str_arg));
            string_t str_lower_case(str.size(), 0);

            std::transform(str.begin(), str.end(), str_lower_case.begin(),
                                  [&](_Elem _Ch1)
                                  {
                                         return tolower<_Elem>(_Ch1, loc);
                                  });
            return str_lower_case;
      }

      template <typename _Elem,
                class tl = tlib::locale
      >
      inline void to_lower(std::basic_string<_Elem>& str, const tl& loc = tl::get_locale_program())
      {
            std::transform(str.begin(), str.end(), str.begin(),
                            [&](_Elem _ch1)
                            {
                                   return tolower<_Elem>(_ch1, loc);
                            });
      }

      template <typename _Elem,
                class tl = tlib::locale
      >
      inline void to_lower(_Elem* str, size_t size_str, const tl& loc = tl::get_locale_program())
      {
            std::transform(&str[0], &str[size_str - 1], &str[0],
                            [&](_Elem _ch1)
                            {
                                   return tolower<_Elem>(_ch1, loc);
                            });
      }

      //////////////////////////////////////////////////////////////////////////////////
      //    Функция для конвертации симолов строки в верхний регистр
      //
      //    string get_upper_str(const string& str, const locale& Locale = tlib::locale::get_locale_program());
      //
      //    void to_upper(string& str, const locale& Locale = tlib::locale::get_locale_program());
      //

      template <class T,
                class _Elem = get_underlying_char_t<T>,
                class string_t = std::basic_string<_Elem>,
                class string_view_t = std::basic_string_view<_Elem>,
                class tl = tlib::locale
      >
      inline string_t get_upper_str(T&& str_arg, const tl& loc = tl::get_locale_program())
      {
            string_view_t str(std::forward<T>(str_arg));
            string_t str_upper_case(str.size(), 0);

            std::transform(str.begin(), str.end(), str_upper_case.begin(),
                                  [&](_Elem _ch1)
                                  {
                                          return toupper<_Elem>(_ch1, loc);
                                  });
            return str_upper_case;
      }

      template <typename _Elem,
                class tl = tlib::locale
      >
      inline void to_upper(std::basic_string<_Elem>& str, const tl& loc = tl::get_locale_program())
      {
            std::transform(str.begin(), str.end(), str.begin(),
                                  [&](_Elem _ch1)
                                  {
                                           return toupper<_Elem>(_ch1, loc);
                                  });
      }

      template <typename _Elem,
                class tl = tlib::locale
      >
      inline void to_upper(_Elem* str, size_t size_str, const tl& loc = tl::get_locale_program())
      {
            std::transform(&str[0], &str[size_str - 1], &str[0],
                                  [&](_Elem _ch1)
                                  {
                                        return toupper<_Elem>(_ch1, loc);
                                  });
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    strcmp_i    -  шаблонная ф-ия которя позволяет сравнить две строки без
      //                 учета регистра символов. И в случае равенства возвращает true
      //
      //    bool strcmp_i( Str1, Str2,  const std::locale& Locale = tlib::locale::get_locale_program());
      //

      template <class T1, class _Elem1 = get_underlying_char_t<T1>,
                class T2, class _Elem2 = get_underlying_char_t<T2>,
                class _Elem = std::enable_if_t<std::is_same_v<_Elem1, _Elem2>, _Elem1>,
                class string_view_t = std::basic_string_view<_Elem>,
                class tl = tlib::locale
      >
      inline bool strcmp_i(T1&& str_arg1, T2&& str_arg2, const tl& loc = tl::get_locale_program())
      {
            string_view_t str1(std::forward<T1>(str_arg1));
            string_view_t str2(std::forward<T2>(str_arg2));

            // версия для двух STL строк
            if (str1.size() != str2.size())
                  return false;

            return std::equal(str1.begin(), str1.end(), str2.begin(),
                                  [&](_Elem _ch1, _Elem _ch2)
                                  {
                                        // сравниваваем два символа без учета их регистра
                                        return toupper<_Elem>(_ch1, loc) == toupper<_Elem>(_ch2, loc);
                                  });
      }


      //////////////////////////////////////////////////////////////////////////////////
      //
      //    find_substr_i  - Перегруженная шаблонная ф-ия которя позволяет искать в строке подстроку
      //                   без учета регистра символов. И в случае нахождения возвращает позицию
      //                   первого с начала строки вхождения подстроки в строку.
      //
      //    size_t find_substr_i( Str, searchStr,  const std::locale& Locale =tlib::locale::get_locale_program());
      //


      template <class T1, class _Elem1 = get_underlying_char_t<T1>,
                class T2, class _Elem2 = get_underlying_char_t<T2>,
                class _Elem = std::enable_if_t<std::is_same_v<_Elem1, _Elem2>, _Elem1>,
                class string_view_t = std::basic_string_view<_Elem>,
                class tl = tlib::locale
      >
      inline size_t find_substr_i(T1&& str_arg, T2&& search_str_arg, const tl& loc = tl::get_locale_program())
      {
            string_view_t str(std::forward<T1>(str_arg));
            string_view_t search_str(std::forward<T2>(search_str_arg));

            auto iter = std::search(str.begin(), str.end(), search_str.begin(), search_str.end(),
                                        [&](_Elem _ch1, _Elem _ch2)
                                        {
                                              // сравниваваем два символа без учета их регистра
                                              return toupper<_Elem>(_ch1, loc) == toupper<_Elem>(_ch2, loc);
                                        });
            if (iter == str.end())
                  return npos;
            else
                  return iter - str.begin();
      }

      template <class T1, class _Elem1 = get_underlying_char_t<T1>,
                class T2, class _Elem2 = get_underlying_char_t<T2>,
                class _Elem = std::enable_if_t<std::is_same_v<_Elem1, _Elem2>, _Elem1>,
                class string_view_t = std::basic_string_view<_Elem>,
                class tl = tlib::locale
      >
      inline size_t find_substr_i(T1&& str_arg, size_t start_position,
                                  T2&& search_str_arg,  const tl& loc = tl::get_locale_program())
      {
            string_view_t str(std::forward<T1>(str_arg));
            string_view_t search_str(std::forward<T2>(search_str_arg));

            if ((str.begin() + start_position) > str.end())
                  return npos;

            auto iter = std::search(str.begin() + start_position, str.end(), search_str.begin(), search_str.end(),
                                        [&](_Elem _Ch1, _Elem _Ch2)
                                        {
                                              // сравниваваем два символа без учета их регистра
                                              return toupper<_Elem>(_Ch1, loc) == toupper<_Elem>(_Ch2, loc);
                                        });
            if (iter == str.end())
                  return npos;
            else
                  return iter - str.begin();
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    find_graph_symbol  - шаблонная ф-ия которя позволяет искать позицую первого
      //                       печатного символа с начала строки. Тоесть любого не пробельного.
      //                       Не пробельный символ определяется при помощи ф-ии std::isgraph.
      //          size_t pos - параметр определяющий позицию в строке с которой будет осуществляться
      //                       поиск.
      //
      //    size_t find_graph_symbol( Str,  size_t pos=0, const std::locale& Locale = tlib::locale::get_locale_program())
      //

      template <class T,
                class _Elem = get_underlying_char_t<T>,
                class string_view_t = std::basic_string_view<_Elem>,
                class tl = tlib::locale
      >
      inline size_t find_graph_symbol(T&& str_arg, size_t pos = 0, const tl& loc = tl::get_locale_program())
      {
            string_view_t str(std::forward<T>(str_arg));
            
            auto start_iter = str.begin() + pos;
            auto iter = std::find_if(start_iter, str.end(),
                                          [&](_Elem _ch1)
                                          {
                                                // Это печатный символ?
                                                return isgraph<_Elem>(_ch1, loc);
                                          });
            if (iter == str.end())
                  return npos;
            else
                  return iter - str.begin();
      }      

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    rfind_graph_symbol - Перегруженная шаблонная ф-ия которя позволяет искать позицую первого
      //                       печатного символа с конца строки. Тоесть любого не пробельного.
      //                       Не пробельный символ определяется при помощи ф-ии std::isgraph.
      //          size_t pos - параметр определяющий позицию в строке с которой будет осуществляться
      //                       поиск.
      //
      //    size_t rfind_graph_symbol( Str,  size_t pos = npos, const std::locale& Locale = tlib::locale::get_locale_program())
      //

      template <class T,
                class _Elem = get_underlying_char_t<T>,
                class string_view_t = std::basic_string_view<_Elem>,
                class tl = tlib::locale
      >
      inline size_t rfind_graph_symbol(T&& str_arg, size_t pos=npos, const tl& loc = tl::get_locale_program())
      {
            string_view_t str(std::forward<T>(str_arg));
            if (pos == npos) pos = str.size();

            auto start = str.rbegin();
            advance(start, str.size() - pos); // смещаем итератор start в позицию pos, где pos задается от начала строки

            auto iter = std::find_if(start, str.rend(),
                                          [&](_Elem _ch1)
                                          {
                                                // Это печатный символ?
                                                return isgraph<_Elem>(_ch1, loc);
                                          });
            if (iter == str.rend())
                  return npos;
            else
                  return str.size() - distance(str.rbegin(), iter) - 1;
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    remove_first_spaces - шаблонная ф-ия которя удаляет лидирующие пробелы из строки.
      //
      //    void remove_first_spaces( Str, const std::locale& Locale = tlib::locale::get_locale_program())
      //
      template <class T,
                class _Elem = get_underlying_char_t<T>,
                class string_t = std::basic_string<_Elem>,
                class string_view_t = std::basic_string_view<_Elem>,
                class tl = tlib::locale
      >
      inline string_t remove_first_spaces(T&& str_arg, const tl& loc = tl::get_locale_program())
      {
            string_view_t str(std::forward<T>(str_arg));

            if (!str.empty())
            {
                  string_view_t new_view_str = str.substr(find_graph_symbol(str, 0, loc));
                  return { new_view_str.data(), new_view_str.size() };
            }
            return { str.data(), str.size() };
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    remove_trailing_spaces - шаблонная ф-ия которя удаляет пробелы из конца строки.
      //
      //    void remove_trailing_spaces( Str, const std::locale& Locale = tlib::locale::get_locale_program())
      //

      template <class T,
                class _Elem = get_underlying_char_t<T>,
                class string_t = std::basic_string<_Elem>,
                class string_view_t = std::basic_string_view<_Elem>,
                class tl = tlib::locale
      >
      inline string_t remove_trailing_spaces(T&& str_arg, const tl& loc = tl::get_locale_program())
      {
            string_view_t str(std::forward<T>(str_arg));

            if (!str.empty())
            {
                  string_view_t new_view_str = str.substr(0, rfind_graph_symbol(str, str.size(), loc) + 1);
                  return { new_view_str.data(), new_view_str.size() };
            }
            return { str.data(), str.size() };
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    remove_space - шаблонная ф-ия которя удаляет пробелы из конца и начала строки.
      //
      //    void remove_space( Str, const std::locale& Locale = tlib::locale::get_locale_program())
      //

      template <class T,
                class _Elem = get_underlying_char_t<T>,
                class string_t = std::basic_string<_Elem>,
                class string_view_t = std::basic_string_view<_Elem>,
                class tl = tlib::locale
      >
      inline string_t remove_space(T&& str_arg, const tl& loc = tl::get_locale_program())
      {
            string_view_t str(std::forward<T>(str_arg));

            return remove_trailing_spaces(remove_first_spaces(str, loc), loc);
      }

}
