#pragma once

#include <string_view>
#include <string>
#include <cstdlib>
#include <Ttype.h>
#include <mutex>
#include <Tstring.h>


namespace tlib::v1
{
      class env
      {
            static std::mutex env_mutex;
                  
        public:
            template <class T,
                      class _Elem = get_underlying_char_t<T>,
                      class string_t = std::basic_string<_Elem>>
            inline static string_t get_var(T&& name)
            {
                  std::lock_guard<std::mutex> env_guard(env_mutex);
#ifdef _WIN32
                  tstring name_str(templateStr_tstr(std::forward<T>(name)));

                  size_t requiredSize = 0;
                  _tgetenv_s(&requiredSize, NULL, 0, name_str.c_str());
                  if (requiredSize == 0)
                        return string_t();

                  tstring value(requiredSize, _T('\0'));
                  _tgetenv_s(&requiredSize, value.data(), value.size(), name_str.c_str());

                  return tstr_templateStr<_Elem>(value);
#elif __linux__
                  return cstr_templateStr<_Elem>(getenv(templateStr_cstr(std::forward<T>(name)).c_str()));
#endif // _WIN32
            }

            template <class T1, class T2,
                      class _Elem1 = get_underlying_char_t<T1>,
                      class _Elem2 = get_underlying_char_t<T2>,
                      typename = std::enable_if_t<std::is_same_v<_Elem1, _Elem2>>
            >
            inline static void set_var(T1&& name, T2&& value)
            {
                  std::lock_guard<std::mutex> env_guard(env_mutex);
#ifdef _WIN32
                  _tputenv_s(templateStr_tstr(std::forward<T1>(name)).c_str(),
                      templateStr_tstr(std::forward<T2>(value)).c_str());
#elif __linux__
                  setenv(templateStr_cstr(std::forward<T1>(name)).c_str(),
                         templateStr_cstr(std::forward<T2>(value)).c_str(), 1);
#endif // _WIN32
            }

            template <class T>
            inline static void del_var(T&& name)
            {
                  std::lock_guard<std::mutex> env_guard(env_mutex);
#ifdef _WIN32
                  _tputenv_s(templateStr_tstr(std::forward<T>(name)).c_str(), _T("\0"));
#elif __linux__
                  unsetenv(templateStr_cstr(std::forward<T>(name)).c_str());
#endif // _WIN32
            }
      };


}
