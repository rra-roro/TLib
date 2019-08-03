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
using name_l = string;
using codepage_l = string;

optional<pair<name_l, codepage_l>> split_locale_name(std::string_view loc_name)
{
      cmatch match_str;
      regex reg_exp("(.*)[.](.*)", std::regex_constants::icase);

      if (!regex_match(loc_name.data(), match_str, reg_exp))
      {
            return {};
      }
      else
      {
            return pair<name_l, codepage_l>(match_str.str(1), match_str.str(2));
      }
}

std::optional<std::string> checkup_locale(std::string language, std::string_view codepage)
{
      auto toupper = [](char ch) { return std::use_facet<std::ctype<char>>(std::locale()).toupper(ch); };

      const auto try_set_locale = [](std::string_view loc_name_a) -> std::optional<std::string> {
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

      std::string cp[8];
      cp[2] = cp[1] = cp[0] = codepage;
      cp[1].erase(std::remove(cp[1].begin(), cp[1].end(), '-'), cp[1].end());
      cp[2].erase(std::remove(cp[2].begin(), cp[2].end(), '_'), cp[2].end());
      cp[3] = cp[0];
      cp[4] = cp[1];
      cp[5] = cp[2];
      std::transform(cp[3].begin(), cp[3].end(), cp[3].begin(), toupper);
      std::transform(cp[4].begin(), cp[4].end(), cp[4].begin(), toupper);
      std::transform(cp[5].begin(), cp[5].end(), cp[5].begin(), toupper);
      cp[6] = "cp" + cp[0];
      cp[7] = "CP" + cp[0];

      for (size_t i = 0; i < 8; i++)
      {
            if (auto ret = try_set_locale(language + "." + cp[i]))
                  return *ret;
      }

      return {};
}



////////////////////////////////////////////////////////////////////////////////////////////////////
bool tlib::locale::detect_multibyte_codepage(std::string_view loc_name)
{
      string language;
      string code_page;
      auto split_name = split_locale_name(loc_name);

      if(split_name)
      {
            auto [language_ref, code_page_ref] = *split_name;
            language = language_ref;
            code_page = code_page_ref;
      }

#ifdef _WIN32
      if (code_page.empty())
      {
            auto short_name = get_all_locale_names().find_short(loc_name.data());
            if (!short_name.empty())
                  language = short_name;
            else
            {
                  auto long_name = get_all_locale_names().find_long(loc_name.data());
                  short_name = get_all_locale_names().find_short(long_name);
                  if (!short_name.empty())
                        language = short_name;
            }

            wstring locale_name_w = cstr_wstr(language);

            int size_data = 256;
            wstring lc_data(size_data, '\0');

            int ret = GetLocaleInfoEx(locale_name_w.c_str(),
                                      LOCALE_IDEFAULTANSICODEPAGE,
                                      lc_data.data(), size_data);
            if (ret == 0)
                  return true;

            lc_data.erase(ret - 1);

            if (ret == 2 && lc_data == L"0")
                  return true;

            code_page = wstr_cstr(lc_data.c_str());
      }
#elif __linux__
      if (code_page.empty())
      {
            code_page = nl_langinfo_l(CODESET, (locale_t)newlocale(LC_ALL_MASK, loc_name.data(), (locale_t)0));
      }
#endif
      return get_all_code_pages().is_mb(code_page);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//

std::string tlib::locale::locale_name_fix_codepage(std::string_view loc_name)
{
      auto split_ln = split_locale_name(loc_name);
      if (!split_ln)
      {
            return try_get_locname(loc_name);
      }
      else
      {
            auto [language, code_page] = *split_ln;

            language = try_get_locname(language);

#ifdef __linux__
            if (language.empty())
                  language = "C";
#endif
            string codepage = code_page;
            std::use_facet<std::ctype<char>>(std::locale()).tolower(codepage.data(), codepage.data() + codepage.size());
            codepage.erase(std::remove_if(codepage.begin(), codepage.end(), [](auto& ch) { return ch == '-' || ch == '_'; }),
                codepage.end());

            auto [it_cp_begin, it_cp_end] = get_all_code_pages().get_cp(codepage);
            if (it_cp_begin == it_cp_end)
            {
                  if (auto ret = checkup_locale(language, code_page))
                        return *ret;
            }
            else
            {
                  if (auto ret = checkup_locale(language, code_page))
                        return *ret;

                  for (; it_cp_begin != it_cp_end; it_cp_begin++)
                  {
                        if (auto ret = checkup_locale(language, it_cp_begin->second.first))
                              return *ret;
                  }
            }

            throw std::runtime_error("locale name not found");
      }
}
