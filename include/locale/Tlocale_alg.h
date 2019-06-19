#pragma once

#include <sstream>
#include <functional>
#include <algorithm>
#include <locale>
#include <locale/Tlocale_class.h>


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

      template <class T, class _Elem = get_underlying_char_t<T>>
      inline std::basic_string<_Elem> get_lower_str(T&& str_arg,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            std::basic_string_view<_Elem> Str(std::forward<T>(str_arg));
            std::basic_string<_Elem> StrLowerCase(Str.size(), 0);

            std::transform(Str.begin(), Str.end(), StrLowerCase.begin(),
                [&](_Elem _Ch1) {
                      return tolower<_Elem>(_Ch1, Locale);
                });

            return StrLowerCase;
      }

      template <typename _Elem>
      inline void to_lower(std::basic_string<_Elem>& Str,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            std::transform(Str.begin(), Str.end(), Str.begin(),
                [&](_Elem _Ch1) {
                      return tolower<_Elem>(_Ch1, Locale);
                });
      }

      template <typename _Elem>
      inline void to_lower(_Elem* Str,
          size_t SizeStr,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            std::transform(&Str[0], &Str[SizeStr - 1], &Str[0],
                [&](_Elem _Ch1) {
                      return tolower<_Elem>(_Ch1, Locale);
                });
      }

      //////////////////////////////////////////////////////////////////////////////////
      //    Функция для конвертации симолов строки в верхний регистр
      //
      //    string get_upper_str(const string& str, const locale& Locale = tlib::locale::get_locale_program());
      //
      //    void to_upper(string& str, const locale& Locale = tlib::locale::get_locale_program());
      //

      template <class T, class _Elem = get_underlying_char_t<T>>
      inline std::basic_string<_Elem> get_upper_str(T&& str_arg,
            const std::locale& Locale = tlib::locale::get_locale_program())
      {
            std::basic_string_view<_Elem> Str(std::forward<T>(str_arg));
            std::basic_string<_Elem> StrUpperCase(Str.size(), 0);

            std::transform(Str.begin(), Str.end(), StrUpperCase.begin(),
                [&](_Elem _Ch1) {
                      return toupper<_Elem>(_Ch1, Locale);
                });

            return StrUpperCase;
      }

      template <typename _Elem>
      inline void to_upper(std::basic_string<_Elem>& Str,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            std::transform(Str.begin(), Str.end(), Str.begin(),
                [&](_Elem _Ch1) {
                      return toupper<_Elem>(_Ch1, Locale);
                });
      }

      template <typename _Elem>
      inline void to_upper(_Elem* Str,
          size_t SizeStr,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            std::transform(&Str[0], &Str[SizeStr - 1], &Str[0],
                [&](_Elem _Ch1) {
                      return toupper<_Elem>(_Ch1, Locale);
                });
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    strcmp_i    - Перегруженная шаблонная ф-ия которя позволяет сравнить две строки без
      //                 учета регистра символов. И в случае равенства возвращает true
      //
      //    bool strcmp_i( Str1, Str2,  const std::locale& Locale = tlib::locale::get_locale_program());
      //

      template <typename _Elem>
      inline bool strcmp_i(std::basic_string_view<_Elem> Str1,
          std::basic_string_view<_Elem> Str2,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            // версия для двух STL строк
            if (Str1.size() != Str2.size())
                  return false;

            return std::equal(Str1.begin(), Str1.end(), Str2.begin(),
                [&](_Elem _Ch1, _Elem _Ch2) {
                      // сравниваваем два символа без учета их регистра
                      return toupper<_Elem>(_Ch1, Locale) == toupper<_Elem>(_Ch2, Locale);
                });
      }

      template <class T1, class _Elem1 = get_underlying_char_t<T1>,
          class T2, class _Elem2 = get_underlying_char_t<T2>>
      inline bool strcmp_i(T1 Str1, T2 Str2,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            return strcmp_i(std::basic_string_view<_Elem1>(Str1),
                std::basic_string_view<_Elem2>(Str2), Locale);
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    find_substr_i  - Перегруженная шаблонная ф-ия которя позволяет искать в строке подстроку
      //                   без учета регистра символов. И в случае нахождения возвращает позицию
      //                   первого с начала строки вхождения подстроки в строку.
      //
      //    size_t find_substr_i( Str, searchStr,  const std::locale& Locale =tlib::locale::get_locale_program());
      //

      template <typename _Elem>
      inline size_t find_substr_i(std::basic_string_view<_Elem> Str,
          std::basic_string_view<_Elem> searchStr,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {

            auto iter = std::search(Str.begin(), Str.end(), searchStr.begin(), searchStr.end(),
                [&](_Elem _Ch1, _Elem _Ch2) {
                      // сравниваваем два символа без учета их регистра
                      return toupper<_Elem>(_Ch1, Locale) == toupper<_Elem>(_Ch2, Locale);
                });

            if (iter == Str.end())
                  return npos;
            else
                  return iter - Str.begin();
      }

      template <class T1, class _Elem1 = get_underlying_char_t<T1>,
          class T2, class _Elem2 = get_underlying_char_t<T2>>
      inline size_t find_substr_i(T1 Str, T2 searchSymbolStr,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            return find_substr_i(std::basic_string_view<_Elem1>(Str),
                std::basic_string_view<_Elem2>(searchSymbolStr), Locale);
      }

      template <typename _Elem>
      inline size_t find_substr_i(std::basic_string_view<_Elem> Str,
          size_t StartPosition,
          std::basic_string_view<_Elem> searchStr,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {

            if ((Str.begin() + StartPosition) > Str.end())
                  return npos;

            auto iter = std::search(Str.begin() + StartPosition, Str.end(), searchStr.begin(), searchStr.end(),
                [&](_Elem _Ch1, _Elem _Ch2) {
                      // сравниваваем два символа без учета их регистра
                      return toupper<_Elem>(_Ch1, Locale) == toupper<_Elem>(_Ch2, Locale);
                });

            if (iter == Str.end())
                  return npos;
            else
                  return iter - Str.begin();
      }

      template <class T1, class _Elem1 = get_underlying_char_t<T1>,
          class T2, class _Elem2 = get_underlying_char_t<T2>>
      inline size_t find_substr_i(T1 Str, size_t StartPosition, T2 searchSymbolStr,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            return find_substr_i(std::basic_string_view<_Elem1>(Str),
                StartPosition,
                std::basic_string_view<_Elem2>(searchSymbolStr), Locale);
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    find_print_symbol  - Перегруженная шаблонная ф-ия которя позволяет искать позицую первого
      //                       печатного символа с начала строки. Тоесть любого не пробельного.
      //                       Не пробельный символ определяется при помощи ф-ии std::isgraph.
      //          size_t pos - параметр определяющий позицию в строке с которой будет осуществляться
      //                       поиск.
      //
      //    size_t find_print_symbol( Str,  const std::locale& Locale = tlib::locale::get_locale_program())
      //    size_t find_print_symbol( Str,  size_t pos, const std::locale& Locale = tlib::locale::get_locale_program())
      //

      template <typename _Elem>
      inline size_t find_print_symbol(std::basic_string_view<_Elem> Str,
          size_t pos,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            auto start_iter = Str.begin() + pos;
            auto iter = std::find_if(start_iter, Str.end(), [&](_Elem _Ch1) {
                  // Это печатный символ?
                  return isgraph<_Elem>(_Ch1, Locale);
            });
            if (iter == Str.end())
                  return npos;
            else
                  return iter - Str.begin();
      }

      template <class T, class _Elem = get_underlying_char_t<T>>
      inline size_t find_print_symbol(T Str, size_t pos,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            return find_print_symbol(std::basic_string_view<_Elem>(Str), pos, Locale);
      }

      template <class T, class _Elem = get_underlying_char_t<T>>
      inline size_t find_print_symbol(T Str,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            return find_print_symbol(std::basic_string_view<_Elem>(Str), 0, Locale);
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    rfind_print_symbol - Перегруженная шаблонная ф-ия которя позволяет искать позицую первого
      //                       печатного символа с конца строки. Тоесть любого не пробельного.
      //                       Не пробельный символ определяется при помощи ф-ии std::isgraph.
      //          size_t pos - параметр определяющий позицию в строке с которой будет осуществляться
      //                       поиск.
      //
      //    size_t rfind_print_symbol( Str,  const std::locale& Locale = tlib::locale::get_locale_program())
      //    size_t rfind_print_symbol( Str,  size_t pos, const std::locale& Locale = tlib::locale::get_locale_program())
      //

      template <typename _Elem>
      inline size_t rfind_print_symbol(std::basic_string_view<_Elem> Str,
          size_t pos,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            auto start = Str.rbegin();
            advance(start, Str.size() - pos); // смещаем итератор start в позицию pos, где pos задается от начала строки

            auto iter = std::find_if(start, Str.rend(), [&](_Elem _Ch1) {
                  // Это печатный символ?
                  return isgraph<_Elem>(_Ch1, Locale);
            });

            if (iter == Str.rend())
                  return npos;
            else
                  return Str.size() - distance(Str.rbegin(), iter) - 1;
      }

      template <class T, class _Elem = get_underlying_char_t<T>>
      inline size_t rfind_print_symbol(T Str,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            std::basic_string_view<_Elem> StrInternal(Str);
            return rfind_print_symbol(StrInternal, StrInternal.size(), Locale);
      }

      template <class T, class _Elem = get_underlying_char_t<T>>
      inline size_t rfind_print_symbol(T Str, size_t pos,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            return rfind_print_symbol(std::basic_string_view<_Elem>(Str), pos, Locale);
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    remove_first_spaces - шаблонная ф-ия которя удаляет лидирующие пробелы из строки.
      //
      //    void remove_first_spaces( Str, const std::locale& Locale = tlib::locale::get_locale_program())
      //
      template <typename _Elem>
      inline std::basic_string<_Elem> remove_first_spaces(std::basic_string_view<_Elem> Str,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            if (!Str.empty())
            {
                  std::basic_string_view<_Elem> new_view_str = Str.substr(find_print_symbol(Str, 0, Locale));
                  return { new_view_str.data(), new_view_str.size() };
            }
            return { Str.data(), Str.size() };
      }

      template <class T, class _Elem = get_underlying_char_t<T>>
      inline std::basic_string<_Elem> remove_first_spaces(T Str,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            return remove_first_spaces(std::basic_string_view<_Elem>(Str), Locale);
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    remove_trailing_spaces - шаблонная ф-ия которя удаляет пробелы из конца строки.
      //
      //    void remove_trailing_spaces( Str, const std::locale& Locale = tlib::locale::get_locale_program())
      //

      template <typename _Elem>
      inline std::basic_string<_Elem> remove_trailing_spaces(std::basic_string_view<_Elem> Str,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            if (!Str.empty())
            {
                  std::basic_string_view<_Elem> new_view_str = Str.substr(0, rfind_print_symbol(Str, Str.size(), Locale) + 1);
                  return { new_view_str.data(), new_view_str.size() };
            }
            return { Str.data(), Str.size() };
      }

      template <class T, class _Elem = get_underlying_char_t<T>>
      inline std::basic_string<_Elem> remove_trailing_spaces(T Str,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            return remove_trailing_spaces(std::basic_string_view<_Elem>(Str), Locale);
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    remove_space - шаблонная ф-ия которя удаляет пробелы из конца и начала строки.
      //
      //    void remove_space( Str, const std::locale& Locale = tlib::locale::get_locale_program())
      //

      template <typename _Elem>
      inline std::basic_string<_Elem> remove_space(std::basic_string_view<_Elem> Str,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            return remove_trailing_spaces(remove_first_spaces(Str, Locale), Locale);
      }

      template <class T, class _Elem = get_underlying_char_t<T>>
      inline std::basic_string<_Elem> remove_space(T Str,
          const std::locale& Locale = tlib::locale::get_locale_program())
      {
            return remove_space(std::basic_string_view<_Elem>(Str), Locale);
      }

}
