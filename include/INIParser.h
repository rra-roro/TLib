//////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Файл INIParser.h  - содержит объявление класса INIParser, который является парсером INI файлов.
//            INI – Файл должен иметь, например такой формат:
//
//               [Section1]
//               ; Comment
//               param1 = value     ; Comment
//               param2 = “;vlue;”  ; Если точка с запятой используется в значении,
//                                           ; нужно чтобы значение было в кавычках
//               [Section2]
//               param1 = value     ; Имя параметра, должно быть уникальным в пределах
//               param2 = “;vlue;”  ; секции
//
//            Если имя параметра не будет уникальным, то получить значение второго 
//            параметра с одинаковым именем с помощью ф-ии GetValueByName будет невозможно.
//            Однако его можно получить, перебрав с помощью ф-ии GetSectionLine все пары
//            имя-параметра/значение для данной секции (это не удобно).
//
//            Чтобы распарсить INI файл нужно использовать объект INIParser 
//            у которого такой интерфейс:

//               INIParser(const tstring& ini_filename)  - конструктор, получает имя ini файла.
//               Parser()                                - запускает процесс анализа файла.
//               SectionList                             - член класса, предоставляющий интерфейс
//                                                         к данным ini файла:
//
//            SectionList.size()                         - возвращает секций в файле 
//            SectionList.empty()                        - возвращает true, если файл не содержит распознаных секций
//            SectionList.GetSectionByName(tstring& SectionName)                  - возвращает указатель на секцию
//            SectionList.GetSectionByName(..).GetValueByName(tstring& ParamName) - возвращает значение параметра, 
//                                                                                  для данной секции
//            SectionList.GetSectionByName(..).SectionName                        - содержит имя секции
//            SectionList.GetSectionByName(..).size()                             - возвращает число параметров в секции
//            SectionList.GetSectionByName(..).empty()                            - возвращает true, если секция пуста
//            SectionList.GetSectionByName(..).GetSectionLine(size_t Num)         - возвращает параметр/значение по номеру
//
//
//    Версия файла 1.00  - первая версия.
//
//


#ifndef INI_PARSER
#define INI_PARSER

#include <Tstring.h>
#include <Tlocale.h>
#include <Tiostream.h>
#include <vector>

class INIParser;

/////////////////////////////////////////////////////////////////////////////////
//
//     class Section - содержит интерфейс доступа к параметрам и значениям секции
//
//
class Section          
{
    friend INIParser;
      std::vector<tlib::tstring> ParamName;
    std::vector<tlib::tstring> Value;
      tlib::tstring EmptyString;

  public:
	// содержит имя секции
	tlib::tstring SectionName;

	// возвращает значение параметра, для данной секции
	const tlib::tstring& GetValueByName(const tlib::tstring& argParamName)const;
	
	// возвращает параметр/значение по номеру
	std::pair<tlib::tstring,tlib::tstring> GetSectionLine(size_t Num)const;

	// возвращает число параметров в секции
	size_t size()const
	{
		return ParamName.size();
	}

	// возвращает true, если секция пуста
	bool empty()const
	{
		return ParamName.empty();
	}

};

/////////////////////////////////////////////////////////////////////////////////
//
//     class SectionListClass - содержит интерфейс доступа к списку секций
//
//
class SectionListClass       
{
	friend INIParser;
	std::vector<Section> SectionList;
	Section EmptySection;
public:
	// Возвращает секцию по ее имени
	const Section& GetSectionByName(const tlib::tstring& SectionName)const;

	// Возвращает число секций
	size_t size()const
	{
		return SectionList.size();
	}
	
	// возвращает true, если есть хоть одна распознаная секция
	bool empty()const
	{
		return SectionList.empty();
	}

};

/////////////////////////////////////////////////////////////////////////////////
//
//     class ErrorLineListClass - содержит интерфейс к списку не распознаных строк 
//
//
class ErrorLineListClass       
{
	friend INIParser;
	std::vector<tlib::tstring> ErrorLine;
	tlib::tstring EmptyErrorLine;
public:
	// Перебирает не распознанные строки по имени 
	const tlib::tstring& operator[](size_t Num)const
	{
      if(Num >= ErrorLine.size()) return EmptyErrorLine;
	  return ErrorLine[Num];
	}

	// Возвращает число не распознаных строк 
	size_t size()const
	{
		return ErrorLine.size();
	}

	// возвращает true, если есть хоть одна не распознаная строка 
	bool empty()const
	{
		return ErrorLine.empty();
	}

};

///////////////////////////////////////////////////////////////////////////////////
//
//      class INIParser - Основной класс, через который предоставляется интерфейс
//                        к INI файлу.
//

class INIParser
{
	tlib::tstring ini_filename;
	size_t FindComment(const tlib::tstring& str);
	void InternalParser(tlib::tifstream& fs,tlib::tstring& prev_str);
public:
	// Cписок секций
	SectionListClass SectionList;
	// Cписок не распознаных строк
	ErrorLineListClass ErrorLineList;
	// Конструктор по имени INI файла
	INIParser(const tlib::tstring& arg_ininame):ini_filename(arg_ininame){};
	// Функция начала процесса разбора ini файла
	void Parser(const std::locale& Locale = tlib::GetLocaleGUI());
	
};

#if !(defined(__linux__) || defined(_LIB) || defined(UNDER_CE) || defined(WINCE))

#define _LIB_NAME "tlib_parsers"

#include <win/select_lib.h>

#undef _LIB_NAME

#endif // !(defined(__linux__) || defined(_LIB) || defined(UNDER_CE) || defined(WINCE))

#endif //INI_PARSER
