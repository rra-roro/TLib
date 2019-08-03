#include "stdafx.h"
#include <Tlocale.h>
#include <Tiostream.h>
#include <vector>
#include <string>
#include <filesystem>

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
      thread_local tlib::locale loc_prg = tlib::locale::get_locale_program();
      [[maybe_unused]] thread_local auto old_gl_locale = std::locale::global(loc_prg);

      [[maybe_unused]] thread_local auto old_name_c_locale = setlocale(LC_ALL, tlib::locale::get_locale_name_program().c_str());

      // Интересно, но как выяснилось покрайней мере в Windows 10 - setlocale задает локаль для строк, используемых в программе,
      // можно задавать как однобайтовые локали, так и многобайтовые, т.е. можно задавать utf-8
      //
      // После этого, все объекты cout, wcout, ucout, cin, wcin, и т.д. будут конвертировать строки из локали программы в локаль консоли
      // автоматически без нашего участия. Но не только, setlocale также влияет на ф-ии Си работы со строками, заставляя их корректно работать.
      //
      // Таким образом нам не нужно подменять stream_buffer и устанавивать локаль через ф-ию член imbue().
      //
      // Заметим при этом, что это все влияет на С/C++ CRT библиотеку,
      // а кодовая страница консоли остается той же что и была (866 по умолчанию)

#ifdef __linux__

      // Консоль Linux не сособна выводить wchar_t строки, заменим у
      // wide iostream потоков потоковый буфер, перенаправив тем самым
      // wcout в cout

      wcout.rdbuf(&out_bufferconvert_w_u8);
      wcin.rdbuf(&in_bufferconvert_w_u8);
      wcerr.rdbuf(&err_bufferconvert_w_u8);
      wclog.rdbuf(&log_bufferconvert_w_u8);

#endif

      auto set_locale = [](auto& stream, const tlib::locale& loc) -> bool {stream.imbue(loc); stream.clear(); return true; };

      // Установим локаль на основе кодировки строк программы для всех консольных потоков ввода/вывода
      // В Windows, после установки setlocale все будет перекодировано в кодировку консоли
      // В Linux, кодировка консоли совпадает с кодировкой программы - UTF8
      // (нужно рассмотреть и протестировать случай, если в linux кодировки не совпадают) 
      // В любом случае нам нужно установить локаль для консольных потоков ввода/вывода,
      //  чтобы они правильно отображали даты, цифры, валюту и пр.

      [[maybe_unused]] thread_local bool b1 = set_locale(cout, loc_prg);
      [[maybe_unused]] thread_local bool b2 = set_locale(wcout, loc_prg);
      [[maybe_unused]] thread_local bool b3 = set_locale(ucout, loc_prg);
      [[maybe_unused]] thread_local bool b4 = set_locale(cin, loc_prg);
      [[maybe_unused]] thread_local bool b5 = set_locale(wcin, loc_prg);
      [[maybe_unused]] thread_local bool b6 = set_locale(ucin, loc_prg);
      [[maybe_unused]] thread_local bool b7 = set_locale(cerr, loc_prg);
      [[maybe_unused]] thread_local bool b8 = set_locale(wcerr, loc_prg);
      [[maybe_unused]] thread_local bool b9 = set_locale(ucerr, loc_prg);
      [[maybe_unused]] thread_local bool b10 = set_locale(clog, loc_prg);
      [[maybe_unused]] thread_local bool b11 = set_locale(wclog, loc_prg);
      [[maybe_unused]] thread_local bool b12 = set_locale(uclog, loc_prg);

      return 0;
}

/////////////////////////////////////////////////////////
#ifdef _WIN32
BOOL CALLBACK MyFuncLocaleEx(LPWSTR pStr, DWORD dwFlags, LPARAM lparam)
{
      std::string locname = wstr_cstr(pStr);
      size_t pos = locname.find("-");
      if (pos != npos)
            locname.replace(pos, 1, "_");
      if (IsValidLocaleName(pStr))
      {
            vector<string>& locales = *(vector<string>*)lparam;
            locales.push_back(locname);
      }
      return TRUE;
}
#endif

vector<string> tlib::get_available_locale_names()
{
      vector<string> locales;

#ifdef _WIN32

      EnumSystemLocalesEx(MyFuncLocaleEx, LOCALE_SPECIFICDATA, (LPARAM)&locales, NULL);

#else
      namespace fs = std::filesystem;

      string path = "/usr/lib/locale";
      for (const auto& entry : fs::directory_iterator(path))
      {
            if (entry.is_directory())
                  locales.push_back(entry.path().filename());
      }
#endif
      return locales;
}


#ifdef _WIN32
int tlib::InitIO(void)
{
      tstringstream temp;
      temp.imbue(tlib::locale::get_locale_GUI());
      return 0;
}
#endif
