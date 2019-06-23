//////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Файл CLParser.cpp - содержит реализацию класса command_line_t, который инкапсулирует
//                        функциональность разбора командной строки. Так же в этом файле
//                        содержится объявление и реализация класса GetOpt, который является оберткой
//                        вокруг GNU ф-ии _getopt_internal(), которая и выполняет всю черновую работу по
//                        разбору командной строки.
//
//    Версия файла 1.01  - внесены незначительные изменения
//
//

#include "stdafx.h"
#include <Tcl_parser.h>
#include <Tstring.h>
#include <Tiostream.h>
#include <Tregex.h>
#include <Tregex.h>
#include <third_party_library/expected.hpp>

using namespace std;
using namespace tlib;

namespace tlib::parser
{
      //=====================================================================================================
      /*         class GetOpt   - является оберткой вокруг переделанной GNU ф-ии _getopt_internal().
 *                          Эта функция была тщательно переделана. На текущий момент ф-ия _getopt_internal()
 *                          и класс GetOpt, реализуют PERMUTE ф-иональность прежней GNU ф-ии.
 *                          Это значит, что в процесе анализа ARGV мы переставляем содержимое ARGV,
 *                          так, что бы все неопции оказались в конце списка ARGV. Это позволяет задавать опции
 *                          в любом порядке.
 */
      class command_line_t::get_option_t
      {
        private:
            /////////////////////////////////////////////////////////////////////////////////////////
            //Если процесс сканирование коротких опций еще не закончен, то true

            bool status_scan_short_option;

            /////////////////////////////////////////////////////////////////////////////////////////
            // struct Option    - задает формат длинной опции

            struct Option
            {
                  tlib::tstring name; // Имя опции
                  _argtype has_arg;   // Задаем необходимость параметров
            };

            // long_opts_array   - список форматов длинных опций
            vector<Option> long_opts_array;

            /////////////////////////////////////////////////////////////////////////////////////////
            // Если не опции предшествуют опциям, меняем их местами
            // Это индексы параметров неопций которые были пропущены и нуждаются в перестановке.

            size_t first_nonopt; // Index в ARGV первого из этих параметров
            size_t last_nonopt;  // Index в ARGV последнего из этих параметров

            /////////////////////////////////////////////////////////////////////////////////////////
            // Функция, которая меняет местами опции и не опции

            
            void exchange(vector<tstring>& argv)
            {
                  /* Поменять две смежных подпоследовательности ARGV.
                     Одна подпоследовательность - множество элементов [first_nonopt,last_nonopt),
                     который содержит все неопции, которые были пока пропущены.
                     Другое множество элементов - это [last_nonopt, optindex), которое содержит все
                     обработанные опции, так как выше описанные неопции были пропущены.

                     Перемещение осуществляется так, что после перемещения 'first_nonopt' и 'last_nonopt'
                     содержат новые значения индексов неопций в последовательности ARGV, полученных после
                     перемещения.
                  */
                  size_t nonopts_size = last_nonopt - first_nonopt;

                  /*Interchange the two blocks of data in ARGV.  */
                  tstring* temp = new tstring[nonopts_size];

                  for (size_t i = 0; i < nonopts_size; i++)
                  {
                        temp[i] = argv[first_nonopt + i];
                  }

                  for (size_t i = 0; i < (optindex - last_nonopt); i++)
                  {
                        argv[first_nonopt + i] = argv[last_nonopt + i];
                  }

                  for (size_t i = 0; i < nonopts_size; i++)
                  {
                        argv[first_nonopt + optindex - last_nonopt + i] = temp[i];
                  }


                  /* Update records for the slots the non-options now occupy.  */

                  first_nonopt += (optindex - last_nonopt);
                  last_nonopt = optindex;
            }


            /////////////////////////////////////////////////////////////////////////////////////////
            // Введем функцию, которая определяет является ли переданный символ, символом разделителем
            // Возвращает true если переданный символ, это символ разделитель

            bool is_separator_TCHAR(TCHAR Ch)
            {
                  return (Ch == _T('-') || Ch == this->separator_char);
            }

            /////////////////////////////////////////////////////////////////////////////////////////
            // Группа функция анализирующая опции
            
            // analiz_initialisation() - предварительный анализ ARGV и вызов exchange() при неоходимсти
            bool analiz_initialisation(vector<tstring>& argv)
            {
                  /////////////////////////////////////////////////////////////////////////////////////////
                  // Введем функцию, которая определяет переданная строка содержит опцию или нет
                  // Возвращает true если строка это не опция

                  auto is_none_option = [&](tstring& ArgStr)
                  {
                        return (!is_separator_TCHAR(ArgStr[0]) || ArgStr[1] == _T('\0'));
                  };


                  // Если  у нас есть неопции в начале argv, то их нужно переставить после всех опций в argv
                  {
                        // Если мы только что обработали некоторые опции, которые распологаются
                        // после некоторых неопций, поменяем их местами так, чтобы опции были
                        // на первом месте.

                        if (this->first_nonopt != this->last_nonopt && this->last_nonopt != this->optindex)
                              exchange(argv);
                        else if (this->last_nonopt != this->optindex)
                              first_nonopt = optindex;

                        // Теперь пропустим любые дополнительные неопции и расширим диапазон ранее пропущенных неопций.

                        while (this->optindex < argv.size() && is_none_option(argv[this->optindex]))
                              this->optindex++;

                        this->last_nonopt = this->optindex;
                  }

                  //  Специальный ARGV-элемент '--' означает преждевременный конец опций.
                  //  Пропускаем его, как нулевую опцию, затем меняем местами с предыдущими
                  //  неопциями, как будто элемент '--' была опция. Затем останавливаем сканирование.
                  //  Пропуская все остальное, как неопции. В результате все опции и неопции, что стоят после '--'
                  //  попадут в список не распознаных опций NotIdentifyArg, как и все неопции, что стояли до '--'

                  if (this->optindex != argv.size() && argv[this->optindex] == _T("--"))
                  {
                        this->optindex++;

                        if (this->first_nonopt != this->last_nonopt && this->last_nonopt != this->optindex)
                              exchange(argv);
                        else if (this->first_nonopt == this->last_nonopt)
                              this->first_nonopt = this->optindex;

                        this->last_nonopt = argv.size();
                        this->optindex = argv.size(); // optindex  на конец списка, что приведет к окончанию сканирования
                  }

                  // Если мы обработали все ARGV-элементы, остановим сканирование
                  // this->optindex будет указывать на первую не опцию

                  if (this->optindex == argv.size())
                  {
                        // Установим optindex на первую  неопцию, которые мы ранее
                        // пропустили, таким образом вызывающая программа сможет получить их.
                        if (this->first_nonopt != this->last_nonopt)
                              this->optindex = this->first_nonopt;

                        return false;
                  }

                  // продолжаем оработку
                  return true;
            }


            // AnalizLongOption()     - распознавание длинных опций
            tlib::tstring analiz_long_option(tstring& OptionStr, tstring_view ParamStr, vector<Option>::iterator* ptrLongOptInd)
            {
                  // Очистим строку опции от лидирующих разделителей "-" или "--" или "/"
                  tstring OptionStrWithoutSeparator = OptionStr.substr(is_separator_TCHAR(OptionStr[0]) + is_separator_TCHAR(OptionStr[1]));

                  // Найдем позицию символа "=", отделяющего Имя опции от Параметра опции
                  size_t PosSeparator = OptionStrWithoutSeparator.find_first_of(_T('='));

                  // Получим имя текущей опции
                  tstring NameOptionStr = OptionStrWithoutSeparator.substr(0, PosSeparator);

                  // Получим параметр текущей опции
                  tstring ParamOptionStr = (PosSeparator != npos) ? OptionStrWithoutSeparator.substr(PosSeparator + 1) : tstring();


                  bool ambig = false; // Флаг устанавливается в true если найденная опция двусмысленна
                  bool exact = false; // Флаг устанавливается в true если найденная опция строго
                                      // соответствует одной из опций в списке

                  vector<Option>::iterator ptrFoundOption = this->long_opts_array.end();

                  // Тестируем текущую опцию на точное или на сокращенные соответствие одному из
                  // шаблонов длинных опций из long_opts_array
                  for (auto ptrOption = this->long_opts_array.begin(); ptrOption != this->long_opts_array.end(); ptrOption++)
                  {
                        // Если текущая опция NameOptionStr является началом шаблона длинной опции ptrOption
                        if (ptrOption->name.find(NameOptionStr) == 0)
                        {
                              if (ptrOption->name.length() == NameOptionStr.length()) // и если их длинна совпадает
                              {
                                    ptrFoundOption = ptrOption;
                                    exact = true; // Строгое соответствие найдено.
                                    break;        // Дальше можно не сканировать
                              }
                              else if (ptrFoundOption == this->long_opts_array.end())
                              {
                                    ptrFoundOption = ptrOption; // Первое не строгое соответствие найдено.
                              }
                              else
                                    ambig = true; // Второе не строгое соответствие найдено.
                        }
                  }

                  //В случае не строго соответствия выведем ошибку
                  if (ambig && !exact)
                  {
                        optindex++;

                        command_line_t::unknown_otion_t unknown_otion;
                        unknown_otion.id = command_line_t::ambiguous_opt;
                        unknown_otion.name = OptionStr;
                        throw unknown_otion;
                  }

                  //Если опция определена, получим для нее параметры.
                  if (ptrFoundOption != this->long_opts_array.end())
                  {
                        //Указываем на следующий элемент, возможно параметр опции
                        optindex++;

                        //Если у нас остался не обработанный параметр опции, в строке опции
                        if (!ParamOptionStr.empty())
                        {
                              if (ptrFoundOption->has_arg != no_argument) //Если параметр необходим
                                    optarg = ParamOptionStr;
                              else
                              {
                                    //Если параметр есть, но он не нужен
                                    command_line_t::unknown_otion_t unknown_otion;
                                    unknown_otion.id = command_line_t::not_need_arg;
                                    unknown_otion.name = OptionStr.substr(0, OptionStr.find_first_of(_T('=')));
                                    throw unknown_otion;
                              }
                        }
                        else
                            //Если в строке опции нет параметра, но он необходим для этой опции
                            if (ptrFoundOption->has_arg != no_argument)
                        {
                              switch (ptrFoundOption->has_arg)
                              {
                              case required_argument: // Параметр обязателен
                                    if (ParamStr != _T(""))
                                    {
                                          optarg = ParamStr; // Получаем параметр
                                          optindex++;        // переходим к следующему элементу
                                    }
                                    else
                                    { // Его нет, ошибка
                                          command_line_t::unknown_otion_t unknown_otion;
                                          unknown_otion.id = command_line_t::requires_arg;
                                          unknown_otion.name = OptionStr;
                                          throw unknown_otion;
                                    }
                                    break;

                              case optional_argument: // Параметр не обязателен
                                    if (ParamStr != _T(""))
                                    {
                                          optarg = ParamStr; // Получаем параметр
                                          optindex++;        // переходим к следующему элементу
                                    }
                              default:
                                    break;
                              }
                        }

                        // Параметры получены успешно, опция определена,
                        // вернем в ptrLongOptInd указатель на правило в long_opts_array, которое
                        // определило опцию.
                        if (ptrLongOptInd != NULL)
                              *ptrLongOptInd = ptrFoundOption;
                        // И вернем символ найденной опции
                        return ptrFoundOption->name;
                  }
                  // Опция не найдена, вернем 0, что позволит попытаться
                  // проанализировать опцию, как короткую опцию, если short_opts_str определена
                  return {};
            }

            // analiz_short_option()    - распознавание коротких опций
            tlib::tstring analiz_short_option(tstring& ArgStr, tstring_view ParamStr)
            {
                  // Короткие символы могут быть заданы или так “-o –t –i –o –n” или так “-otion”
                  // Мы должны пробежаться по строке OptionStr, и проанализировать все
                  // входящие в нее символы, не увеличивая optindex пока не дойдем до конца OptionStr
                  // Что бы сохранить состояние ф-ии при повторном входе в нее, и
                  // возобновить сканирование строки с места, где мы его прервали,
                  // зададим указатель nextchar и прочее, как статическии переменные ф-ии.
                  // Сканирование продолжаем пока nextchar не укажет на конец OptionStr.

                  // Инициализация статических переменных будет выполнена один раз
                  // при первом вызове AnalizShortOption()

                  static tstring OptionStr = ArgStr;                        // Скопируем в статическую переменную, строку с опцией
                  static tstring::iterator BeginArgStr = OptionStr.begin(); // Зададим начало опциональной строки
                  static tstring::iterator EndArgStr = OptionStr.end();     // Зададим конец опциональной строки
                  static tstring::iterator nextchar = BeginArgStr + 1;      // опции начинающиеся на '--' этой ф-ией мы не обрабатываем.

                  // Если нам передали новую группу коротких опций
                  if (OptionStr != ArgStr)
                  {
                        OptionStr = ArgStr;
                        BeginArgStr = OptionStr.begin();
                        EndArgStr = OptionStr.end();
                        nextchar = BeginArgStr + 1;
                  }

                  status_scan_short_option = true;

                  //Прочитаем новый символ опции из строки опций, и найдем его в short_opts_str
                  TCHAR c = *nextchar++;
                  size_t IndexShOtion = short_opts_str.find(c);

                  // Увеличим значение 'optindex', когда мы дошли до последнего символа OptionStr.
                  if (nextchar == EndArgStr)
                  {
                        ++optindex;
                        status_scan_short_option = false;
                  }

                  //Еслии символ опции не найден вернем ошибку
                  if (IndexShOtion == npos || c == _T(':')) //':' не может быть использована, как короткий символ опции
                  {
                        command_line_t::unknown_otion_t unknown_otion;
                        unknown_otion.id = command_line_t::unrecognized_opt;
                        unknown_otion.name = tstring(_T("-")) + c;
                        throw unknown_otion;
                  }

                  // Если опции нужен параметр получим его
                  if (short_opts_str[IndexShOtion + 1] == _T(':'))
                  {
                        if (short_opts_str[IndexShOtion + 2] == _T(':'))
                        {
                              /* Эта опция требует не обязательные параметры.  */

                              // Если параметров нет у опции
                              if (!(nextchar != EndArgStr && nextchar[0] == _T('=')) &&
                                  !(nextchar == EndArgStr && ParamStr != _T("") && !is_separator_TCHAR(ParamStr[0])))
                                    optarg.clear();

                              // Если параметры опции записаны через пробел, то это последний символ опции и следовательно
                              // мы уже увеличивали `optindex'; увеличим его еще раз после получения параметра
                              if (nextchar == EndArgStr && ParamStr != _T("") && !is_separator_TCHAR(ParamStr[0]))
                              {
                                    optarg = ParamStr;
                                    optindex++;
                              }

                              // Если параметры опции записаны через равно
                              if (nextchar != EndArgStr && nextchar[0] == _T('='))
                              {
                                    optarg = OptionStr.substr(nextchar - BeginArgStr + 1);
                                    optindex++;
                                    nextchar = EndArgStr;
                                    status_scan_short_option = false;
                              }
                        }
                        else
                        {
                              /* Эта опция требует обязательные параметры */

                              //Если параметр задан через символ равно
                              if (nextchar != EndArgStr && nextchar[0] == _T('='))
                              {
                                    optarg = OptionStr.substr(nextchar - BeginArgStr + 1);
                                    optindex++;
                              }

                              // Если параметры опции записаны через пробел, то это последний символ опции и следовательно
                              // мы уже увеличивали `optindex'; увеличим его еще раз после получения параметра
                              if (nextchar == EndArgStr && ParamStr != _T(""))
                              {
                                    optarg = ParamStr;
                                    optindex++;
                              }

                              //В случае если у опции нет параметра
                              if (!(nextchar != EndArgStr && nextchar[0] == _T('=')) &&
                                  !(nextchar == EndArgStr && ParamStr != _T("")))
                              {
                                    command_line_t::unknown_otion_t unknown_otion;
                                    unknown_otion.id = command_line_t::requires_arg;
                                    unknown_otion.name = tstring(_T("-")) + c;
                                    throw unknown_otion;
                              }
                              nextchar = EndArgStr;
                              status_scan_short_option = false;
                        }
                  }
                  return { 1, c }; //Вернем найденный символ
            }



        public:
            /////////////////////////////////////////////////////////////////////////////////////////
            //   Если  ShowErrorFlag == true, то выводим в cerr сообщение об ошибках (по умолчанию)
            //   Если  ShowErrorFlag == false, то не выводим

            bool show_error_flag;

            /////////////////////////////////////////////////////////////////////////////////////////
            //   Основной символ разделитель опций '-'. В эту переменную можно записать дополнительный
            //   символ разделитель, который отделяет одну опцию от другой. По умолчанию конструктор
            //   задает дополнительный символ разделитель равным '/'.

            TCHAR separator_char;

            /////////////////////////////////////////////////////////////////////////////////////////
            //  Содержит строку в которой описан формат коротких опций

            tstring short_opts_str;

            /////////////////////////////////////////////////////////////////////////////////////////
            // В эту переменную помещается распознаный параметр опции, если _getopt_internal() его находит

            tstring optarg;

            /////////////////////////////////////////////////////////////////////////////////////////
            // Если мы не можем распознать опцию, или у опции нет обязательного параметра
            // То код ошибки или имя проблемной опции возвращается в эту структуру,
            // в момент когда _getopt_internal() возвращает 0;

            command_line_t::unknown_otion_t unknown_otion;

            /////////////////////////////////////////////////////////////////////////////////////////
            //    Индекс в ARGV следующего элемента, который сканируется.
            //    Когда cканирование закончено и _getopt_internal() возвращает EOF
            //    optindex - содержит значение индекса первого из элементов которые не являются опцией,
            //    после сканирования все неопции будут размещены в конце ARGV
            //    Не опции вызывающая программа должна сканировать самостоятельно.

            size_t optindex;

            /////////////////////////////////////////////////////////////////////////////////////////
            //    Конструктор класса. Инициализируем данные необходимые для работы ф-ии_getopt_internal()
            //    Начинаем обрабатывать опции с ARGV-элемента 1. так как ARGV-элемент 0 является именем программы

            get_option_t()
            {
                  first_nonopt = last_nonopt = optindex = 1;
                  status_scan_short_option = false;
                  show_error_flag = true;
                  separator_char = _T('/');
                  short_opts_str = _T("");
            }

            /////////////////////////////////////////////////////////////////////////////////////////
            //    Основная функция анализирующая ARGV элементы. При каждом вызове возвращает либо символьный
            //    Псевдоним короткой опции, либо 0 в случае ошибки распознавания, либо EOF в случае если все распознано
            enum class status_get_option
            {
                  options_over,
                  unknoun_option
            };
            
            ////////////////////////////////////////////////////////////////////////////////////////////////
            /*
               На вход ф-ии _getopt_internal() подается:
                  vector<tstring>& argv    - список аргументов, для анализа
                  Order ordering           - метод обработки аргументов
                      iterator* ptrLongOptInd  - сюда возвращаем указатель на правило
                                                 из списка форматов длинных опций,
                                                 которое распознало опцию.

               Если '_getopt_internal()' неоднократно вызывается, то при каждом вызове эта ф-ия возвращает
               последовательно, короткие символы опций для каждой найденной опции.

               Функция _getopt_internal() работает с членами класса GetOpt.

               Функция _getopt_internal() вызывает три функции члена класса GetOpt:
                  AnalizInitialisation()   - Предварительная обработка argv, и упорядочение ее элементов
                      AnalizLongOption()       - Анализ длинных оппций
                      AnalizShortOption()      - Анализ коротких оппций

               Если '_getopt_internal()' находит следующий символ опции, он возвращает этот символ,
               обновляя 'GetOpt::optindex', если обрабатывается короткие опции, то эта ф-ия возвращает
               символ короткой опции, и 'GetOpt::optindex' не изменяется между вызовами '_getopt_internal()'
               пока все короткие опции в одном элименте argv, не будут проанализированы.

               Если нет больше символов опции, '_getopt_internal()' возвращает `EOF'.
               'GetOpt::optindex' - при этом будет указывать на первый символ не опцию.
               (ф-ия analiz_initialisation() перставит argv-элементы так, чтобы все неопции
                оказались в конце списка опций)

               Если найдена опция, которая не может быть распознана, ни как короткая опция, ни как длинная,
               или возникла иная ошибка, то ф-ия '_getopt_internal()' возвращает 0. Иноформация об ошибке
               помещается в OptOpt. И вывводим сообщение об ошибке, если 'ShowErrorFlag' установлен в true.

               Если у опции есть параметры они будут возвращены в 'optarg', иначе 'optarg'
               устанавливается в нуль.

            */
            nonstd::expected<tlib::tstring, status_get_option> _getopt_internal(vector<tstring>& argv, vector<Option>::iterator* ptrLongOptInd)
            {
                  this->optarg.clear();

                  //----------------1-й IF----------------------
                  //  analiz_initialisation() анализирует ordering, и выполняет соответсвующии дейcтвия
                  //  Выборка следующего элемента argv для сканирования.
                  //  analiz_initialisation() переставляет местами строки в argv,
                  //  в случае если необходимо, что бы все неопции были в конце
                  //  Если опция это '--' analiz_initialisation() прерывает сканирование

                  if (!status_scan_short_option)
                  {
                        bool ret = analiz_initialisation(argv);
                        if (!ret)
                              return nonstd::make_unexpected(status_get_option::options_over); // опций больше нет - венем EOF
                  }

                   /////////////////////////////////////////////////////////////////////////////////////////
                  // Введем функцию, которая определяет начинается ли опция с "--" или  другого двойнго разделителя
                  // Возвращает true если опция имеет длинный разделитель

                  auto id_loong_separator = [&](tstring& ArgStr)
                  {
                        return is_separator_TCHAR(ArgStr[0]) && is_separator_TCHAR(ArgStr[1]);
                  };

                  // Если во время анализа опций возникает ошибка, отловим ее
                  try
                  {
                        //----------------2-й IF-----------------------
                        // Интерпретируем текущую опцию как длинную опцию.
                        // Это возможно если задан long_opts_array, и опция задана, как длинная '--'
                        // или если опция не начинается с символа короткой опции.

                        if ( !status_scan_short_option && !this->long_opts_array.empty() &&
                            ( id_loong_separator(argv[optindex]) ||
                              argv[optindex].size() > 2 ||
                              short_opts_str.find(argv[optindex][1]) == npos
                            )
                           )
                        {
                              tstring empty_param(_T(""));
                              // Проверим существует ли вероятный параметр опции,
                              // или у нас последняя опция в списке или параметр пропущен и далее идет другая опция
                              tstring_view ParamStr = ((optindex + 1) < argv.size() &&
                                                          !is_separator_TCHAR(argv[this->optindex + 1][0]))
                                                          ? argv[this->optindex + 1]
                                                          : empty_param;

                              tlib::tstring ret = analiz_long_option(argv[this->optindex], ParamStr, ptrLongOptInd);
                              // AnalizLongOption() возвращает пустую строку, если опция не была найдена среди длинных опций
                              // Если пустая строка, значит попытаемся искать среди коротких опций, иначе опция определена,
                              // И мы возвращаем символ найденной опции.
                              if (!ret.empty())
                                    return ret;
                        }

                        //-----------------3-й IF ----------------------
                        // Интерпретируем опцию, как короткую опцию.
                        // Мы последовательно пройдемся по всем символам опции, на предмет выявления в них коротких опций
                        // optindex не будет увеличен пока, мы не дойдем до последнеко символа текущей опции

                        if (!short_opts_str.empty() &&
                            !is_separator_TCHAR(argv[this->optindex][1]) &&
                            short_opts_str.find(argv[this->optindex][1]) != npos)
                        {
                              tstring empty_param(_T(""));
                              // Если short_opts_str не пустая строка и текущая опция не начинается с '--'
                              // продолжим сканировать argv[this->optindex], на предмет поиска коротких символов
                              tstring_view ParamStr = ((optindex + 1) < argv.size() &&
                                                          !is_separator_TCHAR(argv[this->optindex + 1][0]))
                                                          ? argv[this->optindex + 1]
                                                          : empty_param;
                              // Cмотрим и обработаем следующую символ опции
                              return analiz_short_option(argv[this->optindex], ParamStr);
                        }
                        else
                        {
                              // Не возможно найти текущую опцию как длинную опцию и как короткую опцию
                              command_line_t::unknown_otion_t unknown_otion;
                              unknown_otion.id = command_line_t::unrecognized_opt;
                              unknown_otion.name = argv[this->optindex];
                              optindex++;
                              throw unknown_otion;
                        }
                  }
                  catch (command_line_t::unknown_otion_t error_option)
                  {
                        // При необходимости выведим сообщение об ошибке
                        if (show_error_flag)
                        {
                              tstring ErrorMsg;
                              switch (error_option.id)
                              {
                              case command_line_t::unrecognized_opt:
                                    ErrorMsg = _T("Unrecognized option '") + error_option.name + _T("'");
                                    break;

                              case command_line_t::ambiguous_opt:
                                    ErrorMsg = _T("Оption '") + error_option.name + _T("' is ambiguous");
                                    break;

                              case command_line_t::not_need_arg:
                                    ErrorMsg = _T("Option `") + error_option.name + _T("' doesn't allow an argument");
                                    break;

                              case command_line_t::requires_arg:
                                    ErrorMsg = _T("Option `") + error_option.name + _T("' requires an argument");
                                    break;
                              }

                              tcerr << argv[0] << _T(": ") << ErrorMsg << _T("\n");
                        }

                        unknown_otion = error_option;

                        return nonstd::make_unexpected(status_get_option::unknoun_option); //Вернем пустую строку в случае ошибки
                  }
            }

            /////////////////////////////////////////////////////////////////////////////////////////
            //    Функия, которая инициализирует список содержащий формат длинных опций.

            void add_format_of_option(tlib::tstring name, _argtype has_arg)
            {
                  Option Temp_Otion;
                  Temp_Otion.name = name;
                  Temp_Otion.has_arg = has_arg;
                  long_opts_array.push_back(Temp_Otion);
            }
      };

      ////////////////////////////////////////////////////////////////////////////////////////////////
      //
      //                             Реализация класса     command_line_t
      //

      command_line_t::command_line_t(void)
      {
            get_option = new get_option_t;
      }

      command_line_t::~command_line_t(void)
      {
            delete get_option;
      }


      void command_line_t::set_show_error(bool ShowErrorFl)
      {
            get_option->show_error_flag = ShowErrorFl;
      }

      void command_line_t::set_separator_char(TCHAR Ch)
      {
            get_option->separator_char = Ch;
      }

      void command_line_t::set_short_format(tlib::tstring Str)
      {
            get_option->short_opts_str = Str;
      }

      void command_line_t::add_format(tlib::tstring name, _argtype has_arg)
      {
            get_option->add_format_of_option(name, has_arg);
      }

      // Две функции parse() с разными входными параметрами
      template <class _Elem>
      void command_line_t::parse(int argc, _Elem* argv[])
      {
            // сконвертируем стандартный вид списка аргументов ф-ии main()
            // в аналогичный список строк tstring
            vector<tstring> ArgV_p;
            ArgV_p.resize(argc, _T(""));

            for (int i = 0; i < argc; i++)
            {
                  ArgV_p[i] = templateStr_tstr(argv[i]);
            }

            parse(ArgV_p);
      }

      template void command_line_t::parse(int argc, char* argv[]);
      template void command_line_t::parse(int argc, wchar_t* argv[]);

      template <class _Elem>
      void command_line_t::parse(const std::basic_string<_Elem>& ArgV_str, bool bProgramName)
      {
            // 1) Конвертируем строку параметров ArgV_str из произвольного типа в TempStr типа tstring
            // 2) Пробежимся по строке TempStr и разобьем ее на подстроки с помощью tregex
            //    либо подстрока в двойных кавычках \"[^\"]+\"
            //    либо подстрока в одинарных кавычках \'[^\']+\'
            //    либо подстрока без пробельных символов \\S+
            // 3) Из полученной подстроки уберем обрамляющие кавычки с помощью лямбды
            // 4) И вставим результирующую строку в конец вектора ArgV_p

            tstring TempStr = templateStr_tstr(ArgV_str);

            vector<tstring> ArgV_p;

            if (bProgramName == false)
                  ArgV_p.push_back(_T(""));

            tregex treg_exp(_T("(\"[^\"]+\"|\'[^\']+\'|[^\\s]+)"));

            transform(tsregex_token_iterator(TempStr.begin(), TempStr.end(), treg_exp, 1),
                tsregex_token_iterator(),
                back_inserter(ArgV_p),
                [](const tstring& str) {
                      if ((str[0] == _T('\"') && str[str.size() - 1] == _T('\"')) ||
                          (str[0] == _T('\'') && str[str.size() - 1] == _T('\'')))
                            return str.substr(1, str.size() - 2);
                      else
                            return str;
                });

            parse(ArgV_p);
      }

      template void command_line_t::parse(const std::string& ArgV_str, bool bProgramName);
      template void command_line_t::parse(const std::wstring& ArgV_str, bool bProgramName);

      void command_line_t::parse(vector<tstring>& ArgV_p)
      {
            using sgo = get_option_t::status_get_option;

            //Разбор коммандной строки
            while (true)
            {
                  auto new_option = get_option->_getopt_internal(ArgV_p, 0);
                  if (new_option.error() == sgo::options_over)
                  {
                        break;
                  }
                  else if (new_option.error() != sgo::unknoun_option)
                  {
                        //Проверим нет ли в списке опций такой опции
                        auto options_iter = options.options_map.find(*new_option);

                        if (options_iter != options.options_map.end())
                        {
                              //Если опция встретилась повторно, добавим новый параметр опции если он есть
                              if (get_option->optarg != _T(""))
                                    options_iter->second.params.args.push_back(get_option->optarg);
                        }
                        else
                        {
                              //Опция встретилась в первый раз, сохраним ее в списке
                              options_t::option_t temp_option;
                              temp_option.name_option = *new_option;
                              temp_option.is_set = true;
                              if (get_option->optarg != _T(""))
                                    temp_option.params.args.push_back(get_option->optarg);
                              options.options_map[*new_option] = temp_option;
                        }
                  }
                  else
                  {
                        //Сохраним ошибочные опции в списке ошибочных опций
                        unknown_otions.vector_non_options.push_back(get_option->unknown_otion);
                  }
            }

            //Сохраним не опции в списке неопций
            if (get_option->optindex < ArgV_p.size())
            {
                  non_options.vector_non_options.clear();
                  non_options.vector_non_options.resize(ArgV_p.size() - get_option->optindex, tstring());
                  for (size_t i = 0; (get_option->optindex + i) < ArgV_p.size(); i++)
                  {
                        non_options.vector_non_options[i] = ArgV_p[get_option->optindex + i];
                  }
            }
      }

}
