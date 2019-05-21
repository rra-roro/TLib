// testINIParser.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <Tstring.h>
#include <Tlocale.h>
#include <CLParser.h>
#include <INIParser.h>


using namespace tlib;
using namespace std;
void help()
{
	tcout << _T("testINIParse.exe [-help] ini_filename") << endl;
}

#ifdef _WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char *argv[])
#endif
{
	InitConsolIO();
	ParserCommandLine PCL;
	PCL.AddFormatOfArg(_T("?"),no_argument,_T('?'));
	PCL.AddFormatOfArg(_T("help"),no_argument,_T('?'));

	// Отключим вывод ошибок парсером
	PCL.SetShowError(false);

	//Начнем парсить аргументы
#ifdef _WIN32
	PCL.Parser((tstring)GetCommandLine());
#else
	PCL.Parser(argc, argv);
#endif

	//Если есть опция с ошибкой или запрошен хелп выходим 
	if(PCL.Option[_T('?')] || PCL.ErrorOption.size()!=0 || PCL.NonOption.size()!=1)
	{
		for(size_t i = PCL.ErrorOption.size(); i > 0; i--)
		{
			tcout << _T("Error: ");
			tcout << _T(" in otion ") << PCL.ErrorOption[i-1].optopt << endl;
		}
		help();
		return 0;
	}
	
	// Создадим объект для анализа INI файла, и укажем, какойфайл нужно анализировать
  	INIParser ini(PCL.NonOption[0]);
    
	// Проанализируем файл
	ini.Parser();

	//Если есть не распознаная строка или вообще ничего не распознано сообщим об этой строке и выйдем
	if( !ini.ErrorLineList.empty() || ini.SectionList.empty())
	{
		if(!ini.ErrorLineList.empty()) tcout << _T("Error in INI file: ") << endl;;
		for(size_t i = ini.ErrorLineList.size(); i > 0; i--)
		{		
			tcout << _T("        in string: ") << ini.ErrorLineList[i] << endl;
		}
		return 0;
	}
    
	// Получим значение параметра "param1" секции "Header"
	tstring SectionName = ini.SectionList.GetSectionByName(_T("Header")).GetValueByName(_T("param1"));

	// Получим значения всех параметров секции "Header"
	tcout << _T("[Header]") <<  endl;
	for(size_t i = ini.SectionList.GetSectionByName(_T("Header")).size(); i > 0; i--)
	{
		pair<tstring,tstring> line = ini.SectionList.GetSectionByName(_T("Header")).GetSectionLine(i-1);
		tstring paramname = line.first;
		tstring value = line.second;
		tcout << paramname  << _T(" = ") << value <<  endl;
	}
	

	return 0;
}

