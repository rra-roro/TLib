/////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Файл Tlocale.h     - содержит макроссы и ф-ии необходимые для поддержки национальных символов
//                         а также ф-ии для обработки строк, поддерживающие национальные символы
//
//    Версия файла 1.00  - первая версия.
//
//

#ifndef TLOCALE
#define TLOCALE

#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <linux/linux.h>
#endif

#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#endif

#include <sstream>
#include <functional>
#include <algorithm>
#include <locale>

#ifdef _WIN32
#include <win/locale_char16_t.h>
extern template std::locale::id std::collate<char16_t>::id;
#elif __linux__
#include <linux/locale_char16_t.h>
#endif

#include <Tstring.h>

namespace tlib
{
      /*---
	*   CodePageCharInSrc - задает имя кодировки строковых литералов и типа char
	*                       в скомпилированной программе.
	*   Это не то же самое, что имя кодировка исходного текста програмы, которая по
	*   соглашению всегда UTF-8 BOM.
	*   Кодировка строковых литералов и типа char задается компилятором в момент
	*   компиляции программы и устанавливается такой же, как кодировка системы в
	*   момент компиляции.
	*/
#ifdef _WIN32
      constexpr const char* CodePageCharInSrc = "Russian_Russia.1251";
#elif __linux__
      constexpr const char* CodePageCharInSrc = "ru_RU.utf8";
#else
#error "Unknown compiler"
#endif

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


      ///////////////////////////////////////////////////////////////////////////////////////
      //
      //   Внесем в наше пространство имен основной класс представляющий локаль
      //
      using std::locale;

      ///////////////////////////////////////////////////////////////////////////////////////
      //
      //   Ф-ия AddFacet<>(std::locale& loc) - добавляет к переданной локали фасет, заданный в
      //                                       параметре шаблона
      //  Примечание: с помощью этой ф-ии нельзя подключить
      //	            std::time_get<> и std::time_put<> фасеты
      //

      template <class _Facet>
      inline void AddFacet(std::locale& loc)
      {
#ifdef _WIN32
            // Добавим в переданную локаль Фасет заданный в параметре шаблона
            std::locale loc_tmp(loc, new _Facet(std::_Locinfo(loc.name().c_str())));
            // Исправим у полученной локали имя, и вернем ее обратно
            loc = locale(loc_tmp, loc.name().c_str(), std::locale::time);

#elif __linux__
            if constexpr (std::is_same<_Facet, std::collate<char16_t>>::value)
            {
                  // Добавим в переданную локаль Фасет заданный в параметре шаблона
                  loc = std::locale(loc, new _Facet());
            }
            else
            {
                  // Добавим в переданную локаль Фасет заданный в параметре шаблона
                  loc = std::locale(loc, new _Facet(loc));
            }
#endif
      }

      ///////////////////////////////////////////////////////////////////////////////////////
      //
      //  Ф-ия CreateLocaleByName() - создает локаль по ее имени.
      //
      //  Имена локалей имеют формат: "C" или "language[_area[.codepage]]" или ".codepage"
      //  Например: "Russian_Russia.866", "", "C", ".1251"
      //
      //  Примечание: в локаль создаваемую этой ф-ией будут добавлены дополнительные фасеты
      //          	ctype_wchar_t и ctype<char16_t>
      //  Примечание: следует иметь в виду, что код MS CRT до сих пор не поддерживает utf-8,
      //              в нем есть прямой запрет на установку таких локалей как CP_UTF8 == 65001
      //              установка такой кодовой страницы вызовет исключение.
      //              Судя по комментариям скоро поддержку utf-8 они все же добавят.
      //
      //     name_new_locale      - имя локали которую мы хотим создать
      //     name_number_locale   - имя локали из которой мы хотим добавить способ отображение цифр
      //                            в создаваемую локаль
      //
      //     return  - возвращаем созданную локаль
      //

      inline std::locale CreateLocaleByName(std::string_view name_new_locale, std::string_view name_number_locale)
      {
            int LocaleType = std::locale::collate | std::locale::ctype |
                             std::locale::monetary | std::locale::time | std::locale::messages;
            std::locale NewLocale(std::locale(name_number_locale.data()), name_new_locale.data(), LocaleType);

            AddFacet<std::ctype<char16_t>>(NewLocale);
            AddFacet<std::collate<char16_t>>(NewLocale);

            return NewLocale;
      }

      inline std::locale CreateLocaleByName(std::string_view name_new_locale)
      {
            return CreateLocaleByName(name_new_locale, "C");
      }
#ifdef _WIN32
      ///////////////////////////////////////////////////////////////////////////////////////
      //
      //  Ф-ия GetLocaleGUI() - возвращает текущую локаль установленную в OS для GUI.
      //                        Т.е.возвращает локаль для CP_ACP
      //

      inline std::locale GetLocaleGUI()
      {
            std::stringstream stream;
            stream << "Russian_Russia." << std::dec << GetACP();

            return CreateLocaleByName(stream.str(), "C");
      }

      ///////////////////////////////////////////////////////////////////////////////////////
      //
      //  Ф-ия GetCodePageGUI() - возвращает текущую кодовую страницу
      //                          установленную в OS для GUI.
      //

      inline std::string GetLocaleNameGUI()
      {
            return GetLocaleGUI().name();
      }
#endif
      ///////////////////////////////////////////////////////////////////////////////////////
      //
      //  Ф-ия GetLocaleProgram() - возвращает текущую локаль соответствующую кодировке
      //                            строковых литералов типа char, в нашей откомпилированной
      //                            программе
      //

      inline std::locale GetLocaleProgram()
      {
            return CreateLocaleByName(CodePageCharInSrc);
      }

      ///////////////////////////////////////////////////////////////////////////////////////
      //
      //  Ф-ия GetCodePageProgram() - возвращает текущую кодовую страницу, соответствующую кодовой
      //                              странице кодировки строковых литералов типа char, в нашей
      //                              откомпилированной  программе
      //

      inline std::string GetLocaleNameProgram()
      {
            return CodePageCharInSrc;
      }

      ///////////////////////////////////////////////////////////////////////////////////////
      //
      //  Ф-ия GetLocaleConsole() - возвращает текущую локаль установленную в OS для консоли
      //                            Т.е.возвращает локаль для CP_OEMCP
      //

      inline std::locale GetLocaleConsole()
      {
            std::stringstream stream;
#ifdef _WIN32
            UINT concp = GetConsoleOutputCP();
            if (concp == 0)
                  concp = 866;
            stream << "Russian_Russia." << std::dec << concp;
#elif __linux__
            //stream << GetConsoleOutputCP();
            stream << CodePageUTF8;
#endif
            return CreateLocaleByName(stream.str(), "C");
      }

      ///////////////////////////////////////////////////////////////////////////////////////
      //
      //  Ф-ия GetCodePageConsole() - возвращает текущую кодовую страницу
      //                              установленную в OS для консоли.
      //

      inline std::string GetLocaleNameConsole()
      {
            return GetLocaleConsole().name();
      }

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

#ifdef _WIN32
      //////////////////////////////////////////////////////////////////////////////////////////////////////////
      //  Иногда, когда мы внедряемся в чужую программу, в чужой программе еще не была инициализирована
      //  CRT подсистема, и как следствие в процесс не загруженны локали. А мы хотим их уже использовать
      //  Воспользуемся следующей ф-ией, которая принудительно подгружает локали.

      int InitIO(void);
#endif

      //////////////////////////////////////////////////////////////////////////////////
      // Функции проверки из STL принадлежности символов группе
      // Ф-ии одинаково хорошо работают и c Ansi и с Unicode

      // isalnum () возвращает, true, если его параметр - или буква алфавита или цифра.
      //            Если символ не является алфавитно-цифровым, возвращается false.
      // template<class _Elem> inline bool isalnum(_Elem _Ch, const locale& _Loc);
      using std::isalnum;

      // isalpha () возвращает, true, если ch - буква алфавита; иначе возвращается false.
      //            То, что составляет букву алфавита, может измениться в разных языках.
      //            Для английского языка это диапазон верхних и нижних символов от A до Z.
      // template<class _Elem> inline bool isalpha(_Elem _Ch, const locale& _Loc)
      using std::isalpha;

      // iscntrl () возвращает, true, если ch находится в диапазон между 0 и 0x1F или равен 0x7F (DEL);
      //            иначе возвращается false.
      // template<class _Elem> inline bool iscntrl(_Elem _Ch, const locale& _Loc)
      using std::iscntrl;

      // isdigit () возвращает, true, если ch - цифра, то есть, 0 до 9. Иначе возвращается false.
      // template<class _Elem> inline bool isdigit(_Elem _Ch, const locale& _Loc)
      using std::isdigit;

      // isgraph () возвращает, true, если ch - какой-нибудь печатаемый символ исключая пробел;
      //            иначе возвращается false. Печатаемые символы для английского языка обычно находятся в
      //            диапазоне от 0x21 до 0x7E.
      // template<class _Elem> inline bool isgraph(_Elem _Ch, const locale& _Loc)
      using std::isgraph;

      // islower () возвращает, true, если ch - строчная буква; иначе возвращается false.
      // template<class _Elem> inline bool islower(_Elem _Ch, const locale& _Loc)
      using std::islower;

      // isprint () возвращает, true, если ch - печатаемый символ, включая пробел(пространство);
      //            иначе возвращается false. Печатаемые символы часто находятся в диапазоне от 0x20 до 0x7E.
      // template<class _Elem> inline bool isprint(_Elem _Ch, const locale& _Loc)
      using std::isprint;

      // ispunct () возвращает, true, если ch - символ пунктуации; иначе возвращается false.
      //            Термин "пунктуация", как определено этой функцией, включает все литеры,
      //            которые не являются ни алфавитно-цифровыми символами, ни пробелом.
      // template<class _Elem> inline bool ispunct(_Elem _Ch, const locale& _Loc)
      using std::ispunct;

      // isspace () возвращает, true, если ch - или пробел, или горизонтальная табуляция, или
      //            вертикальная табуляция, прогон страницы, символ возврата каретки, или символ новой строки;
      //            иначе возвращается false.
      // template<class _Elem> inline bool isspace(_Elem _Ch, const locale& _Loc)
      using std::isspace;

      // isupper () возвращает, true, если ch - символ верхнего регистра; иначе возвращается false.
      // template<class _Elem> inline bool isupper(_Elem _Ch, const locale& _Loc)
      using std::isupper;

      // isxdigit () возвращает, true, если ch - шестнадцатеричная цифра; иначе возвращается false.
      //             Шестнадцатеричная цифра будет в одном из этих диапазонов: A-F, a-f, или 0-9.
      // template<class _Elem> inline bool isxdigit(_Elem _Ch, const locale& _Loc)
      using std::isxdigit;

      //////////////////////////////////////////////////////////////////////////////////
      // Функции проверки является ли символ слэшем.
      //		Аргумент имеет тип wchar_t.
      //		Если передать char, то ф-ия отработает, как следует. Так как символ слеша
      //		успешно конвертируется из типа  char в  wchar_t

      inline bool isslashr(wchar_t _Ch)
      { // проверяет является ли символ правым слешем "/"
            return (_Ch == L'/') ? true : false;
      }

      inline bool isslashl(wchar_t _Ch)
      { // проверяет является ли символ левым слешем "\"
            return (_Ch == L'\\') ? true : false;
      }

      inline bool isslash(wchar_t _Ch)
      { // проверяет является ли символ слешем
            return (isslashl(_Ch) || isslashr(_Ch)) ? true : false;
      }


      //////////////////////////////////////////////////////////////////////////////////
      // Функции из STL смены регистра символа

      // convert character to lower case, locale specific
      // template<class _Elem> inline _Elem tolower(_Elem _Ch, const locale& _Loc)
      using std::tolower;

      // convert character to upper case, locale specific
      // template<class _Elem> inline _Elem toupper(_Elem _Ch, const locale& _Loc);
      using std::toupper;

      //////////////////////////////////////////////////////////////////////////////////
      //    Функция для конвертации симолов строки  в нижний регистр
      //
      //    string GetLowerStr(const string& str, const locale& Locale = GetLocaleProgram());
      //
      //    void toLowerStr(string& str, const locale& Locale = GetLocaleProgram());
      //

      template <class _Elem>
      inline std::basic_string<_Elem> GetLowerStr(std::basic_string_view<_Elem> Str,
          const std::locale& Locale = GetLocaleProgram())
      {
            std::basic_string<_Elem> StrLowerCase(Str.size(), 0);

            std::transform(Str.begin(), Str.end(), StrLowerCase.begin(),
                [&](_Elem _Ch1) {
                      return tolower<_Elem>(_Ch1, Locale);
                });

            return StrLowerCase;
      }

      template <class T, class _Elem = typename Type_Str<T>::type>
      inline std::basic_string<_Elem> GetLowerStr(T Str,
          const std::locale& Locale = GetLocaleProgram())
      {
            return GetLowerStr(std::basic_string_view<_Elem>(Str), Locale);
      }

      template <typename _Elem>
      inline void toLowerStr(std::basic_string<_Elem>& Str,
          const std::locale& Locale = GetLocaleProgram())
      {
            std::transform(Str.begin(), Str.end(), Str.begin(),
                [&](_Elem _Ch1) {
                      return tolower<_Elem>(_Ch1, Locale);
                });
      }

      template <typename _Elem>
      inline void toLowerStr(_Elem* Str,
          size_t SizeStr,
          const std::locale& Locale = GetLocaleProgram())
      {
            std::transform(&Str[0], &Str[SizeStr - 1], &Str[0],
                [&](_Elem _Ch1) {
                      return tolower<_Elem>(_Ch1, Locale);
                });
      }

      //////////////////////////////////////////////////////////////////////////////////
      //    Функция для конвертации симолов строки в верхний регистр
      //
      //    string GetUpperStr(const string& str, const locale& Locale = GetLocaleProgram());
      //
      //    void toUpperStr(string& str, const locale& Locale = GetLocaleProgram());
      //

      template <typename _Elem>
      inline std::basic_string<_Elem> GetUpperStr(std::basic_string_view<_Elem> Str,
          const std::locale& Locale = GetLocaleProgram())
      {
            std::basic_string<_Elem> StrUpperCase(Str.size(), 0);

            std::transform(Str.begin(), Str.end(), StrUpperCase.begin(),
                [&](_Elem _Ch1) {
                      return toupper<_Elem>(_Ch1, Locale);
                });

            return StrUpperCase;
      }

      template <class T, class _Elem = typename Type_Str<T>::type>
      inline std::basic_string<_Elem> GetUpperStr(T Str,
          const std::locale& Locale = GetLocaleProgram())
      {
            return GetUpperStr(std::basic_string_view<_Elem>(Str), Locale);
      }

      template <typename _Elem>
      inline void toUpperStr(std::basic_string<_Elem>& Str,
          const std::locale& Locale = GetLocaleProgram())
      {
            std::transform(Str.begin(), Str.end(), Str.begin(),
                [&](_Elem _Ch1) {
                      return toupper<_Elem>(_Ch1, Locale);
                });
      }

      template <typename _Elem>
      inline void toUpperStr(_Elem* Str,
          size_t SizeStr,
          const std::locale& Locale = GetLocaleProgram())
      {
            std::transform(&Str[0], &Str[SizeStr - 1], &Str[0],
                [&](_Elem _Ch1) {
                      return toupper<_Elem>(_Ch1, Locale);
                });
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    StrCmpI    - Перегруженная шаблонная ф-ия которя позволяет сравнить две строки без
      //                 учета регистра символов. И в случае равенства возвращает true
      //
      //    bool StrCmpI( Str1, Str2,  const std::locale& Locale = GetLocaleProgram());
      //

      template <typename _Elem>
      inline bool StrCmpI(std::basic_string_view<_Elem> Str1,
          std::basic_string_view<_Elem> Str2,
          const std::locale& Locale = GetLocaleProgram())
      {
            // версия для двух STL строк
            if (Str1.size() != Str2.size())
                  return false;

            return std::equal(Str1.begin(), Str1.end(), Str2.begin(),
                [&](_Elem _Ch1, _Elem _Ch2) {
                      // сравниваваем два символа без учета их регистра
                      return toupper<_Elem>(_Ch1, Locale) == toupper<_Elem>(_Ch2, Locale);
                });
      }

      template <class T1, class _Elem1 = typename Type_Str<T1>::type,
          class T2, class _Elem2 = typename Type_Str<T2>::type>
      inline bool StrCmpI(T1 Str1, T2 Str2,
          const std::locale& Locale = GetLocaleProgram())
      {
            return StrCmpI(std::basic_string_view<_Elem1>(Str1),
                std::basic_string_view<_Elem2>(Str2), Locale);
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    FindSubStrI  - Перегруженная шаблонная ф-ия которя позволяет искать в строке подстроку
      //                   без учета регистра символов. И в случае нахождения возвращает позицию
      //                   первого с начала строки вхождения подстроки в строку.
      //
      //    size_t FindSubStrI( Str, searchStr,  const std::locale& Locale = GetLocaleProgram());
      //

      template <typename _Elem>
      inline size_t FindSubStrI(std::basic_string_view<_Elem> Str,
          std::basic_string_view<_Elem> searchStr,
          const std::locale& Locale = GetLocaleProgram())
      {

            auto iter = std::search(Str.begin(), Str.end(), searchStr.begin(), searchStr.end(),
                [&](_Elem _Ch1, _Elem _Ch2) {
                      // сравниваваем два символа без учета их регистра
                      return toupper<_Elem>(_Ch1, Locale) == toupper<_Elem>(_Ch2, Locale);
                });

            if (iter == Str.end())
                  return npos;
            else
                  return iter - Str.begin();
      }

      template <class T1, class _Elem1 = typename Type_Str<T1>::type,
          class T2, class _Elem2 = typename Type_Str<T2>::type>
      inline size_t FindSubStrI(T1 Str, T2 searchSymbolStr,
          const std::locale& Locale = GetLocaleProgram())
      {
            return FindSubStrI(std::basic_string_view<_Elem1>(Str),
                std::basic_string_view<_Elem2>(searchSymbolStr), Locale);
      }

      template <typename _Elem>
      inline size_t FindSubStrI(std::basic_string_view<_Elem> Str,
          size_t StartPosition,
          std::basic_string_view<_Elem> searchStr,
          const std::locale& Locale = GetLocaleProgram())
      {

            if ((Str.begin() + StartPosition) > Str.end())
                  return npos;

            auto iter = std::search(Str.begin() + StartPosition, Str.end(), searchStr.begin(), searchStr.end(),
                [&](_Elem _Ch1, _Elem _Ch2) {
                      // сравниваваем два символа без учета их регистра
                      return toupper<_Elem>(_Ch1, Locale) == toupper<_Elem>(_Ch2, Locale);
                });

            if (iter == Str.end())
                  return npos;
            else
                  return iter - Str.begin();
      }

      template <class T1, class _Elem1 = typename Type_Str<T1>::type,
          class T2, class _Elem2 = typename Type_Str<T2>::type>
      inline size_t FindSubStrI(T1 Str, size_t StartPosition, T2 searchSymbolStr,
          const std::locale& Locale = GetLocaleProgram())
      {
            return FindSubStrI(std::basic_string_view<_Elem1>(Str),
                StartPosition,
                std::basic_string_view<_Elem2>(searchSymbolStr), Locale);
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    FindPrintSymbol  - Перегруженная шаблонная ф-ия которя позволяет искать позицую первого
      //                       печатного символа с начала строки. Тоесть любого не пробельного.
      //                       Не пробельный символ определяется при помощи ф-ии std::isgraph.
      //          size_t pos - параметр определяющий позицию в строке с которой будет осуществляться
      //                       поиск.
      //
      //    size_t FindPrintSymbol( Str,  const std::locale& Locale = GetLocaleProgram())
      //    size_t FindPrintSymbol( Str,  size_t pos, const std::locale& Locale = GetLocaleProgram())
      //

      template <typename _Elem>
      inline size_t FindPrintSymbol(std::basic_string_view<_Elem> Str,
          size_t pos,
          const std::locale& Locale = GetLocaleProgram())
      {
            auto start_iter = Str.begin() + pos;
            auto iter = std::find_if(start_iter, Str.end(), [&](_Elem _Ch1) {
                  // Это печатный символ?
                  return isgraph<_Elem>(_Ch1, Locale);
            });
            if (iter == Str.end())
                  return npos;
            else
                  return iter - Str.begin();
      }

      template <class T, class _Elem = typename Type_Str<T>::type>
      inline size_t FindPrintSymbol(T Str, size_t pos,
          const std::locale& Locale = GetLocaleProgram())
      {
            return FindPrintSymbol(std::basic_string_view<_Elem>(Str), pos, Locale);
      }

      template <class T, class _Elem = typename Type_Str<T>::type>
      inline size_t FindPrintSymbol(T Str,
          const std::locale& Locale = GetLocaleProgram())
      {
            return FindPrintSymbol(std::basic_string_view<_Elem>(Str), 0, Locale);
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    RFindPrintSymbol - Перегруженная шаблонная ф-ия которя позволяет искать позицую первого
      //                       печатного символа с конца строки. Тоесть любого не пробельного.
      //                       Не пробельный символ определяется при помощи ф-ии std::isgraph.
      //          size_t pos - параметр определяющий позицию в строке с которой будет осуществляться
      //                       поиск.
      //
      //    size_t RFindPrintSymbol( Str,  const std::locale& Locale = GetLocaleProgram())
      //    size_t RFindPrintSymbol( Str,  size_t pos, const std::locale& Locale = GetLocaleProgram())
      //

      template <typename _Elem>
      inline size_t RFindPrintSymbol(std::basic_string_view<_Elem> Str,
          size_t pos,
          const std::locale& Locale = GetLocaleProgram())
      {
            auto start = Str.rbegin();
            advance(start, Str.size() - pos); // смещаем итератор start в позицию pos, где pos задается от начала строки

            auto iter = std::find_if(start, Str.rend(), [&](_Elem _Ch1) {
                  // Это печатный символ?
                  return isgraph<_Elem>(_Ch1, Locale);
            });

            if (iter == Str.rend())
                  return npos;
            else
                  return Str.size() - distance(Str.rbegin(), iter) - 1;
      }

      template <class T, class _Elem = typename Type_Str<T>::type>
      inline size_t RFindPrintSymbol(T Str,
          const std::locale& Locale = GetLocaleProgram())
      {
            std::basic_string_view<_Elem> StrInternal(Str);
            return RFindPrintSymbol(StrInternal, StrInternal.size(), Locale);
      }

      template <class T, class _Elem = typename Type_Str<T>::type>
      inline size_t RFindPrintSymbol(T Str, size_t pos,
          const std::locale& Locale = GetLocaleProgram())
      {
            return RFindPrintSymbol(std::basic_string_view<_Elem>(Str), pos, Locale);
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    RemovalFirstSpaces - шаблонная ф-ия которя удаляет лидирующие пробелы из строки.
      //
      //    void RemovalFirstSpaces( Str, const std::locale& Locale = GetLocaleProgram())
      //
      template <typename _Elem>
      inline std::basic_string<_Elem> RemovalFirstSpaces(std::basic_string_view<_Elem> Str,
          const std::locale& Locale = GetLocaleProgram())
      {
            if (!Str.empty())
            {
                  std::basic_string_view<_Elem> new_view_str = Str.substr(FindPrintSymbol(Str, 0, Locale));
                  return { new_view_str.data(), new_view_str.size() };
            }
            return { Str.data(), Str.size() };
      }

      template <class T, class _Elem = typename Type_Str<T>::type>
      inline std::basic_string<_Elem> RemovalFirstSpaces(T Str,
          const std::locale& Locale = GetLocaleProgram())
      {
            return RemovalFirstSpaces(std::basic_string_view<_Elem>(Str), Locale);
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    RemovalTrailingSpaces - шаблонная ф-ия которя удаляет пробелы из конца строки.
      //
      //    void RemovalTrailingSpaces( Str, const std::locale& Locale = GetLocaleProgram())
      //

      template <typename _Elem>
      inline std::basic_string<_Elem> RemovalTrailingSpaces(std::basic_string_view<_Elem> Str,
          const std::locale& Locale = GetLocaleProgram())
      {
            if (!Str.empty())
            {
                  std::basic_string_view<_Elem> new_view_str = Str.substr(0, RFindPrintSymbol(Str, Str.size(), Locale) + 1);
                  return { new_view_str.data(), new_view_str.size() };
            }
            return { Str.data(), Str.size() };
      }

      template <class T, class _Elem = typename Type_Str<T>::type>
      inline std::basic_string<_Elem> RemovalTrailingSpaces(T Str,
          const std::locale& Locale = GetLocaleProgram())
      {
            return RemovalTrailingSpaces(std::basic_string_view<_Elem>(Str), Locale);
      }

      //////////////////////////////////////////////////////////////////////////////////
      //
      //    RemovalSpace - шаблонная ф-ия которя удаляет пробелы из конца и начала строки.
      //
      //    void RemovalSpace( Str, const std::locale& Locale = GetLocaleProgram())
      //

      template <typename _Elem>
      inline std::basic_string<_Elem> RemovalSpace(std::basic_string_view<_Elem> Str,
          const std::locale& Locale = GetLocaleProgram())
      {
            return RemovalTrailingSpaces(RemovalFirstSpaces(Str, Locale), Locale);
      }

      template <class T, class _Elem = typename Type_Str<T>::type>
      inline std::basic_string<_Elem> RemovalSpace(T Str,
          const std::locale& Locale = GetLocaleProgram())
      {
            return RemovalSpace(std::basic_string_view<_Elem>(Str), Locale);
      }

}

#endif
