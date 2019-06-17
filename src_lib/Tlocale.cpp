#include "stdafx.h"
#include <Tlocale.h>
#include <Tiostream.h>
#include <Tcodecvt.h>
#include <regex>
#include <algorithm>
#include <optional>
#include <clocale>

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
      tlib::locale loc_prg = GetLocaleProgram();
      std::locale::global(loc_prg);

      // Создадим локаль соответствующую текущей кодировке консоли
      tlib::locale loc = GetLocaleConsole();

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
      temp.imbue(GetLocaleGUI());
      return 0;
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////
//

std::string tlib::locale::locale_name_fix_codepage(std::string_view loc_name)
{
      cmatch match_str;
      regex reg_exp("(.*)([.].*)", std::regex_constants::icase);

      if (!regex_match(loc_name.data(), match_str, reg_exp))
      {
            return loc_name.data();
      }
      {
            string language = match_str.str(1);

            string code_page = match_str.str(2);

            if (code_page.find("-") == npos)
            {
                  string low_tmp_str = code_page;
                  std::use_facet<std::ctype<char>>(std::locale()).tolower(low_tmp_str.data(), low_tmp_str.data() + low_tmp_str.size());
                  size_t pos_utf = low_tmp_str.find("utf");
                  if (pos_utf != npos)
                  {
                        low_tmp_str.replace(pos_utf, 3, "utf-");
                        code_page = low_tmp_str;
                  }                  
            }

            string codepage = code_page;
            if (codepage.find("-") != npos)
                  codepage.erase(remove(codepage.begin(), codepage.end(), '-'));

            auto toupper = [](char ch) { return std::use_facet<std::ctype<char>>(std::locale()).toupper(ch); };

            string code_page_up = code_page;
            std::transform(code_page_up.begin(), code_page_up.end(), code_page_up.begin(), toupper);
            string codepage_up = codepage;
            std::transform(codepage_up.begin(), codepage_up.end(), codepage_up.begin(), toupper);

            auto tolower = [](char ch) { return std::use_facet<std::ctype<char>>(std::locale()).tolower(ch); };

            string code_page_lo = code_page;
            std::transform(code_page_lo.begin(), code_page_lo.end(), code_page_lo.begin(), tolower);
            string codepage_lo = codepage;
            std::transform(codepage_lo.begin(), codepage_lo.end(), codepage_lo.begin(), tolower);

            const auto try_call_fn = [](std::string_view loc_name_a) -> std::optional<std::string> {
                  try
                  {
                        std::locale loc(loc_name_a.data());
                        return loc_name_a.data();
                  }
                  catch (...)
                  {
                        return {};
                  }
            };

            if (auto ret = try_call_fn(language + code_page))
                  return *ret;
            if (auto ret = try_call_fn(language + codepage))
                  return *ret;
            if (auto ret = try_call_fn(language + code_page_up))
                  return *ret;
            if (auto ret = try_call_fn(language + codepage_up))
                  return *ret;
            if (auto ret = try_call_fn(language + code_page_lo))
                  return *ret;
            if (auto ret = try_call_fn(language + codepage_lo))
                  return *ret;

            throw std::runtime_error("locale name not found");
      }
}
