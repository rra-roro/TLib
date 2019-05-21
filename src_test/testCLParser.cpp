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

void help()
{
	tcout << _T(R"(
      ParserCommandLine работает с опциями заданными по следующим правилам:

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
  ParserCommandLine::SetShortFormatOfArg(“ab:c::”);

  Где    а   – имя опции без параметра
         b:  – имя опции с обязательным параметром
         с:: – имя опции с не обязательным параметром

  Формат длинно-именованных опции задается функцией
  ParserCommandLine::AddFormatOfArg(tstring name,              //- длинное имя опции
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


В случае если ParserCommandLine в процессе работы не смог распознать опцию
Не распознаные опции помещаются в массив опций  ParserCommandLine::ErrorOption[];
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


template<class _Elem>
void my_spawn(const _Elem *cmdname, const char *const argv[])
{
#ifdef _WIN32
	_spawnve(_P_WAIT, TemplateStr2str(cmdname).c_str(), argv, 0);
#else
        string Cmd = TemplateStr2str(cmdname).c_str();
        Cmd = Cmd + " " + string(argv[1]);
        char* chCmd = new char[Cmd.size()];
        Cmd.copy(chCmd,npos);

        const char *const argv_sh[] = {"/bin/sh", "-c", chCmd, NULL};
	int status;
	pid_t child_pid = 0;
	posix_spawn(&child_pid, argv_sh[0], 0, 0, (char *const*)argv_sh, 0);
        waitpid(child_pid, &status, 0);

        delete[] chCmd;
#endif
}


#ifdef _WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char *argv[])
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
		const char *const my_argv[] =
		{
			"testCLParser ",
			"-t=лоло -u 'шшш' -qrs -z=ooo --x --o=zz -op=io -b 'параметр опции b' -j dddd -c -a \"не опция\" -klm \"-e\" w \"-f w\" -- --help",
			NULL
		};

		cout << "Запускаем тест "    << my_argv[0] << " со следующими параметрами:\n";
		cout << "\n" << Color(green) << my_argv[0] << Color(yellow) << my_argv[1] << Color() << "\n";

		my_spawn(argv[0], my_argv);

		return 0;
	}
	
	ParserCommandLine PCL;
	PCL.AddFormatOfArg(_T("?"), no_argument, _T('?'));
	PCL.AddFormatOfArg(_T("help"), no_argument, _T('?'));
	PCL.AddFormatOfArg(_T("a"), no_argument, _T('a'));
	PCL.AddFormatOfArg(_T("b"), required_argument, _T('b'));
	PCL.AddFormatOfArg(_T("j"), optional_argument, _T('j'));
	PCL.AddFormatOfArg(_T("c"), optional_argument, _T('c'));
	PCL.AddFormatOfArg(_T("e"), required_argument, _T('e'));
	PCL.AddFormatOfArg(_T("f"), required_argument, _T('f'));
	PCL.AddFormatOfArg(_T("x"), required_argument, _T('x'));
	PCL.AddFormatOfArg(_T("z"), no_argument, _T('z'));
	PCL.AddFormatOfArg(_T("option"), required_argument, _T('o'));
	PCL.AddFormatOfArg(_T("ogogo"), required_argument, _T('g'));
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
	PCL.Parser((tstring)GetCommandLine());
#else
	PCL.Parser(argc, argv);
#endif

	//Если есть опция с ошибкой или запрошен хелп выходим 
	if (PCL.Option[_T('?')])
	{
		help();
		return 0;
	}

	tcout << _T("\nЧисло распознанных опций: ") << PCL.Option.size();
	tcout << _T("\n Это опции: {");
	for (auto& op : PCL.Option)
	{
		tcout << "-" << op.GetName() << " ";
	}
	tcout << _T("}");

	tcout << _T("\n Список распознанных опций с параметрами:");
	if (PCL.Option[_T('?')]) tcout << _T("\n -") << PCL.Option[_T('?')].GetName() << _T(" ") << PCL.Option[_T('?')].ParamOption[0];
	if (PCL.Option[_T('a')]) tcout << _T("\n -") << PCL.Option[_T('a')].GetName() << _T(" ") << PCL.Option[_T('a')].ParamOption[0];
	if (PCL.Option[_T('b')]) tcout << _T("\n -") << PCL.Option[_T('b')].GetName() << _T(" ") << PCL.Option[_T('b')].ParamOption[0];
	if (PCL.Option[_T('j')]) tcout << _T("\n -") << PCL.Option[_T('j')].GetName() << _T(" ") << PCL.Option[_T('j')].ParamOption[0];
	if (PCL.Option[_T('c')]) tcout << _T("\n -") << PCL.Option[_T('c')].GetName() << _T(" ") << PCL.Option[_T('c')].ParamOption[0];
	if (PCL.Option[_T('e')]) tcout << _T("\n -") << PCL.Option[_T('e')].GetName() << _T(" ") << PCL.Option[_T('e')].ParamOption[0];
	if (PCL.Option[_T('f')]) tcout << _T("\n -") << PCL.Option[_T('f')].GetName() << _T(" ") << PCL.Option[_T('f')].ParamOption[0];
	if (PCL.Option[_T('x')]) tcout << _T("\n -") << PCL.Option[_T('x')].GetName() << _T(" ") << PCL.Option[_T('x')].ParamOption[0];
	if (PCL.Option[_T('z')]) tcout << _T("\n -") << PCL.Option[_T('z')].GetName() << _T(" ") << PCL.Option[_T('z')].ParamOption[0];

	if (PCL.Option[_T('o')]) tcout << _T("\n -") << PCL.Option[_T('o')].GetName() << _T(" ") << PCL.Option[_T('o')].ParamOption[0];
	if (PCL.Option[_T('g')]) tcout << _T("\n -") << PCL.Option[_T('g')].GetName() << _T(" ") << PCL.Option[_T('g')].ParamOption[0];

	if (PCL.Option[_T('t')]) tcout << _T("\n -") << PCL.Option[_T('t')].GetName() << _T(" ") << PCL.Option[_T('t')].ParamOption[0];
	if (PCL.Option[_T('u')]) tcout << _T("\n -") << PCL.Option[_T('u')].GetName() << _T(" ") << PCL.Option[_T('u')].ParamOption[0];


	if (PCL.Option[_T('k')]) tcout << _T("\n -") << PCL.Option[_T('k')].GetName() << _T(" ") << PCL.Option[_T('k')].ParamOption[0];
	if (PCL.Option[_T('l')]) tcout << _T("\n -") << PCL.Option[_T('l')].GetName() << _T(" ") << PCL.Option[_T('l')].ParamOption[0];
	if (PCL.Option[_T('m')]) tcout << _T("\n -") << PCL.Option[_T('m')].GetName() << _T(" ") << PCL.Option[_T('m')].ParamOption[0];

	if (PCL.Option[_T('q')]) tcout << _T("\n -") << PCL.Option[_T('q')].GetName() << _T(" ") << PCL.Option[_T('q')].ParamOption[0];
	if (PCL.Option[_T('r')]) tcout << _T("\n -") << PCL.Option[_T('r')].GetName() << _T(" ") << PCL.Option[_T('r')].ParamOption[0];
	if (PCL.Option[_T('s')]) tcout << _T("\n -") << PCL.Option[_T('s')].GetName() << _T(" ") << PCL.Option[_T('s')].ParamOption[0];

	tcout << _T("\n\nЧисло не опций: ") << PCL.NonOption.size();
	for (size_t i = PCL.NonOption.size(); i > 0; i--)
	{
		tcout << _T("\n НЕ опция: ") << PCL.NonOption[i - 1];
	}

	tcout << _T("\n\nЧисло ошибочных опций: ") << PCL.ErrorOption.size();
	for( auto& ErrorOption: PCL.ErrorOption)
	{
		tcout << _T("\n В опции: ") << Color(red) << ErrorOption.optopt << Color();
		tcout << _T(" Error: ");
		switch (ErrorOption.ErrorID)
		{
			case ErrorOpt::unrecognized_opt:
				tcout << _T("Опция не распознана. ");
				break;
			case ErrorOpt::ambiguous_opt:
				tcout << _T("Неоднозначная опция. ");
				break;
			case ErrorOpt::not_need_arg:
				tcout << _T("Параметр для опции не нужен. ");
				break;
			case ErrorOpt::requires_arg:
				tcout << _T("Для опции нужен параметр. ");
				break;
		}	
		
	}

	tcout << _T("\n");

    return 0;
}

