#include "gtest/gtest.h"
#include "gtest/gtest_prod.h"

#include "Tlib_version.h"
#include <string_view>
#include <algorithm>
//#include <unistd.h>
//#include <io.h>
#include <fcntl.h>

#define PRIVATE_TEST 1

#include <Tlocale.h>
#include <Tiostream.h>

TEST(version, test1)
{
      ASSERT_TRUE(version() > 0);
}

namespace tlib
{
      //class PublisherMixinTest : public testing::Test
      //{
      //  public:
      //      void SetUp() {}
      //      void TearDown() {}
      //};

      //TEST_F(PublisherMixinTest, UniqueAddSubscribers1)
      //{
      //      //pbl.subscribe(subscriber_functor()); //  rvalue - добавляем повторно
      //      //ASSERT_TRUE(pbl.subscribers.size() == 7);
      //}


      TEST(Tlocale, Create_locale_by_any_name)
      {
            // Проверяем всевозможные форматы задание имени локали
            // Формат имени локали не должен зависеть от платформы

            try
            {
                  auto locnames = tlib::get_available_locale_names();
                  if (locnames.empty())
                        std::cout << "locales not found\n";
                  else
                        std::copy(locnames.begin(), locnames.end(), std::ostream_iterator<std::string>(std::cout, "\n"));

                  //auto end_ru_locale = std::partition(locnames.begin(), locnames.end(), [](auto& lcn) { return lcn.find("en_") != npos; });
                  

                  

                  tlib::locale("C");
                  tlib::locale("POSIX");
                  //tlib::locale("ru_RU");
                  //std::cout << "3\n";
                  //tlib::locale("ru-RU");
                  //std::cout << "4\n";
                  //tlib::locale("Russian_Russia");
                  //std::cout << "5\n";
                  //tlib::locale("Russian-Russia");

                  tlib::locale(".utf-8");
                  std::cout << "6\n";
                  tlib::locale("ru_RU.Utf-8");
                  std::cout << "7\n";
                  tlib::locale("ru_RU.uTf-8");
                  std::cout << "8\n";
                  tlib::locale("ru_RU.uTf8");
                  std::cout << "9\n";
                  //tlib::locale("ru_RU.cp1251");
                  //std::cout << "10\n";
                  //tlib::locale("ru_RU.koi8-r");
                  //std::cout << "10.a\n";

                  tlib::locale("ru_RU.utf-8");
                  std::cout << "11\n";
                  tlib::locale("ru-RU.utf-8");
                  std::cout << "12\n";
                  tlib::locale("Russian_Russia.utf-8");
                  std::cout << "13\n";
                  tlib::locale("Russian-Russia.utf-8");
                  std::cout << "13.a\n";
            }
            catch (...)
            {
                  ASSERT_TRUE(false);
            }
            ASSERT_TRUE(true);
      }

      TEST(Tlocale, facets)
      {
#ifdef _WIN32
            constexpr const char* CodePageCharInSrc = "Russian_Russia.1251";
#elif __linux__
            constexpr const char* CodePageCharInSrc = "ru_RU.utf8";
#else
#error "Unknown compiler"
#endif

            auto loc = tlib::locale(CodePageCharInSrc);

            ASSERT_TRUE(std::use_facet<std::ctype<wchar_t>>(loc).toupper(L'б') == L'Б');
            ASSERT_TRUE(std::use_facet<std::ctype<char16_t>>(loc).toupper(u'б') == u'Б');
            ASSERT_TRUE(std::use_facet<std::ctype<TCHAR>>(loc).toupper(_T('б')) == _T('Б'));

            ASSERT_TRUE(std::use_facet<std::ctype<wchar_t>>(loc).tolower(L'Б') == L'б');
            ASSERT_TRUE(std::use_facet<std::ctype<char16_t>>(loc).tolower(u'Б') == u'б');
            ASSERT_TRUE(std::use_facet<std::ctype<TCHAR>>(loc).tolower(_T('Б')) == _T('б'));

            ASSERT_TRUE(std::use_facet<std::numpunct<char>>(loc).truename() == "true");
            ASSERT_TRUE(std::use_facet<std::numpunct<wchar_t>>(loc).truename() == L"true");
            ASSERT_TRUE(std::use_facet<std::numpunct<char16_t>>(loc).truename() == u"true");
            ASSERT_TRUE(std::use_facet<std::numpunct<TCHAR>>(loc).truename() == _T("true"));


            ASSERT_TRUE(std::use_facet<std::moneypunct<char>>(loc).curr_symbol() == u8"\u20bd" ||   // Unicode символ рубля
                        std::use_facet<std::moneypunct<char>>(loc).curr_symbol() == "руб."     ||
                        std::use_facet<std::moneypunct<char>>(loc).curr_symbol() == "руб"      ||
                        std::use_facet<std::moneypunct<char>>(loc).curr_symbol() == "RUB");

            ASSERT_TRUE(std::use_facet<std::moneypunct<wchar_t>>(loc).curr_symbol() == L"\u20bd" ||
                        std::use_facet<std::moneypunct<wchar_t>>(loc).curr_symbol() == L"руб."   ||
                        std::use_facet<std::moneypunct<wchar_t>>(loc).curr_symbol() == L"руб"    ||
                        std::use_facet<std::moneypunct<wchar_t>>(loc).curr_symbol() == L"RUB");

            ASSERT_TRUE(std::use_facet<std::moneypunct<char16_t>>(loc).curr_symbol() == u"\u20bd" ||
                        std::use_facet<std::moneypunct<char16_t>>(loc).curr_symbol() == u"руб."   ||
                        std::use_facet<std::moneypunct<char16_t>>(loc).curr_symbol() == u"руб"    ||
                        std::use_facet<std::moneypunct<char16_t>>(loc).curr_symbol() == u"RUB");

            ASSERT_TRUE(std::use_facet<std::moneypunct<TCHAR>>(loc).curr_symbol() == _T("\u20bd") || 
                        std::use_facet<std::moneypunct<TCHAR>>(loc).curr_symbol() == _T("руб.")   ||
                        std::use_facet<std::moneypunct<TCHAR>>(loc).curr_symbol() == _T("руб")    ||
                        std::use_facet<std::moneypunct<TCHAR>>(loc).curr_symbol() == _T("RUB"));


            ASSERT_TRUE((std::use_facet<std::moneypunct<char, true>>(loc).curr_symbol() == "RUB"));
            ASSERT_TRUE((std::use_facet<std::moneypunct<wchar_t, true>>(loc).curr_symbol() == L"RUB"));
            ASSERT_TRUE((std::use_facet<std::moneypunct<char16_t, true>>(loc).curr_symbol() == u"RUB"));
            ASSERT_TRUE((std::use_facet<std::moneypunct<TCHAR, true>>(loc).curr_symbol() == _T("RUB")));

            auto test_numbers = [](auto& os) -> bool {
                  auto get_test_str = [&](auto _Elem) -> std::basic_string<decltype(_Elem)>
                  {                        
                        using char_type = decltype(_Elem);
                        using string_type = std::basic_string<char_type>;

                        char_type decimal_point = std::use_facet<std::moneypunct<char_type>>(os.getloc()).decimal_point();
                        char_type thousands_sep = std::use_facet<std::moneypunct<char_type>>(os.getloc()).thousands_sep();
                        string_type curr_symbol = std::use_facet<std::moneypunct<char_type>>(os.getloc()).curr_symbol();

#ifdef _WIN32
   #define __hexfloat__ "\nhexfloat double: 0x1,3880fdp+13"
#else
   #define __hexfloat__ "\nhexfloat double: 0x1,3880fcb923a2ap+13"
#endif
                        string_type result = TemplateTypeOfStr("\nbool : 1 0"
                                                               "\nboolalpha: true false"
                                                               "\ninteger: 10\240000"
                                                               "\ndouble: 10\240000,1"
                                                               "\n\nscientific double: 1,000012e+04"
                                                               "\nfixed double: 10\240000,123400"
                                                               __hexfloat__
                                                               "\ndefaultfloat double: 10\240000,1"
                                                               "\n\nсколько денег: 12\240345\240678,91 руб", char_type);
                                                              // "\n\nвремя: Четверг Чт 27 июн 2019 09:51:09", char_type);

                        std::replace(result.begin(), result.end(), TemplateTypeOfCh('\240', char_type), thousands_sep);

                        result.replace(result.find(TemplateTypeOfStr("руб", char_type)),
                                       string_type(TemplateTypeOfStr("руб", char_type)).size(),  curr_symbol);

                        result.replace(result.find(TemplateTypeOfStr("8,9", char_type)), 3, TemplateTypeOfStr("8", char_type) + string_type(1, decimal_point) + TemplateTypeOfStr("9", char_type));

                        return result;
                  };

                  using char_type = typename std::remove_reference<decltype(os)>::type::char_type;

#define _tt(str) TemplateTypeOfStr(str, char_type)

                  os << _tt("\n") << _tt("bool : ") << true << _tt(" ") << false;
                  os << _tt("\n") << _tt("boolalpha: ") << std::boolalpha << true << _tt(" ") << false;
                  os << _tt("\n") << _tt("integer: ") << 10000;
                  os << _tt("\n") << _tt("double: ") << 10000.1234;

                  os << _tt("\n\n") << _tt("scientific double: ") << std::scientific << 10000.1234;
                  os << _tt("\n") << _tt("fixed double: ") << std::fixed << 10000.1234;
                  os << _tt("\n") << _tt("hexfloat double: ") << std::hexfloat << 10000.1234;
                  os << _tt("\n") << _tt("defaultfloat double: ") << std::defaultfloat << 10000.1234;

                  os << _tt("\n\n") << _tt("сколько денег: ") << std::showbase << std::put_money(1234567891);

                  //std::time_t t = 0x000000005d14c9cd;
                  //os << _tt("\n\n") << _tt("время: ") << std::put_time<char_type>(std::localtime(&t), _tt("%A %c"));

                  return os.str() == get_test_str(TemplateTypeOfCh('\0', char_type));
#undef _tt
            };

            std::stringstream ss_utf8;
            ss_utf8.imbue(tlib::locale("ru_RU.utf8"));
            ASSERT_TRUE(test_numbers(ss_utf8));

            std::wstringstream ws_utf8;
            ws_utf8.imbue(tlib::locale("ru_RU.utf8"));
            ASSERT_TRUE(test_numbers(ws_utf8));

            tlib::u16stringstream us_utf8;
            us_utf8.imbue(tlib::locale("ru_RU.utf8"));
            ASSERT_TRUE(test_numbers(us_utf8));

            tlib::tstringstream ts_utf8;
            ts_utf8.imbue(tlib::locale("ru_RU.utf8"));
            ASSERT_TRUE(test_numbers(ts_utf8));
      }

      TEST(Tlocale_alg, IsSymbol)
      {
             auto test_symbols_true = [](auto _Elem, tlib::locale loc) -> bool
            {
                  using char_type = decltype(_Elem);

                  return isspace(TemplateTypeOfCh(' ', char_type), loc) &&
                         isspace(TemplateTypeOfCh('\t', char_type), loc) &&
                         isspace(TemplateTypeOfCh('\n', char_type), loc) &&
                         isspace(TemplateTypeOfCh('\r', char_type), loc) &&
                         iscntrl(TemplateTypeOfCh('\t', char_type), loc) &&
                         iscntrl(TemplateTypeOfCh('\n', char_type), loc) &&
                         iscntrl(TemplateTypeOfCh('\r', char_type), loc) &&
                         iscntrl(TemplateTypeOfCh('\b', char_type), loc) &&
                         iscntrl(TemplateTypeOfCh('\a', char_type), loc) &&
                         isslash(TemplateTypeOfCh('\\', char_type)) &&
                         isslash(TemplateTypeOfCh('/', char_type));
            };

            auto test_symbols_false = [](auto _Elem, tlib::locale loc) -> bool {
                  using char_type = decltype(_Elem);

                  return isspace(TemplateTypeOfCh('\b', char_type), loc) &&
                         isspace(TemplateTypeOfCh('\a', char_type), loc) &&
                         iscntrl(TemplateTypeOfCh(' ', char_type), loc);
            };

            ASSERT_TRUE(test_symbols_true('\0', tlib::locale::get_locale_program()));
            ASSERT_TRUE(test_symbols_true(L'\0', tlib::locale::get_locale_program()));
            ASSERT_TRUE(test_symbols_true(u'\0', tlib::locale::get_locale_program()));
            ASSERT_TRUE(test_symbols_true(_T('\0'), tlib::locale::get_locale_program()));

            ASSERT_FALSE(test_symbols_false('\0', tlib::locale::get_locale_program()));
            ASSERT_FALSE(test_symbols_false(L'\0', tlib::locale::get_locale_program()));
            ASSERT_FALSE(test_symbols_false(u'\0', tlib::locale::get_locale_program()));
            ASSERT_FALSE(test_symbols_false(_T('\0'), tlib::locale::get_locale_program()));
                

      }


      TEST(Tlocale_alg, StringTransforation)
      {
            // Test  <Tlocale.h>: сравнение строк и смена регистра

            ASSERT_TRUE(get_lower_str("Мама Мыла Раму") == "мама мыла раму");
            ASSERT_TRUE(get_lower_str(L"Мама Мыла Раму") == L"мама мыла раму");
            ASSERT_TRUE(get_lower_str(u"Мама Мыла Раму") == u"мама мыла раму");
            ASSERT_TRUE(get_lower_str(_T("Мама Мыла Раму")) == _T("мама мыла раму"));

            ASSERT_TRUE(get_lower_str(std::string("Мама Мыла Раму")) == "мама мыла раму");
            ASSERT_TRUE(get_lower_str(std::wstring(L"Мама Мыла Раму")) == L"мама мыла раму");
            ASSERT_TRUE(get_lower_str(std::u16string(u"Мама Мыла Раму")) == u"мама мыла раму");
            ASSERT_TRUE(get_lower_str(tlib::tstring(_T("Мама Мыла Раму"))) == _T("мама мыла раму"));

            ASSERT_TRUE(get_upper_str("Мама Мыла Раму") == "МАМА МЫЛА РАМУ");
            ASSERT_TRUE(get_upper_str(L"Мама Мыла Раму") == L"МАМА МЫЛА РАМУ");
            ASSERT_TRUE(get_upper_str(u"Мама Мыла Раму") == u"МАМА МЫЛА РАМУ");
            ASSERT_TRUE(get_upper_str(_T("Мама Мыла Раму")) == _T("МАМА МЫЛА РАМУ"));

            ASSERT_TRUE(get_upper_str(std::string("Мама Мыла Раму")) == "МАМА МЫЛА РАМУ");
            ASSERT_TRUE(get_upper_str(std::wstring(L"Мама Мыла Раму")) == L"МАМА МЫЛА РАМУ");
            ASSERT_TRUE(get_upper_str(std::u16string(u"Мама Мыла Раму")) == u"МАМА МЫЛА РАМУ");
            ASSERT_TRUE(get_upper_str(tlib::tstring(_T("Мама Мыла Раму"))) == _T("МАМА МЫЛА РАМУ"));

            ASSERT_TRUE(strcmp_i("МАМА МЫЛА РАМУ", "мама мыла раму"));
            ASSERT_TRUE(strcmp_i(L"МАМА МЫЛА РАМУ", L"мама мыла раму"));
            ASSERT_TRUE(strcmp_i(u"МАМА МЫЛА РАМУ", u"мама мыла раму"));
            ASSERT_TRUE(strcmp_i(_T("МАМА МЫЛА РАМУ"), _T("мама мыла раму")));

            ASSERT_TRUE(find_substr_i("МАМА МЫЛА РАМУ", "мыла") == 5);
            ASSERT_TRUE(find_substr_i(L"МАМА МЫЛА РАМУ", L"мыла") == 5);
            ASSERT_TRUE(find_substr_i(u"МАМА МЫЛА РАМУ", u"мыла") == 5);
            ASSERT_TRUE(find_substr_i(_T("МАМА МЫЛА РАМУ"), _T("мыла")) == 5);

            ASSERT_TRUE(find_substr_i("МАМА МЫЛА РАМУ", "ела") == tlib::npos);
            ASSERT_TRUE(find_substr_i(L"МАМА МЫЛА РАМУ", L"ела") == tlib::npos);
            ASSERT_TRUE(find_substr_i(u"МАМА МЫЛА РАМУ", u"ела") == tlib::npos);
            ASSERT_TRUE(find_substr_i(_T("МАМА МЫЛА РАМУ"), _T("ела")) == tlib::npos);

            ASSERT_TRUE(find_graph_symbol("  \n \b \a \v \t \b Мама Мыла Раму  ") == 14);
            ASSERT_TRUE(find_graph_symbol(L"  \n \b \a \v \t \b Мама Мыла Раму  ") == 14);
            ASSERT_TRUE(find_graph_symbol(u"  \n \b \a \v \t \b Мама Мыла Раму  ") == 14);
            ASSERT_TRUE(find_graph_symbol(_T("  \n \b \a \v \t \b Мама Мыла Раму  ")) == 14);

            ASSERT_TRUE(rfind_graph_symbol("  Мама Мыла Раму  \n \b \a \v \t \b ") == 15);
            ASSERT_TRUE(rfind_graph_symbol(L"  Мама Мыла Раму  \n \b \a \v \t \b ") == 15);
            ASSERT_TRUE(rfind_graph_symbol(u"  Мама Мыла Раму  \n \b \a \v \t \b ") == 15);
            ASSERT_TRUE(rfind_graph_symbol(_T("  Мама Мыла Раму  \n \b \a \v \t \b ")) == 15);

            ASSERT_TRUE(remove_first_spaces("  \n \b \a \v \t \b Мама Мыла Раму  ") == "Мама Мыла Раму  ");
            ASSERT_TRUE(remove_first_spaces(L"  \n \b \a \v \t \b Мама Мыла Раму  ") == L"Мама Мыла Раму  ");
            ASSERT_TRUE(remove_first_spaces(u"  \n \b \a \v \t \b Мама Мыла Раму  ") == u"Мама Мыла Раму  ");
            ASSERT_TRUE(remove_first_spaces(_T("  \n \b \a \v \t \b Мама Мыла Раму  ")) == _T("Мама Мыла Раму  "));

            ASSERT_TRUE(remove_trailing_spaces("  Мама Мыла Раму  \n \b \a \v \t \b ") == "  Мама Мыла Раму");
            ASSERT_TRUE(remove_trailing_spaces(L"  Мама Мыла Раму  \n \b \a \v \t \b ") == L"  Мама Мыла Раму");
            ASSERT_TRUE(remove_trailing_spaces(u"  Мама Мыла Раму  \n \b \a \v \t \b ") == u"  Мама Мыла Раму");
            ASSERT_TRUE(remove_trailing_spaces(_T("  Мама Мыла Раму  \n \b \a \v \t \b ")) == _T("  Мама Мыла Раму"));

            ASSERT_TRUE(remove_space("  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ") == "Мама Мыла Раму");
            ASSERT_TRUE(remove_space(L"  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ") == L"Мама Мыла Раму");
            ASSERT_TRUE(remove_space(u"  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ") == u"Мама Мыла Раму");
            ASSERT_TRUE(remove_space(_T("  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ")) == _T("Мама Мыла Раму"));
      }

      TEST(Tcodecvt, StringConversationFunctions)
      {
            ASSERT_TRUE(cstr_wstr("Мама мыла раму") == L"Мама мыла раму");
            ASSERT_TRUE(wstr_cstr(L"Мама мыла раму") == "Мама мыла раму");

            ASSERT_TRUE(u8str_wstr(u8"Мама мыла раму") == L"Мама мыла раму");
            ASSERT_TRUE(wstr_u8str(L"Мама мыла раму") == u8"Мама мыла раму");

            ASSERT_TRUE(u8str_u16str(u8"Мама мыла раму") == u"Мама мыла раму");
            ASSERT_TRUE(u16str_u8str(u"Мама мыла раму") == u8"Мама мыла раму");

            ASSERT_TRUE(u16str_wstr(u"Мама мыла раму") == L"Мама мыла раму");
            ASSERT_TRUE(wstr_u16str(L"Мама мыла раму") == u"Мама мыла раму");

            using cstr = std::string;
            using wstr = std::wstring;
            using ustr = std::u16string;

            ASSERT_TRUE(str2tstr(cstr("Мама мыла раму")) == _T("Мама мыла раму"));
            ASSERT_TRUE(wstr2tstr(wstr(L"Мама мыла раму")) == _T("Мама мыла раму"));
            ASSERT_TRUE(ustr2tstr(ustr(u"Мама мыла раму")) == _T("Мама мыла раму"));
            //ASSERT_TRUE(u8str2tstr(u8"Мама мыла раму") == _T("Мама мыла раму"));

            ASSERT_TRUE(tstr2str(_T("Мама мыла раму")) == cstr("Мама мыла раму"));
            ASSERT_TRUE(tstr2wstr(_T("Мама мыла раму")) == wstr(L"Мама мыла раму"));
            ASSERT_TRUE(tstr2ustr(_T("Мама мыла раму")) == ustr(u"Мама мыла раму"));
            //ASSERT_TRUE(tstr2u8str(_T("Мама мыла раму")) == u8"Мама мыла раму");         
      }



      TEST(Tiostream, set_locale)
      {
            // Проверяем всевозможные форматы задание имени локали
            // Формат имени локали не должен зависеть от платформы

            /*int pipePair[2];
            pipe(pipePair);

            dup2(pipePair[1], STDOUT_FILENO);

            InitConsolIO();
            std::wcout << L"Мама мыла раму" << std::endl;

            std::string buf(100, u'\0');
            size_t size = read(pipePair[0], &buf[0], 99);

            if (std::string_view(buf.c_str()) == u8"Мама мыла раму\n")
            {
                  ASSERT_TRUE(true);   
            }*/
            /////////////////////
            /* int pipePair[2];
            auto qq = _pipe(pipePair, 256, _O_BINARY);

            _dup2(pipePair[1], _fileno(stdout));


            InitConsolIO();
            std::wcout << L"QQQ" << std::endl;
            ucout << u"WWW" << std::endl;
            ucout << u"Мама мыла раму" << std::endl;

            _flushall();

            std::string buf(100, '\0');
            size_t size = _read(pipePair[0], &buf[0], 99);

            if (std::string_view(buf.c_str()) == u8"Мама мыла раму\n")
            {
                  ASSERT_TRUE(true);
            }

            _close(pipePair[0]);
            _close(pipePair[1]);*/
      }

}
