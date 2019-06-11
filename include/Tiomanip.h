/*
*   Файл Tiomanip.h - содержит классы для создания манипуляторов потоков с аргументами
*
*   Версия файла 1.0
*   Версия файла 2.0 - Добавил манипулятр TabRight -выравнивает текст в колонку, прижатую
*                      к правой стороне окна.
*
*/


#ifndef TIO_MANIP
#define TIO_MANIP

#ifdef _WIN32
      #include <Windows.h>
#elif __linux__
      #include <linux/linux.h>
#endif

#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
      #define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#endif

#include <Tstring.h>
#include <Tstdlib.h>
#include <Tiostream.h>
#include <Tlocale.h>
#include <iomanip>

namespace tlib
{

      //========================================================================================
      //Внесем в область видимости стандартные манипуляторы из <ios>

      using std::dec; // Для изменения отображения системы счисления у чисел
      using std::hex;
      using std::oct;

      using std::showbase; // Показывать или нет основание системы счисления
      using std::noshowbase;

      using std::uppercase; // Выводить числа маленькими или большими буквами
      using std::nouppercase;

      using std::boolalpha; // Выводить значение bool надписями true/false или цифрами 1/0
      using std::noboolalpha;

      using std::showpos; // Выводить знак "+" у положительного числа
      using std::noshowpos;

      using std::showpoint;   // Выводить ли точку и последующии нули у чисел с плавоющей точкой,
      using std::noshowpoint; // если число строго равно целому числу. (Например: вывести 5.0000 или 5)

      using std::fixed;      // Выводим число так:   100.23
      using std::scientific; // или так:             1.002300e+002

      using std::right;
      using std::left;

      using std::internal;
      using std::setw; // Задаем размер поля, этот флаг сбрасывается в 0 после каждого вывода
                       // переменной, текста или цифры, например для каждого ConversionRadixUL
                       // нужно повторно выставить этот флаг:
                       // tcout << setw(2) << ConversionRadixULL(0xf,16) << setw(2) << ConversionRadixULL(0xf,16) << endl;

      using std::skipws;
      using std::noskipws;

      using std::unitbuf;
      using std::nounitbuf;


      //========================================================================================
      //Внесем в область видимости стандартные манипуляторы из <ostream>

      using std::endl;  //Добавляет в поток символ перевода строки '\n'
      using std::ends;  //Добавляет в поток нулевой символ строки '\0'
      using std::flush; //Если в буфере потока остались символы довыводит их, и очищает буфер

      //========================================================================================

      template <class _Elem>
      inline auto tsetfill(_Elem _Ch)
      {
            return std::setfill<_Elem>(_Ch);
      }

      //#ifdef __linux__
      //
      //	template<>
      //	inline auto tsetfill(char16_t _Ch)
      //	{
      //		return std::setfill<char>(std::use_facet<std::ctype<char16_t> >(GetLocaleProgram()).narrow(_Ch,'\0'));
      //	}
      //
      //#endif // __linux__

      //========================================================================================
      // Объявим манипуляторы с одним аргументом:

      // Манипулятор outGuid() Превращает GUID в строку
      class outGuid
      {
            const GUID& guid;

        public:
            outGuid(const GUID& guid_in) : guid(guid_in){};

            template <class _Elem>
            friend inline std::basic_ostream<_Elem>& operator<<(std::basic_ostream<_Elem>& os, const outGuid& oG)
            {
                  return oG.PrintGuid(os);
            }

        private:
            template <typename _Elem>
            std::basic_ostream<_Elem>& PrintGuid(std::basic_ostream<_Elem>& iostr) const
            {
                  std::streamsize W = iostr.width();
                  _Elem F = iostr.fill();
                  iostr << hex << tsetfill<_Elem>(TemplateTypeOfCh('0', _Elem));

                  iostr << setw(8) << guid.Data1;
                  iostr << setw(4) << guid.Data2;
                  iostr << setw(4) << guid.Data3;
                  for (int i = 0; i < 8; i++)
                        iostr << setw(2) << ((unsigned char*)&guid.Data4)[i];

                  iostr.fill(F);
                  iostr.width(W);
                  return iostr;
            }
      };

      extern template std::basic_ostream<char>& outGuid::PrintGuid(std::basic_ostream<char>& iostr) const;
      extern template std::basic_ostream<wchar_t>& outGuid::PrintGuid(std::basic_ostream<wchar_t>& iostr) const;


#ifdef _WIN32
      // Манипулятор TabRight()
      // Выводит строку str_in, так что бы в случае если она не уместится
      // на одной строке экрана, на новой строке экрана она выводилась строго
      // под началом первой строки. Как бы колоночку формирует.
      class TabRight
      {
            tstring str;

        public:
            TabRight(const tstring& str_in) : str(str_in){};
            TabRight(const TCHAR* str_in) : str(str_in){};

            template <class _Elem>
            friend inline std::basic_ostream<_Elem>& operator<<(std::basic_ostream<_Elem>& os, const TabRight& tr)
            {
                  return tr.PrintTabRight(os);
            }

        private:
            inline COORD GetCursorPosition()
            {
                  HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
                  CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
                  GetConsoleScreenBufferInfo(hConsoleOutput, &csbiInfo);
                  return csbiInfo.dwCursorPosition;
            }

            inline COORD GetSizeWindow()
            {
                  HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
                  CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
                  GetConsoleScreenBufferInfo(hConsoleOutput, &csbiInfo);
                  return csbiInfo.dwSize;
            }

            template <typename _Elem>
            std::basic_ostream<_Elem>& PrintTabRight(std::basic_ostream<_Elem>& iostr)
            {
                  COORD CurCoord = GetCursorPosition();
                  COORD SizeWindow = GetSizeWindow();
                  size_t LenghtStr = SizeWindow.X - CurCoord.X - 1;
                  std::basic_string<_Elem> strView;
                  //Удаление повторяющихся пробелов
                  while (true)
                  {
                        size_t Pos = str.find(_T("  "));
                        if (Pos == npos)
                              break;
                        str.replace(Pos, 2, _T(" "));
                  }

                  while (str.size() > LenghtStr)
                  {
                        tstring strLeft = str.substr(0, str.find_last_of(_T(' '), LenghtStr));
                        str = str.substr(str.find_last_of(_T(' '), LenghtStr) + 1, npos);
                        for (size_t Pos = strLeft.find_first_of(_T(' '), 0); strLeft.size() < LenghtStr; Pos = strLeft.find_first_of(_T(' '), Pos))
                        {
                              if (Pos != npos)
                                    strLeft.insert(Pos, 1, _T(' '));
                              Pos += 2;
                        }

                        strLeft = strLeft + _T("\n") + tstring(CurCoord.X, _T(' '));

                        strView = Tstr2TemplateStr<_Elem>(strLeft);

                        iostr << strView << endl;
                  }

                  strView = Tstr2TemplateStr<_Elem>(str);

                  return iostr << strView;
            }
      };

      extern template std::basic_ostream<char>& TabRight::PrintTabRight(std::basic_ostream<char>& iostr);
      extern template std::basic_ostream<wchar_t>& TabRight::PrintTabRight(std::basic_ostream<wchar_t>& iostr);

      // Манипулятор outFILETIME() Превращает FILETIME в строку
      class outFILETIME
      {
            const FILETIME& filetime;

        public:
            outFILETIME(const FILETIME& filetime_in) : filetime(filetime_in){};

            template <class _Elem>
            friend inline std::basic_ostream<_Elem>& operator<<(std::basic_ostream<_Elem>& os, const outFILETIME& oF)
            {
                  return oF.PrintFileTime(os);
            }

        private:
            template <typename _Elem>
            std::basic_ostream<_Elem>& PrintFileTime(std::basic_ostream<_Elem>& iostr) const
            {
                  SYSTEMTIME SystemTime = { 0 };
                  BOOL ret = FileTimeToSystemTime(&filetime, &SystemTime);
                  if (ret)
                  {
                        //streamsize W = iostr.width();
                        _Elem F = iostr.fill();
                        iostr << dec << tsetfill<_Elem>(TemplateTypeOfCh('0', _Elem));
                        iostr << setw(2) << SystemTime.wDay << TemplateTypeOfStr(".", _Elem);
                        iostr << setw(2) << SystemTime.wMonth << TemplateTypeOfStr(".", _Elem);
                        iostr << setw(4) << SystemTime.wYear << TemplateTypeOfStr(" ", _Elem);
                        iostr << setw(2) << SystemTime.wHour << TemplateTypeOfStr(":", _Elem);
                        iostr << setw(2) << SystemTime.wMinute << TemplateTypeOfStr(":", _Elem);
                        iostr << setw(2) << SystemTime.wSecond << TemplateTypeOfStr(":", _Elem);
                        iostr << setw(4) << SystemTime.wMilliseconds;
                        iostr.fill(F);
                        //iostr.width(W);
                  }

                  return iostr;
            }
      };

      extern template std::basic_ostream<char>& outFILETIME::PrintFileTime(std::basic_ostream<char>& iostr) const;
      extern template std::basic_ostream<wchar_t>& outFILETIME::PrintFileTime(std::basic_ostream<wchar_t>& iostr) const;

#endif

      //========================================================================================
      // Объявим манипуляторы с двумя аргументами:

      // Манипулятор Color()
      // для изменения цвета выводимого текста
      // ColorText         - Цвет текста
      // ColorBackground   - Цвет фона

      enum EnumColor
      {
            black = 49,
            dark_red = 31,
            dark_green = 32,
            dark_yellow = 33,
            dark_blue = 34,
            dark_pink = 35,
            dark_cyan = 36,
            dark_grey = 37,
            red = 91,
            green = 92,
            yellow = 93,
            blue = 94,
            pink = 95,
            cyan = 96,
            grey = 97,
            white = 39,
      };

      class Color
      {
            EnumColor ColorText;
            EnumColor ColorFone;

        public:
            Color(EnumColor ColorText = grey, EnumColor ColorFone = black) : ColorText(ColorText), ColorFone(ColorFone){};

            friend inline std::iostream& operator<<(std::iostream& ios, [[maybe_unused]] const Color& c) { return ios; };
            friend inline std::wiostream& operator<<(std::wiostream& wios, [[maybe_unused]] const Color& c) { return wios; };

            template <class _Elem>
            friend inline std::basic_ostream<_Elem>& operator<<(std::basic_ostream<_Elem>& os, const Color& c)
            {
                  //os << TemplateTypeOfStr("\033[", _Elem) << (c.ColorFone + 10) << TemplateTypeOfStr(";", _Elem) << c.ColorText << TemplateTypeOfStr("m", _Elem);
                  //os.clear();
                  return os;
            }
      };

      //========================================================================================
      // Объявим манипуляторы с тремя аргументами:

      // Манипуляторы     ConversionRadixULL() и ConversionRadixLL()
      // для вывода в поток числа в произвольной системе счисления
      // value   - конвертируемое число
      // radix   - требуемое основание системы счисления
      // group   - разбивать ли число на групы символов (скажем по 8 бит в 2-ой сист счисл)
      //           и задаем количество символов в группе.
      // 1) с TCHAR потоками можно вызывать так
      //        ConversionRadixULL(unsigned long value,int radix,int group)
      // 2) с char потоками требуется указывать спецификацию шаблона так
      //        ConversionRadixULL <char> (unsigned long value,int radix,int group)
      // 3) с wchar_t потоками требуется указывать спецификацию шаблона так
      //	     ConversionRadixULL <wchar_t> (unsigned long value,int radix,int group)

      // После того, как будет произведена запись в поток, поток сбросит ширину поля
      // (поток вызовет iostr.width(0)), таким образом ширину поля нужно выставлять
      // перед каждой записью числа в поток, в нашем случае перед каждым вызовом ConversionRadixULL().
      // Например:
      //    1)      MemBuffer << setw(2)<< setfill('0');
      //            MemBuffer << ConversionRadixULL<char>(0xf,16) << "\t"
      // 	                    << ConversionRadixULL<char>(0xf,16) << "\t";      Ошибка!!!
      //
      //    2)      MemBuffer << setfill('0');
      //            MemBuffer << setw(2) << ConversionRadixULL<char>(0xf,16) << "\t"
      // 	                    << setw(2) << ConversionRadixULL<char>(0xf,16) << "\t";      Ok!!!

      class ConversionRadix
      {
            tstring value_str;
            int radix;
            int group;
            bool IsSigned;

        public:
            template <class T, bool isSigned = (T(-1) < T(0))>
            ConversionRadix(T value, int radix, int group = 0) : value_str(GetStrOfDig(value, radix)),
                                                                 radix(radix), group(group),
                                                                 IsSigned(isSigned && (value < 0))
            {
            }

            template <class _Elem>
            friend inline std::basic_ostream<_Elem>& operator<<(std::basic_ostream<_Elem>& os, const ConversionRadix& cr)
            {
                  return cr.C_Radix(os);
            }

        private:
            template <typename _Elem>
            std::basic_ostream<_Elem>& C_Radix(std::basic_ostream<_Elem>& iostr) const
            {
                  if (radix < 2 || radix > 36)
                  {
                        iostr << TemplateTypeOfStr("Error Radix", _Elem);
                        return iostr;
                  }

                  int FlagShowbase = iostr.flags() & std::ios::showbase;
                  int FlagUppercase = iostr.flags() & std::ios::uppercase;

                  std::basic_string<_Elem> IntegerStr = Tstr2TemplateStr<_Elem>(value_str);

                  if (radix > 10 && FlagUppercase)
                        IntegerStr = GetUpperStr(IntegerStr);

                  // вычислим количество нулей необходимых для дополнения числа до ширины поля
                  if (IsSigned)
                        iostr << setw(0);
                  std::streamsize Width = iostr.width() - IntegerStr.length();

                  if (Width > 0 && !IsSigned)
                  {
                        IntegerStr = std::basic_string<_Elem>((unsigned int)Width, _T('0')) + IntegerStr;
                  }

                  if (group != 0)
                  {
                        size_t i = IntegerStr.length() - group;
                        for (; i < IntegerStr.length() && i != 0; i -= group) //проверяем, чтобы i не переполнилось при вычитани,
                        {                                                     //т.е. не стало меньше или равно 0
                              if (FlagShowbase)
                              {
                                    switch (radix)
                                    {
                                    case 2:
                                          IntegerStr.insert(i, TemplateTypeOfStr("b ", _Elem));
                                          break;
                                    case 8:
                                          IntegerStr.insert(i, TemplateTypeOfStr(" 0", _Elem));
                                          break;
                                    case 16:
                                          IntegerStr.insert(i, TemplateTypeOfStr(" 0x", _Elem));
                                          break;
                                    default:
                                          IntegerStr.insert(i, TemplateTypeOfStr(" ", _Elem));
                                    }
                              }
                              else
                                    IntegerStr.insert(i, TemplateTypeOfStr(" ", _Elem));
                        }
                  }

                  if (FlagShowbase)
                        switch (radix)
                        {
                        case 2:
                              IntegerStr = IntegerStr + TemplateTypeOfStr("b", _Elem);
                              break;
                        case 8:
                              IntegerStr = TemplateTypeOfStr("0", _Elem) + IntegerStr;
                              break;
                        case 16:
                              IntegerStr = TemplateTypeOfStr("0x", _Elem) + IntegerStr;
                        }

                  return iostr << IntegerStr;
            }
      };

      extern template std::basic_ostream<char>& ConversionRadix::C_Radix(std::basic_ostream<char>& iostr) const;
      extern template std::basic_ostream<wchar_t>& ConversionRadix::C_Radix(std::basic_ostream<wchar_t>& iostr) const;
      extern template std::basic_ostream<char16_t>& ConversionRadix::C_Radix(std::basic_ostream<char16_t>& iostr) const;

}


#endif // TIO_MANIP
