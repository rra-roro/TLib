#include "stdafx.h"
#include <Tlocale.h>
#include <Tiostream.h>
#include <Tcodecvt.h>

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
      std::locale::global(GetLocaleProgram());

      // Создадим локаль соответствующую текущей кодировке консоли
      locale loc = GetLocaleConsole();

#ifdef __linux__
      // Консоль Linux не сособна выводить wchar_t строки, заменим у
      // wide iostream потоков потоковый буфер, перенаправив тем самым
      // wcout в cout

      wcout.rdbuf(&out_bufferconvert_w_u8);
      wcin.rdbuf(&in_bufferconvert_w_u8);
      wcerr.rdbuf(&err_bufferconvert_w_u8);
      wclog.rdbuf(&log_bufferconvert_w_u8);

#elif _WIN32
      // Консоль Windows по умолчанию имеет кодировку отличную от кодировки строк в программе.
      // Заменим у iostream потоков потоковый буфер, на тот который осуществляет правильное перекодирование символов
      cout.rdbuf(&out_bufferconvert_programCP_consoleCP);
      cin.rdbuf(&in_bufferconvert_programCP_consoleCP);
      cerr.rdbuf(&err_bufferconvert_programCP_consoleCP);
      clog.rdbuf(&log_bufferconvert_programCP_consoleCP);

#endif

      // Установим локаль соответствующую текущей кодировке консоли
      // для всех консольных потоков ввода/вывода

      auto set_locale = [&](auto& stream) {stream.imbue(loc); stream.clear(); };

      set_locale(wcout);
      set_locale(ucout);
      set_locale(cout);
      set_locale(wcin);
      set_locale(ucin);
      set_locale(cin);
      set_locale(wcerr);
      set_locale(ucerr);
      set_locale(cerr);
      set_locale(wclog);
      set_locale(uclog);
      set_locale(clog);

      return 0;
}

#ifdef _WIN32
int tlib::InitIO(void)
{
      tstringstream temp;
      temp.imbue(GetLocaleGUI());
      return 0;
}
#endif
