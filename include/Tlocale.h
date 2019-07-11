/////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Файл Tlocale.h     - содержит макроссы и ф-ии необходимые для поддержки национальных символов
//                         а также ф-ии для обработки строк, поддерживающие национальные символы
//
//    Версия файла 1.00  - первая версия.
//
//
#pragma once

#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <linux/linux.h>
#endif

#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#endif

#include <locale>
#include <vector>
#include <string>

#ifdef _WIN32
#include <win/locale_facets_win.h>
extern template std::locale::id std::collate<char16_t>::id;
extern template std::locale::id std::numpunct<char16_t>::id;
extern template std::locale::id std::moneypunct<char16_t, false>::id;
extern template std::locale::id std::moneypunct<char16_t, true>::id;
extern template std::locale::id std::money_get<char16_t>::id;
extern template std::locale::id std::money_put<char16_t>::id;
#elif __linux__
#include <linux/locale_facets_linux.h>
#endif

#include <Tlocale_config.h>
#include <locale/Tlocale_facets.h>
#include <locale/Tlocale_class.h>

#ifdef _WIN32
#include <win/locale_mbrlen_win.h>
#elif __linux__
#include <linux/locale_mbrlen_linux.h>
#endif

#include <locale/Tlocale_alg.h>



namespace tlib
{
      /*---
	*   CodePageUTF8 - задает имя кодировки UTF-8 строковых литералов и типа char.
      *
	*   Имена для кодировки UTF-8 отличаются в Windows и Linux
	*/
#ifdef _WIN32
      constexpr const char* CodePageUTF8 = "Russian_Russia.65001";
#elif __linux__
      constexpr const char* CodePageUTF8 = "ru_RU.utf8";
#else
#error "Unknown compiler"
#endif

      //////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Для работы со стандартными потоками IO нужно установить локаль с кодовой страницей, как у консоли.
      // По умолчанию используется локаль "C"  - русские буквы вообще не выводятся
      // locale loc("");                        создать локаль со страницей по умолчанию
      // locale loc("Russian_Russia.1251");     создать локаль со страницей ANSI 1251
      // locale loc("Russian_Russia.866");      создать локаль со страницей ОЕМ 866
      // По умолчанию в консоли Windows используется страница ОЕМ 866
      // Даже после установки локали с кодовой страницей ОЕМ 866
      // для cout и cin, страница при выводе будет 1251 {cout и cin не учитывают кодовую страницу при выводе}
      // для wcout и wcin, все работает нормально страница при выводе будет ОЕМ 866
      // В следствии этого помимо смены локали на 866, нужно перегрузить оператор operator<<  и operator>>
      // Далее мы поменяем локаль на ОЕМ 866, но тут возникает еще одна проблема,
      // руская локаль "Russian_Russia.866", не корректно выводит символы в шестнадцатиричном формате.
      // Для решения этого комплекса проблем составим такую локаль, которая бралабы отображение цифр
      // из локали "C", а все остальные настройки из локали "Russian_Russia.866".
      // Примечание: Числа в 16-ом формате выводятся в локале "Russian_Russia.866",
      //             с группировкой по три цифры. 0xF0F0F0F0 ->  F0 F0F 0F0
      // Примечание: Строка с именем локали имеет следующий формат:
      //                             язык_страна.codepage
      //             все три составляющии (язык,страна,codepage) должны
      //             быть корректными, но они  не зависят друг от друга
      //             т.е. можно задать такое:
      //                                     "Russian_United Kingdom.10007"
      //             за кодировку букв отвечает только codepage.
      // Примечание: следует иметь в виду, что код MS CRT до сих пор не поддерживает utf-8,
      //             в нем есть прямой запрет на установку таких локалей как CP_UTF8 == 65001
      //             установка такой кодовой страницы вызовет исключение.
      //             Судя по комментариям скоро поддержку utf-8 они все же добавят.

      int InitConsolIO(void);

      //////////////////////////////////////////////////////////////////////////////////////////////////////////
      //
      //

      std::vector<std::string> get_available_locale_names();



#ifdef _WIN32
      //////////////////////////////////////////////////////////////////////////////////////////////////////////
      //  Иногда, когда мы внедряемся в чужую программу, в чужой программе еще не была инициализирована
      //  CRT подсистема, и как следствие в процесс не загруженны локали. А мы хотим их уже использовать
      //  Воспользуемся следующей ф-ией, которая принудительно подгружает локали.

      int InitIO(void);
#endif

}

