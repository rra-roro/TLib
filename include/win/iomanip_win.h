#pragma once

#include <Windows.h>
#include <Tstring.h>
#include <Tiostream.h>
#include <type_traits>
#include <Ttype.h>
#include <time.h>

namespace tlib
{
      // Манипулятор tab_right()
      // Выводит строку str_in, так что бы в случае если она не уместится
      // на одной строке экрана, на новой строке экрана она выводилась строго
      // под началом первой строки. Как бы колоночку формирует.
      template <class _Elem>
      class tab_right_t
      {
        public:
            using string_type = std::basic_string<_Elem>;
            using string_view_type = std::basic_string_view<_Elem>;

            tab_right_t(string_view_type str) : tmp_str(str){};

            friend inline std::basic_ostream<_Elem>& operator<<(std::basic_ostream<_Elem>& os, const tab_right_t& tr)
            {
                  return tr.print_tab_right(os);
            }

        private:
            const string_view_type tmp_str;

            inline COORD get_cursor_position() const
            {
                  HANDLE console_output_h = GetStdHandle(STD_OUTPUT_HANDLE);
                  CONSOLE_SCREEN_BUFFER_INFO csbi_info;
                  GetConsoleScreenBufferInfo(console_output_h, &csbi_info);
                  return csbi_info.dwCursorPosition;
            }

            inline COORD get_size_window() const
            {
                  HANDLE console_output_h = GetStdHandle(STD_OUTPUT_HANDLE);
                  CONSOLE_SCREEN_BUFFER_INFO csbi_info;
                  GetConsoleScreenBufferInfo(console_output_h, &csbi_info);
                  return csbi_info.dwSize;
            }

            std::basic_ostream<_Elem>& print_tab_right(std::basic_ostream<_Elem>& iostr) const
            {
                  string_type str = tmp_str.data();
                  COORD cur_coord = get_cursor_position();
                  COORD size_window = get_size_window();
                  size_t lenght_str = size_window.X - cur_coord.X - 1;

                  //Удаление повторяющихся пробелов
                  while (true)
                  {
                        size_t pos = str.find(TemplateTypeOfStr("  ", _Elem));
                        if (pos == npos)
                              break;
                        str.replace(pos, 2, TemplateTypeOfStr(" ", _Elem));
                  }

                  // Разбиваем строку на длинну оставшуюся до конца экрана,
                  // и добавляем лидирующие прбелы для выравниния
                  while (str.size() > lenght_str)
                  {
                        const _Elem space_ch = TemplateTypeOfCh(' ', _Elem);

                        string_type strLeft = str.substr(0, str.find_last_of(space_ch, lenght_str));

                        str = str.substr(str.find_last_of(space_ch, lenght_str) + 1, npos);

                        for (size_t pos = strLeft.find_first_of(space_ch, 0);
                             strLeft.size() < lenght_str;
                             pos = strLeft.find_first_of(space_ch, pos))
                        {
                              if (pos != npos)
                                    strLeft.insert(pos, 1, space_ch);
                              pos += 2;
                        }

                        strLeft = strLeft + TemplateTypeOfStr("\n", _Elem) + string_type(cur_coord.X, space_ch);

                        iostr << strLeft;
                  }

                  return iostr << str;
            }
      };

      extern template tab_right_t<char>;
      extern template tab_right_t<wchar_t>;
      extern template tab_right_t<char16_t>;

      template <class T, class _Elem = get_underlying_char_t<T>>
      constexpr tab_right_t<_Elem> tab_right(T&& str)
      {
            return tab_right_t<_Elem>(std::forward<T>(str));
      }

      // Манипулятор outFILETIME() Превращает FILETIME в строку
      class put_filetime
      {
            const FILETIME& filetime;

        public:
            put_filetime(const FILETIME& filetime_in) : filetime(filetime_in){};

            template <class _Elem>
            friend inline std::basic_ostream<_Elem>& operator<<(std::basic_ostream<_Elem>& os, const put_filetime& oF)
            {
                  return oF.PrintFileTime(os);
            }

        private:
            time_t filetime_to_time_t(const FILETIME& filetime) const
            {
                  // Microseconds between 1601-01-01 00:00:00 UTC and 1970-01-01 00:00:00 UTC
                  static const uint64_t EPOCH_DIFFERENCE_MICROS = 11644473600000000ull;

                  // First convert 100-ns intervals to microseconds, then adjust for the
                  // epoch difference
                  uint64_t total_us = (((uint64_t)filetime.dwHighDateTime << 32) | (uint64_t)filetime.dwLowDateTime) / 10;
                  total_us -= EPOCH_DIFFERENCE_MICROS;

                  // Convert to (seconds, microseconds)
                  return (time_t)(total_us / 1000000);
            }

            template <typename _Elem>
            std::basic_ostream<_Elem>& PrintFileTime(std::basic_ostream<_Elem>& iostr) const
            {
                  std::time_t t = filetime_to_time_t(filetime);
                  std::tm loc_time = {};

                  localtime_s(&loc_time, &t);

                  iostr << std::put_time<_Elem>(&loc_time, TemplateTypeOfStr("%A %c", _Elem));
                  return iostr;
            }
      };

      extern template std::basic_ostream<char>& put_filetime::PrintFileTime(std::basic_ostream<char>& iostr) const;
      extern template std::basic_ostream<wchar_t>& put_filetime::PrintFileTime(std::basic_ostream<wchar_t>& iostr) const;
      extern template std::basic_ostream<char16_t>& put_filetime::PrintFileTime(std::basic_ostream<char16_t>& iostr) const;

}
