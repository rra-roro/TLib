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
#include <algorithm>
#include <Ttype.h>

extern const char* lang_name[][2];
extern const char* code_page[][3];

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
                                support_char16(true),
                                is_multibyte_codepage(detect_multibyte_codepage(name_locale))
            {
                  add_support_char16();
                  add_fix_other_facets();
                  add_if_cfg_facets(num_money_punct_cfg);
            };

            locale(const tlib::locale& other) noexcept : name_locale(other.name_locale),
                                                         internal_locale(other.internal_locale),
                                                         is_multibyte_codepage(detect_multibyte_codepage(name_locale))
            {
                  add_support_char16();
                  add_fix_other_facets();
            };

            locale(const std::locale& other) noexcept : name_locale(other.name()),
                                                        internal_locale(other),
                                                        is_multibyte_codepage(detect_multibyte_codepage(name_locale))
            {
                  add_support_char16();
                  add_fix_other_facets();
            };

            explicit locale(std::string_view loc_name, type_punct tp = num_money_punct_sys) : name_locale(locale_name_fix_codepage(loc_name)),
                                                                                              internal_locale(name_locale),
                                                                                              is_multibyte_codepage(detect_multibyte_codepage(name_locale))
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


            const std::string& name() const
            {
                  return name_locale;
            }

            bool is_mb_codepage() const
            {
                  return is_multibyte_codepage;
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
                  if constexpr (is_only_constructible_v<_Facet, std::string_view>)
                  {
                        // Добавим наши фасеты по имени локали
                        internal_locale = std::locale(internal_locale, new _Facet(name_locale));
                        return;
                  }
#ifdef _WIN32
                  else
                  {
                        // Добавим стандартые фасеты
                        internal_locale = std::locale(internal_locale, new _Facet(std::_Locinfo(name_locale.c_str())));
                  }
#elif __linux__
                  else if constexpr (is_only_constructible_v<_Facet, locale_t, size_t>)
                  {
                        // Добавим стандартые фасеты
                        internal_locale = std::locale(internal_locale, new _Facet(newlocale(LC_ALL_MASK, name_locale.c_str(), (locale_t)0)));
                        return;
                  }
                  else if constexpr (is_only_constructible_v<_Facet, locale_t, const char*, size_t>)
                  {
                        // Добавим стандартые фасеты
                        internal_locale = std::locale(internal_locale, new _Facet(newlocale(LC_ALL_MASK, name_locale.c_str(), (locale_t)0), name_locale.c_str()));
                        return;
                  }
                  else
                  {
                        // Добавим стандартые фасеты
                        internal_locale = std::locale(internal_locale, new _Facet());
                  }
#endif
            }

        private:
            struct lang_names
            {
                  const std::ctype<char>& facet = std::use_facet<std::ctype<char>>(std::locale());

                  std::map<std::string, const char*> map_short_long;
                  std::map<std::string, const char*> map_long_short;

                  lang_names()
                  {
                        for (size_t i = 0; lang_name[i][0][0] != '\0'; i++)
                        {
                              std::string short_name_key = lang_name[i][0];

                              facet.tolower(short_name_key.data(), short_name_key.data() + short_name_key.size());

                              short_name_key.erase(std::remove_if(short_name_key.begin(), short_name_key.end(), [](auto& ch) { return ch == '-' || ch == '_'; }),
                                  short_name_key.end());

                              map_short_long[short_name_key] = lang_name[i][1];

                              std::string long_name_key = lang_name[i][1];

                              facet.tolower(long_name_key.data(), long_name_key.data() + long_name_key.size());

                              long_name_key.erase(std::remove_if(long_name_key.begin(), long_name_key.end(), [](auto& ch) { return ch == '-' || ch == '_'; }),
                                  long_name_key.end());

                              map_long_short[long_name_key] = lang_name[i][0];
                        }
                  }

                  std::string find_long(std::string short_locname)
                  {
                        facet.tolower(short_locname.data(), short_locname.data() + short_locname.size());

                        short_locname.erase(std::remove_if(short_locname.begin(), short_locname.end(), [](auto& ch) { return ch == '-' || ch == '_'; }),
                            short_locname.end());

                        auto it = map_short_long.find(short_locname);

                        if (it != map_short_long.end())
                              return (*it).second;
                        else
                              return {};
                  }

                  std::string find_short(std::string long_locname)
                  {
                        facet.tolower(long_locname.data(), long_locname.data() + long_locname.size());

                        long_locname.erase(std::remove_if(long_locname.begin(), long_locname.end(), [](auto& ch) { return ch == '-' || ch == '_'; }),
                            long_locname.end());

                        auto it = map_long_short.find(long_locname);

                        if (it != map_long_short.end())
                              return (*it).second;
                        else
                              return {};
                  }
            };

            struct code_pages
            {
                  std::multimap<std::string, std::pair<const char*, bool>> map_name_cp;
                  std::multimap<std::string, std::pair<const char*, bool>> map_cp_name;

                  code_pages()
                  {
                        for (size_t i = 0; code_page[i][0][0] != '\0'; i++)
                        {
                              std::string code_page_key = code_page[i][1];

                              code_page_key.erase(std::remove_if(code_page_key.begin(), code_page_key.end(), [](auto& ch) { return ch == '-' || ch == '_'; }),
                                                  code_page_key.end());

                              map_name_cp.emplace(std::make_pair(code_page_key, std::make_pair(code_page[i][0], code_page[i][2][0] == 't' ? true : false)));

                              map_cp_name.emplace(std::make_pair(code_page[i][0], std::make_pair(code_page[i][1], code_page[i][2][0] == 't' ? true : false)));
                        }
                  }

                  auto get_cp(std::string_view cp)
                  {
#ifdef _WIN32
                        return map_name_cp.equal_range(cp.data());
#elif __linux__
                        return map_cp_name.equal_range(cp.data());
#endif
                  }

                  bool is_mb(std::string codepage)
                  {
                        std::use_facet<std::ctype<char>>(std::locale()).tolower(codepage.data(), codepage.data() + codepage.size());
                        codepage.erase(std::remove_if(codepage.begin(), codepage.end(), [](auto& ch) { return ch == '-' || ch == '_'; }),
                            codepage.end());
                        auto map_name_cp_ret = map_name_cp.find(codepage.data());
                        auto map_cp_name_ret = map_cp_name.find(codepage.data());

                        if (map_name_cp_ret != map_name_cp.end())
                              return map_name_cp_ret->second.second;
                        else if (map_cp_name_ret != map_cp_name.end())
                              return map_cp_name_ret->second.second;
                        else
                              return true;
                  }
            };

            static lang_names all_locale_names;
            static code_pages all_code_pages;

            std::string name_locale;
            std::locale internal_locale;
            bool support_char16;
            bool is_multibyte_codepage;

            bool detect_multibyte_codepage(std::string_view loc_name);


            //  Имена локалей имеют формат: "", "C", "POSIX" или "language[_area[.codepage]]" или ".codepage"
            //  Например: "Russian_Russia.866", "", "C", ".1251"

            std::string locale_name_fix_codepage(std::string_view loc_name);

            static std::string get_locname(std::string_view str)
            {
                  auto long_name = all_locale_names.find_long(str.data());
                  if (!long_name.empty())
                        return long_name;
                  else
                        throw std::runtime_error("locale name not found");
            };

            static std::string try_get_locname(std::string_view str)
            {
                  if (str == "") return "";
#ifdef _WIN32
                  if (str == "POSIX" || str == "C") return "C";

                  // Предпологаем, что нам передали кроткое имя, получим из него длинное:
                  auto long_name = all_locale_names.find_long(str.data());
                  if (!long_name.empty())
                        return long_name; // окажемся тут, если имя ru-RU или ru_RU, сконвертируем его в Russian_Russia

                  // Если нам передали не короткое имя, найдем короткое,
                  // а из него длинное в нормализованном виде
                  auto short_name = all_locale_names.find_short(str.data());
                  long_name = all_locale_names.find_long(short_name);
                  if (!long_name.empty())
                        return long_name; // окажемся тут, если нам передали корректное длинное имя
                                          // вернем нормализованное имя вида Russian_Russia

                  return str.data();
#elif __linux__
                  if (str == "POSIX" || str == "C")
                  {
                        return str.data();
                  }                        

                  // Предпологаем, что нам передали длинное имя, получим из него короткое:
                  auto short_name = all_locale_names.find_short(str.data());
                  if (!short_name.empty())
                  {
                        std::replace(short_name.begin(), short_name.end(), '-', '_');
                        return short_name;  // окажемся тут, если имя Russian_Russia или Russian-Russia, сконвертируем его в ru_RU
                  }

                  // Если нам передали короткое имя, найдем длинное,
                  // а из него короткое в нормализованном виде

                  auto long_name = all_locale_names.find_long(str.data());
                  short_name = all_locale_names.find_short(long_name);
                  if (!short_name.empty())
                  {
                        std::replace(short_name.begin(), short_name.end(), '-', '_');
                        return short_name; // окажемся тут, если нам передали корректное короткое имя
                                           // вернем его в нормализованное виде ru_RU
                  }

                  return str.data();
#endif
            };


            void add_support_char16()
            {
                  add_facet<std::collate<char16_t>>();
                  add_facet<tlib::inner_impl::ctype_byname<char16_t>>();
#ifdef _WIN32
                  add_facet<tlib::inner_impl::numpunct_byname<char16_t>>();
                  add_facet<tlib::inner_impl::moneypunct_byname<char16_t>>();
                  add_facet<tlib::inner_impl::moneypunct_byname<char16_t, true>>();
                  add_facet<tlib::inner_impl::time_put_byname<char16_t>>();
#elif __linux__
                  add_facet<tlib::inner_impl::numpunct_byname<char16_t>>();
                  add_facet<tlib::inner_impl::moneypunct_byname<char16_t>>();
                  add_facet<tlib::inner_impl::moneypunct_byname<char16_t, true>>();
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
                  add_facet<tlib::inner_impl::numpunct_byname<char>>();
                  add_facet<tlib::inner_impl::numpunct_byname<wchar_t>>();
                  add_facet<tlib::inner_impl::moneypunct_byname<char>>();
                  add_facet<tlib::inner_impl::moneypunct_byname<char, true>>();
                  add_facet<tlib::inner_impl::moneypunct_byname<wchar_t>>();
                  add_facet<tlib::inner_impl::moneypunct_byname<wchar_t, true>>();

#elif __linux__
                  add_facet<tlib::inner_impl::numpunct_byname<char>>();
                  add_facet<tlib::inner_impl::numpunct_byname<wchar_t>>();
                  add_facet<tlib::inner_impl::moneypunct_byname<char>>();
                  add_facet<tlib::inner_impl::moneypunct_byname<char, true>>();
                  add_facet<tlib::inner_impl::moneypunct_byname<wchar_t>>();
                  add_facet<tlib::inner_impl::moneypunct_byname<wchar_t, true>>();
#endif
            }

            void add_if_cfg_facets(type_punct tp)
            {
                  if (tp == num_punct_cfg || tp == num_money_punct_cfg)
                  {
                        add_facet<numpunct_cfg_byname<char>>();
                        add_facet<numpunct_cfg_byname<char16_t>>();
                        add_facet<numpunct_cfg_byname<wchar_t>>();
                  };

                  if (tp == money_punct_cfg || tp == num_money_punct_cfg)
                  {
                        add_facet<moneypunct_cfg_byname<char>>();
                        add_facet<moneypunct_cfg_byname<char16_t>>();
                        add_facet<moneypunct_cfg_byname<wchar_t>>();
                  }
            }
      };
}
