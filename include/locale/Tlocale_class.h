#pragma once
#include <locale>
#include <locale/Tlocale_facets.h>

namespace tlib
{
      class locale
      {
            std::string name_locale;
            std::locale internal_locale;
            bool support_char16;

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
            //   Ф-ия AddFacet<>(std::locale& loc) - добавляет к переданной локали фасет, заданный в
            //                                       параметре шаблона
            //  Примечание: с помощью этой ф-ии нельзя подключить
            //	            std::time_get<> и std::time_put<> фасеты
            //

            template <class _Facet>
            void add_facet()
            {
#ifdef _WIN32
                  // Добавим в переданную локаль Фасет заданный в параметре шаблона
                  internal_locale = std::locale(internal_locale, new _Facet(std::_Locinfo(name_locale.c_str())));
#elif __linux__
                  if constexpr (std::is_same_v<_Facet, std::ctype<char16_t>>)
                  {
                        // Добавим в переданную локаль Фасет заданный в параметре шаблона
                        internal_locale = std::locale(internal_locale, new _Facet(internal_locale));
                  } else
                  if constexpr (std::is_same_v<_Facet, std::numpunct<char16_t>> ||
                                std::is_same_v<_Facet, tlib::numpunct<char>> )
                  {
                        internal_locale = std::locale(internal_locale, new _Facet(newlocale(LC_ALL_MASK, name_locale.c_str(), (locale_t)0)));             
                  } else
                  if constexpr (std::is_same_v<_Facet, std::moneypunct<char16_t>> ||
                                std::is_same_v<_Facet, std::__timepunct<char16_t>> ||
                                std::is_same_v<_Facet, tlib::moneypunct<char, false>>)
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
            //  Имена локалей имеют формат: "", "C", "POSIX" или "language[_area[.codepage]]" или ".codepage"
            //  Например: "Russian_Russia.866", "", "C", ".1251"

            std::string locale_name_fix_codepage(std::string_view loc_name);

            void add_support_char16()
            {
                  add_facet<std::ctype<char16_t>>();
                  add_facet<std::collate<char16_t>>();                                  
#ifdef _WIN32
                  add_facet<tlib::numpunct<char16_t>>();       // need windows
                  add_facet<tlib::time_put<char16_t>>();   // need windows
                  add_facet<tlib::moneypunct<char16_t>>();     // need windows
#elif __linux__
                  add_facet<std::numpunct<char16_t>>();
                  add_facet<std::__timepunct<char16_t>>();
                  add_facet<std::moneypunct<char16_t>>();      
#endif
                  add_facet<std::num_put<char16_t>>();
                  add_facet<std::num_get<char16_t>>();
                  add_facet<std::time_put<char16_t>>();
                  add_facet<std::time_get<char16_t>>();
                  add_facet<std::money_get<char16_t>>();
                  add_facet<std::money_put<char16_t>>();                 
            }

            void add_fix_other_facets()
            {
#ifdef _WIN32
                  add_facet<tlib::moneypunct<wchar_t>>();
                  add_facet<tlib::moneypunct<char, false>>();
#elif __linux__
                  add_facet<tlib::numpunct<char>>();
                  add_facet<tlib::moneypunct<char,false>>();
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
