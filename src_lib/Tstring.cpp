﻿/*
*   Файл Tstring.cpp - содержит макроссы и классы необходимые для реализации строк
*                      в независемости от типа кодировки Ansi или Unicode
*   Версия файла 1.0
*   Версия файла 2.0 - Исправил вывод hex значений
*   Версия файла 3.0 - Добавил в область видимости Tstring константу npos
*                      А также определил tostream и tistream
*   Версия файла 4.0 - Добавил поддержку платформы WINCE
*   Версия файла 5.0 - Перекрыл все конструкторы базового класса
*   Версия файла 6.0 - Отказался от промежуточного класса _string/_wstring,
*                      исправил вывод для cout в зависемости от заданной локали,
*                      задал функции для конвертирования string и wstring друг в друга,
*                      все буфферы реализованы через вектора.
*   Версия файла 7.0 - Ввел новы ф-ии конвертации строк из tstring в string и wstring и обратно.
*                      Локали стандартных потоков ввода вывода теперь работают только в консольных _CONSOLE
*                      приложениях, иначи могут вызывать сбой. В силу отсутствия таких потоков в GUI прогах.
*                      Изменил способ конвертаци символов из OEM в ANSI, отказавшись от OemToCharA.
*                      Таким образом, избавился от зависимости с user32.dll
*   Версия файла 8.0 - Добавил макросы TemplateTypeOfStr(str,type) и TemplateTypeOfCh(str,type), 
*                      которые позволяют задавать в явном виде тип char и wchar_t строки и символа в Шаблонах
*
*/
#include "stdafx.h"
#include <Tstring.h>
#include <Tlocale.h>

using namespace std;

namespace tlib
{

//Создадим ссылку на глобальные объекты консоли в зависемости от того Ansi или Unicode
#ifdef _WIN32

	#ifdef _UNICODE
	   const size_t npos = std::wstring::npos;
	#else
	   const size_t npos = std::string::npos;
	#endif

#elif __linux__

	const size_t npos = std::u16string::npos;

#endif

   //   ProgramCodePage <--> wchar_t
   thread_local std::wstring_convert<codecvt_w> wconv(new codecvt_w(GetLocaleNameProgram()));

   //   utf8 <--> char16_t
   thread_local std::wstring_convert<codecvt_u16, char16_t> u16conv(new codecvt_u16(GetLocaleNameProgram()));

   //   utf8 <--> wchar_t
   thread_local std::wstring_convert<std::codecvt_utf8<wchar_t>> u8conv;
}
