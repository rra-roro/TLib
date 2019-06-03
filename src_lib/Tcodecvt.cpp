/*
*
*/
#include "stdafx.h"
#include <Tlocale.h>

using namespace std;

namespace tlib
{
      //   String converter wchar_t <--> char  (UTF-16  <--> ProgramCodePage )
      thread_local std::wstring_convert<codecvt_w_o> strconvert_w_сodepage(new codecvt_w_o(GetLocaleNameProgram()));

      //   String converter char16_t <--> char8_t  (UTF-16  <--> UTF-8)
      thread_local std::wstring_convert<codecvt_u16_u8, char16_t> strconvert_u16_u8(new codecvt_u16_u8(CodePageUTF8));

      //   String converter wchar_t <--> char8_t   (Wide  <--> UTF-8)
      thread_local std::wstring_convert<codecvt_w_u8> strconvert_w_u8(new codecvt_w_u8(CodePageUTF8));

      //   Convertering buffer char16_t --> char8_t  (UTF-16  --> UTF-8)
      thread_local std::wbuffer_convert<codecvt_u16_u8, char16_t> in_bufferconvert_u16_u8(cin.rdbuf(), new codecvt_u16_u8(CodePageUTF8));
      thread_local std::wbuffer_convert<codecvt_u16_u8, char16_t> out_bufferconvert_u16_u8(cout.rdbuf(), new codecvt_u16_u8(CodePageUTF8));
      thread_local std::wbuffer_convert<codecvt_u16_u8, char16_t> err_bufferconvert_u16_u8(cerr.rdbuf(), new codecvt_u16_u8(CodePageUTF8));
      thread_local std::wbuffer_convert<codecvt_u16_u8, char16_t> log_bufferconvert_u16_u8(clog.rdbuf(), new codecvt_u16_u8(CodePageUTF8));
}








