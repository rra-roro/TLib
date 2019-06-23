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
      {
            string language = match_str.str(1);

            language = try_get_locname(language);

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
