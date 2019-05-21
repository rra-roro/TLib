#include "stdafx.h"
#include <Tlocale.h>
#include <Tiostream.h>

using namespace tlib;
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//Ф-ия  InitConsolIO объевлена в Tstring.h следующим образом
//
//DWORD InitConsolIO(const string& LocaleName  = "Russian_Russia.866",
//                     const string& NumberLocale = "C");
//
int tlib::InitConsolIO(void)
{
	// Системная локаль по умолчанию "C". Проинициализируем CRT, установив для нее 
	// локаль, соответствующую кодировке строковых литералов типа char, в нашей 
    // откомпилированной программе, заданной при ее компиляции	
	std::locale::global(GetLocaleProgram());

	// Создадим локаль соответствующую текущей кодировке консоли
	locale loc = GetLocaleConsole();

	// Установим локаль соответствующую текущей кодировке консоли
	// для всех консольных потоков ввода/вывода
	wcout.imbue(loc); wcout.clear(); 
	cout.imbue(loc); cout.clear(); 
	wcin.imbue(loc); wcin.clear(); 
	cin.imbue(loc); cin.clear(); 
	wcerr.imbue(loc); wcerr.clear(); 
	cerr.imbue(loc); cerr.clear(); 
	wclog.imbue(loc); wclog.clear(); 
	clog.imbue(loc); clog.clear(); 

	return 0;
}

#ifdef _WIN32
int tlib::InitIO(void)
{
	tstringstream temp;
	temp.imbue(GetLocaleGUI());
	return 0;
}
#endif
