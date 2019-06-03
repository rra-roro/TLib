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
#endif

      // Установим локаль соответствующую текущей кодировке консоли
      // для всех консольных потоков ввода/вывода
      wcout.imbue(loc); wcout.clear();
      ucout.imbue(loc); ucout.clear();
      cout.imbue(loc); cout.clear();
      wcin.imbue(loc); wcin.clear();
      ucin.imbue(loc); ucin.clear();
      cin.imbue(loc); cin.clear();
      wcerr.imbue(loc); wcerr.clear();
      ucerr.imbue(loc); ucerr.clear();
      cerr.imbue(loc); cerr.clear();
      wclog.imbue(loc); wclog.clear();
      uclog.imbue(loc); uclog.clear();
      clog.imbue(loc); clog.clear();

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
