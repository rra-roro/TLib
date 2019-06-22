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

#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
      #define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#endif

#include <Tstring.h>
#include <Tiostream.h>
#include <Tlocale.h>
#include <iomanip>

#include <iomanip/Tiomanip_standart.h>

#ifdef _WIN32
      #include <win/iomanip_win.h>
#endif

#include <iomanip/Tiomanip_extentions.h>


namespace tlib
{

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
            friend inline std::basic_ostream<_Elem>& operator<<(std::basic_ostream<_Elem>& os, [[maybe_unused]]  const Color& c)
            {
                  //os << TemplateTypeOfStr("\033[", _Elem) << (c.ColorFone + 10) << TemplateTypeOfStr(";", _Elem) << c.ColorText << TemplateTypeOfStr("m", _Elem);
                  //os.clear();
                  return os;
            }
      };



}


#endif // TIO_MANIP
