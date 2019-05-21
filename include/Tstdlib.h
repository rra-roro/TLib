//////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Файл Tstdlib.h     - Содержит ф-ии конвертации чисел в строку
//                         
//    Версия файла 1.00  - первая версия.
//
//

#ifndef TSTDLIB
#define TSTDLIB

#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#endif

#include <Tstring.h>

namespace tlib
{
	////////////////////////////////////////////////////////////////////////////////
	//
	//     Перегруженная ф-ия для конвертации числа в строку в произвольной системе счисления
	//

	// Шаблон который Выполняет основную работу по конвертации
	template<class  _Elem>
		inline bool xtox_s( unsigned long long val, std::basic_string<_Elem>& buf, unsigned radix, int is_neg, bool UpShift )
		{
				unsigned digval;
				if(radix < 2 || radix > 36 )return false;

				if (is_neg) {
						/* negative, so output '-' and negate */
						val = (unsigned long long)(-(long long)val);
				}

				do {
						digval = (unsigned) (val % radix);
						val /= radix;       /* get next digit */

						/* convert to ascii and store */
						if (digval > 9)
								if(UpShift)buf =  std::basic_string<_Elem> (1,((_Elem)(digval - 10 + TemplateTypeOfCh('A',_Elem)))) + buf;  /* a letter */
								else   buf =  std::basic_string<_Elem> (1,((_Elem)(digval - 10 + TemplateTypeOfCh('a',_Elem)))) + buf;  /* a letter */
						else
								buf =  std::basic_string<_Elem> (1,((_Elem)(digval + TemplateTypeOfCh('0',_Elem)))) + buf;       /* a digit */
				} while (val > 0);

				if (is_neg) {
						/* negative, so output '-' and negate */
						buf = TemplateTypeOfCh('-',_Elem) + buf;
				}
				return true;
		}

	//    dig_to_tstr  - конвертирует указатель в строку в заданной системе счисления	
	template<class  _Elem>
		bool  dig_to_tstr(void* val, std::basic_string<_Elem>& tstr, int radix, bool UpShift=false)
		{
			return xtox_s((unsigned long long)val, tstr, radix, 0, UpShift);
		}
	
	//    dig_to_tstr  - конвертирует число long long в строку в заданной системе счисления	
	template<class  _Elem>
		bool  dig_to_tstr(long long val, std::basic_string<_Elem>& tstr, int radix, bool UpShift=false)
		{
			return xtox_s((unsigned long long)val, tstr, radix, (radix == 10 && val < 0), UpShift);
		}

	//    dig_to_tstr  - конвертирует число unsigned long long в строку в заданной системе счисления
	template<class  _Elem>
		bool  dig_to_tstr(unsigned long long val, std::basic_string<_Elem>& tstr, int radix, bool UpShift=false)
		{
			return xtox_s(val, tstr, radix, 0, UpShift);
		}

	//    dig_to_tstr  - конвертирует число long в строку в заданной системе счисления	
	template<class  _Elem>
		bool  dig_to_tstr(long val, std::basic_string<_Elem>& tstr, int radix, bool UpShift=false)
		{
			if (radix == 10 && val < 0)
				return xtox_s((unsigned long long)val, tstr, radix, 1, UpShift);
			else
				return xtox_s((unsigned long long)(unsigned long)val, tstr, radix, 0, UpShift);
		}

	//    dig_to_tstr  - конвертирует число unsigned long в строку в заданной системе счисления
	template<class  _Elem>
		bool  dig_to_tstr(unsigned long val, std::basic_string<_Elem>& tstr, int radix, bool UpShift=false)
		{
			return xtox_s((unsigned long long)val, tstr, radix, 0, UpShift);
		}

	//    dig_to_tstr  - конвертирует число short в строку в заданной системе счисления
	template<class  _Elem>
		bool  dig_to_tstr(short val, std::basic_string<_Elem>& tstr, int radix, bool UpShift=false)
		{
			if (radix == 10 && val < 0)
				return xtox_s((unsigned long long)val, tstr, radix, 1, UpShift);
			else
				return xtox_s((unsigned long long)(unsigned long)(unsigned short)val, tstr, radix, 0, UpShift);
		}

	//    dig_to_tstr  - конвертирует число unsigned short в строку в заданной системе счисления
	template<class  _Elem>
		bool  dig_to_tstr(unsigned short val, std::basic_string<_Elem>& tstr, int radix, bool UpShift=false)
		{
			return xtox_s((unsigned long long)(unsigned long)(unsigned short)val, tstr, radix, 0, UpShift);
		}

	//    dig_to_tstr  - конвертирует число int в строку в заданной системе счисления
	template<class  _Elem>
		bool  dig_to_tstr(int val, std::basic_string<_Elem>& tstr, int radix, bool UpShift=false)
		{
			if (radix == 10 && val < 0)
				return xtox_s((unsigned long long)val, tstr, radix, 1, UpShift);
			else
				return xtox_s((unsigned long long)(unsigned long)(unsigned int)val, tstr, radix, 0, UpShift);
		}

	//    dig_to_tstr  - конвертирует число unsigned int в строку в заданной системе счисления
	template<class  _Elem>
		bool  dig_to_tstr(unsigned int val, std::basic_string<_Elem>& tstr, int radix, bool UpShift=false)
		{
			return xtox_s((unsigned long long)(unsigned long)(unsigned int)val, tstr, radix, 0, UpShift);
		}

	//    dig_to_tstr  - конвертирует число char в строку в заданной системе счисления
	template<class  _Elem>
		bool  dig_to_tstr(char val, std::basic_string<_Elem>& tstr, int radix, bool UpShift=false)
		{
			if (radix == 10 && val < 0)
				return xtox_s((unsigned long long)val, tstr, radix, 1, UpShift);
			else
				return xtox_s((unsigned long long)(unsigned long)(unsigned char)val, tstr, radix, 0, UpShift);
		}

	//    dig_to_tstr  - конвертирует число unsigned char в строку в заданной системе счисления
	template<class  _Elem>
		bool  dig_to_tstr(unsigned char val, std::basic_string<_Elem>& tstr, int radix, bool UpShift=false)
		{
			return xtox_s((unsigned long long)(unsigned long)(unsigned char)val, tstr, radix, 0, UpShift);
		}

	////////////////////////////////////////////////////////////////////////////////
	//
	//     Ф-ии для конвертации числа в строку в произвольной системе счисления
	//
        template <typename D>
        inline tlib::tstring GetStrOfDig (D val, int radix, bool UpShift=false)
        {
            tlib::tstring TempStr;
            if(!dig_to_tstr (val, TempStr,radix, UpShift)) return tlib::tstring();
            else return TempStr;
        }

}

#endif // TSTDLIB
