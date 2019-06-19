#include "stdafx.h"
#include <Tlocale.h>
#include <Tiostream.h>

using namespace tlib;
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//Ф-ия  InitConsolIO объевлена в Tstring.h следующим образом
//
//DWORD InitConsolIO(const string& LocaleName  = "Russian_Russia.866",
//                     const string& NumberLocale = "C");
//
int tlib::InitConsolIO(void)
{
      // Системная локаль по умолчанию "C". Проинициализируем CRT, установив для нее
      // локаль, соответствующую кодировке строковых литералов типа char, в нашей
      // откомпилированной программе, заданной при ее компиляции
      tlib::locale loc_prg = tlib::locale::get_locale_program();
      std::locale::global(loc_prg);

      // Создадим локаль соответствующую текущей кодировке консоли
      tlib::locale loc = tlib::locale::get_locale_console();

      auto set_locale = [](auto& stream, const tlib::locale& loc) {stream.imbue(loc); stream.clear(); };

#ifdef __linux__
      // Консоль Linux не сособна выводить wchar_t строки, заменим у
      // wide iostream потоков потоковый буфер, перенаправив тем самым
      // wcout в cout

      wcout.rdbuf(&out_bufferconvert_w_u8);
      wcin.rdbuf(&in_bufferconvert_w_u8);
      wcerr.rdbuf(&err_bufferconvert_w_u8);
      wclog.rdbuf(&log_bufferconvert_w_u8);

      set_locale(cout, loc);
      set_locale(cin, loc);
      set_locale(cerr, loc);
      set_locale(clog, loc);

#elif _WIN32
      // Консоль Windows по умолчанию имеет кодировку отличную от кодировки строк в программе.
      // Заменим у iostream потоков потоковый буфер, на тот который осуществляет правильное перекодирование символов
      cout.rdbuf(&out_bufferconvert_programCP_consoleCP);
      cin.rdbuf(&in_bufferconvert_programCP_consoleCP);
      cerr.rdbuf(&err_bufferconvert_programCP_consoleCP);
      clog.rdbuf(&log_bufferconvert_programCP_consoleCP);

      set_locale(cout, loc_prg);
      set_locale(cin, loc_prg);
      set_locale(cerr, loc_prg);
      set_locale(clog, loc_prg);

#endif

      // Установим локаль соответствующую текущей кодировке консоли
      // для всех консольных потоков ввода/вывода      
      set_locale(wcout, loc);
      set_locale(ucout, loc);
      set_locale(wcin, loc);
      set_locale(ucin, loc);
      set_locale(wcerr, loc);
      set_locale(ucerr, loc);
      set_locale(wclog, loc);
      set_locale(uclog, loc);

      return 0;
}

#ifdef _WIN32
int tlib::InitIO(void)
{
      tstringstream temp;
      temp.imbue(tlib::locale::get_locale_GUI());
      return 0;
}
#endif
