//////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Файл CLParser.h   - содержит объявление класса ParserСommandLine, который инкапсулирует
//                        функциональность разбора командной строки.
//
//    Версия файла 1.01  - внесены незначительные изменения
//

#pragma once


#include <Tstring.h>
#include <vector>
#include <map>
#include <algorithm>

namespace tlib::parser
{
      /*
      command_line_t работает с опциями заданными по следующим правилам:

      1)        Опции и не опции могут задаваться в любом порядке.

      2)        “-- ” если встречается этот символ, то дальнейшая обработка строки прекращается
                и все что указано после него считается не именованными параметрами

      3)        “--option=abc” или “--option abc” задает  длинную опцию с параметром

      4)        Короткие опции могут быть заданы или так “-o -p –t –i” или так “-opti”,
                “-opti”    - будет списком коротких опций, если нет аналогичной длинной
                                    опции начинающейся на “-opti” например “-option”
                “-o”       - будет проанализированна, как длинная опция только, если нет
                             такой же короткой опции. Если короткая опция “-o” существует,
                             то анализ этой опции, как длинной не будет выполнен никогда.

      6)        Длинные опции могут быть сокращены, пока такое сокращение однозначно идентифицирует
                опцию. Например, если задан только список длинных опций или если задан список
                        длинных опций и коротких опций то:
                        a.        Если есть длинная опция "fubar", и ARGV-элемент "-fu",
                                Тогда "-fu" распознается, как опция "fubar"
                        b.        Если длинной опции начинающейся на "-fu" нет, это
                                список коротких опций.

      5)        “-option=abc” или “-option abc”
                a.        если задан только список длинных опций, то это длинная опция с параметром.
                b.        если задан только список коротких опций, то это последовательность коротких опций.
                        И опция n имеет параметр “abc”
                c.        если задан список длинных опций и коротких опций то:
                        i.        Если существует длинная опция “-option” - Это длинная опция с параметром.
                        ii.        Иначе это список коротких опций, и опция “n“ имеет параметр “abc”

      6)        Длинные опции могут быть сокращены, пока такое сокращение однозначно идентифицирует
                опцию. Например, если задан только список длинных опций или если задан список
                        длинных опций и коротких опций то:
                        a.        Если есть длинная опция "fubar", и ARGV-элемент "-fu",
                                Тогда "-fu" распознается, как опция "fubar"
                        b.        Если длинной опции начинающейся на "-fu" нет, это
                            список коротких опций.

      7)        Если задана “-o” -короткая опция с обязательным параметром, то следующая строка
                    “-option” ошибочна,  для задания параметра требуется ставить равно или пробел.
                    “-o=ption” или “-o ption” верное задание параметра.

      Символы '-' не являются символами опции, а только разделителями опции, по-этому для задания
          формата опции этот символ не используется.

  Короткие опции задаются строкой следующего формата:
  command_line_t::SetShortFormatOfArg(“ab:c::”);

  Где    а   – имя опции без параметра
         b:  – имя опции с обязательным параметром
         с:: – имя опции с не обязательным параметром

  Формат длинно-именованных опции задается функцией
  command_line_t::AddFormatOfArg(tstring name,              //- длинное имя опции
                                    _argtype has_arg,
                                    TCHAR val);                //-сокращенное имя опции из одного символа

  Где  _argtype это перечисление, и
  'has_arg' может принимать следующие значения:
*/

      enum _argtype
      {
            no_argument,       //  (или 0), если опция без параметра,
            required_argument, //  (или 1), если опция требует параметр,
            optional_argument  //  (или 2), если опция может быть с не обязательным параметром.
      };



      //////////////////////////////////////////////////////////////////////////////////////////////////
      //        Объявляем основной пользовательский класс command_line_t,
      //        который парсит аргументы командной строки
      //

      class command_line_t
      {
        public:
            //////////////////////////////////////////////////////////////////////////////////////////////////
            //        Здесь объявляем вспомогательные классы для представляния
            //        распознаных опций класса command_line_t

            // В случае если command_line_t в процессе работы не смог распознать опцию
            // Не распознаные опции помещаются в массив опций  command_line_t::ErrorOption[];
            // Где каждый элемент этого массива представляет из себя следующую структуру
            enum error_code_e
            {
                  unrecognized_opt, //Опция не распознана
                  ambiguous_opt,    //Не однозначная опция
                  not_need_arg,     //Параметр для опции не нужен
                  requires_arg      //Параметр для опции нужен
            };

            struct unknown_otion_t
            {
                  error_code_e id;    // Код ошибки
                  tlib::tstring name; // Имя опции которая была не распознана
            };

            template <class Type>
            class non_options_t //Шаблон обертка, для списка не распознаных и ошибочных опций
            {
                  friend command_line_t;
                  std::vector<Type> vector_non_options;

              public:
                  const Type& operator[](size_t Index) const
                  {
                        return vector_non_options[Index];
                  }
                  size_t size() const
                  {
                        return vector_non_options.size();
                  }

                  bool empty() const
                  {
                        return vector_non_options.empty();
                  }

                  auto begin() noexcept
                  {
                        return vector_non_options.cbegin();
                  }

                  auto end() noexcept
                  {
                        return vector_non_options.cend();
                  }

                  auto cbegin() const noexcept
                  {
                        return vector_non_options.cbegin();
                  }

                  auto cend() const noexcept
                  {
                        return vector_non_options.cend();
                  }
            };

            class options_t //Эмуляция массива представляющего список распознаных опций
            {
                  friend command_line_t;
                  class option_t;
                  class option_const_iter_t;
                  using options_map_t = std::map<tlib::tstring, option_t>;

              public:
                  using const_iterator = option_const_iter_t;

                  const_iterator begin() noexcept;
                  const_iterator end() noexcept;
                  const_iterator cbegin() const noexcept;
                  const_iterator cend() const noexcept;

                  const option_t& operator[](tlib::tstring_view str);
                  
                  size_t size() const;
                  bool empty() const;

              private:
                  class option_t // Пользовательское представление одной распознаной опции
                  {
                    private:
                        friend command_line_t;
                        friend options_t;

                        class args_of_option // Параметры опции
                        {
                          private:
                              friend command_line_t;
                              friend options_t;

                              std::vector<tlib::tstring> args;

                              tlib::tstring empty_string = _T("");

                          public:
                              using const_iterator = std::vector<tlib::tstring>::const_iterator;
                              const_iterator begin() noexcept;
                              const_iterator end() noexcept;
                              const_iterator cbegin() const noexcept;
                              const_iterator cend() const noexcept;

                              operator bool() const;
                              size_t size() const;
                              const tlib::tstring& operator[](size_t n) const;
                        };

                        bool is_set = false;
                        tlib::tstring name_option;

                    public:
                        args_of_option params;

                        operator bool() const;
                        tlib::tstring name() const;
                  };

                  class option_const_iter_t
                  {
                        using internal_iter_t = options_map_t::const_iterator;
                        internal_iter_t current_node;

                    public:
                        using iterator_category = std::bidirectional_iterator_tag;
                        using value_type = option_t;
                        using difference_type = ptrdiff_t;
                        using pointer = option_t*;
                        using reference = option_t&;

                        option_const_iter_t() noexcept : current_node() {}
                        option_const_iter_t(internal_iter_t node) noexcept : current_node(node) {}
                        option_const_iter_t(const option_const_iter_t& iter) noexcept : current_node(iter.current_node) {}
                        option_const_iter_t& operator=(const option_const_iter_t& iter) noexcept;

                        const option_t* operator->();
                        const option_t& operator*() noexcept;
                        bool operator==(const option_const_iter_t& iter) const noexcept;
                        bool operator!=(const option_const_iter_t& iter) const noexcept;
                        option_const_iter_t& operator++() noexcept;
                        option_const_iter_t operator++(int);
                  };
              private:
                  options_map_t options_map;
                  option_t empty_option;
            };

            //////////////////////////////////////////////////////////////////////////////////////////////////
            //
            //        Интерфейс класса command_line_t

            //Список неопций
            non_options_t<tlib::tstring> non_options;
            //Список ошибочных опций
            non_options_t<unknown_otion_t> unknown_otions;
            //Список опций
            options_t options;

            //Конструктор и деструктор
            command_line_t(void);
            ~command_line_t(void);

            // Установим флаг вывода ошибок
            void set_show_error(bool ShowErrorFl);

            // Установим дополнительный символ разделитель опций
            void set_separator_char(TCHAR Ch);

            // Установим формат коротких опций
            void set_short_format(tlib::tstring Str);

            // Добавим формат для очердной длинной опции
            void add_format(tlib::tstring name, _argtype has_arg);

            // Две функции parse() с разными входными параметрами
            template <class _Elem>
            void parse(int argc, _Elem* argv[]);

            void parse(std::vector<tlib::tstring>& ArgV_p);

            // Еще одна функция parse(), которая принимает для разбра строку
            // bProgramName, этот параметр задает правила интерпритации первой опции в строке ArgV_str:
            // Если  bProgramName == true, то первая опция в строке это имя программы,
            //                             эта опция не будет проанализирована
            // Если  bProgramName == false, то в начале строки отсутствует путь и имя программы,
            //                              и первый аргумент строки будет включен в анализ опций
            //void parse(tlib::tstring& ArgV_str, bool bProgramName=true);
            template <class _Elem>
            void parse(const std::basic_string<_Elem>& ArgV_str, bool bProgramName = true);

        private:
            class get_option_t;
            get_option_t* get_option;
      };

      extern template void command_line_t::parse(int argc, char* argv[]);
      extern template void command_line_t::parse(int argc, wchar_t* argv[]);
      extern template void command_line_t::parse(const std::string& ArgV_str, bool bProgramName);
      extern template void command_line_t::parse(const std::wstring& ArgV_str, bool bProgramName);
}




#if !(defined(__linux__) || defined(_LIB))

#define _LIB_NAME "tlib_parsers"

#include <win/select_lib.h>

#undef _LIB_NAME

#endif //!(defined(_LIB))

