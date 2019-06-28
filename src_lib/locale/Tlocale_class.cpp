#include "stdafx.h"
#include <Tlocale.h>
#include <Tcodecvt.h>
#include <regex>
#include <algorithm>
#include <optional>
#include <clocale>

using namespace tlib;
using namespace std;

/////////////////////////////////////

tlib::locale::lang_names tlib::locale::all_locale_names;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//

std::string tlib::locale::locale_name_fix_codepage(std::string_view loc_name)
{

      cmatch match_str;
      regex reg_exp("(.*)([.].*)", std::regex_constants::icase);

      if (!regex_match(loc_name.data(), match_str, reg_exp))
      {
            return try_get_locname(loc_name);
      }
      else
      {
            string language = match_str.str(1);

            language = try_get_locname(language);

#ifdef __linux__
            if (language.empty())
                  language = "C";
#endif
            
            string codepage = match_str.str(2);

            std::use_facet<std::ctype<char>>(std::locale()).tolower(codepage.data(), codepage.data() + codepage.size());
            codepage.erase(std::remove(codepage.begin(), codepage.end(), '-'), codepage.end());

#ifdef _WIN32
            if (codepage.find("utf7") != npos)
                  codepage = ".65000";
            else if (codepage.find("utf8") != npos)
                  codepage = ".65001";
            else if (codepage.find("utf16") != npos)
                  codepage = ".1200";
            else if (codepage.find("utf32") != npos)
                  codepage = ".12000";
            else if (codepage.find("utf32be") != npos)
                  codepage = ".12001";
            else if (codepage.find("koi8r") != npos)
                  codepage = ".20866";
            else if (codepage.find("koi8u") != npos)
                  codepage = ".21866";
            else
            {
                  regex reg_exp("^[.][^[:d:]]*([[:d:]]+).*$", std::regex_constants::icase);

                  if (regex_match(codepage.data(), match_str, reg_exp))
                  {
                        codepage = "." + match_str.str(1);
                  }
            }
            
            try
            {
                  std::locale loc((language + codepage).c_str());
                  return language + codepage;
            }
            catch (...)
            {
                  return {};
            }

#elif __linux__

            string code_page = codepage;
            size_t pos_utf = code_page.find("utf");
            if (pos_utf != npos)
            {
                  code_page.replace(pos_utf, 3, "utf-");
            }
            size_t pos_koi8 = code_page.find("koi8");
            if (pos_koi8 != npos)
            {
                  code_page.replace(pos_koi8, 4, "koi8-");
            }
 
            auto toupper = [](char ch) { return std::use_facet<std::ctype<char>>(std::locale()).toupper(ch); };

            string code_page_up = code_page;
            std::transform(code_page_up.begin(), code_page_up.end(), code_page_up.begin(), toupper);
            string codepage_up = codepage;
            std::transform(codepage_up.begin(), codepage_up.end(), codepage_up.begin(), toupper);

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

            throw std::runtime_error("locale name not found");
#endif
      }
}
