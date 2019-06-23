// testCLParser.cpp : Defines the entry point for the console application.
//

// #include "stdafx.h"
#include <Tstring.h>
#include <Tlocale.h>
#include <Tiostream.h>
#include <Tiomanip.h>
#include <CLParser.h>
#include <stdio.h>

#ifdef _WIN32
#include <process.h>
#elif __linux__
#include <spawn.h>
#include <sys/wait.h>
#endif

using namespace std;
using namespace tlib;
using namespace tlib::parser;

void help()
{
      tcout << _T(R"(
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

      8)        Передача параметров программы через: int argc, char *argv[] - отбрасывает кавычки от
                    параметров, таким образом передача: -c "-a" - это не одна опция -с с аргументом "-a",
                    это две опции -с и -а. Это верно и для Windows, и для Linux. К сожалению повлиять 
                    на это поведение мы не можем. 

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


enum _argtype
{
        no_argument,                //  (или 0), если опция без параметра,
        required_argument,          //  (или 1), если опция требует параметр,
        optional_argument           //  (или 2), если опция может быть с не обязательным параметром.
};


В случае если command_line_t в процессе работы не смог распознать опцию
Не распознаные опции помещаются в массив опций  command_line_t::ErrorOption[];
Где каждый элемент этого массива представляет из себя следующую структуру

struct ErrorOpt
{
    enum    ErrorCode
    {
        unrecognized_opt,                // Опция не распознана
        ambiguous_opt,                   // Не однозначная опция
        not_need_arg,                    // Параметр для опции не нужен
        requires_arg                     // Параметр для опции нужен
    };
    ErrorCode  ErrorID;                  // Код ошибки
	tlib::tstring    optopt;          // Имя опции которая была не распознана
};

)") << endl;
}


template <class _Elem>
void my_spawn(const _Elem* cmdname, const char* const argv[])
{
#ifdef _WIN32
      _spawnve(_P_WAIT, templateStr_cstr(cmdname).c_str(), argv, 0);
#else
      string Cmd = templateStr_cstr(cmdname).c_str();
      Cmd = Cmd + " " + string(argv[1]);
      char* chCmd = new char[Cmd.size()];
      Cmd.copy(chCmd, npos);

      const char* const argv_sh[] = { "/bin/sh", "-c", chCmd, NULL };
      int status;
      pid_t child_pid = 0;
      posix_spawn(&child_pid, argv_sh[0], 0, 0, (char* const*)argv_sh, 0);
      waitpid(child_pid, &status, 0);

      delete[] chCmd;
#endif
}


#ifdef _WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
#ifdef _WIN32
      HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_SCREEN_BUFFER_INFO scrBuffInfo = { 0 };
      GetConsoleScreenBufferInfo(hConsole, &scrBuffInfo);

      scrBuffInfo.dwSize.X = 147;
      scrBuffInfo.dwSize.Y = 600;
      SetConsoleScreenBufferSize(hConsole, scrBuffInfo.dwSize);

      scrBuffInfo.srWindow.Right = 146;
      scrBuffInfo.srWindow.Bottom = 50;
      SetConsoleWindowInfo(hConsole, true, &scrBuffInfo.srWindow);
#endif

      InitConsolIO();
      if (argc == 1)
      {
            const char* const my_argv[] = {
                  "testCLParser ",
                  "-t=лоло -u 'шшш' -qrs -z=ooo --x --o=zz -op=io -b 'параметр опции b' -j dddd -c -a \"не опция\" -klm \"-e\" w \"-f w\" -- --help",
                  NULL
            };

            cout << "Запускаем тест " << my_argv[0] << " со следующими параметрами:\n";
            cout << "\n"
                 << Color(green) << my_argv[0] << Color(yellow) << my_argv[1] << Color() << "\n";

            my_spawn(argv[0], my_argv);

            return 0;
      }

      command_line_t PCL;
      PCL.AddFormatOfArg(_T("?"), no_argument);
      PCL.AddFormatOfArg(_T("help"), no_argument);
      PCL.AddFormatOfArg(_T("a"), no_argument);
      PCL.AddFormatOfArg(_T("b"), required_argument);
      PCL.AddFormatOfArg(_T("j"), optional_argument);
      PCL.AddFormatOfArg(_T("c"), optional_argument);
      PCL.AddFormatOfArg(_T("e"), required_argument);
      PCL.AddFormatOfArg(_T("f"), required_argument);
      PCL.AddFormatOfArg(_T("x"), required_argument);
      PCL.AddFormatOfArg(_T("z"), no_argument);
      PCL.AddFormatOfArg(_T("option"), required_argument);
      PCL.AddFormatOfArg(_T("ogogo"), required_argument);
      PCL.SetShortFormatOfArg(_T("k::l::mqrs:t::u:"));

      tcout << _T("\nТаблица поддерживаемых опций:") << _T(R"(
	PCL.AddFormatOfArg( "?",    no_argument,       '?')
	PCL.AddFormatOfArg( "help", no_argument,       '?')
	PCL.AddFormatOfArg( "a",    no_argument,       'a')
	PCL.AddFormatOfArg( "b",    required_argument, 'b')
	PCL.AddFormatOfArg( "j",    optional_argument, 'j')
	PCL.AddFormatOfArg( "c",    optional_argument, 'c')
	PCL.AddFormatOfArg( "e",    required_argument, 'e')
	PCL.AddFormatOfArg( "f",    required_argument, 'f')
	PCL.AddFormatOfArg( "x",    required_argument, 'x')
	PCL.AddFormatOfArg( "z",    no_argument,       'z')
	PCL.AddFormatOfArg( "option", required_argument, 'o')
	PCL.AddFormatOfArg( "ogogo",  required_argument, 'g')
	PCL.SetShortFormatOfArg("k::l::mqrs:t::u:")
)");

      // Отключим вывод ошибок парсером
      PCL.SetShowError(false);

      //Начнем парсить аргументы
#ifdef _WIN32
      PCL.parse((tstring)GetCommandLine());
#else
      PCL.parse(argc, argv);
#endif

      //Если есть опция с ошибкой или запрошен хелп выходим
      if (PCL.options[_T("?")])
      {
            help();
            return 0;
      }

      tcout << _T("\nЧисло распознанных опций: ") << PCL.options.size();
      tcout << _T("\n Это опции: {");
      for (auto& op : PCL.options)
      {
            tcout << "-" << op.name() << " ";
      }
      tcout << _T("}");

      tcout << _T("\n Список распознанных опций с параметрами:");
      if (PCL.options[_T("?")])
            tcout << _T("\n -") << PCL.options[_T("?")].name() << _T(" ") << PCL.options[_T("?")].params[0];
      if (PCL.options[_T("a")])
            tcout << _T("\n -") << PCL.options[_T("a")].name() << _T(" ") << PCL.options[_T("a")].params[0];
      if (PCL.options[_T("b")])
            tcout << _T("\n -") << PCL.options[_T("b")].name() << _T(" ") << PCL.options[_T("b")].params[0];
      if (PCL.options[_T("j")])
            tcout << _T("\n -") << PCL.options[_T("j")].name() << _T(" ") << PCL.options[_T("j")].params[0];
      if (PCL.options[_T("c")])
            tcout << _T("\n -") << PCL.options[_T("c")].name() << _T(" ") << PCL.options[_T("c")].params[0];
      if (PCL.options[_T("e")])
            tcout << _T("\n -") << PCL.options[_T("e")].name() << _T(" ") << PCL.options[_T("e")].params[0];
      if (PCL.options[_T("f")])
            tcout << _T("\n -") << PCL.options[_T("f")].name() << _T(" ") << PCL.options[_T("f")].params[0];
      if (PCL.options[_T("x")])
            tcout << _T("\n -") << PCL.options[_T("x")].name() << _T(" ") << PCL.options[_T("x")].params[0];
      if (PCL.options[_T("z")])
            tcout << _T("\n -") << PCL.options[_T("z")].name() << _T(" ") << PCL.options[_T("z")].params[0];

      if (PCL.options[_T("o")])
            tcout << _T("\n -") << PCL.options[_T("o")].name() << _T(" ") << PCL.options[_T("o")].params[0];
      if (PCL.options[_T("g")])
            tcout << _T("\n -") << PCL.options[_T("g")].name() << _T(" ") << PCL.options[_T("g")].params[0];

      if (PCL.options[_T("t")])
            tcout << _T("\n -") << PCL.options[_T("t")].name() << _T(" ") << PCL.options[_T("t")].params[0];
      if (PCL.options[_T("u")])
            tcout << _T("\n -") << PCL.options[_T("u")].name() << _T(" ") << PCL.options[_T("u")].params[0];


      if (PCL.options[_T("k")])
            tcout << _T("\n -") << PCL.options[_T("k")].name() << _T(" ") << PCL.options[_T("k")].params[0];
      if (PCL.options[_T("l")])
            tcout << _T("\n -") << PCL.options[_T("l")].name() << _T(" ") << PCL.options[_T("l")].params[0];
      if (PCL.options[_T("m")])
            tcout << _T("\n -") << PCL.options[_T("m")].name() << _T(" ") << PCL.options[_T("m")].params[0];

      if (PCL.options[_T("q")])
            tcout << _T("\n -") << PCL.options[_T("q")].name() << _T(" ") << PCL.options[_T("q")].params[0];
      if (PCL.options[_T("r")])
            tcout << _T("\n -") << PCL.options[_T("r")].name() << _T(" ") << PCL.options[_T("r")].params[0];
      if (PCL.options[_T("s")])
            tcout << _T("\n -") << PCL.options[_T("s")].name() << _T(" ") << PCL.options[_T("s")].params[0];

      tcout << _T("\n\nЧисло не опций: ") << PCL.non_options.size();
      for (size_t i = PCL.non_options.size(); i > 0; i--)
      {
            tcout << _T("\n НЕ опция: ") << PCL.non_options[i - 1];
      }

      tcout << _T("\n\nЧисло ошибочных опций: ") << PCL.unknown_otions.size();
      for (auto& unknown_otion: PCL.unknown_otions)
      {
            tcout << _T("\n В опции: ") << Color(red) << unknown_otion.name << Color();
            tcout << _T(" Error: ");
            switch (unknown_otion.id)
            {
            case command_line_t::unrecognized_opt:
                  tcout << _T("Опция не распознана. ");
                  break;
            case command_line_t::ambiguous_opt:
                  tcout << _T("Неоднозначная опция. ");
                  break;
            case command_line_t::not_need_arg:
                  tcout << _T("Параметр для опции не нужен. ");
                  break;
            case command_line_t::requires_arg:
                  tcout << _T("Для опции нужен параметр. ");
                  break;
            }
      }

      tcout << _T("\n");

      return 0;
}
