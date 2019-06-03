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

      //   u16string <--> wstring   ???? ToDo: need fix for Linux

      inline std::wstring u16str_wstr(std::u16string_view u16str)
      {
            return { u16str.begin(), u16str.end() };
      }

      inline std::u16string wstr_u16str(std::wstring_view wstr)
      {
            return { wstr.begin(), wstr.end() };
      }

#ifdef _WIN32
      #ifdef _UNICODE
            #define str2tstr(str)  tlib::cstr_wstr(str)
            #define wstr2tstr(str) str
            #define ustr2tstr(str) tlib::u16str_wstr(str)
            #define tstr2str(str)  tlib::wstr_cstr(str)
            #define tstr2wstr(str) str
            #define tstr2ustr(str) tlib::wstr_u16str(str)
      #else
            #define str2tstr(str)  str
            #define wstr2tstr(str) tlib::wstr_cstr(str)
            #define ustr2tstr(str) tlib::wstr_cstr(u16str_wstr(str))
            #define tstr2str(str)  str
            #define tstr2wstr(str) tlib::cstr_wstr(str)
            #define tstr2ustr(str) tlib::wstr_u16str(cstr_wstr(str))
      #endif
#elif __linux__
      #define str2tstr(str)  tlib::u8str_u16str(str)
      #define wstr2tstr(str) tlib::wstr_u16str(str)       // u8str_u16str(wstr_u8str(str))     ???? ToDo: need fix for Linux
      #define ustr2tstr(str) str
      #define tstr2str(str)  tlib::u16str_u8str(str)
      #define tstr2wstr(str) tlib::u16str_wstr(str)
      #define tstr2ustr(str) str
#endif
      //=======================================================================
      //  Вводим макросы, которые позволяют задавать в явном виде тип char и wchar_t строки и символа
      //  Предназначен строго для использования в шаблонах, type - это или char или wchar_t
      //
#define TemplateTypeOfStr(str, type) ((is_wchar_v<type>) ? (type *)L##str : (is_char16_v<type>) ? (type *)u##str : (type *)str)
#define TemplateTypeOfCh(str, type) ((is_wchar_v<type>) ? (type)L##str : (is_char16_v<type>) ? (type)u##str : (type)str)

      template <class T, class _Elem = typename Type_Str<T>::type>
      inline tstring TemplateStr2Tstr(T str)
      {
            if constexpr (is_wchar_v<_Elem>)
            {
                  return wstr2tstr(str);
            }
            else if constexpr (is_char16_v<_Elem>)
            {
                  return ustr2tstr(str);
            }
            else
            {
                  return str2tstr(str);
            }
      }

      template <class _Elem>
      inline std::basic_string<_Elem> Tstr2TemplateStr(const tstring& str)
      {
            if constexpr (is_wchar_v<_Elem>)
            {
                  return tstr2wstr(str);
            }
            else if constexpr (is_char16_v<_Elem>)
            {
                  return tstr2ustr(str);
            }
            else
            {
                  return tstr2str(str);
            }
      }

      template <class T, class _Elem = typename Type_Str<T>::type>
      inline std::string TemplateStr2str(T str)
      {
            if constexpr (is_wchar_v<_Elem>)
            {
#ifdef _WIN32
                  return wstr_cstr(str);
#else
                  return wstr_u8str(str);
#endif
            }
            else if constexpr (is_char16_v<_Elem>)
            {
#ifdef _WIN32
                  return wstr_cstr(u16str_wstr(str));
#else
                  return u16str_u8str(str);
#endif
            }
            else
            {
                  return str;
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
