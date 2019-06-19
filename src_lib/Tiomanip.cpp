/*
*   Файл Tiomanip.h - содержит реализацию манипуляторов
*
*   Версия файла 1.0
*   Версия файла 2.0 - Добавил манипулятр tab_right -выравнивает текст в колонку, прижатую
*                      к правой стороне окна.
*
*/

#include "stdafx.h"
#include <Tiomanip.h>
#include <iostream>



using namespace std;

namespace tlib
{
	template std::basic_ostream<char>& put_guid::PrintGuid(std::basic_ostream<char>& iostr) const;
      template std::basic_ostream<wchar_t>& put_guid::PrintGuid(std::basic_ostream<wchar_t>& iostr) const;
      template std::basic_ostream<char16_t>& put_guid::PrintGuid(std::basic_ostream<char16_t>& iostr) const;

#ifdef _WIN32
      template tab_right_t<char>;
      template tab_right_t<wchar_t>;
      template tab_right_t<char16_t>;

      template std::basic_ostream<char>& put_filetime::PrintFileTime(std::basic_ostream<char>& iostr) const;
      template std::basic_ostream<wchar_t>& put_filetime::PrintFileTime(std::basic_ostream<wchar_t>& iostr) const;
      template std::basic_ostream<char16_t>& put_filetime::PrintFileTime(std::basic_ostream<char16_t>& iostr) const;
#endif

	template std::basic_ostream<char>& put_intger_by_radix<char>::C_Radix(std::basic_ostream<char>& iostr) const;
      template std::basic_ostream<wchar_t>& put_intger_by_radix<wchar_t>::C_Radix(std::basic_ostream<wchar_t>& iostr) const;
      template std::basic_ostream<char16_t>& put_intger_by_radix<char16_t>::C_Radix(std::basic_ostream<char16_t>& iostr) const;
}






