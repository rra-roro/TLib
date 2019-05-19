/*
*   Файл Tiomanip.h - содержит реализацию манипуляторов
*
*   Версия файла 1.0
*   Версия файла 2.0 - Добавил манипулятр TabRight -выравнивает текст в колонку, прижатую
*                      к правой стороне окна.
*
*/

#include "stdafx.h"
#include <Tiomanip.h>
#include <Tstdlib.h>
#include <Tlocale.h>

using namespace std;

namespace Tstring
{
	template std::basic_ostream<char>& outGuid::PrintGuid(std::basic_ostream<char>& iostr) const;
	template std::basic_ostream<wchar_t>& outGuid::PrintGuid(std::basic_ostream<wchar_t>& iostr) const;

#ifdef _WIN32
	template std::basic_ostream<char>& TabRight::PrintTabRight(std::basic_ostream<char>& iostr);
	template std::basic_ostream<wchar_t>& TabRight::PrintTabRight(std::basic_ostream<wchar_t>& iostr);

    template std::basic_ostream<char>& outFILETIME::PrintFileTime(std::basic_ostream<char>& iostr) const;
	template std::basic_ostream<wchar_t>& outFILETIME::PrintFileTime(std::basic_ostream<wchar_t>& iostr) const;
#endif

	template std::basic_ostream<char>& ConversionRadix::C_Radix(std::basic_ostream<char>& iostr) const;
	template std::basic_ostream<wchar_t>& ConversionRadix::C_Radix(std::basic_ostream<wchar_t>& iostr) const;
}






