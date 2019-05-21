// testTstringConsole.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h"

#include <Tstring.h>
#include <Tlocale.h>
#include <Tstdlib.h>
#include <Tiomanip.h>
#include <Tiostream.h>
#include <Tcodecvt.h>
#include <Tregex.h>
#include <Tconsole.h>
#include <algorithm>

using namespace Tstring;
using namespace std;

void PrintTest11(int r, const TCHAR* Str_Std, streamsize Tab1, const tstring& Str_My, streamsize Tab2)
{
	tcout << _T("base") << setw(2) << r << _T(": ");
#ifdef _WIN32
        tcout << setw(Tab1) << Str_Std << _T(" | ");
#endif
        tcout << setw(Tab2) << Str_My << _T(" | ");
#ifdef _WIN32
        tcout << ((Str_My != tstring(Str_Std)) ? _T("Error!!!") : _T("Ok")) << endl;
#elif  __linux__
        tcout << endl;
#endif
}

void PrintBuildInfo()
{
	tcout << Color(green) << _T("Build Info: ");

    #ifdef _M_X64 
	     tcout << _T("x64 ");
    #elif _M_IX86
	     tcout << _T("x86 ");
    #endif

    #ifdef _UNICODE
	     tcout << _T("Unicode ");
    #else
	     tcout << _T("Ansi ");
    #endif

    #if !(defined(_DLL))
		 tcout << _T("static link ");
    #else
		 tcout << _T("dynamic link ");
    #endif

    #ifdef _DEBUG
		 tcout << _T("debug");
    #else
 		 tcout << _T("release");
    #endif
		 
    (is_same<tstring, wstring>::value) ? wcout << L"\ntstring is - wstring" : wcout << L"\ntstring is - string";
	wcout << L"\ntcout is - " << typeid(tcout).name();

	tcout << Color();
};


#ifdef _WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char *argv[])
#endif
{

#ifdef _WIN32
	TConsole console;

	console.SetScreenBufferSize(147, 950);
	console.SetWindowSize(147, 50);
#endif
	PrintBuildInfo();

	// =============================================================================================
	// Тестируем функционал <Tlocale.h>

	// Тестируем получение информации о локалях:
	tcout << Color(yellow);
	tcout << _T("\n\nTest 1 <Tlocale.h>: get locale info");
	tcout << _T("\n------------------------------------------------------------------------\n");
	tcout << Color();

	cout << "\nGetLocaleNameProgram() -> " << GetLocaleNameProgram();
	cout << "\nGetLocaleProgram() -> " << GetLocaleProgram().name() << "\n";
	cout << "\nGetLocaleNameConsole() -> " << GetLocaleNameConsole();
	cout << "\nGetLocaleConsole() -> " << GetLocaleConsole().name() << "\n";
#ifdef _WIN32
	cout << "\nGetLocaleNameGUI() -> " << GetLocaleNameGUI();
	cout << "\nGetLocaleGUI() -> " << GetLocaleGUI().name() << "\n";
#endif	
	cout << "\nGet global locale: locale() -> " << locale().name();

	// Тестируем установку локалей для потоков:
	tcout << Color(yellow);
	tcout << _T("\n\nTest 2 <Tlocale.h>: set locale for streams - InitConsolIO()");
	tcout << _T("\n------------------------------------------------------------------------\n");
	tcout << Color();

	wcout << L"\nwcout: Next string shoud not be show: " << L"Мама мыла раму"; wcout.clear();
	wcout << L"\n       Set local for console. InitConsolIO()";   InitConsolIO();
	wcout << L"\nwcout: Next string shoud be show: " << L"Мама мыла раму";
	cout << "\ncout:  Next string shoud be show: " << "Мама мыла раму";

	// Тестируем получение информации о глобальной локали:
	tcout << Color(yellow);
	tcout << _T("\n\nTest 3 <Tlocale.h>: get global locale info, after call InitConsolIO()");
	tcout << _T("\n------------------------------------------------------------------------\n");
	tcout << Color();

	cout << "\nGet global locale. Shoud be:\n\"" << GetLocaleProgram().name() << "\"\nThere's locale() -> \n" << locale().name();

	// Проверяем ф-ии конвертации строк с учетом кодировки:
	tcout << Color(yellow);
	tcout << _T("\n\nTest 4 <Tstring.h>: проверяем ф-ии конвертации строк с учетом кодировки");
	tcout << _T("\n------------------------------------------------------------------------\n");
	tcout << Color();

	cout << "\nstring " << GetLocaleNameProgram() << " -> wstring | ";
	tcout << _T("cstr_wstr(\"Мама мыла раму\") -> "); wcout << cstr_wstr("Мама мыла раму");

	cout << "\nwstring -> string " << GetLocaleNameProgram() << " | ";
	tcout << _T("wstr_cstr(L\"Мама мыла раму\") -> "); cout << wstr_cstr(L"Мама мыла раму");

	tcout << _T("\n\nstring UTF-8 -> wstring | ");
	tcout << _T("u8str_wstr(u8\"Мама мыла раму\") -> "); wcout << u8str_wstr(u8"Мама мыла раму");

	tcout << _T("\nwstring -> string UTF-8 | ");
#ifdef _WIN32
	tcout << _T("wstr_u8str(L\"Мама мыла раму\") -> ") << _T("пропускаем тест");
#elif __linux__
	tcout << _T("wstr_u8str(L\"Мама мыла раму\") -> ") << str2tstr(wstr_u8str(L"Мама мыла раму"));
#endif

	tcout << _T("\n\nstring UTF-8 -> u16string | ");
	tcout << _T("u8str_u16str(u8\"Мама мыла раму\") -> ") << ustr2tstr(u8str_u16str(u8"Мама мыла раму"));

	tcout << _T("\nu16string -> string UTF-8 | ");
#ifdef _WIN32
	tcout << _T("u16str_u8str(u\"Мама мыла раму\") -> ") << _T("пропускаем тест");
#elif __linux__
	tcout << _T("u16str_u8str(u\"Мама мыла раму\") -> ") << str2tstr(u16str_u8str(u"Мама мыла раму"));
#endif

	tcout << _T("\n\nu16string -> wstring | ");
	tcout << _T("u16str_wstr(u\"Мама мыла раму\") -> ") << wstr2tstr(u16str_wstr(u"Мама мыла раму"));

	tcout << _T("\nwstring -> u16string | ");
	tcout << _T("wstr_u16str(L\"Мама мыла раму\") -> ") << ustr2tstr(wstr_u16str(L"Мама мыла раму"));

	// Проверяем ф-ии конвертации строк с учетом кодировки:
	tcout << Color(yellow);
	tcout << _T("\n\nTest 5 <Tstring.h>: проверяем tstr ф-ии конвертации строк");
	tcout << _T("\n------------------------------------------------------------------------\n");
	tcout << Color();

	tcout << _T("\nstring -> tstring | ");
	tcout << _T("str2tstr(\"Мама мыла раму\") -> "); tcout << str2tstr("Мама мыла раму");

	tcout << _T("\ntstring -> string | ");
	tcout << _T("tstr2str(_T(\"Мама мыла раму\")) -> "); cout << tstr2str(_T("Мама мыла раму"));

	tcout << _T("\nwstring -> tstring | ");
	tcout << _T("wstr2tstr(L\"Мама мыла раму\") -> "); tcout << wstr2tstr(L"Мама мыла раму");

	tcout << _T("\ntstring -> wstring | ");
	tcout << _T("tstr2wstr(_T(\"Мама мыла раму\")) -> "); wcout << tstr2wstr(_T("Мама мыла раму"));

	tcout << _T("\nu16string -> tstring | ");
	tcout << _T("ustr2tstr(u\"Мама мыла раму\") -> "); tcout << ustr2tstr(u"Мама мыла раму");

	tcout << _T("\ntstring -> u16string | ");
	tcout << _T("tstr2ustr(_T(\"Мама мыла раму\")) -> "); tcout << ustr2tstr(tstr2ustr(_T("Мама мыла раму")));


	// Проверяем ф-ии сравнения строк и смена регистра:
	tcout << Color(yellow);
	tcout << _T("\n\nTest 6 <Tlocale.h>: сравнение строк и смена регистра GetLowerStr, GetUpperStr, StrCmpI");
	tcout << _T("\n------------------------------------------------------------------------\n");
	tcout << Color();

	TCHAR const* str_char = _T("Мама Мыла Раму");
	tstring str_str = _T("Мама Мыла Раму");

	tcout << _T("\nПреобразуем TCHAR * строку \"Мама Мыла Раму\" в нижний регистр: ") << GetLowerStr(str_char);
	tcout << _T("\nПреобразуем tstring строку \"Мама Мыла Раму\" в нижний регистр: ") << GetLowerStr(str_str);
	tcout << _T("\nПреобразуем строку _T(\"Мама Мыла Раму\") в нижний регистр: ") << GetLowerStr(_T("Мама Мыла Раму"));
	tcout << _T("\n\nПреобразуем TCHAR * строку \"Мама Мыла Раму\" в верхний регистр: ") << GetUpperStr(str_char);
	tcout << _T("\nПреобразуем tstring строку \"Мама Мыла Раму\" в верхний регистр: ") << GetUpperStr(str_str);
	tcout << _T("\nПреобразуем строку _T(\"Мама Мыла Раму\") в верхний регистр: ") << GetUpperStr(_T("Мама Мыла Раму"));

	tstring LoStr = GetLowerStr(str_char);
	tstring UpStr = GetUpperStr(str_str);

	tcout << _T("\n\nРезультат сравнения: ");
	tcout << _T("\n   \"") << LoStr << _T("\" и \"") << UpStr << _T("\" без учета регистра. Строки равны?: ") << StrCmpI(LoStr, UpStr);

	tcout << _T("\nРезультат сравнения: ");
	tcout << _T("\n   \"") << LoStr << _T("\" и \"") << UpStr << _T("\" c учетом регистра. Строки равны?: ") << (LoStr == UpStr);

	// Проверяем ф-ии работы с пробельными символами данной локали:
	tcout << Color(yellow);
	tcout << _T("\n\nTest 7 <Tlocale.h>: ф-ии детектироваения пробельных и управляющих символов локали");
	tcout << _T("\n------------------------------------------------------------------------\n");
	tcout << Color();

	tcout << _T("\n Этот ' '  символ пробельный? isspace(): ") << isspace(' ', GetLocaleProgram());
	tcout << _T("\n Этот '\\t' символ пробельный? isspace(): ") << isspace('\t', GetLocaleProgram());
	tcout << _T("\n Этот '\\n' символ пробельный? isspace(): ") << isspace('\n', GetLocaleProgram());
	tcout << _T("\n Этот '\\r' символ пробельный? isspace(): ") << isspace('\r', GetLocaleProgram());
	tcout << _T("\n Этот '\\b' символ пробельный? isspace(): ") << isspace('\b', GetLocaleProgram());
	tcout << _T("\n Этот '\\a' символ пробельный? isspace(): ") << isspace('\a', GetLocaleProgram());

	tcout << _T("\n\n Это ' '  управляющий символ? iscntrl(): ") << iscntrl(' ', GetLocaleProgram());
	tcout << _T("\n Это '\\t' управляющий символ? iscntrl(): ") << iscntrl('\t', GetLocaleProgram());
	tcout << _T("\n Это '\\n' управляющий символ? iscntrl(): ") << iscntrl('\n', GetLocaleProgram());
	tcout << _T("\n Это '\\r' управляющий символ? iscntrl(): ") << iscntrl('\r', GetLocaleProgram());
	tcout << _T("\n Это '\\b' управляющий символ? iscntrl(): ") << iscntrl('\b', GetLocaleProgram());
	tcout << _T("\n Это '\\a' управляющий символ? iscntrl(): ") << iscntrl('\a', GetLocaleProgram());

	tcout << _T("\n\n Этот '\\' символ слеш? isslash(): ") << isslash('\\');
	tcout << _T("\n Этот '/' символ слеш? isslash(): ") << isslash('/');

	// Проверяем ф-ии поиска подстроки:
	tcout << Color(yellow);
	tcout << _T("\n\nTest 8 <Tlocale.h>: поиск подстроки FindSubStrI(), FindPrintSymbol, RFindPrintSymbol");
	tcout << _T("\n------------------------------------------------------------------------\n");
	tcout << Color();

	tcout << _T("\nНайдем в \"Мама Мыла Раму\" подстроку \"МЫЛА\" без учета регистра: ") << FindSubStrI(_T("Мама Мыла Раму"), _T("МЫЛА"));
	tcout << _T("\nНайдем в \"  \\n \\b \\a \\v \\t \\b Мама Мыла Раму  \" первый печатный символ: ") << FindPrintSymbol(_T("  \n \b \a \v \t \b Мама Мыла Раму  "));
	tcout << _T("\nНайдем в \"  Мама Мыла Раму  \\n \\b \\a \\v \\t \\b \" последний печатный символ в конце строки: ") << RFindPrintSymbol(_T("  Мама Мыла Раму  \n \b \a \v \t \b "));

	// Проверяем ф-ии удаления пробелов из строки:
	tcout << Color(yellow);
	tcout << _T("\n\nTest 9 <Tlocale.h>: удаление пробелов из строки");
	tcout << _T("\n------------------------------------------------------------------------\n");
	tcout << Color();

	tcout << _T("\nУдалим в \"  \\n \\b \\a \\v \\t \\b Мама Мыла Раму  \"    лидирующие пробелы: ") << RemovalFirstSpaces(_T("  \n \b \a \v \t \b Мама Мыла Раму  ")) << _T("<-- end");
	tcout << _T("\nУдалим в \"  Мама Мыла Раму  \\n \\b \\a \\v \\t \\b \"   заверщающие пробелы: ") << RemovalTrailingSpaces(_T("  Мама Мыла Раму  \n \b \a \v \t \b ")) << _T("<-- end");
	tcout << _T("\nУдалим в \"  \\n \\b \\a \\v \\t \\b Мама Мыла Раму  \\n \\b \\a \\v \\t \\b \" пробелы c двух сторон: ") << RemovalSpace(_T("  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ")) << _T("<-- end");

	// Проверяем ф-ии фасета ctype<>:
	tcout << Color(yellow);
	tcout << _T("\n\nTest 10 <Tlocale.h>: функции фасета ctype<>");
	tcout << _T("\n------------------------------------------------------------------------\n");
	tcout << Color();
#ifdef _WIN32
	cout << "\nФункция установки верхнего регистра use_facet<ctype<char> >(loc).toupper('б'): " << use_facet<ctype<char> >(GetLocaleProgram()).toupper('б');
	wcout << L"\nФункция установки верхнего регистра use_facet<ctype<wchar_t> >(loc).toupper(L'б'): " << use_facet<ctype<wchar_t> >(GetLocaleProgram()).toupper(L'б');
	wcout << L"\nФункция установки верхнего регистра use_facet<ctype<char16_t> >(loc).toupper(u'б'): " << (wchar_t)(use_facet<ctype<char16_t> >(GetLocaleProgram()).toupper(u'б'));
#elif __linux__		
	tcout << _T("\nФункция установки верхнего регистра use_facet<ctype<char> >(loc).toupper('б'): ") << use_facet<ctype<char> >(GetLocaleProgram()).toupper(u8'б');
	tcout << _T("\nФункция установки верхнего регистра use_facet<ctype<wchar_t> >(loc).toupper(L'б'): ") << use_facet<ctype<wchar_t> >(GetLocaleProgram()).toupper(L'б');
	tcout << _T("\nФункция установки верхнего регистра use_facet<ctype<char16_t> >(loc).toupper(u'б'): ") << use_facet<ctype<char16_t> >(GetLocaleProgram()).toupper(u'б');
#endif


	// Проверяем ф-ии конвертирования чисел в строку:
	tcout << Color(yellow);
	tcout << _T("\n\nTest 11 <Tstdlib.h>: конвертирование чисел в строку в системы счисления по основанию от 16 до 2-х");
	tcout << _T("\n------------------------------------------------------------------------\n");
	tcout << Color();

	TCHAR Str_Std[256] = { 0 };
	tstring Str_My;

	//  Конвертируем -10LL в строку в системы счисления по основанию от 16 до 2-х
	tcout << Color(cyan);
	tcout << _T("\nКонвертируем dig_to_tstr( -10LL, Str_My, r) :");
	tcout << Color(dark_cyan);
#ifdef _WIN32
	tcout << _T("\n      : _i64tot_s()         | dig_to_tstr()       |  compare status") << endl;
#elif __linux__
	tcout << _T("\n      : dig_to_tstr()       |") << endl;
#endif
	tcout << Color();
	for (int r = 16; r >= 2; --r)
	{
#ifdef _WIN32
		_i64tot_s(-10LL, Str_Std, 256, r);
#endif
		dig_to_tstr(-10LL, Str_My, r);

		PrintTest11(r, Str_Std, 19, Str_My, 19);

		Str_My.clear();
	}

	//  Конвертируем (unsigned long long) -10LL в строку в системы счисления по основанию от 16 до 2-х
	tcout << Color(cyan);
	tcout << _T("\nКонвертируем dig_to_tstr( (unsigned long long) -10LL, Str_My, r) :");
	tcout << Color(dark_cyan);
#ifdef _WIN32
	tcout << _T("\n      : _ui64tot_s()     | dig_to_tstr()    |  compare status") << endl;
#elif __linux__
	tcout << _T("\n      : dig_to_tstr()       |") << endl;
#endif
	tcout << Color();

	for (int r = 16; r >= 2; --r)
	{
#ifdef _WIN32
		_ui64tot_s((unsigned long long) - 10LL, Str_Std, 256, r);
#endif
		dig_to_tstr((unsigned long long) - 10LL, Str_My, r);

		PrintTest11(r, Str_Std, 10, Str_My, 10);

		Str_My.clear();
	}

	//  Конвертируем -10L в строку в системы счисления по основанию от 16 до 2-х
	tcout << Color(cyan);
	tcout << _T("\nКонвертируем dig_to_tstr(-10L, Str_My, r) :");
	tcout << Color(dark_cyan);
#ifdef _WIN32
	tcout << _T("\n      : _ltot_s()  | dig_to_tstr()|  compare status") << endl;
#elif __linux__
	tcout << _T("\n      : dig_to_tstr()       |") << endl;
#endif
	tcout << Color();

	for (int r = 16; r >= 2; --r)
	{
#ifdef _WIN32
		_ltot_s(-10L, Str_Std, 256, r);
#endif
		dig_to_tstr(-10L, Str_My, r);

		PrintTest11(r, Str_Std, 10, Str_My, 12);

		Str_My.clear();
	}

	//  Конвертируем (unsigned long)-10 в строку в системы счисления по основанию от 16 до 2-х
	tcout << Color(cyan);
	tcout << _T("\nКонвертируем dig_to_tstr( (unsigned long)-10L, Str_My, r) :");
	tcout << Color(dark_cyan);
#ifdef _WIN32
	tcout << _T("\n      : _ultot_s() | dig_to_tstr()|  compare status") << endl;
#elif __linux__
	tcout << _T("\n      : dig_to_tstr()       |") << endl;
#endif
	tcout << Color();

	for (int r = 16; r >= 2; --r)
	{
#ifdef _WIN32
		_ultot_s((unsigned long)-10L, Str_Std, 256, r);
#endif
		dig_to_tstr((unsigned long)-10L, Str_My, r);

		PrintTest11(r, Str_Std, 10, Str_My, 12);

		Str_My.clear();
	}

	//  Конвертируем (short)-10L в строку в системы счисления по основанию от 16 до 2-х
	tcout << Color(cyan);
	tcout << _T("\nКонвертируем dig_to_tstr( (short)-10L, Str_My, r) :");
	tcout << Color(dark_cyan);
#ifdef _WIN32
	tcout << _T("\n      : _itot_s() | dig_to_tstr() |  compare status") << endl;
#elif __linux__
	tcout << _T("\n      : dig_to_tstr()       |") << endl;
#endif
	tcout << Color();

	for (int r = 16; r >= 2; --r)
	{
#ifdef _WIN32
		(r == 10) ? _itot_s((short)-10L, Str_Std, 256, r) : _itot_s((unsigned short)-10L, Str_Std, 256, r);
#endif
		dig_to_tstr((short)-10L, Str_My, r);

		PrintTest11(r, Str_Std, 9, Str_My, 13);

		Str_My.clear();
	}

	//  Конвертируем (unsigned short)-10L в строку в системы счисления по основанию от 16 до 2-х
	tcout << Color(cyan);
	tcout << _T("\nКонвертируем dig_to_tstr( (unsigned short)-10LL, Str_My, r) :");
	tcout << Color(dark_cyan);
#ifdef _WIN32
	tcout << _T("\n      : _itot_s() | dig_to_tstr() |  compare status") << endl;
#elif __linux__
	tcout << _T("\n      : dig_to_tstr()       |") << endl;
#endif
	tcout << Color();

	for (int r = 16; r >= 2; --r)
	{
#ifdef _WIN32
		_itot_s((unsigned short)-10L, Str_Std, 256, r);
#endif
		dig_to_tstr((unsigned short)-10L, Str_My, r);

		PrintTest11(r, Str_Std, 9, Str_My, 13);

		Str_My.clear();
	}

	//  Конвертируем (int)-10L в строку в системы счисления по основанию от 16 до 2-х
	tcout << Color(cyan);
	tcout << _T("\nКонвертируем dig_to_tstr( (int)-10L, Str_My, r) :");
	tcout << Color(dark_cyan);
#ifdef _WIN32
	tcout << _T("\n      : _itot_s() | dig_to_tstr() |  compare status") << endl;
#elif __linux__
	tcout << _T("\n      : dig_to_tstr()       |") << endl;
#endif
	tcout << Color();

	for (int r = 16; r >= 2; --r)
	{
#ifdef _WIN32
		_itot_s((int)-10L, Str_Std, 256, r);
#endif
		dig_to_tstr((int)-10L, Str_My, r);

		PrintTest11(r, Str_Std, 9, Str_My, 13);

		Str_My.clear();
	}

	//  Конвертируем (unsigned int)-10L в строку в системы счисления по основанию от 16 до 2-х
	tcout << Color(cyan);
	tcout << _T("\nКонвертируем dig_to_tstr( (unsigned int)-10L, Str_My, r) :");
	tcout << Color(dark_cyan);
#ifdef _WIN32
	tcout << _T("\n      : _ultot_s() | dig_to_tstr() |  compare status") << endl;
#elif __linux__
	tcout << _T("\n      : dig_to_tstr()       |") << endl;
#endif
	tcout << Color();

	for (int r = 16; r >= 2; --r)
	{
#ifdef _WIN32
		_ultot_s((unsigned int)-10L, Str_Std, 256, r);
#endif
		dig_to_tstr((unsigned int)-10L, Str_My, r);

		PrintTest11(r, Str_Std, 10, Str_My, 13);

		Str_My.clear();
	}

	//  Конвертируем (char)-10L в строку в системы счисления по основанию от 16 до 2-х
	tcout << Color(cyan);
	tcout << _T("\nКонвертируем dig_to_tstr( (char)-10L, Str_My, r) :");
	tcout << Color(dark_cyan);
#ifdef _WIN32
	tcout << _T("\n      : _itot_s() | dig_to_tstr() |  compare status") << endl;
#elif __linux__
	tcout << _T("\n      : dig_to_tstr()       |") << endl;
#endif
	tcout << Color();

	for (int r = 16; r >= 2; --r)
	{
#ifdef _WIN32
		(r == 10) ? _itot_s((char)-10L, Str_Std, 256, r) : _itot_s((unsigned char)-10L, Str_Std, 256, r);
#endif
		dig_to_tstr((char)-10L, Str_My, r);

		PrintTest11(r, Str_Std, 9, Str_My, 13);

		Str_My.clear();
	}

	//  Конвертируем (unsigned char)-10L в строку в системы счисления по основанию от 16 до 2-х
	tcout << Color(cyan);
	tcout << _T("\nКонвертируем dig_to_tstr( (unsigned char)-10L, Str_My, r) :");
	tcout << Color(dark_cyan);
#ifdef _WIN32
	tcout << _T("\n      : _itot_s() | dig_to_tstr() |  compare status") << endl;
#elif __linux__
	tcout << _T("\n      : dig_to_tstr()       |") << endl;
#endif
	tcout << Color();

	for (int r = 16; r >= 2; --r)
	{
#ifdef _WIN32
		_itot_s((unsigned char)-10L, Str_Std, 256, r);
#endif
		dig_to_tstr((unsigned char)-10L, Str_My, r);

		PrintTest11(r, Str_Std, 9, Str_My, 13);

		Str_My.clear();
	}

	// Тестируем манипулятор ConversionRadix предназначенный для конвертирования числа
	// в текст в произвольной системе считления:
	tcout << Color(yellow);
	tcout << _T("\n\nTest 12 <Tiomanip.h>: вывод числа в поток, как текст, в заданной системе счисления");
	tcout << _T("\n------------------------------------------------------------------------\n");
	tcout << Color();

	// Выводим число 15 в char поток в системах счисления: BIN, DEC и HEX
	tcout << Color(cyan);
	tcout << _T("\nВыводим число 15 в char потоки в системах счисления: BIN, DEC и HEX");
	tcout << _T("\nно без заполнения поля лидирующими Нулями:");
	tcout << Color(dark_cyan);
	tcout << _T("\n  BIN\t|\tDEC\t|\tHEX") << endl;
	tcout << Color();

	// Примечание 1: После того, как будет произведена запись в поток, поток сбросит ширину поля
	//               (поток вызовет iostr.width(0)), таким образом ширину поля нужно выставлять 
	//                 перед каждой записью числа в поток, в нашем случае перед каждым вызовом ConversionRadixULL().
	//               Таким образом, несмотря на: setw(2) << setfill('0') 
	//               Следующий код не будет выставлять НОЛЬ для заполнения поля перед числом

	stringstream MemBuffer_WithoutZero;
	MemBuffer_WithoutZero << setw(2) << tsetfill('0');
	MemBuffer_WithoutZero << ConversionRadix(0xf, 2) << "\t|\t"
		<< ConversionRadix(0xf, 10) << "\t|\t"
		<< ConversionRadix(0xf, 16) << endl;
	MemBuffer_WithoutZero << showbase;
	MemBuffer_WithoutZero << ConversionRadix(0xf, 2) << "\t|\t"
		<< ConversionRadix(0xf, 10) << "\t|\t"
		<< ConversionRadix(0xf, 16) << endl;
	cout << MemBuffer_WithoutZero.str();

	// Выводим число 15 в char поток в системах счисления: BIN, DEC и HEX
	tcout << Color(cyan);
	tcout << _T("\nВыводим число 15 в char потоки в системах счисления: BIN, DEC и HEX");
	tcout << _T("\nзаполнения поля (размер поля 2 символа) лидирующими Нулями:");
	tcout << Color(dark_cyan);
	tcout << _T("\n  BIN\t|\tDEC\t|\tHEX") << endl;
	tcout << Color();

	stringstream MemBuffer;
	MemBuffer << tsetfill('0');
	MemBuffer << setw(2) << ConversionRadix(0xf, 2) << "\t|\t"
		<< setw(2) << ConversionRadix(0xf, 10) << "\t|\t"
		<< setw(2) << ConversionRadix(0xf, 16) << endl;
	MemBuffer << showbase;
	MemBuffer << setw(2) << ConversionRadix(0xf, 2) << "\t|\t"
		<< setw(2) << ConversionRadix(0xf, 10) << "\t|\t"
		<< setw(2) << ConversionRadix(0xf, 16) << endl;
	cout << MemBuffer.str();

	// Выводим число 15 в char поток в системах счисления: BIN, DEC и HEX
	tcout << Color(cyan);
	tcout << _T("\nВыводим число 15 в wchar_t потоки в системах счисления: BIN, DEC и HEX");
	tcout << _T("\nзаполнения поля (размер поля 3 символа) лидирующими Нулями:");
	tcout << Color(dark_cyan);
	tcout << _T("\n  BIN\t|\tDEC\t|\tHEX") << endl;
	tcout << Color();

	wstringstream MemBufferW;
	MemBufferW << tsetfill(L'0');
	MemBufferW << setw(3) << ConversionRadix(0xf, 2) << L"\t|\t"
		<< setw(3) << ConversionRadix(0xf, 10) << L"\t|\t"
		<< setw(3) << ConversionRadix(0xf, 16) << endl;
	MemBufferW << showbase;
	MemBufferW << setw(3) << ConversionRadix(0xf, 2) << L"\t|\t"
		<< setw(3) << ConversionRadix(0xf, 10) << L"\t|\t"
		<< setw(3) << ConversionRadix(0xf, 16) << endl;
	wcout << MemBufferW.str();

	// Выводим число 15 в TCHAR поток в системах счисления: BIN, DEC и HEX
	tcout << Color(cyan);
	tcout << _T("\nВыводим число 15 в TCHAR потоки в системах счисления: BIN, DEC и HEX");
	tcout << _T("\nзаполнения поля (размер поля 4 символа) лидирующими Нулями:");
	tcout << Color(dark_cyan);
	tcout << _T("\n  BIN\t|\tDEC\t|\tHEX") << endl;
	tcout << Color();

	tstringstream MemBufferT;
	MemBufferT << tsetfill(_T('0'));
	MemBufferT << setw(4) << ConversionRadix(0xf, 2) << _T("\t|\t")
		<< setw(4) << ConversionRadix(0xf, 10) << _T("\t|\t")
		<< setw(4) << ConversionRadix(0xf, 16) << endl;
	MemBufferT << showbase;
	MemBufferT << setw(4) << ConversionRadix(0xf, 2) << _T("\t|\t")
		<< setw(4) << ConversionRadix(0xf, 10) << _T("\t|\t")
		<< setw(4) << ConversionRadix(0xf, 16) << endl;
	tcout << MemBufferT.str();

	// Выводим число -10 в TCHAR поток в системах счисления: BIN, DEC и HEX
	tcout << Color(cyan);
	tcout << _T("\nВыводим число -10 в TCHAR потоки в системах счисления: BIN, DEC и HEX");
	tcout << _T("\nхотя размер поля установлен в 4 символа, он не учитывается для отрицательных чисел");
	tcout << _T("\nдля отрицательных чисел размер поля определяется типом числа(-10 -это int, т.е. 4-е байта )");

	tcout << Color(dark_cyan);
	tcout << _T("\n  BIN\t\t\t\t\t|\tDEC\t|\tHEX") << endl;
	tcout << Color();

	tstringstream MemSBufferT;
	MemSBufferT << tsetfill(_T('0'));
	MemSBufferT << setw(4) << ConversionRadix(-10, 2) << _T("\t|\t")
		<< setw(4) << ConversionRadix(-10, 10) << _T("\t|\t")
		<< setw(4) << ConversionRadix(-10, 16) << endl;
	MemSBufferT << showbase;
	MemSBufferT << setw(4) << ConversionRadix(-10, 2) << _T("\t|\t")
		<< setw(4) << ConversionRadix(-10, 10) << _T("\t|\t")
		<< setw(4) << ConversionRadix(-10, 16) << endl;
	tcout << MemSBufferT.str();

	// Выводим число (char)-10 в TCHAR поток в системах счисления: BIN, DEC и HEX
	tcout << Color(cyan);
	tcout << _T("\nВыводим число (char)-10 в TCHAR потоки в системах счисления: BIN, DEC и HEX");
	tcout << _T("\nхотя размер поля установлен в 4 символа, он не учитывается для отрицательных чисел");
	tcout << _T("\nдля отрицательных чисел размер поля определяется типом числа (char)-10 , т.е. 1 байт");

	tcout << Color(dark_cyan);
	tcout << _T("\n  BIN\t\t|\tDEC\t|\tHEX") << endl;
	tcout << Color();

	tstringstream MemScharBufferT;
	MemScharBufferT << tsetfill(_T('0'));
	MemScharBufferT << setw(4) << ConversionRadix((char)-10, 2) << _T("\t|\t")
		<< setw(4) << ConversionRadix((char)-10, 10) << _T("\t|\t")
		<< setw(4) << ConversionRadix((char)-10, 16) << endl;
	MemScharBufferT << showbase;
	MemScharBufferT << setw(4) << ConversionRadix((char)-10, 2) << _T("\t|\t")
		<< setw(4) << ConversionRadix((char)-10, 10) << _T("\t|\t")
		<< setw(4) << ConversionRadix((char)-10, 16) << endl;
	tcout << MemScharBufferT.str();


	// Выводим массив из 250 байт в TCHAR поток, как HEX значения
	tcout << Color(cyan);
	tcout << _T("\nВыводим массив из 250 байт в TCHAR поток, как HEX значения:\n");
	tcout << Color();

	tstringstream ArrayHEXdig;
	ArrayHEXdig << tsetfill(_T('0'));
	ArrayHEXdig << uppercase;

	unsigned int CountByte = 250;
	unsigned char* PtrMem = new unsigned char[CountByte];
	for (unsigned int i = 0; i < CountByte; i++)
	{
		ArrayHEXdig << setw(2) << ConversionRadix(PtrMem[i], 16) << _T(" ");
	}
	delete[] PtrMem;
	MemBufferT << endl;

	tcout << ArrayHEXdig.str() << "\n";

	// Тестируем ф-ии из <regex> с разными типами строк:
	tcout << Color(yellow);
	tcout << _T("\n\nTest 13 <regex>: работы регулярных выражений для разных типов строк");
	tcout << _T("\n------------------------------------------------------------------------\n");
	tcout << Color();	

	smatch match_str;
	smatch match_u8str;
	wsmatch match_wstr;
	usmatch match_u16str;
	tsmatch match_tstr;

	// Вызываем regex_match() для разных типов строк
	tcout << Color(cyan);
	tcout << _T("\nВызываем regex_match(), которая проверяет полное соответствие строки выражению regexp ");
	tcout << _T("\nесли строка соответствует, получаем соответствующие подстроки:\n");
	tcout << Color();

	{
		string rus_str = "Мама Мыла Раму";
		string rus_u8str = u8"Мама Мыла Раму";
		wstring rus_wstr = L"Мама Мыла Раму";
		u16string rus_u16str = u"Мама Мыла Раму";
		tstring rus_tstr = _T("Мама Мыла Раму");

		regex   reg_exp("(.*)(мыла)(.*)", std::regex_constants::icase);
		regex   u8reg_exp(u8"(.*)(мыла)(.*)", std::regex_constants::icase);
		wregex  wreg_exp(L"(.*)(мыла)(.*)", std::regex_constants::icase);
		uregex  u16reg_exp(u"(.*)(мыла)(.*)", std::regex_constants::icase);
		tregex  treg_exp(_T("(.*)(мыла)(.*)"), std::regex_constants::icase);

		// char - regex_match()
		tcout << _T("\n\nСодержит ли строка \"Мама Мыла Раму\" выражение \"(.*)(мыла)(.*)\":     regex_match(rus_str, reg_exp)     -> ") << regex_match(rus_str, match_str, reg_exp);
		cout << "\n\tready=" << match_str.ready();
		cout << "\n\tempty=" << match_str.empty();
		cout << "\n\tsize="  << match_str.size();
		cout << "\n\tprefix().matched=" << match_str.prefix().matched;
		cout << "\n\tprefix()=" << string(match_str.prefix());      // match_str.prefix() возвращает вектор, сконвертируем его в строку
		cout << "\n\tsuffix().matched=" << match_str.suffix().matched;
		cout << "\n\tsuffix()=" << string(match_str.suffix());      // match_str.suffix() возвращает вектор, сконвертируем его в строку
		for (size_t i = 0; i < match_str.size(); ++i)
		{
			cout << "\n\t  match_str[" << i << "].matched=" << match_str[i].matched;
			cout << "\n\t  match_str[" << i << "]=" << string(match_str[i]);  // match_str[i] возвращает вектор, сконвертируем его в строку
			cout << "\n\t  match_str.position(" << i << ")=" << match_str.position(i);			
			cout << "\n\t  match_str.length(" << i << ")=" << match_str.length(i);
			cout << "\n\t  match_str.str(" << i << ")=" << match_str.str(i);
			cout << "\n";
		}

		// char utf-8 - regex_match()
		tcout << _T("\nСодержит ли строка u8\"Мама Мыла Раму\" выражение u8\"(.*)(мыла)(.*)\": regex_match(rus_u8str, u8reg_exp) -> ") << regex_match(rus_u8str, match_u8str, u8reg_exp);
		cout << "\n\tready=" << match_u8str.ready();
		cout << "\n\tempty=" << match_u8str.empty();
		cout << "\n\tsize=" << match_u8str.size();
		cout << "\n\tprefix().matched=" << match_u8str.prefix().matched;
		cout << "\n\tprefix()=" << string(match_u8str.prefix());      // match_str.prefix() возвращает вектор, сконвертируем его в строку
		cout << "\n\tsuffix().matched=" << match_u8str.suffix().matched;
		cout << "\n\tsuffix()=" << string(match_u8str.suffix());      // match_str.suffix() возвращает вектор, сконвертируем его в строку
		for (size_t i = 0; i < match_u8str.size(); ++i)
		{
			cout << "\n\t  match_u8str[" << i << "].matched=" << match_u8str[i].matched;
			cout << "\n\t  match_u8str[" << i << "]=" << string(match_u8str[i]);  // match_str[i] возвращает вектор, сконвертируем его в строку
			cout << "\n\t  match_u8str.position(" << i << ")=" << match_u8str.position(i);
			cout << "\n\t  match_u8str.length(" << i << ")=" << match_u8str.length(i);
			cout << "\n\t  match_u8str.str(" << i << ")=" << match_u8str.str(i);
			cout << "\n";
		}

		// wchar_t - regex_match()
		tcout << _T("\nСодержит ли строка L\"Мама Мыла Раму\" выражение L\"(.*)(мыла)(.*)\":   regex_match(rus_wstr, wreg_exp)   -> ") << regex_match(rus_wstr, match_wstr, wreg_exp);
		wcout << L"\n\tready=" << match_wstr.ready();
		wcout << L"\n\tempty=" << match_wstr.empty();
		wcout << L"\n\tsize=" << match_wstr.size();
		wcout << L"\n\tprefix().matched=" << match_wstr.prefix().matched;
		wcout << L"\n\tprefix()=" << wstring(match_wstr.prefix());      // match_str.prefix() возвращает вектор, сконвертируем его в строку
		wcout << L"\n\tsuffix().matched=" << match_wstr.suffix().matched;
		wcout << L"\n\tsuffix()=" << wstring(match_wstr.suffix());      // match_str.suffix() возвращает вектор, сконвертируем его в строку
		for (size_t i = 0; i < match_wstr.size(); ++i)
		{
			wcout << L"\n\t  match_wstr[" << i << L"].matched=" << match_wstr[i].matched;
			wcout << L"\n\t  match_wstr[" << i << L"]=" << wstring(match_wstr[i]);  // match_str[i] возвращает вектор, сконвертируем его в строку
			wcout << L"\n\t  match_wstr.position(" << i << L")=" << match_wstr.position(i);
			wcout << L"\n\t  match_wstr.length(" << i << L")=" << match_wstr.length(i);
			wcout << L"\n\t  match_wstr.str(" << i << L")=" << match_wstr.str(i);
			wcout << L"\n";
		}

		// char16_t - regex_match()
		tcout << _T("\nСодержит ли строка u\"Мама Мыла Раму\" выражение u\"(.*)(мыла)(.*)\":   regex_match(rus_u16str, u16reg_exp) -> ") << regex_match(rus_u16str, match_u16str, u16reg_exp);
		ucout << u"\n\tready=" << match_u16str.ready();
		ucout << u"\n\tempty=" << match_u16str.empty();
		ucout << u"\n\tsize=" << match_u16str.size();
		ucout << u"\n\tprefix().matched=" << match_u16str.prefix().matched;
		ucout << u"\n\tprefix()=" << u16string(match_u16str.prefix());      // match_str.prefix() возвращает вектор, сконвертируем его в строку
		ucout << u"\n\tsuffix().matched=" << match_u16str.suffix().matched;
		ucout << u"\n\tsuffix()=" << u16string(match_u16str.suffix());      // match_str.suffix() возвращает вектор, сконвертируем его в строку
		for (size_t i = 0; i < match_u16str.size(); ++i)
		{
			ucout << u"\n\t  match_u16str[" << i << u"].matched=" << match_u16str[i].matched;
			ucout << u"\n\t  match_u16str[" << i << u"]=" << u16string(match_u16str[i]);  // match_str[i] возвращает вектор, сконвертируем его в строку
			ucout << u"\n\t  match_u16str.position(" << i << u")=" << match_u16str.position(i);
			ucout << u"\n\t  match_u16str.length("  << i << u")=" << match_u16str.length(i);
			ucout << u"\n\t  match_u16str.str(" << i << u")=" << match_u16str.str(i);
			ucout << u"\n";
		}

		// TCHAR - regex_match()
		tcout << _T("\nСодержит ли строка _T(\"Мама Мыла Раму\") выражение _T(\"(.*)(мыла)(.*)\"):   regex_match(rus_tstr, treg_exp)   -> ") << regex_match(rus_tstr, match_tstr, treg_exp);
		tcout << _T("\n\tready=") << match_tstr.ready();
		tcout << _T("\n\tempty=") << match_tstr.empty();
		tcout << _T("\n\tsize=")  << match_tstr.size();
		tcout << _T("\n\tprefix().matched=") << match_tstr.prefix().matched;
		tcout << _T("\n\tprefix()=") << tstring(match_tstr.prefix());      // match_str.prefix() возвращает вектор, сконвертируем его в строку
		tcout << _T("\n\tsuffix().matched=") << match_tstr.suffix().matched;
		tcout << _T("\n\tsuffix()=") << tstring(match_tstr.suffix());      // match_str.suffix() возвращает вектор, сконвертируем его в строку
		for (size_t i = 0; i < match_tstr.size(); ++i)
		{
			tcout << _T("\n\t  match_tstr[") << i << _T("].matched=") << match_tstr[i].matched;
			tcout << _T("\n\t  match_tstr[") << i << _T("]=") << tstring(match_tstr[i]);  // match_str[i] возвращает вектор, сконвертируем его в строку
			tcout << _T("\n\t  match_tstr.position(") << i << _T(")=") << match_tstr.position(i);
			tcout << _T("\n\t  match_tstr.length(")   << i << _T(")=") << match_tstr.length(i);
			tcout << _T("\n\t  match_tstr.str(") << i << _T(")=") << match_tstr.str(i);
			tcout << _T("\n");
		}
		
	}

	// Вызываем regex_search() для разных типов строк
	tcout << Color(cyan);
	tcout << _T("\nВызываем regex_search(), которая проверяет частичное соответствие строки выражению regexp ");
	tcout << _T("\nесли строка соответствует, получаем соответствующие подстроки:\n");
	tcout << Color();

	{
		string rus_str = "Мама Мыла Раму";
		string rus_u8str = u8"Мама Мыла Раму";
		wstring rus_wstr = L"Мама Мыла Раму";
		u16string rus_u16str = u"Мама Мыла Раму";
		tstring rus_tstr = _T("Мама Мыла Раму");

		regex   reg_exp(R"(\sмыла\s)", std::regex_constants::icase);
		regex   u8reg_exp(u8R"(\sмыла\s)", std::regex_constants::icase);
		wregex  wreg_exp(LR"(\sмыла\s)", std::regex_constants::icase);
		uregex  u16reg_exp(uR"(\sмыла\s)", std::regex_constants::icase);
		tregex  treg_exp(_T(R"(\sмыла\s)"), std::regex_constants::icase);

		// char - regex_search()
		tcout << _T("\nСодержит ли строка \"Мама Мыла Раму\" выражение R\"(\\sмыла\\s)\":     regex_search(rus_str, reg_exp)     -> ") << regex_search(rus_str, match_str, reg_exp);
		cout << "\n\tready=" << match_str.ready();
		cout << "\n\tempty=" << match_str.empty();
		cout << "\n\tsize=" << match_str.size();
		cout << "\n\tprefix().matched=" << match_str.prefix().matched;
		cout << "\n\tprefix()=" << string(match_str.prefix());      // match_str.prefix() возвращает вектор, сконвертируем его в строку
		cout << "\n\tsuffix().matched=" << match_str.suffix().matched;
		cout << "\n\tsuffix()=" << string(match_str.suffix());      // match_str.suffix() возвращает вектор, сконвертируем его в строку
		for (size_t i = 0; i < match_str.size(); ++i)
		{
			cout << "\n\t  match_str[" << i << "].matched=" << match_str[i].matched;
			cout << "\n\t  match_str[" << i << "]=" << string(match_str[i]);  // match_str[i] возвращает вектор, сконвертируем его в строку
			cout << "\n\t  match_str.position(" << i << ")=" << match_str.position(i);
			cout << "\n\t  match_str.length("   << i << ")=" << match_str.length(i);
			cout << "\n\t  match_str.str(" << i << ")=" << match_str.str(i);
			cout << "\n";
		}

		// char utf-8 - regex_search()
		tcout << _T("\nСодержит ли строка u8\"Мама Мыла Раму\" выражение u8R\"(\\sмыла\\s)\": regex_search(rus_u8str, u8reg_exp) -> ") << regex_search(rus_u8str, match_u8str, u8reg_exp);
		cout << "\n\tready=" << match_u8str.ready();
		cout << "\n\tempty=" << match_u8str.empty();
		cout << "\n\tsize=" << match_u8str.size();
		cout << "\n\tprefix().matched=" << match_u8str.prefix().matched;
		cout << "\n\tprefix()=" << string(match_u8str.prefix());      // match_str.prefix() возвращает вектор, сконвертируем его в строку
		cout << "\n\tsuffix().matched=" << match_u8str.suffix().matched;
		cout << "\n\tsuffix()=" << string(match_u8str.suffix());      // match_str.suffix() возвращает вектор, сконвертируем его в строку
		for (size_t i = 0; i < match_u8str.size(); ++i)
		{
			cout << "\n\t  match_u8str[" << i << "].matched=" << match_u8str[i].matched;
			cout << "\n\t  match_u8str[" << i << "]=" << string(match_u8str[i]);  // match_str[i] возвращает вектор, сконвертируем его в строку
			cout << "\n\t  match_u8str.position(" << i << ")=" << match_u8str.position(i);
			cout << "\n\t  match_u8str.length("   << i << ")=" << match_u8str.length(i);
			cout << "\n\t  match_u8str.str(" << i << ")=" << match_u8str.str(i);
			cout << "\n";
		}

		// wchar_t - regex_search()
		tcout << _T("\nСодержит ли строка L\"Мама Мыла Раму\" выражение LR\"(\\sмыла\\s)\":   regex_search(rus_wstr, wreg_exp)   -> ") << regex_search(rus_wstr, match_wstr, wreg_exp);
		wcout << L"\n\tready=" << match_wstr.ready();
		wcout << L"\n\tempty=" << match_wstr.empty();
		wcout << L"\n\tsize=" << match_wstr.size();
		wcout << L"\n\tprefix().matched=" << match_wstr.prefix().matched;
		wcout << L"\n\tprefix()=" << wstring(match_wstr.prefix());      // match_str.prefix() возвращает вектор, сконвертируем его в строку
		wcout << L"\n\tsuffix().matched=" << match_wstr.suffix().matched;
		wcout << L"\n\tsuffix()=" << wstring(match_wstr.suffix());      // match_str.suffix() возвращает вектор, сконвертируем его в строку
		for (size_t i = 0; i < match_wstr.size(); ++i)
		{
			wcout << L"\n\t  match_wstr[" << i << L"].matched=" << match_wstr[i].matched;
			wcout << L"\n\t  match_wstr[" << i << L"]=" << wstring(match_wstr[i]);  // match_str[i] возвращает вектор, сконвертируем его в строку
			wcout << L"\n\t  match_wstr.position(" << i << L")=" << match_wstr.position(i);
			wcout << L"\n\t  match_wstr.length(" << i << L")=" << match_wstr.length(i);
			wcout << L"\n\t  match_wstr.str(" << i << L")=" << match_wstr.str(i);
			wcout << L"\n";
		}

		// char16_t - regex_search()
		tcout << _T("\nСодержит ли строка u\"Мама Мыла Раму\" выражение uR\"(\\sмыла\\s)\":   regex_search(rus_u16str, u16reg_exp) -> ") << regex_search(rus_u16str, match_u16str, u16reg_exp);
		ucout << u"\n\tready=" << match_u16str.ready();
		ucout << u"\n\tempty=" << match_u16str.empty();
		ucout << u"\n\tsize=" << match_u16str.size();
		ucout << u"\n\tprefix().matched=" << match_u16str.prefix().matched;
		ucout << u"\n\tprefix()=" << u16string(match_u16str.prefix());      // match_str.prefix() возвращает вектор, сконвертируем его в строку
		ucout << u"\n\tsuffix().matched=" << match_u16str.suffix().matched;
		ucout << u"\n\tsuffix()=" << u16string(match_u16str.suffix());      // match_str.suffix() возвращает вектор, сконвертируем его в строку
		for (size_t i = 0; i < match_u16str.size(); ++i)
		{
			ucout << u"\n\t  match_u16str[" << i << u"].matched=" << match_u16str[i].matched;
			ucout << u"\n\t  match_u16str[" << i << u"]=" << u16string(match_u16str[i]);  // match_str[i] возвращает вектор, сконвертируем его в строку
			ucout << u"\n\t  match_u16str.position(" << i << u")=" << match_u16str.position(i);
			ucout << u"\n\t  match_u16str.length(" << i << u")=" << match_u16str.length(i);
			ucout << u"\n\t  match_u16str.str(" << i << u")=" << match_u16str.str(i);
			ucout << u"\n";
		}

		// TCHAR - regex_search()
		tcout << _T("\nСодержит ли строка _T(\"Мама Мыла Раму\") выражение _T(R\"(\\sмыла\\s)\"):   regex_search(rus_tstr, treg_exp) -> ") << regex_search(rus_tstr, match_tstr, treg_exp);
		tcout << _T("\n\tready=") << match_tstr.ready();
		tcout << _T("\n\tempty=") << match_tstr.empty();
		tcout << _T("\n\tsize=") << match_tstr.size();
		tcout << _T("\n\tprefix().matched=") << match_tstr.prefix().matched;
		tcout << _T("\n\tprefix()=") << tstring(match_tstr.prefix());      // match_str.prefix() возвращает вектор, сконвертируем его в строку
		tcout << _T("\n\tsuffix().matched=") << match_tstr.suffix().matched;
		tcout << _T("\n\tsuffix()=") << tstring(match_tstr.suffix());      // match_str.suffix() возвращает вектор, сконвертируем его в строку
		for (size_t i = 0; i < match_tstr.size(); ++i)
		{
			tcout << _T("\n\t  match_tstr[") << i << _T("].matched=") << match_tstr[i].matched;
			tcout << _T("\n\t  match_tstr[") << i << _T("]=") << tstring(match_tstr[i]);  // match_str[i] возвращает вектор, сконвертируем его в строку
			tcout << _T("\n\t  match_tstr.position(") << i << _T(")=") << match_tstr.position(i);
			tcout << _T("\n\t  match_tstr.length(") << i << _T(")=") << match_tstr.length(i);
			tcout << _T("\n\t  match_tstr.str(") << i << _T(")=") << match_tstr.str(i);
			tcout << _T("\n");
		}
	}

	// Вызываем regex_replace() для разных типов строк
	tcout << Color(cyan);
	tcout << _T("\nВызываем regex_replace(), которая находит подвыражение regexp и заменяет его");
	tcout << _T("\nесли подстрока найдена и заменена, получаем новую измененную строку:\n");
	tcout << Color();

	{
		string rus_str = "Мама Мыла Раму";
		string rus_u8str = u8"Мама Мыла Раму";
		wstring rus_wstr = L"Мама Мыла Раму";
		u16string rus_u16str = u"Мама Мыла Раму";
		tstring rus_tstr = _T("Мама Мыла Раму");

		regex   reg_exp(R"(раму)", std::regex_constants::icase);
		regex   u8reg_exp(u8R"(раму)", std::regex_constants::icase);
		wregex  wreg_exp(LR"(раму)", std::regex_constants::icase);
		uregex  u16reg_exp(uR"(раму)", std::regex_constants::icase);
		tregex  treg_exp(_T(R"(раму)"), std::regex_constants::icase);

		cout << "\nЗаменим в строке \"Мама Мыла Раму\" выражение R\"(раму)\", на \"Рому\":    regex_replace(rus_str, reg_exp,\"Рому\")     -> " << regex_replace(rus_str, reg_exp, "Рому");		
		
		try { cout << "\nЗаменим в строке u8\"Мама Мыла Раму\" выражение u8R\"(раму)\", на u8\"Рому\":  regex_replace(rus_str, reg_exp, u8\"Рому\")     -> " << regex_replace(rus_u8str, u8reg_exp, u8"Рому");}
		catch (...) { cout.clear(); };

		wcout << L"\nЗаменим в строке L\"Мама Мыла Раму\" выражение LR\"(раму)\", на L\"Рому\":  regex_replace(rus_wstr, wreg_exp, L\"Рому\")   -> " << regex_replace(rus_wstr, wreg_exp, L"Рому");
		ucout << u"\nЗаменим в строке u\"Мама Мыла Раму\" выражение uR\"(раму)\", на u\"Рому\":  regex_replace(rus_u16str, u16reg_exp, u\"Рому\") -> " << regex_replace(rus_u16str, u16reg_exp, u"Рому");
		tcout << _T("\nЗаменим в строке _T(\"Мама Мыла Раму\") выражение _T(R\"(раму)\"), на _T(\"Рому\"):  regex_replace(rus_tstr, treg_exp, _T(\"Рому\")) -> ") << regex_replace(rus_tstr, treg_exp, _T("Рому"));
	}

	// Тестируем итераторы токенов из <regex> с разными типами строк:
	tcout << Color(yellow);
	tcout << _T("\n\nTest 14 <regex>: работы итераторов токенов регулярных выражений для разных типов строк");
	tcout << _T("\n------------------------------------------------------------------------\n");
	tcout << Color();

	string str = "-b 'параметр опции b' -j dddd -c -a \"не опция\" -klm \"-e\" w \"-f w\" -- --help";
	string u8str = u8"-b 'параметр опции b' -j dddd -c -a \"не опция\" -klm \"-e\" w \"-f w\" -- --help";
	wstring wstr = L"-b 'параметр опции b' -j dddd -c -a \"не опция\" -klm \"-e\" w \"-f w\" -- --help";
	u16string ustr = u"-b 'параметр опции b' -j dddd -c -a \"не опция\" -klm \"-e\" w \"-f w\" -- --help";
	tstring tstr = _T("-b 'параметр опции b' -j dddd -c -a \"не опция\" -klm \"-e\" w \"-f w\" -- --help");

	regex  reg_exp("(\"[^\"]+\"|\'[^\']+\'|\\S+)");
	regex  u8_reg_exp(u8"(\"[^\"]+\"|\'[^\']+\'|\\S+)");
	wregex  w_reg_exp(L"(\"[^\"]+\"|\'[^\']+\'|\\S+)");
	uregex  u16_reg_exp(u"(\"[^\"]+\"|\'[^\']+\'|\\S+)");
	tregex  t_reg_exp(_T("(\"[^\"]+\"|\'[^\']+\'|\\S+)"));

	tcout << Color(cyan);
	tcout << _T("\nРазобьем строку ") << Color(green) << _T("string") << Color(cyan) << _T(":\n\n");
	tcout << Color(green) << _T("\t\"") << Color() << _T("-b 'параметр опции b' -j dddd -c -a \"не опция\" -klm \"-e\" w \"-f w\" -- --help") << Color(green) << _T("\"");
	tcout << Color(cyan);
	tcout << _T("\n\nна токены, используя итераторы ") << Color(green) << _T("sregex_token_iterator") << Color(cyan) << _T(" регулярного выражения: (\"[^\"]+\"|\'[^\']+\'|\\S+)");
	tcout << _T("\nесли подстрока найдена и заменена, получаем новую измененную строку:\n\t");
	tcout << Color();

	copy(sregex_token_iterator(str.begin(), str.end(), reg_exp, 1),
		 sregex_token_iterator(),
		 ostream_iterator<string>(cout, "\n\t")
	    );

	tcout << Color(cyan);
	tcout << _T("\nРазобьем строку ") << Color(green) << _T("string utf-8") << Color(cyan) << _T(":\n\n");
	tcout << Color(green) << _T("\tu8\"") << Color() << _T("-b 'параметр опции b' -j dddd -c -a \"не опция\" -klm \"-e\" w \"-f w\" -- --help") << Color(green) << _T("\"");
	tcout << Color(cyan);
	tcout << _T("\n\nна токены, используя итераторы ") << Color(green) << _T("sregex_token_iterator") << Color(cyan) << _T(" регулярного выражения: (\"[^\"]+\"|\'[^\']+\'|\\S+)");
	tcout << _T("\nесли подстрока найдена и заменена, получаем новую измененную строку:\n\t");
	tcout << Color();
	try {
		copy(sregex_token_iterator(u8str.begin(), u8str.end(), u8_reg_exp, 1),
			sregex_token_iterator(),
			ostream_iterator<string>(cout, u8"\n\t")
		);
	}
	catch (...) { cout.clear(); };

	tcout << Color(cyan);
	tcout << _T("\nРазобьем строку ") << Color(green) << _T("wstring") << Color(cyan) << _T(":\n\n");
	tcout << Color(green) << _T("\tL\"") << Color() << _T("-b 'параметр опции b' -j dddd -c -a \"не опция\" -klm \"-e\" w \"-f w\" -- --help") << Color(green) << _T("\"");
	tcout << Color(cyan);
	tcout << _T("\n\nна токены, используя итераторы ") << Color(green) << _T("wsregex_token_iterator") << Color(cyan) << _T(" регулярного выражения: (\"[^\"]+\"|\'[^\']+\'|\\S+)");
	tcout << _T("\nесли подстрока найдена и заменена, получаем новую измененную строку:\n\t");
	tcout << Color();

	copy(wsregex_token_iterator(wstr.begin(), wstr.end(), w_reg_exp, 1),
		 wsregex_token_iterator(),
		 ostream_iterator<wstring, wchar_t>(wcout, L"\n\t")
	);

	tcout << Color(cyan);
	tcout << _T("\nРазобьем строку ") << Color(green) << _T("u16string") << Color(cyan) << _T(":\n\n");
	tcout << Color(green) << _T("\tu\"") << Color() << _T("-b 'параметр опции b' -j dddd -c -a \"не опция\" -klm \"-e\" w \"-f w\" -- --help") << Color(green) << _T("\"");
	tcout << Color(cyan);
	tcout << _T("\n\nна токены, используя итераторы ") << Color(green) << _T("usregex_token_iterator") << Color(cyan) << _T(" регулярного выражения: (\"[^\"]+\"|\'[^\']+\'|\\S+)");
	tcout << _T("\nесли подстрока найдена и заменена, получаем новую измененную строку:\n\t");
	tcout << Color();

	copy(usregex_token_iterator(ustr.begin(), ustr.end(), u16_reg_exp, 1),
		 usregex_token_iterator(),
		 ostream_iterator<u16string, char16_t>(ucout, u"\n\t")
	);

	tcout << Color(cyan);
	tcout << _T("\nРазобьем строку ") << Color(green) << _T("tstring") << Color(cyan) << _T(":\n\n");
	tcout << Color(green) << _T("\t_T(\"") << Color() << _T("-b 'параметр опции b' -j dddd -c -a \"не опция\" -klm \"-e\" w \"-f w\" -- --help") << Color(green) << _T("\")");
	tcout << Color(cyan);
	tcout << _T("\n\nна токены, используя итераторы ") << Color(green) << _T("tsregex_token_iterator") << Color(cyan) << _T(" регулярного выражения: (\"[^\"]+\"|\'[^\']+\'|\\S+)");
	tcout << _T("\nесли подстрока найдена и заменена, получаем новую измененную строку:\n\t");
	tcout << Color();
	
	copy(tsregex_token_iterator(tstr.begin(), tstr.end(), t_reg_exp, 1),
		 tsregex_token_iterator(),
		 ostream_iterator<tstring, TCHAR>(tcout, _T("\n\t"))
	    );


	// Тестируем итераторы из <regex> с разными типами строк:
	tcout << Color(yellow);
	tcout << _T("\n\nTest 15 <regex>: работы итераторов регулярных выражений для разных типов строк");
	tcout << _T("\n------------------------------------------------------------------------\n");
	tcout << Color();
	{
		string str = "-b 'параметр опции b'  \"не опция\" \"-e\" w \"-f w\" ";
		string u8str = u8"-b 'параметр опции b'  \"не опция\" \"-e\" w \"-f w\" ";
		wstring wstr = L"-b 'параметр опции b'  \"не опция\" \"-e\" w \"-f w\" ";
		u16string ustr = u"-b 'параметр опции b'  \"не опция\" \"-e\" w \"-f w\" ";
		tstring tstr = _T("-b 'параметр опции b'  \"не опция\" \"-e\" w \"-f w\" ");

		regex   reg_exp("\"[^\"]+\"|\'[^\']+\'|\\S+");
		regex   u8_reg_exp(u8"\"[^\"]+\"|\'[^\']+\'|\\S+");
		wregex  w_reg_exp(L"\"[^\"]+\"|\'[^\']+\'|\\S+");
		uregex  u16_reg_exp(u"\"[^\"]+\"|\'[^\']+\'|\\S+");
		tregex  t_reg_exp(_T("\"[^\"]+\"|\'[^\']+\'|\\S+"));

		tcout << Color(cyan);
		tcout << _T("\nРазобьем строку ") << Color(green) << _T("string") << Color(cyan) << _T(":\n\n");
		tcout << Color(green) << _T("\t\"") << Color() << _T("-b 'параметр опции b'  \"не опция\" \"-e\" w \"-f w\" ") << Color(green) << _T("\"");
		tcout << Color(cyan);
		tcout << _T("\n\nна токены, используя итераторы ") << Color(green) << _T("sregex_iterator") << Color(cyan) << _T(" регулярного выражения: \"[^\"]+\"|\'[^\']+\'|\\S+");
		tcout << _T("\nесли подстрока найдена и заменена, получаем новую измененную строку:\n\t");
		tcout << Color();

		auto beg = sregex_iterator(str.begin(), str.end(), reg_exp);
		auto end = sregex_iterator();

		cout << "\nВсего найдено " << std::distance(beg, end) << " соответствий для данного регулярного выражения.";

		for_each(beg, end, [&](const smatch& match)
		{
			cout << "\n\tready=" << match.ready();
			cout << "\n\tempty=" << match.empty();
			cout << "\n\tsize=" << match.size();
			cout << "\n\tsuffix()=" << string(match.suffix());      // match_str.suffix() возвращает вектор, сконвертируем его в строку
			for (size_t i = 0; i < match.size(); ++i)
			{
				cout << "\n\t  match_str.position(" << i << ")=" << match.position(i);
				cout << "\n\t  match_str.length(" << i << ")=" << match.length(i);
				cout << "\n\t  match_str.str(" << i << ")=" << match.str(i);
				cout << "\n";
			}
		});

		tcout << Color(cyan);
		tcout << _T("\nРазобьем строку ") << Color(green) << _T("string utf-8") << Color(cyan) << _T(":\n\n");
		tcout << Color(green) << _T("\tu8\"") << Color() << _T("-b 'параметр опции b'  \"не опция\" \"-e\" w \"-f w\" ") << Color(green) << _T("\"");
		tcout << Color(cyan);
		tcout << _T("\n\nна токены, используя итераторы ") << Color(green) << _T("sregex_iterator") << Color(cyan) << _T(" регулярного выражения: \"[^\"]+\"|\'[^\']+\'|\\S+");
		tcout << _T("\nесли подстрока найдена и заменена, получаем новую измененную строку:\n\t");
		tcout << Color();

		auto u8beg = sregex_iterator(u8str.begin(), u8str.end(), u8_reg_exp);
		auto u8end = sregex_iterator();

		cout << "\nВсего найдено " << std::distance(u8beg, u8end) << " соответствий для данного регулярного выражения.";
		try 
		{
			for_each(u8beg, u8end, [&](const smatch& match)
			{
				cout << "\n\tready=" << match.ready();
				cout << "\n\tempty=" << match.empty();
				cout << "\n\tsize=" << match.size();
				cout << "\n\tsuffix()=" << string(match.suffix());      // match_str.suffix() возвращает вектор, сконвертируем его в строку
				for (size_t i = 0; i < match.size(); ++i)
				{
					cout << "\n\t  match_str.position(" << i << ")=" << match.position(i);
					cout << "\n\t  match_str.length(" << i << ")=" << match.length(i);
					cout << "\n\t  match_str.str(" << i << ")=" << match.str(i);
					cout << "\n";
				}
			});
		}
		catch (...) { cout.clear(); }

		tcout << Color(cyan);
		tcout << _T("\nРазобьем строку ") << Color(green) << _T("wstring") << Color(cyan) << _T(":\n\n");
		tcout << Color(green) << _T("\tL\"") << Color() << _T("-b 'параметр опции b'  \"не опция\" \"-e\" w \"-f w\" ") << Color(green) << _T("\"");
		tcout << Color(cyan);
		tcout << _T("\n\nна токены, используя итераторы ") << Color(green) << _T("wsregex_iterator") << Color(cyan) << _T(" регулярного выражения: \"[^\"]+\"|\'[^\']+\'|\\S+");
		tcout << _T("\nесли подстрока найдена и заменена, получаем новую измененную строку:\n\t");
		tcout << Color();

		auto wbeg = wsregex_iterator(wstr.begin(), wstr.end(), w_reg_exp);
		auto wend = wsregex_iterator();

		wcout << L"\nВсего найдено " << std::distance(wbeg, wend) << L" соответствий для данного регулярного выражения.";

		for_each(wbeg, wend, [&](const wsmatch& match)
		{
			wcout << "\n\tready=" << match.ready();
			wcout << "\n\tempty=" << match.empty();
			wcout << "\n\tsize=" << match.size();
			wcout << "\n\tsuffix()=" << wstring(match.suffix());      // match_str.suffix() возвращает вектор, сконвертируем его в строку
			for (size_t i = 0; i < match.size(); ++i)
			{
				wcout << L"\n\t  match_str.position(" << i << L")=" << match.position(i);
				wcout << L"\n\t  match_str.length(" << i << L")=" << match.length(i);
				wcout << L"\n\t  match_str.str(" << i << L")=" << match.str(i);
				wcout << L"\n";
			}
		});

		tcout << Color(cyan);
		tcout << _T("\nРазобьем строку ") << Color(green) << _T("u16string") << Color(cyan) << _T(":\n\n");
		tcout << Color(green) << _T("\tu\"") << Color() << _T("-b 'параметр опции b'  \"не опция\" \"-e\" w \"-f w\" ") << Color(green) << _T("\"");
		tcout << Color(cyan);
		tcout << _T("\n\nна токены, используя итераторы ") << Color(green) << _T("usregex_iterator") << Color(cyan) << _T(" регулярного выражения: \"[^\"]+\"|\'[^\']+\'|\\S+");
		tcout << _T("\nесли подстрока найдена и заменена, получаем новую измененную строку:\n\t");
		tcout << Color();

		auto ubeg = usregex_iterator(ustr.begin(), ustr.end(), u16_reg_exp);
		auto uend = usregex_iterator();

		ucout << u"\nВсего найдено " << std::distance(ubeg, uend) << u" соответствий для данного регулярного выражения.";

		for_each(ubeg, uend, [&](const usmatch& match)
		{
			ucout << u"\n\tready=" << match.ready();
			ucout << u"\n\tempty=" << match.empty();
			ucout << u"\n\tsize=" << match.size();
			ucout << u"\n\tsuffix()=" << u16string(match.suffix());      // match_str.suffix() возвращает вектор, сконвертируем его в строку
			for (size_t i = 0; i < match.size(); ++i)
			{
				ucout << u"\n\t  match_str.position(" << i << u")=" << match.position(i);
				ucout << u"\n\t  match_str.length(" << i << u")=" << match.length(i);
				ucout << u"\n\t  match_str.str(" << i << u")=" << match.str(i);
				ucout << u"\n";
			}
		});

		tcout << Color(cyan);
		tcout << _T("\nРазобьем строку ") << Color(green) << _T("tstring") << Color(cyan) << _T(":\n\n");
		tcout << Color(green) << _T("\t_T(\"") << Color() << _T("-b 'параметр опции b'  \"не опция\" \"-e\" w \"-f w\" ") << Color(green) << _T("\")");
		tcout << Color(cyan);
		tcout << _T("\n\nна токены, используя итераторы ") << Color(green) << _T("tsregex_iterator") << Color(cyan) << _T(" регулярного выражения: \"[^\"]+\"|\'[^\']+\'|\\S+");
		tcout << _T("\nесли подстрока найдена и заменена, получаем новую измененную строку:\n\t");
		tcout << Color();

		auto tbeg = tsregex_iterator(tstr.begin(), tstr.end(), t_reg_exp);
		auto tend = tsregex_iterator();

		tcout << _T("\nВсего найдено ") << std::distance(ubeg, uend) << _T(" соответствий для данного регулярного выражения.");

		for_each(tbeg, tend, [&](const tsmatch& match)
		{
			tcout << _T("\n\tready=") << match.ready();
			tcout << _T("\n\tempty=") << match.empty();
			tcout << _T("\n\tsize=") << match.size();
			tcout << _T("\n\tsuffix()=") << tstring(match.suffix());      // match_str.suffix() возвращает вектор, сконвертируем его в строку
			for (size_t i = 0; i < match.size(); ++i)
			{
				tcout << _T("\n\t  match_str.position(") << i << _T(")=") << match.position(i);
				tcout << _T("\n\t  match_str.length(") << i << _T(")=") << match.length(i);
				tcout << _T("\n\t  match_str.str(") << i << _T(")=") << match.str(i);
				tcout << _T("\n");
			}
		});
	}

	tcout << _T("\n\n");

	
	return 0;
}


