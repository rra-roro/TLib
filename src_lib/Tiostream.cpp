﻿/*
*/
#include "stdafx.h"
#include <Tiostream.h>
#include <Tlocale.h>
#include <Tcodecvt.h>

using namespace std;
using namespace tlib;

namespace tlib
{

#ifdef _WIN32
      wostream& ucout = wcout;
      wistream& ucin = wcin;
      wostream& ucerr = wcerr;
      wostream& uclog = wclog;
#else
      u16ostream ucout(&out_bufferconvert_u16_u8);
      u16istream ucin(&in_bufferconvert_u16_u8);
      u16ostream ucerr(&err_bufferconvert_u16_u8);
      u16ostream uclog(&log_bufferconvert_u16_u8);
#endif

      //Создадим ссылку на глобальные объекты консоли в зависимости от того Ansi или Unicode
#if (defined(_WIN32) && defined(_UNICODE))
      wostream& tcout = wcout;
      wistream& tcin = wcin;
      wostream& tcerr = wcerr;
      wostream& tclog = wclog;
#else
      u16ostream& tcout = ucout;
      u16istream& tcin = ucin;
      u16ostream& tcerr = ucerr;
      u16ostream& tclog = uclog;

#endif

#ifdef _WIN32

      using stream_codecvt_w = deletable_facet<codecvt_byname<wchar_t, char, std::mbstate_t>>;

      thread_local std::wstring_convert<stream_codecvt_w> convert_ProgramCP_wide(new stream_codecvt_w(GetLocaleNameProgram()));
      thread_local std::wstring_convert<stream_codecvt_w> convert_ConsoleCP_wide(new stream_codecvt_w(GetLocaleNameConsole()));

#elif __linux__

      using stream_codecvt_u16 = deletable_facet<codecvt_byname<char16_t, char, std::mbstate_t>>;
      thread_local std::wstring_convert<stream_codecvt_u16, char16_t> convert_ConsoleCP_wide(new stream_codecvt_u16(GetLocaleNameConsole()));

#endif

}

#if !(defined(__linux__) || defined(UNDER_CE) || defined(WINCE))
//================================================================================
//  Корректируем ввод вывод на консоль win32
//  Отключаю их только для WINCE

ostream& tlib::operator<<(ostream& s, char x)
{
      string ClassName(typeid(s).name());
      if (ClassName.find("basic_ostream") != npos)
      {
            std::wstring wstr = convert_ProgramCP_wide.from_bytes(x);

            // Выведем строку в поток
            std::operator<<(s, convert_ConsoleCP_wide.to_bytes(wstr));
      }
      else
      {
            std::operator<<(s, x);
      }
      return s;
}

ostream& tlib::operator<<(ostream& s, const char* x)
{
      string ClassName(typeid(s).name());
      if (ClassName.find("basic_ostream") != npos)
      {
            std::wstring wstr = convert_ProgramCP_wide.from_bytes(x);

            // Выведем строку в поток
            std::operator<<(s, convert_ConsoleCP_wide.to_bytes(wstr));
      }
      else
      {
            std::operator<<(s, x);
      }
      return s;
}

ostream& tlib::operator<<(ostream& s, const string& x)
{
      string ClassName(typeid(s).name());
      if (ClassName.find("basic_ostream") != npos)
      {
            std::wstring wstr = convert_ProgramCP_wide.from_bytes(x);

            // Выведем строку в поток
            std::operator<<(s, convert_ConsoleCP_wide.to_bytes(wstr));
      }
      else
      {
            return std::operator<<(s, x);
      }
      return s;
}

ostream& tlib::operator<<(ostream& s, string_view x)
{
      string ClassName(typeid(s).name());
      if (ClassName.find("basic_ostream") != npos)
      {
            std::wstring wstr = convert_ProgramCP_wide.from_bytes(x.data(), x.data() + x.size());

            // Выведем строку в поток
            std::operator<<(s, convert_ConsoleCP_wide.to_bytes(wstr));
      }
      else
      {
            return std::operator<<(s, x);
      }
      return s;
}

istream& tlib::operator>>(istream& s, string& x)
{
      string ClassName(typeid(s).name());
      if (ClassName.find("basic_istream") != npos)
      {
            string InputStr;
            std::operator>>(s, InputStr);

            std::wstring wstr = convert_ConsoleCP_wide.from_bytes(InputStr);

            x.assign(convert_ProgramCP_wide.to_bytes(wstr));
      }
      else
      {
            std::operator>>(s, x);
      }
      return s;
}

// Выводим в wostream строки u16stream

wostream& tlib::operator<<(wostream& s, char16_t x)
{
      // Выведем строку в поток
      std::operator<<(s, (wchar_t)(x));
      return s;
}

wostream& tlib::operator<<(wostream& s, const char16_t* x)
{
      // Выведем строку в поток
      std::operator<<(s, (wchar_t*)(x));
      return s;
}

wostream& tlib::operator<<(wostream& s, const u16string& x)
{
      // Выведем строку в поток
      std::operator<<(s, (wchar_t*)x.c_str());
      return s;
}

wostream& tlib::operator<<(wostream& s, u16string_view x)
{
      // Выведем строку в поток
      std::operator<<(s, wstring_view((wchar_t*)x.data(), x.size()));
      return s;
}

wistream& tlib::operator>>(wistream& s, u16string& x)
{
      wstring InputStr;
      std::operator>>(s, InputStr);

      x.assign(InputStr.begin(), InputStr.end());
      return s;
}

#endif //!(defined(UNDER_CE) || defined(WINCE))
