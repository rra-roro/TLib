/*
*/
#pragma once 

#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#endif

#ifdef _WIN32
#include <tchar.h>
#elif __linux__
#include <linux/tchar.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <iterator>

namespace tlib
{
      using std::endl;

      using u16istream = std::basic_istream<char16_t>;
      using u16ostream = std::basic_ostream<char16_t>;
      using u16ifstream = std::basic_ifstream<char16_t>;
      using u16ofstream = std::basic_ofstream<char16_t>;
      using u16fstream = std::basic_fstream<char16_t>;
      using u16istringstream = std::basic_istringstream<char16_t>;
      using u16ostringstream = std::basic_ostringstream<char16_t>;
      using u16stringstream = std::basic_stringstream<char16_t>;
      using u16stringbuf = std::basic_stringbuf<char16_t>;

      extern u16ostream ucout;
      extern u16istream ucin;
      extern u16ostream ucerr;
      extern u16ostream uclog;

      using tistream = std::basic_istream<TCHAR>;
      using tostream = std::basic_ostream<TCHAR>;
      using tifstream = std::basic_ifstream<TCHAR>;
      using tofstream = std::basic_ofstream<TCHAR>;
      using tfstream = std::basic_fstream<TCHAR>;
      using tistringstream = std::basic_istringstream<TCHAR>;
      using tostringstream = std::basic_ostringstream<TCHAR>;
      using tstringstream = std::basic_stringstream<TCHAR>;
      using tstringbuf = std::basic_stringbuf<TCHAR>;

#if (defined(_WIN32) && defined(_UNICODE))
      extern std::wostream& tcout;
      extern std::wistream& tcin;
      extern std::wostream& tcerr;
      extern std::wostream& tclog;
#else
      extern u16ostream& tcout;
      extern u16istream& tcin;
      extern u16ostream& tcerr;
      extern u16ostream& tclog;
#endif


#ifdef _WIN32

#ifdef Q_OS_WIN
      inline std::ostream& operator<<(std::ostream& s, const QString& x)
      {
            return s << static_cast<const char*>(x.toLocal8Bit());
      }

      inline std::wostream& operator<<(std::wostream& s, const QString& x)
      {
            return s << x.toStdWString();
      }
#endif // Q_OS_WIN
#endif // _WIN32
}


#if !(defined(__linux__) || defined(_LIB) || defined(UNDER_CE) || defined(WINCE))

#define _LIB_NAME "tlib"

#include <win/select_lib.h>

#undef _LIB_NAME

#endif // !(defined(_LIB) || defined(UNDER_CE) || defined(WINCE))


