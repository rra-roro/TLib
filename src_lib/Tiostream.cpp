/*
*/
#include "stdafx.h"
#include <Tiostream.h>
#include <Tlocale.h>
#include <Tcodecvt.h>

using namespace std;
using namespace tlib;

namespace tlib
{
      // Windows замена у cout поткового буфера, корректирующего вывод на консоль,
      // осуществляется в int tlib::InitConsolIO(void) в файле Tlocale.cpp

      // Linux замена у wcout поткового буфера, корректирующего вывод на консоль,
      // осуществляется в int tlib::InitConsolIO(void) в файле Tlocale.cpp

#ifdef __linux__
      u16ostream ucout(&out_bufferconvert_u16_u8);
      u16istream ucin(&in_bufferconvert_u16_u8);
      u16ostream ucerr(&err_bufferconvert_u16_u8);
      u16ostream uclog(&log_bufferconvert_u16_u8);
#elif _WIN32
      u16ostream ucout((std::basic_streambuf<char16_t>*)wcout.rdbuf());
      u16istream ucin((std::basic_streambuf<char16_t>*)wcin.rdbuf());
      u16ostream ucerr((std::basic_streambuf<char16_t>*)wcerr.rdbuf());
      u16ostream uclog((std::basic_streambuf<char16_t>*)wclog.rdbuf());
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

#ifdef __linux__
      int _dummy_init_stdout = fputc('\0', stdout);
      int _dummy_init_stderr = fputc('\0', stderr);
#endif

}
