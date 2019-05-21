//////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Файл INIParser.h  - содержит реализацию класса INIParser, который является парсером INI файлов.
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
//            SectionList.GetSectionByName(..).size()                             - возвращает число параметров в секции
//            SectionList.GetSectionByName(..).empty()                            - возвращает true, если секция пуста
//            SectionList.GetSectionByName(..).GetSectionLine(size_t Num)         - возвращает параметр/значение по номеру
//
//
//    Версия файла 1.00  - первая версия.
//
//

#include "stdafx.h"
#include <Tstring.h>
#include <Tlocale.h>
#include <vector>
#include <INIParser.h>

using namespace tlib;
using namespace std;

// Ищем во входящей строке начало коментария и возвращаем его позицию. 
// Коментарий начинается с символа ';', 
size_t INIParser::FindComment(const tstring& str)
{
	// Поиск осуществляется с учетом кавычек. То что в кавычках не коментарий.
	// Поэтому введем два флага. Один для коментария а другой для кавычки.

	size_t pos=0;

	bool flag1=false;                            
	bool flag2_comment=false;                      
	size_t poscoment=0;

	while (true)
	{
		pos = str.find_first_of(_T(";\""),pos);    // Ищем либо кавычки, либо символ коментария
		if(pos==npos)return npos;                  // Если коментарий не найден

		if(str[pos]==_T('\"'))                     // Учтем кавычки
		{
			if(flag1==true)flag2_comment=false;
			flag1=!flag1;
		}
		if(str[pos]==_T(';')){flag2_comment=true;poscoment=pos;}
		if(flag2_comment==true && flag1==false)return poscoment;  //Если коментарий найден
		pos++;
	}	

}

// Читаем Ini файл построчно, выделяем начало следующей секции, распознаем параметры и их значения
// Если найдена новая секция, вызываем эту ф-ию рекурсивно сново и снова для каждой секции.

void INIParser::InternalParser(tifstream& fs,tstring& prev_str)
{
	RemovalSpace(prev_str);
	if(!prev_str.empty() && prev_str[0]==_T('[') && prev_str[prev_str.size()-1]==_T(']'))
	{
		// У нас начало новой секции
		Section NewSection;
		NewSection.SectionName = prev_str.substr(1,prev_str.size()-2);   // Сохраним ее имя

		tstring fileline;
		while (getline<TCHAR,char_traits<TCHAR>,allocator<TCHAR> >(fs,fileline)) // Читаем параметры и их значения пока не встретим новую секцию
		{
			RemovalSpace(fileline); 
			// Если новая секция вызовем InternalParser рекурсивно.
			if( fileline[0]==_T('[') && fileline[fileline.size()-1]==_T(']') ) {InternalParser(fs,fileline); break;}
			else
			{ 
				//Анализируем имена параметров, их значения, и коментарии
				//В случае ошибки, сохраним строку в списке не распознаных строк 
				size_t SeparatorPos = fileline.find_first_of(_T(";="));
				if (SeparatorPos==npos)
				{
					RemovalSpace(fileline);
					if(!fileline.empty())ErrorLineList.ErrorLine.push_back(fileline); 
					continue;
				}
				if (SeparatorPos!=npos && fileline[SeparatorPos]==_T(';'))
				{
					if(SeparatorPos!=FindPrintSymbol(fileline)) ErrorLineList.ErrorLine.push_back(fileline);
					continue;
				}				
				if (SeparatorPos!=npos && fileline[SeparatorPos]==_T('='))
				{
					tstring strParamName = fileline.substr(0,SeparatorPos);
					RemovalSpace(strParamName);
					NewSection.ParamName.push_back(strParamName);
					tstring strValue = fileline.substr(SeparatorPos+1);
					RemovalSpace(strValue);
					size_t pos = FindComment(strValue);
					if(pos!=npos){ strValue=strValue.substr(0,pos);
					RemovalSpace(strValue);}

					NewSection.Value.push_back(strValue);
				}

			}
		}
		// Сохраним секцию в списке секций
		SectionList.SectionList.push_back(NewSection);
	}
}

// Ф-ия которая открывает INI файл, задает для него локаль
// И начинает его парсить.
void INIParser::Parser(const locale& Locale)
{
	tifstream inifilestream;
	inifilestream.imbue(Locale);
	inifilestream.open(ini_filename.c_str());
	tstring fileline;
	getline(inifilestream,fileline);
	InternalParser(inifilestream,fileline);
	inifilestream.close();
}

// Возвращает секцию по имени
const Section& SectionListClass::GetSectionByName(const tstring& SectionName)const
{
	vector<Section>::const_iterator ItrSectionName;
	ItrSectionName = find_if(SectionList.begin(),SectionList.end(), 
								[&](const Section& Sec)
								{
									return StrCmpI(Sec.SectionName, SectionName);
								});
		
	if(ItrSectionName==SectionList.end())return EmptySection;
	size_t pos = distance(SectionList.begin(),ItrSectionName);
	return SectionList[pos];
}

// Возвращает значение параметра 
const tstring& Section::GetValueByName(const tstring& argParamName)const
{
	vector<tstring>::const_iterator ItrParamName;
	ItrParamName = find_if(ParamName.begin(),ParamName.end(),
							[&](const tstring& Param)
							{
								return StrCmpI(Param, argParamName);
							});
		
	if(ItrParamName==ParamName.end())return EmptyString;
	size_t pos = distance(ParamName.begin(),ItrParamName);
	return Value[pos];
}

//Переберает список параметров и их значений
pair<tstring,tstring> Section::GetSectionLine(size_t Num)const
{
	if(Num >= ParamName.size()) return pair<tstring,tstring>(tstring(),tstring());
	return pair<tstring,tstring>(ParamName[Num],Value[Num]);
}
