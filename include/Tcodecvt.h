/*
*
*/

#ifndef TCODECVT
#define TCODECVT

#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#endif

#include <codecvt>
#include <char8_t.h>

namespace tlib
{

      //=======================================================================
      //  Вводим функции, которые позволяют осуществлять преобразования между различными типами строк.
      //  Например, преоразовать string и wstring друг в друга

      template <class Facet>
      struct deletable_facet : Facet
      {
            template <class... Args>
            deletable_facet(Args &&... args) : Facet(std::forward<Args>(args)...)
            {
            }
            // note: создается неявный public деструктор
      };

      // Converter character set Wide <--> Оrdinary
      using codecvt_w_o = deletable_facet<std::codecvt_byname<wchar_t, char, std::mbstate_t>>;

      // Converter character set UTF-16 <--> UTF-8
      using codecvt_u16_u8 = deletable_facet<std::codecvt_byname<char16_t, char8_t, std::mbstate_t>>;

      // Converter character set Wide <--> UTF-8
      using codecvt_w_u8 = deletable_facet<std::codecvt_byname<wchar_t, char8_t, std::mbstate_t>>;


      //   String converter wchar_t <--> char  (UTF-16  <--> ProgramCodePage )
      extern thread_local std::wstring_convert<codecvt_w_o> strconvert_w_codepage;

      //   String converter char16_t <--> char8_t  (UTF-16  <--> UTF-8)
      extern thread_local std::wstring_convert<codecvt_u16_u8, char16_t> strconvert_u16_u8;

      //   String converter wchar_t <--> char8_t   (Wide  <--> UTF-8)
      extern thread_local std::wstring_convert<codecvt_w_u8, wchar_t> strconvert_w_u8;


      //   Convertering buffer char16_t --> char8_t  (UTF-16  --> UTF-8)
      extern thread_local std::wbuffer_convert<codecvt_u16_u8, char16_t>& in_bufferconvert_u16_u8;
      extern thread_local std::wbuffer_convert<codecvt_u16_u8, char16_t>& out_bufferconvert_u16_u8;
      extern thread_local std::wbuffer_convert<codecvt_u16_u8, char16_t>& err_bufferconvert_u16_u8;
      extern thread_local std::wbuffer_convert<codecvt_u16_u8, char16_t>& log_bufferconvert_u16_u8;

      //   Convertering buffer wchar_t --> char8_t  (UTF-16  --> UTF-8)
      extern thread_local std::wbuffer_convert<codecvt_w_u8, wchar_t>& in_bufferconvert_w_u8;
      extern thread_local std::wbuffer_convert<codecvt_w_u8, wchar_t>& out_bufferconvert_w_u8;
      extern thread_local std::wbuffer_convert<codecvt_w_u8, wchar_t>& err_bufferconvert_w_u8;
      extern thread_local std::wbuffer_convert<codecvt_w_u8, wchar_t>& log_bufferconvert_w_u8;

      

}

#if !(defined(__linux__) || defined(_LIB))

#define _LIB_NAME "tlib_char16"

#include <win/select_lib.h>

#undef _LIB_NAME

#endif // !(defined(__linux__) || defined(_LIB))


#endif //TCODECVT
