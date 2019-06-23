#pragma once

#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <linux/linux.h>
#endif

#include <Tstring.h>

#include <locale>
#include <locale/Tlocale_facets.h>

#include <map>
#include <sstream>

extern const char* lang_name[][2];

namespace tlib
{
      class locale
      {
        public:
            enum type_punct
            {
                  num_punct_sys,
                  money_punct_sys,
                  num_money_punct_sys,
                  num_punct_cfg,
                  money_punct_cfg,
                  num_money_punct_cfg
            };

            locale() noexcept : name_locale(std::locale().name()),
                                internal_locale(),
                                support_char16(true)
            {
                  add_support_char16();
                  add_fix_other_facets();
                  add_if_cfg_facets(num_money_punct_cfg);
            };

            locale(const tlib::locale& other) noexcept : name_locale(other.name_locale),
                                                         internal_locale(other.internal_locale)
            {
                  add_support_char16();
                  add_fix_other_facets();
            };

            locale(const std::locale& other) noexcept : name_locale(other.name()),
                                                        internal_locale(other)
            {
                  add_support_char16();
                  add_fix_other_facets();
            };

            explicit locale(std::string_view loc_name, type_punct tp = num_money_punct_sys) : name_locale(locale_name_fix_codepage(loc_name)),
                                                                                              internal_locale(name_locale)
            {
                  add_support_char16();
                  add_fix_other_facets();
                  add_if_cfg_facets(tp);
            };

            operator std::locale&()
            {
                  return internal_locale;
            }

            operator const std::locale&() const
            {
                  return internal_locale;
            }


            std::string name() const
            {
                  return name_locale;
            }

            ///////////////////////////////////////////////////////////////////////////////////////
            //
            //  Ф-ия GetCodePageGUI() - возвращает текущую кодовую страницу
            //                          установленную в OS для GUI.
            //

            static inline std::string get_locale_name_GUI()
            {
                  std::stringstream stream;
#ifdef _WIN32
                  std::wstring lang_tmp(LOCALE_NAME_MAX_LENGTH, L'\0');
                  int size_str = GetUserDefaultLocaleName(lang_tmp.data(), LOCALE_NAME_MAX_LENGTH);
                  std::string lang;
                  if (size_str)
                        lang.assign(lang_tmp.begin(), lang_tmp.begin() + size_str - 1);

                  lang = get_locname(lang);

                  stream << lang << "." << std::dec << GetACP();
#elif __linux__
                  stream << GetConsoleOutputCP();
#endif
                  return stream.str();
            }

            ///////////////////////////////////////////////////////////////////////////////////////
            //
            //  Ф-ия GetLocaleGUI() - возвращает текущую локаль установленную в OS для GUI.
            //                        Т.е.возвращает локаль для CP_ACP
            //

            static inline std::locale get_locale_GUI()
            {
                  return tlib::locale(get_locale_name_GUI());
            }

            ///////////////////////////////////////////////////////////////////////////////////////
            //
            //  Ф-ия GetLocaleProgram() - возвращает текущую локаль соответствующую кодировке
            //                            строковых литералов типа char, в нашей откомпилированной
            //                            программе
            //

            static inline tlib::locale get_locale_program()
            {
                  return tlib::locale(CodePageCharInSrc);
            }

            ///////////////////////////////////////////////////////////////////////////////////////
            //
            //  Ф-ия GetCodePageProgram() - возвращает текущую кодовую страницу, соответствующую кодовой
            //                              странице кодировки строковых литералов типа char, в нашей
            //                              откомпилированной  программе
            //

            static inline std::string get_locale_name_program()
            {
                  return CodePageCharInSrc;
            }

            ///////////////////////////////////////////////////////////////////////////////////////
            //
            //  Ф-ия GetCodePageConsole() - возвращает текущую кодовую страницу
            //                              установленную в OS для консоли.
            //

            static inline std::string get_locale_name_console()
            {
                  std::stringstream stream;
#ifdef _WIN32
                  std::wstring lang_tmp(LOCALE_NAME_MAX_LENGTH, L'\0');
                  int size_str = GetUserDefaultLocaleName(lang_tmp.data(), LOCALE_NAME_MAX_LENGTH);
                  std::string lang;
                  if (size_str)
                        lang.assign(lang_tmp.begin(), lang_tmp.begin() + size_str - 1);

                  lang = get_locname(lang);

                  UINT concp = GetConsoleOutputCP();
                  if (concp == 0)
                        concp = 866;

                  stream << lang << "." << std::dec << concp;
#elif __linux__
                  stream << GetConsoleOutputCP();
#endif
                  return stream.str();
            }

            ///////////////////////////////////////////////////////////////////////////////////////
            //
            //  Ф-ия GetLocaleConsole() - возвращает текущую локаль установленную в OS для консоли
            //                            Т.е.возвращает локаль для CP_OEMCP
            //

            static inline tlib::locale get_locale_console()
            {
                  return tlib::locale(get_locale_name_console());
            }


            ///////////////////////////////////////////////////////////////////////////////////////
            //
            //   Ф-ия AddFacet<>(std::locale& loc) - добавляет к переданной локали фасет, заданный в
            //                                       параметре шаблона
            //

            template <class _Facet>
            void add_facet()
            {
#ifdef _WIN32
                  // Добавим в переданную локаль Фасет заданный в параметре шаблона
                  internal_locale = std::locale(internal_locale, new _Facet(std::_Locinfo(name_locale.c_str())));
#elif __linux__
                  if constexpr (std::is_same_v<_Facet, tlib::inner_impl::ctype<char16_t>> ||
                                std::is_same_v<_Facet, std::numpunct<char16_t>> ||
                                std::is_same_v<_Facet, tlib::inner_impl::numpunct<char>>)
                  {
                        internal_locale = std::locale(internal_locale, new _Facet(newlocale(LC_ALL_MASK, name_locale.c_str(), (locale_t)0)));
                  }
                  else if constexpr (std::is_same_v<_Facet, std::moneypunct<char16_t>> ||
                                     std::is_same_v<_Facet, std::__timepunct<char16_t>> ||
                                     std::is_same_v<_Facet, tlib::inner_impl::moneypunct<char>>)
                  {
                        internal_locale = std::locale(internal_locale, new _Facet(newlocale(LC_ALL_MASK, name_locale.c_str(), (locale_t)0), name_locale.c_str()));
                  }
                  else
                  {
                        // Добавим в переданную локаль Фасет заданный в параметре шаблона
                        internal_locale = std::locale(internal_locale, new _Facet());
                  }
#endif
            }

        private:
            struct lang_names
            {
                  std::map<std::string, const char*> map_short_long;
                  std::map<std::string, const char*> map_long_short;

                  lang_names()
                  {
                        for (size_t i = 0; lang_name[i][0][0] != '\0'; i++)
                        {
                              map_short_long[lang_name[i][0]] = lang_name[i][1];
                              map_long_short[lang_name[i][1]] = lang_name[i][0];
                        }
                  }
            };

            static lang_names all_locale_names;
            std::string name_locale;
            std::locale internal_locale;
            bool support_char16;

            //  Имена локалей имеют формат: "", "C", "POSIX" или "language[_area[.codepage]]" или ".codepage"
            //  Например: "Russian_Russia.866", "", "C", ".1251"

            std::string locale_name_fix_codepage(std::string_view loc_name);

            static std::string get_locname(std::string_view str)
            {
                  auto iter = all_locale_names.map_short_long.find(str.data());
                  if (iter != all_locale_names.map_short_long.end())
                        return (*iter).second;
                  else
                        throw std::runtime_error("locale name not found");
            };

            static std::string try_get_locname(std::string_view str)
            {
#ifdef _WIN32
                  if (str == "POSIX") return "C";

                  // Предпологаем, что нам передали кроткое имя:
                  auto iter = all_locale_names.map_short_long.find(str.data());
                  if (iter != all_locale_names.map_short_long.end()) 
                        return (*iter).second;                    // окажемся тут, если имя ru-RU, сконвертируем его в Russian_Russia
                  else
                  if (str.find("_") != npos)                
                  {                  
                        auto iter = all_locale_names.map_short_long.find(std::string(str).replace(str.find("_"), 1, "-"));
                        if (iter != all_locale_names.map_short_long.end())
                              return (*iter).second;        // окажемся тут, если имя ru_RU, сконвертируем его в Russian_Russia
                  }
                 
                  // Если нам передали не короткое имя, проверим его как длинное
                  iter = all_locale_names.map_long_short.find(str.data());
                  if (iter != all_locale_names.map_long_short.end())
                        return str.data();                               // окажемся тут, если нам передали корректное длинное имя Russian_Russia. вернем его
                  else
                  if (str.find("-") != npos)
                  {
                        std::string tmp_str = str.data();
                        tmp_str.replace(str.find("-"), 1, "_");
                        auto iter = all_locale_names.map_long_short.find(tmp_str);
                        if (iter != all_locale_names.map_long_short.end())
                              return tmp_str;                     // окажемся тут, если нам передали НЕ корректное длинное имя Russian-Russia.
                  }                                               // сконвертируем его в Russian_Russia

                  return str.data();
#elif __linux__

                  auto pos = str.find("-");
                  if (pos != npos)
                        return std::string(str).replace(pos, 1, "_");

                  auto iter = all_locale_names.map_long_short.find(str.data());
                  if (iter != all_locale_names.map_long_short.end())
                  {
                        auto pos = std::string((*iter).second).find("-");
                        if (pos != npos)
                        {
                              return std::string((*iter).second).replace(pos, 1, "_");
                        }
                        else return (*iter).second;
                  }
                  return str.data();
#endif
            };

            void add_support_char16()
            {
                  add_facet<std::collate<char16_t>>();
                  add_facet<tlib::inner_impl::ctype<char16_t>>();
#ifdef _WIN32
                  add_facet<tlib::inner_impl::numpunct<char16_t>>();
                  add_facet<tlib::inner_impl::moneypunct<char16_t>>();
                  add_facet<tlib::inner_impl::time_put<char16_t>>();
#elif __linux__
                  add_facet<std::numpunct<char16_t>>();
                  add_facet<std::moneypunct<char16_t>>();
                  add_facet<std::__timepunct<char16_t>>();
                  add_facet<std::time_put<char16_t>>();
#endif
                  add_facet<std::num_put<char16_t>>();
                  add_facet<std::num_get<char16_t>>();
                  add_facet<std::money_get<char16_t>>();
                  add_facet<std::money_put<char16_t>>();
                  //add_facet<std::time_get<char16_t>>();
            }

            void add_fix_other_facets()
            {
#ifdef _WIN32
                  add_facet<tlib::inner_impl::moneypunct<wchar_t>>();
                  add_facet<tlib::inner_impl::moneypunct<char, false>>();
#elif __linux__
                  add_facet<tlib::inner_impl::numpunct<char>>();
                  add_facet<tlib::inner_impl::moneypunct<char, false>>();
#endif
            }

            void add_if_cfg_facets(type_punct tp)
            {
                  if (tp == num_punct_cfg || tp == num_money_punct_cfg)
                  {
                        add_facet<numpunct_by_cfg<char>>();
                        add_facet<numpunct_by_cfg<char16_t>>();
                        add_facet<numpunct_by_cfg<wchar_t>>();
                  };

                  if (tp == money_punct_cfg || tp == num_money_punct_cfg)
                  {
                        add_facet<moneypunct_by_cfg<char>>();
                        add_facet<moneypunct_by_cfg<char16_t>>();
                        add_facet<moneypunct_by_cfg<wchar_t>>();
                  }
            }
      };
}
