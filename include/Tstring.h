/*
*   Файл Tstring.h - содержит макроссы и классы необходимые для реализации строк
*                    в независемости от типа кодировки Ansi или Unicode
*
*   Версия файла 1.0
*   Версия файла 2.0 - Исправил вывод hex значений
*   Версия файла 3.0 - Добавил в область видимости Tstring константу npos
*                      А также определил tostream и tistream
*   Версия файла 4.0 - Добавил поддержку платформы WINCE
*   Версия файла 5.0 - Перекрыл все конструкторы базового класса
*   Версия файла 6.0 - Отказался от промежуточного класса _string/_wstring, 
*                      исправил вывод для cout в зависемости от заданной локали,
*                      задал функции для конвертирования string и wstring друг в друга,
*                      все буфферы реализованы через вектора.
*   Версия файла 7.0 - Ввел новы ф-ии конвертации строк из tstring в string и wstring и обратно.
*                      Локали стандартных потоков ввода вывода теперь работают только в консольных _CONSOLE
*                      приложениях, иначи могут вызывать сбой. В силу отсутствия таких потоков в GUI прогах.
*                      Изменил способ конвертаци символов из OEM в ANSI, отказавшись от OemToCharA. 
*                      Таким образом, избавился от зависимости с user32.dll
*
*   Версия файла 8.0 - Добавил макросы TemplateTypeOfStr(str,type) и TemplateTypeOfCh(str,type), 
*                      которые позволяют задавать в явном виде тип char и wchar_t строки и символа в Шаблонах
*
*/

#ifndef TSTRING
#define TSTRING

#ifdef _WIN32
#include <tchar.h>
#elif __linux__
#include <linux/tchar.h>
#endif

#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#endif

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4996) // was declared deprecated CodeCvt
#endif

#include <string>
#include <string_view>
#include <locale>
#include <Tcodecvt.h>
#include <Ttype.h>
#include <char8_t.h>


#ifndef __cpp_lib_char8_t

namespace std
{
      using u8string = basic_string<char8_t>;
      using u8string_view = basic_string_view<char8_t>;
}

#endif

namespace tlib
{
      extern const size_t npos;
      using tstring = std::basic_string<TCHAR>;
      using tstring_view = std::basic_string_view<TCHAR>;

      //=======================================================================
      //  Вводим функции, которые позволяют осуществлять преобразования между различными типами строк.
      //  Например, преоразовать string и wstring друг в друга

      //   string <--> wstring

      inline std::wstring cstr_wstr(std::string_view str)
      {
            const char *_first_symbol = str.data();
            return strconvert_w_codepage.from_bytes(_first_symbol, _first_symbol + str.size());
      }

      inline std::string wstr_cstr(std::wstring_view wstr)
      {
            const wchar_t *_first_symbol = wstr.data();
            return strconvert_w_codepage.to_bytes(_first_symbol, _first_symbol + wstr.size());
      }

      inline std::wstring cstr_wstr(std::string_view str, std::string_view loc_name)
      {
            std::wstring_convert<codecvt_w_o> strconvert_w_codepage(new codecvt_w_o(loc_name.data()));

            const char *_first_symbol = str.data();
            return strconvert_w_codepage.from_bytes(_first_symbol, _first_symbol + str.size());
      }

      inline std::string wstr_cstr(std::wstring_view wstr, std::string_view loc_name)
      {
            std::wstring_convert<codecvt_w_o> strconvert_w_codepage(new codecvt_w_o(loc_name.data()));

            const wchar_t *_first_symbol = wstr.data();
            return strconvert_w_codepage.to_bytes(_first_symbol, _first_symbol + wstr.size());
      }

      //   string <--> u16string

      inline std::u16string cstr_u16str(std::string_view str)
      {
            const char *_first_symbol = str.data();
            return strconvert_u16_codepage.from_bytes(_first_symbol, _first_symbol + str.size());
      }

      inline std::string u16str_cstr(std::u16string_view ustr)
      {
            const char16_t *_first_symbol = ustr.data();
            return strconvert_u16_codepage.to_bytes(_first_symbol, _first_symbol + ustr.size());
      }

      inline std::u16string cstr_u16str(std::string_view str, std::string_view loc_name)
      {
            std::wstring_convert<codecvt_u16_o, char16_t> strconvert_u16_codepage(new codecvt_u16_o(loc_name.data()));

            const char *_first_symbol = str.data();
            return strconvert_u16_codepage.from_bytes(_first_symbol, _first_symbol + str.size());
      }

      inline std::string u16str_cstr(std::u16string_view ustr, std::string_view loc_name)
      {
            std::wstring_convert<codecvt_u16_o, char16_t> strconvert_u16_codepage(new codecvt_u16_o(loc_name.data()));

            const char16_t *_first_symbol = ustr.data();
            return strconvert_u16_codepage.to_bytes(_first_symbol, _first_symbol + ustr.size());
      }

      //   u8string <--> u16string

      inline std::u16string u8str_u16str(std::u8string_view u8str)
      {
            const char *_first_symbol = u8str.data();
            return strconvert_u16_u8.from_bytes(_first_symbol, _first_symbol + u8str.size());
      }

      inline std::u8string u16str_u8str(std::u16string_view u16str)
      {
            const char16_t *_first_symbol = u16str.data();
            return strconvert_u16_u8.to_bytes(_first_symbol, _first_symbol + u16str.size());
      }

      //   u8string <--> wstring

      inline std::wstring u8str_wstr(std::u8string_view u8str)
      {
            const char *_first_symbol = u8str.data();
            return strconvert_w_u8.from_bytes(_first_symbol, _first_symbol + u8str.size());
      }

      inline std::u8string wstr_u8str(std::wstring_view wstr)
      {
            const wchar_t *_first_symbol = wstr.data();
            return strconvert_w_u8.to_bytes(_first_symbol, _first_symbol + wstr.size());
      }

      //   u16string <--> wstring   
      inline std::wstring u16str_wstr(std::u16string_view u16str)
      {
#ifdef _WIN32
            return { u16str.begin(), u16str.end() };
#elif __linux__
            std::u8string tmp = u16str_u8str(u16str);
            return u8str_wstr(tmp);
#endif
      }

      inline std::u16string wstr_u16str(std::wstring_view wstr)
      {
#ifdef _WIN32
            return { wstr.begin(), wstr.end() };
#elif __linux__
            std::u8string tmp = wstr_u8str(wstr);
            return u8str_u16str(tmp);
#endif
      }

#ifdef _WIN32
      #ifdef _UNICODE
            #define str2tstr(str)   tlib::cstr_wstr(str)
            #define wstr2tstr(str)  str
            #define ustr2tstr(str)  tlib::u16str_wstr(str)
            #define u8str2tstr(str) tlib::u8str_wstr(str) 
            #define tstr2str(str)   tlib::wstr_cstr(str)
            #define tstr2wstr(str)  str
            #define tstr2ustr(str)  tlib::wstr_u16str(str)
            #define tstr2u8str(str) tlib::wstr_u8str(str) 
      #else
            #define str2tstr(str)   str
            #define wstr2tstr(str)  tlib::wstr_cstr(str)
            #define ustr2tstr(str)  tlib::wstr_cstr(u16str_wstr(str))
            #define u8str2tstr(str) tlib::wstr_cstr(tlib::u8str_wstr(str))
            #define tstr2str(str)   str
            #define tstr2wstr(str)  tlib::cstr_wstr(str)
            #define tstr2ustr(str)  tlib::wstr_u16str(cstr_wstr(str))
            #define tstr2u8str(str) tlib::wstr_u8str(cstr_wstr(str)) 
      #endif
#elif __linux__
      #define str2tstr(str)   tlib::u8str_u16str(str)
      #define wstr2tstr(str)  tlib::wstr_u16str(str)
      #define ustr2tstr(str)  str
      #define u8str2tstr(str) tlib::u8str_u16str(str) 
      #define tstr2str(str)   tlib::u16str_u8str(str)
      #define tstr2wstr(str)  tlib::u16str_wstr(str)
      #define tstr2ustr(str)  str
      #define tstr2u8str(str) tlib::u16str_u8str(str)
#endif
      //=======================================================================
      //  Вводим макросы, которые позволяют задавать в явном виде тип char и wchar_t строки и символа
      //  Предназначен строго для использования в шаблонах, type - это или char или wchar_t
      //
#define TemplateTypeOfStr(str, type) ((is_wchar_v<type>) ? (type *)L##str : (is_char16_v<type>) ? (type *)u##str : (type *)str)
#define TemplateTypeOfCh(str, type) ((is_wchar_v<type>) ? (type)L##str : (is_char16_v<type>) ? (type)u##str : (type)str)

      template <class T, class _Elem = get_underlying_char_t<T>>
      inline tstring templateStr_tstr(T&& str)
      {
            if constexpr (is_wchar_v<_Elem>)
            {
                  return wstr2tstr(std::forward<T>(str));
            }
            else if constexpr (is_char16_v<_Elem>)
            {
                  return ustr2tstr(std::forward<T>(str));
            }
            else
            {
                  return str2tstr(std::forward<T>(str));
            }
      }

      template <class R, class T,
                typename = std::enable_if_t<std::is_same_v<get_underlying_char_t<T>, TCHAR>>,
                class string_r = std::basic_string<R>
      >
      inline string_r tstr_templateStr(T &&str)
      {
            if constexpr (is_wchar_v<R>)
            {
                  return tstr2wstr(std::forward<T>(str));
            }
            else if constexpr (is_char16_v<R>)
            {
                  return tstr2ustr(std::forward<T>(str));
            }
            else
            {
                  return tstr2str(std::forward<T>(str));
            }
      }

      template <class T, class _Elem = get_underlying_char_t<T>>
      inline std::string templateStr_cstr(T&& str)
      {
            if constexpr (is_wchar_v<_Elem>)
            {
#ifdef _WIN32
                  return wstr_cstr(std::forward<T>(str));
#else
                  return wstr_u8str(std::forward<T>(str));
#endif
            }
            else if constexpr (is_char16_v<_Elem>)
            {
#ifdef _WIN32
                  return wstr_cstr(u16str_wstr(std::forward<T>(str)));
#else
                  return u16str_u8str(std::forward<T>(str));
#endif
            }
            else
            {
                  return std::forward<T>(str);
            }
      }

      template <class R, class T,                               
                typename = std::enable_if_t<std::is_same_v<get_underlying_char_t<T>, char>>,
                class string_r = std::basic_string<R>       
      >
      inline string_r cstr_templateStr(T&& str)
      {
            if constexpr (is_wchar_v<R>)
            {
#ifdef _WIN32
                  return cstr_wstr(std::forward<T>(str));                  
#else
                  return u8str_wstr(std::forward<T>(str));
#endif
            }
            else if constexpr (is_char16_v<R>)
            {
#ifdef _WIN32
                  return wstr_u16str(cstr_wstr(std::forward<T>(str)));                  
#else
                  return u8str_u16str(std::forward<T>(str));
#endif
            }
            else
            {
                  return std::forward<T>(str);
            }
      }

      template <class T, class _Elem = get_underlying_char_t<T>>
      inline std::wstring templateStr_wstr(T &&str)
      {
            if constexpr (is_wchar_v<_Elem>)
            {
                  return std::forward<T>(str);
            }
            else if constexpr (is_char16_v<_Elem>)
            {
                  return u16str_wstr(std::forward<T>(str));
            }
            else
            {
                  return cstr_wstr(std::forward<T>(str));
            }
      }

      template <class R, class T,
                typename = std::enable_if_t<std::is_same_v<get_underlying_char_t<T>, wchar_t>>,
                class string_r = std::basic_string<R>>
      inline string_r wstr_templateStr(T &&str)
      {
            if constexpr (is_wchar_v<R>)
            {
                  return std::forward<T>(str);
            }
            else if constexpr (is_char16_v<R>)
            {
                  return wstr_u16str(std::forward<T>(str));
            }
            else
            {
                  return wstr_cstr(std::forward<T>(str));
            }
      }

}


#if !(defined(__linux__) || defined(_LIB) || defined(UNDER_CE) || defined(WINCE))

#define _LIB_NAME "tlib"

#include <win/select_lib.h>

#undef _LIB_NAME

#endif // !(defined(__linux__) || defined(_LIB) || defined(UNDER_CE) || defined(WINCE))

#ifdef _WIN32
#pragma warning(pop) // warning(disable: 4996)
#endif

#endif //TSTRING
