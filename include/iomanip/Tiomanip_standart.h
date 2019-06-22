#pragma once

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

      using std::fixed;        // Выводим число так:   10 000,123400
      using std::scientific;   // или так:             1,000012e+04
      using std::hexfloat;     // или так:           0x1,3880fdp + 13
      using std::defaultfloat; // или так:             10 000,1      

      using std::left;         /* Left fill:
                                   -1.23*******
                                   0x2a********
                                   USD *1.23***
                               */
      using std::internal;    /* Internal fill:
                                 -*******1.23
                                 0x********2a
                                 USD ****1.23
                              */
      using std::right;      /* Right fill:
                                *******-1.23
                                ********0x2a
                                ***USD *1.23
                             */

      using std::setw; // Задаем размер поля, этот флаг сбрасывается в 0 после каждого вывода
                       // переменной, текста или цифры, например для каждого put_intger_by_radix
                       // нужно повторно выставить этот флаг:
                       // tcout << setw(2) << put_intger_by_radix(0xf,16) << setw(2) << put_intger_by_radix(0xf,16) << endl;

      using std::skipws;    // Включает или отключает пропуск начальных пробелов при вводе значений в поток
      using std::noskipws;  //          (по умолчанию включено). Не влияет на вывод.

      using std::unitbuf;    // Включает или отключает автоматическую очистку выходного потока
      using std::nounitbuf;  // после любой операции вывода. Не влияет на ввод.

      //========================================================================================
      //Внесем в область видимости стандартные манипуляторы из <istream>
      
      using std::ws;    // пропускает лидирующие пробелы при вводе. Не влияет на вывод.

      //========================================================================================
      //Внесем в область видимости стандартные манипуляторы из <ostream>

      using std::endl;  //Добавляет в поток символ перевода строки '\n' и флушит поток.
      using std::ends;  //Добавляет в поток нулевой символ строки '\0'
      using std::flush; //Флушит поток, т.е. если в буфере потока остались символы довыводит их, и очищает буфер

      //========================================================================================
      //Внесем в область видимости стандартные манипуляторы из <iomanip>
      
      using std::resetiosflags;
      using std::setiosflags;
      using std::setbase;
      using std::setfill;
      using std::setprecision;
      using std::setw;
      using std::get_money;
      using std::put_money;
      using std::get_time;
      using std::put_time;
      using std::quoted;
  
}
