#include "gtest/gtest.h"
#include "gtest/gtest_prod.h"

#include "Tlib_version.h"
#include <string_view>
#include <algorithm>
#include <fcntl.h>
#include <thread>

#define PRIVATE_TEST 1

#include <Tlocale.h>
#include <Tiostream.h>
#include <Tstdlib.h>


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
                  tlib::locale l1("C");
                  tlib::locale l2("POSIX");

                  tlib::locale l3(".utf-8");

                  tlib::locale l4("ru_RU.Utf-8");
                  tlib::locale l5("ru_RU.uTf-8");
                  tlib::locale l6("ru_RU.uTf8");

                  tlib::locale l7("ru_RU.utf-8");
                  tlib::locale l8("ru-RU.utf-8");
                  tlib::locale l9("Russian_Russia.utf-8");
                  tlib::locale l10("Russian-Russia.utf-8");

                  auto locnames = tlib::get_available_locale_names();

                  auto end_en_locale = std::partition(locnames.begin(), locnames.end(),
                      [](auto& lcn) {
                            return lcn.find("en_") != npos;
                      });
                  if (end_en_locale != locnames.begin())
                        std::for_each(locnames.begin(), end_en_locale, [](auto& locname) { tlib::locale tmp(locname); });

                  auto end_ru_locale = std::partition(locnames.begin(), locnames.end(),
                      [](auto& lcn) {
                            return lcn.find("ru_") != npos;
                      });
                  if (end_ru_locale != locnames.begin())
                        std::for_each(locnames.begin(), end_ru_locale, [](auto& locname) { tlib::locale tmp(locname); });
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


            ASSERT_TRUE(std::use_facet<std::moneypunct<char>>(loc).curr_symbol() == u8"\u20bd" || // Unicode символ рубля
                        std::use_facet<std::moneypunct<char>>(loc).curr_symbol() == "руб." ||
                        std::use_facet<std::moneypunct<char>>(loc).curr_symbol() == "руб" ||
                        std::use_facet<std::moneypunct<char>>(loc).curr_symbol() == "RUB");

            ASSERT_TRUE(std::use_facet<std::moneypunct<wchar_t>>(loc).curr_symbol() == L"\u20bd" ||
                        std::use_facet<std::moneypunct<wchar_t>>(loc).curr_symbol() == L"руб." ||
                        std::use_facet<std::moneypunct<wchar_t>>(loc).curr_symbol() == L"руб" ||
                        std::use_facet<std::moneypunct<wchar_t>>(loc).curr_symbol() == L"RUB");

            ASSERT_TRUE(std::use_facet<std::moneypunct<char16_t>>(loc).curr_symbol() == u"\u20bd" ||
                        std::use_facet<std::moneypunct<char16_t>>(loc).curr_symbol() == u"руб." ||
                        std::use_facet<std::moneypunct<char16_t>>(loc).curr_symbol() == u"руб" ||
                        std::use_facet<std::moneypunct<char16_t>>(loc).curr_symbol() == u"RUB");

            ASSERT_TRUE(std::use_facet<std::moneypunct<TCHAR>>(loc).curr_symbol() == _T("\u20bd") ||
                        std::use_facet<std::moneypunct<TCHAR>>(loc).curr_symbol() == _T("руб.") ||
                        std::use_facet<std::moneypunct<TCHAR>>(loc).curr_symbol() == _T("руб") ||
                        std::use_facet<std::moneypunct<TCHAR>>(loc).curr_symbol() == _T("RUB"));


            ASSERT_TRUE((std::use_facet<std::moneypunct<char, true>>(loc).curr_symbol() == "RUB"));
            ASSERT_TRUE((std::use_facet<std::moneypunct<wchar_t, true>>(loc).curr_symbol() == L"RUB"));
            ASSERT_TRUE((std::use_facet<std::moneypunct<char16_t, true>>(loc).curr_symbol() == u"RUB"));
            ASSERT_TRUE((std::use_facet<std::moneypunct<TCHAR, true>>(loc).curr_symbol() == _T("RUB")));

            auto test_numbers = [](auto& os) -> bool {
                  auto get_test_str = [&](auto _Elem) -> std::basic_string<decltype(_Elem)> {
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
                                                               "\nfixed double: 10\240000,123400" __hexfloat__
                                                               "\ndefaultfloat double: 10\240000,1"
                                                               "\n\nсколько денег: 12\240345\240678,91 руб",
                            char_type);
                        // "\n\nвремя: Четверг Чт 27 июн 2019 09:51:09", char_type);

                        std::replace(result.begin(), result.end(), TemplateTypeOfCh('\240', char_type), thousands_sep);

                        result.replace(result.find(TemplateTypeOfStr("руб", char_type)),
                            string_type(TemplateTypeOfStr("руб", char_type)).size(), curr_symbol);

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
            auto test_symbols_true = [](auto _Elem, tlib::locale loc) -> bool {
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
            ASSERT_TRUE(get_lower_str(u8"Мама Мыла Раму", tlib::locale("ru_RU.utf8")) == u8"мама мыла раму");
            ASSERT_TRUE(get_lower_str(L"Мама Мыла Раму") == L"мама мыла раму");
            ASSERT_TRUE(get_lower_str(u"Мама Мыла Раму") == u"мама мыла раму");
            ASSERT_TRUE(get_lower_str(_T("Мама Мыла Раму")) == _T("мама мыла раму"));

            ASSERT_TRUE(get_lower_str(std::string("Мама Мыла Раму")) == "мама мыла раму");
            ASSERT_TRUE(get_lower_str(std::string(u8"Мама Мыла Раму"), tlib::locale("ru_RU.utf8")) == u8"мама мыла раму");
            ASSERT_TRUE(get_lower_str(std::wstring(L"Мама Мыла Раму")) == L"мама мыла раму");
            ASSERT_TRUE(get_lower_str(std::u16string(u"Мама Мыла Раму")) == u"мама мыла раму");
            ASSERT_TRUE(get_lower_str(tlib::tstring(_T("Мама Мыла Раму"))) == _T("мама мыла раму"));

            std::string str_lo = "Мама Мыла Раму";
            std::string str8_lo = u8"Мама Мыла Раму";
            std::wstring wstr_lo = L"Мама Мыла Раму";
            std::u16string ustr_lo = u"Мама Мыла Раму";
            tlib::tstring tstr_lo = _T("Мама Мыла Раму");

            ASSERT_TRUE(to_lower(str_lo) == "мама мыла раму");
            ASSERT_TRUE(to_lower(str8_lo, tlib::locale("ru_RU.utf8")) == u8"мама мыла раму");
            ASSERT_TRUE(to_lower(wstr_lo) == L"мама мыла раму");
            ASSERT_TRUE(to_lower(ustr_lo) == u"мама мыла раму");
            ASSERT_TRUE(to_lower(tstr_lo) == _T("мама мыла раму"));

            ASSERT_TRUE(get_upper_str("Мама Мыла Раму") == "МАМА МЫЛА РАМУ");
            ASSERT_TRUE(get_upper_str(u8"Мама Мыла Раму", tlib::locale("ru_RU.utf8")) == u8"МАМА МЫЛА РАМУ");
            ASSERT_TRUE(get_upper_str(L"Мама Мыла Раму") == L"МАМА МЫЛА РАМУ");
            ASSERT_TRUE(get_upper_str(u"Мама Мыла Раму") == u"МАМА МЫЛА РАМУ");
            ASSERT_TRUE(get_upper_str(_T("Мама Мыла Раму")) == _T("МАМА МЫЛА РАМУ"));

            ASSERT_TRUE(get_upper_str(std::string("Мама Мыла Раму")) == "МАМА МЫЛА РАМУ");
            ASSERT_TRUE(get_upper_str(std::string(u8"Мама Мыла Раму"), tlib::locale("ru_RU.utf8")) == u8"МАМА МЫЛА РАМУ");
            ASSERT_TRUE(get_upper_str(std::wstring(L"Мама Мыла Раму")) == L"МАМА МЫЛА РАМУ");
            ASSERT_TRUE(get_upper_str(std::u16string(u"Мама Мыла Раму")) == u"МАМА МЫЛА РАМУ");
            ASSERT_TRUE(get_upper_str(tlib::tstring(_T("Мама Мыла Раму"))) == _T("МАМА МЫЛА РАМУ"));

            std::string str_up = "Мама Мыла Раму";
            std::string str8_up = u8"Мама Мыла Раму";
            std::wstring wstr_up = L"Мама Мыла Раму";
            std::u16string ustr_up = u"Мама Мыла Раму";
            tlib::tstring tstr_up = _T("Мама Мыла Раму");

            ASSERT_TRUE(to_upper(str_up) == "МАМА МЫЛА РАМУ");
            ASSERT_TRUE(to_upper(str8_up, tlib::locale("ru_RU.utf8")) == u8"МАМА МЫЛА РАМУ");
            ASSERT_TRUE(to_upper(wstr_up) == L"МАМА МЫЛА РАМУ");
            ASSERT_TRUE(to_upper(ustr_up) == u"МАМА МЫЛА РАМУ");
            ASSERT_TRUE(to_upper(tstr_up) == _T("МАМА МЫЛА РАМУ"));

            ASSERT_TRUE(strcmp_i("МАМА МЫЛА РАМУ", "мама мыла раму"));
            ASSERT_TRUE(strcmp_i(u8"МАМА МЫЛА РАМУ", u8"мама мыла раму", tlib::locale("ru_RU.utf8")));
            ASSERT_TRUE(strcmp_i(L"МАМА МЫЛА РАМУ", L"мама мыла раму"));
            ASSERT_TRUE(strcmp_i(u"МАМА МЫЛА РАМУ", u"мама мыла раму"));
            ASSERT_TRUE(strcmp_i(_T("МАМА МЫЛА РАМУ"), _T("мама мыла раму")));

            ASSERT_TRUE(find_substr_i("МАМА МЫЛА РАМУ", "мыла") == 5);
            ASSERT_TRUE(find_substr_i(u8"МАМА МЫЛА РАМУ", u8"мыла", tlib::locale("ru_RU.utf8")) == 5);
            ASSERT_TRUE(find_substr_i(L"МАМА МЫЛА РАМУ", L"мыла") == 5);
            ASSERT_TRUE(find_substr_i(u"МАМА МЫЛА РАМУ", u"мыла") == 5);
            ASSERT_TRUE(find_substr_i(_T("МАМА МЫЛА РАМУ"), _T("мыла")) == 5);

#ifdef _WIN32
            ASSERT_TRUE(mb_sym_byte("МАМА МЫЛА РАМУ", 5) == 5);
            ASSERT_TRUE(mb_byte_sym("МАМА МЫЛА РАМУ", 5) == 5);
#elif __linux__
            ASSERT_TRUE(mb_sym_byte("МАМА МЫЛА РАМУ", 5) == 9);
            ASSERT_TRUE(mb_byte_sym("МАМА МЫЛА РАМУ", 9) == 5);
#endif
            ASSERT_TRUE(mb_sym_byte(u8"МАМА МЫЛА РАМУ", 5, tlib::locale("ru_RU.utf8")) == 9);
            ASSERT_TRUE(mb_byte_sym(u8"МАМА МЫЛА РАМУ", 9, tlib::locale("ru_RU.utf8")) == 5);

            ASSERT_TRUE(find_substr_i("МАМА МЫЛА МыЛа РАМУ", 6, "мыла") == 10);
            ASSERT_TRUE(find_substr_i(u8"МАМА МЫЛА МыЛа РАМУ", 6, u8"мыла", tlib::locale("ru_RU.utf8")) == 10);
            ASSERT_TRUE(find_substr_i(L"МАМА МЫЛА МыЛа РАМУ", 6, L"мыла") == 10);
            ASSERT_TRUE(find_substr_i(u"МАМА МЫЛА МыЛа РАМУ", 6, u"мыла") == 10);
            ASSERT_TRUE(find_substr_i(_T("МАМА МЫЛА МыЛа РАМУ"), 6, _T("мыла")) == 10);

#ifdef _WIN32
            ASSERT_TRUE(mb_sym_byte("МАМА МЫЛА МыЛа РАМУ", 10) == 10);
            ASSERT_TRUE(mb_byte_sym("МАМА МЫЛА МыЛа РАМУ", 10) == 10);
#elif __linux__
            ASSERT_TRUE(mb_sym_byte("МАМА МЫЛА МыЛа РАМУ", 10) == 18);
            ASSERT_TRUE(mb_byte_sym("МАМА МЫЛА МыЛа РАМУ", 18) == 10);
#endif
            ASSERT_TRUE(mb_sym_byte(u8"МАМА МЫЛА МыЛа РАМУ", 10, tlib::locale("ru_RU.utf8")) == 18);
            ASSERT_TRUE(mb_byte_sym(u8"МАМА МЫЛА МыЛа РАМУ", 18, tlib::locale("ru_RU.utf8")) == 10);

            ASSERT_TRUE(find_substr_i("МАМА МЫЛА РАМУ", "ела") == tlib::npos);
            ASSERT_TRUE(find_substr_i(L"МАМА МЫЛА РАМУ", L"ела") == tlib::npos);
            ASSERT_TRUE(find_substr_i(u"МАМА МЫЛА РАМУ", u"ела") == tlib::npos);
            ASSERT_TRUE(find_substr_i(_T("МАМА МЫЛА РАМУ"), _T("ела")) == tlib::npos);

            ASSERT_TRUE(find_graph_symbol("  \n \b \a \v \t \b Мама Мыла Раму  ") == 14);
            ASSERT_TRUE(find_graph_symbol(u8"  \n \b \a \v \t \b Мама Мыла Раму  ", 0, tlib::locale("ru_RU.utf8")) == 14);
            ASSERT_TRUE(find_graph_symbol(L"  \n \b \a \v \t \b Мама Мыла Раму  ") == 14);
            ASSERT_TRUE(find_graph_symbol(u"  \n \b \a \v \t \b Мама Мыла Раму  ") == 14);
            ASSERT_TRUE(find_graph_symbol(_T("  \n \b \a \v \t \b Мама Мыла Раму  ")) == 14);

            ASSERT_TRUE(rfind_graph_symbol("  Мама Мыла Раму  \n \b \a \v \t \b ") == 15);
            ASSERT_TRUE(rfind_graph_symbol(u8"  Мама Мыла Раму  \n \b \a \v \t \b ", npos, tlib::locale("ru_RU.utf8")) == 15);
            ASSERT_TRUE(rfind_graph_symbol(L"  Мама Мыла Раму  \n \b \a \v \t \b ") == 15);
            ASSERT_TRUE(rfind_graph_symbol(u"  Мама Мыла Раму  \n \b \a \v \t \b ") == 15);
            ASSERT_TRUE(rfind_graph_symbol(_T("  Мама Мыла Раму  \n \b \a \v \t \b ")) == 15);

            ASSERT_TRUE(remove_left_spaces("  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ") == "Мама Мыла Раму  \n \b \a \v \t \b ");
            ASSERT_TRUE(remove_left_spaces(u8"  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ", tlib::locale("ru_RU.utf8")) == u8"Мама Мыла Раму  \n \b \a \v \t \b ");
            ASSERT_TRUE(remove_left_spaces(L"  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ") == L"Мама Мыла Раму  \n \b \a \v \t \b ");
            ASSERT_TRUE(remove_left_spaces(u"  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ") == u"Мама Мыла Раму  \n \b \a \v \t \b ");
            ASSERT_TRUE(remove_left_spaces(_T("  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ")) == _T("Мама Мыла Раму  \n \b \a \v \t \b "));

            ASSERT_TRUE(remove_right_spaces("  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ") == "  \n \b \a \v \t \b Мама Мыла Раму");
            ASSERT_TRUE(remove_right_spaces(u8"  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ", tlib::locale("ru_RU.utf8")) == u8"  \n \b \a \v \t \b Мама Мыла Раму");
            ASSERT_TRUE(remove_right_spaces(L"  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ") == L"  \n \b \a \v \t \b Мама Мыла Раму");
            ASSERT_TRUE(remove_right_spaces(u"  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ") == u"  \n \b \a \v \t \b Мама Мыла Раму");
            ASSERT_TRUE(remove_right_spaces(_T("  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ")) == _T("  \n \b \a \v \t \b Мама Мыла Раму"));

            ASSERT_TRUE(remove_space("  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ") == "Мама Мыла Раму");
            ASSERT_TRUE(remove_space(u8"  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ", tlib::locale("ru_RU.utf8")) == u8"Мама Мыла Раму");
            ASSERT_TRUE(remove_space(L"  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ") == L"Мама Мыла Раму");
            ASSERT_TRUE(remove_space(u"  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ") == u"Мама Мыла Раму");
            ASSERT_TRUE(remove_space(_T("  \n \b \a \v \t \b Мама Мыла Раму  \n \b \a \v \t \b ")) == _T("Мама Мыла Раму"));
      }

      TEST(Tcodecvt, StringConversationFunctions)
      {
            ASSERT_TRUE(cstr_wstr("Мама мыла раму") == L"Мама мыла раму");
            ASSERT_TRUE(wstr_cstr(L"Мама мыла раму") == "Мама мыла раму");

            ASSERT_TRUE(cstr_wstr(u8"Мама мыла раму", "ru_RU.utf8") == L"Мама мыла раму");
            ASSERT_TRUE(wstr_cstr(L"Мама мыла раму", "ru_RU.utf8") == u8"Мама мыла раму");

            ASSERT_TRUE(cstr_u16str("Мама мыла раму") == u"Мама мыла раму");
            ASSERT_TRUE(u16str_cstr(u"Мама мыла раму") == "Мама мыла раму");

            ASSERT_TRUE(cstr_u16str(u8"Мама мыла раму", "ru_RU.utf8") == u"Мама мыла раму");
            ASSERT_TRUE(u16str_cstr(u"Мама мыла раму", "ru_RU.utf8") == u8"Мама мыла раму");

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
            ASSERT_TRUE(u8str2tstr(u8"Мама мыла раму") == _T("Мама мыла раму"));

            ASSERT_TRUE(tstr2str(_T("Мама мыла раму")) == cstr("Мама мыла раму"));
            ASSERT_TRUE(tstr2wstr(_T("Мама мыла раму")) == wstr(L"Мама мыла раму"));
            ASSERT_TRUE(tstr2ustr(_T("Мама мыла раму")) == ustr(u"Мама мыла раму"));
            ASSERT_TRUE(tstr2u8str(_T("Мама мыла раму")) == u8"Мама мыла раму");
      }

      TEST(Tstdlib, NumberToString)
      {
            tlib::tstring str;
            ASSERT_TRUE(tlib::dig_to_tstr(-10LL, str, 16) && str == _T("fffffffffffffff6"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10LL, str, 15) && str == _T("2c1d56b648c6cd106"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10LL, str, 14) && str == _T("8681049adb03db166"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10LL, str, 13) && str == _T("219505a9511a867b66"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10LL, str, 12) && str == _T("839365134a2a240706"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10LL, str, 11) && str == _T("335500516a429071276"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10LL, str, 10) && str == _T("-10"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10LL, str, 9) && str == _T("145808576354216723746"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10LL, str, 8) && str == _T("1777777777777777777766"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10LL, str, 7) && str == _T("45012021522523134134556"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10LL, str, 6) && str == _T("3520522010102100444244410"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10LL, str, 5) && str == _T("2214220303114400424121122411"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10LL, str, 4) && str == _T("33333333333333333333333333333312"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10LL, str, 3) && str == _T("11112220022122120101211020120210210211120"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10LL, str, 2) && str == _T("1111111111111111111111111111111111111111111111111111111111110110"));

            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long long)-10LL, str, 16) && str == _T("fffffffffffffff6"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long long)-10LL, str, 15) && str == _T("2c1d56b648c6cd106"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long long)-10LL, str, 14) && str == _T("8681049adb03db166"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long long)-10LL, str, 13) && str == _T("219505a9511a867b66"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long long)-10LL, str, 12) && str == _T("839365134a2a240706"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long long)-10LL, str, 11) && str == _T("335500516a429071276"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long long)-10LL, str, 10) && str == _T("18446744073709551606"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long long)-10LL, str, 9) && str == _T("145808576354216723746"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long long)-10LL, str, 8) && str == _T("1777777777777777777766"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long long)-10LL, str, 7) && str == _T("45012021522523134134556"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long long)-10LL, str, 6) && str == _T("3520522010102100444244410"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long long)-10LL, str, 5) && str == _T("2214220303114400424121122411"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long long)-10LL, str, 4) && str == _T("33333333333333333333333333333312"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long long)-10LL, str, 3) && str == _T("11112220022122120101211020120210210211120"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long long)-10LL, str, 2) && str == _T("1111111111111111111111111111111111111111111111111111111111110110"));

#ifdef __linux__
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 16) && str == _T("fffffffffffffff6"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 15) && str == _T("2c1d56b648c6cd106"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 14) && str == _T("8681049adb03db166"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 13) && str == _T("219505a9511a867b66"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 12) && str == _T("839365134a2a240706"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 11) && str == _T("335500516a429071276"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 10) && str == _T("-10"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 9) && str == _T("145808576354216723746"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 8) && str == _T("1777777777777777777766"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 7) && str == _T("45012021522523134134556"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 6) && str == _T("3520522010102100444244410"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 5) && str == _T("2214220303114400424121122411"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 4) && str == _T("33333333333333333333333333333312"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 3) && str == _T("11112220022122120101211020120210210211120"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 2) && str == _T("1111111111111111111111111111111111111111111111111111111111110110"));

            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10LL, str, 16) && str == _T("fffffffffffffff6"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10LL, str, 15) && str == _T("2c1d56b648c6cd106"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10LL, str, 14) && str == _T("8681049adb03db166"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10LL, str, 13) && str == _T("219505a9511a867b66"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10LL, str, 12) && str == _T("839365134a2a240706"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10LL, str, 11) && str == _T("335500516a429071276"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10LL, str, 10) && str == _T("18446744073709551606"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10LL, str, 9) && str == _T("145808576354216723746"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10LL, str, 8) && str == _T("1777777777777777777766"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10LL, str, 7) && str == _T("45012021522523134134556"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10LL, str, 6) && str == _T("3520522010102100444244410"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10LL, str, 5) && str == _T("2214220303114400424121122411"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10LL, str, 4) && str == _T("33333333333333333333333333333312"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10LL, str, 3) && str == _T("11112220022122120101211020120210210211120"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10LL, str, 2) && str == _T("1111111111111111111111111111111111111111111111111111111111110110"));
#elif _WIN32
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 16) && str == _T("fffffff6"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 15) && str == _T("1a20dcd76"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 14) && str == _T("2ca5b7458"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 13) && str == _T("535a7987c"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 12) && str == _T("9ba461586"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 11) && str == _T("1904440545"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 10) && str == _T("-10"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 9) && str == _T("12068657443"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 8) && str == _T("37777777766"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 7) && str == _T("211301422341"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 6) && str == _T("1550104015450"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 5) && str == _T("32244002423121"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 4) && str == _T("3333333333333312"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 3) && str == _T("102002022201221111110"));
            ASSERT_TRUE(tlib::dig_to_tstr(-10L, str, 2) && str == _T("11111111111111111111111111110110"));

            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10L, str, 16) && str == _T("fffffff6"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10L, str, 15) && str == _T("1a20dcd76"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10L, str, 14) && str == _T("2ca5b7458"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10L, str, 13) && str == _T("535a7987c"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10L, str, 12) && str == _T("9ba461586"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10L, str, 11) && str == _T("1904440545"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10L, str, 10) && str == _T("4294967286"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10L, str, 9) && str == _T("12068657443"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10L, str, 8) && str == _T("37777777766"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10L, str, 7) && str == _T("211301422341"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10L, str, 6) && str == _T("1550104015450"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10L, str, 5) && str == _T("32244002423121"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10L, str, 4) && str == _T("3333333333333312"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10L, str, 3) && str == _T("102002022201221111110"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned long)-10L, str, 2) && str == _T("11111111111111111111111111110110"));
#endif

            ASSERT_TRUE(tlib::dig_to_tstr((short)-10L, str, 16) && str == _T("fff6"));
            ASSERT_TRUE(tlib::dig_to_tstr((short)-10L, str, 15) && str == _T("14636"));
            ASSERT_TRUE(tlib::dig_to_tstr((short)-10L, str, 14) && str == _T("19c46"));
            ASSERT_TRUE(tlib::dig_to_tstr((short)-10L, str, 13) && str == _T("23a96"));
            ASSERT_TRUE(tlib::dig_to_tstr((short)-10L, str, 12) && str == _T("31b06"));
            ASSERT_TRUE(tlib::dig_to_tstr((short)-10L, str, 11) && str == _T("4525a"));
            ASSERT_TRUE(tlib::dig_to_tstr((short)-10L, str, 10) && str == _T("-10"));
            ASSERT_TRUE(tlib::dig_to_tstr((short)-10L, str, 9) && str == _T("108786"));
            ASSERT_TRUE(tlib::dig_to_tstr((short)-10L, str, 8) && str == _T("177766"));
            ASSERT_TRUE(tlib::dig_to_tstr((short)-10L, str, 7) && str == _T("362016"));
            ASSERT_TRUE(tlib::dig_to_tstr((short)-10L, str, 6) && str == _T("1223210"));
            ASSERT_TRUE(tlib::dig_to_tstr((short)-10L, str, 5) && str == _T("4044101"));
            ASSERT_TRUE(tlib::dig_to_tstr((short)-10L, str, 4) && str == _T("33333312"));
            ASSERT_TRUE(tlib::dig_to_tstr((short)-10L, str, 3) && str == _T("10022212220"));
            ASSERT_TRUE(tlib::dig_to_tstr((short)-10L, str, 2) && str == _T("1111111111110110"));

            ASSERT_TRUE(tlib::dig_to_tstr((unsigned short)-10L, str, 16) && str == _T("fff6"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned short)-10L, str, 15) && str == _T("14636"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned short)-10L, str, 14) && str == _T("19c46"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned short)-10L, str, 13) && str == _T("23a96"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned short)-10L, str, 12) && str == _T("31b06"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned short)-10L, str, 11) && str == _T("4525a"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned short)-10L, str, 10) && str == _T("65526"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned short)-10L, str, 9) && str == _T("108786"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned short)-10L, str, 8) && str == _T("177766"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned short)-10L, str, 7) && str == _T("362016"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned short)-10L, str, 6) && str == _T("1223210"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned short)-10L, str, 5) && str == _T("4044101"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned short)-10L, str, 4) && str == _T("33333312"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned short)-10L, str, 3) && str == _T("10022212220"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned short)-10L, str, 2) && str == _T("1111111111110110"));

            ASSERT_TRUE(tlib::dig_to_tstr((int)-10L, str, 16) && str == _T("fffffff6"));
            ASSERT_TRUE(tlib::dig_to_tstr((int)-10L, str, 15) && str == _T("1a20dcd76"));
            ASSERT_TRUE(tlib::dig_to_tstr((int)-10L, str, 14) && str == _T("2ca5b7458"));
            ASSERT_TRUE(tlib::dig_to_tstr((int)-10L, str, 13) && str == _T("535a7987c"));
            ASSERT_TRUE(tlib::dig_to_tstr((int)-10L, str, 12) && str == _T("9ba461586"));
            ASSERT_TRUE(tlib::dig_to_tstr((int)-10L, str, 11) && str == _T("1904440545"));
            ASSERT_TRUE(tlib::dig_to_tstr((int)-10L, str, 10) && str == _T("-10"));
            ASSERT_TRUE(tlib::dig_to_tstr((int)-10L, str, 9) && str == _T("12068657443"));
            ASSERT_TRUE(tlib::dig_to_tstr((int)-10L, str, 8) && str == _T("37777777766"));
            ASSERT_TRUE(tlib::dig_to_tstr((int)-10L, str, 7) && str == _T("211301422341"));
            ASSERT_TRUE(tlib::dig_to_tstr((int)-10L, str, 6) && str == _T("1550104015450"));
            ASSERT_TRUE(tlib::dig_to_tstr((int)-10L, str, 5) && str == _T("32244002423121"));
            ASSERT_TRUE(tlib::dig_to_tstr((int)-10L, str, 4) && str == _T("3333333333333312"));
            ASSERT_TRUE(tlib::dig_to_tstr((int)-10L, str, 3) && str == _T("102002022201221111110"));
            ASSERT_TRUE(tlib::dig_to_tstr((int)-10L, str, 2) && str == _T("11111111111111111111111111110110"));

            ASSERT_TRUE(tlib::dig_to_tstr((unsigned int)-10L, str, 16) && str == _T("fffffff6"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned int)-10L, str, 15) && str == _T("1a20dcd76"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned int)-10L, str, 14) && str == _T("2ca5b7458"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned int)-10L, str, 13) && str == _T("535a7987c"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned int)-10L, str, 12) && str == _T("9ba461586"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned int)-10L, str, 11) && str == _T("1904440545"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned int)-10L, str, 10) && str == _T("4294967286"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned int)-10L, str, 9) && str == _T("12068657443"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned int)-10L, str, 8) && str == _T("37777777766"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned int)-10L, str, 7) && str == _T("211301422341"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned int)-10L, str, 6) && str == _T("1550104015450"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned int)-10L, str, 5) && str == _T("32244002423121"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned int)-10L, str, 4) && str == _T("3333333333333312"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned int)-10L, str, 3) && str == _T("102002022201221111110"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned int)-10L, str, 2) && str == _T("11111111111111111111111111110110"));

            ASSERT_TRUE(tlib::dig_to_tstr((char)-10L, str, 16) && str == _T("f6"));
            ASSERT_TRUE(tlib::dig_to_tstr((char)-10L, str, 15) && str == _T("116"));
            ASSERT_TRUE(tlib::dig_to_tstr((char)-10L, str, 14) && str == _T("138"));
            ASSERT_TRUE(tlib::dig_to_tstr((char)-10L, str, 13) && str == _T("15c"));
            ASSERT_TRUE(tlib::dig_to_tstr((char)-10L, str, 12) && str == _T("186"));
            ASSERT_TRUE(tlib::dig_to_tstr((char)-10L, str, 11) && str == _T("204"));
            ASSERT_TRUE(tlib::dig_to_tstr((char)-10L, str, 10) && str == _T("-10"));
            ASSERT_TRUE(tlib::dig_to_tstr((char)-10L, str, 9) && str == _T("303"));
            ASSERT_TRUE(tlib::dig_to_tstr((char)-10L, str, 8) && str == _T("366"));
            ASSERT_TRUE(tlib::dig_to_tstr((char)-10L, str, 7) && str == _T("501"));
            ASSERT_TRUE(tlib::dig_to_tstr((char)-10L, str, 6) && str == _T("1050"));
            ASSERT_TRUE(tlib::dig_to_tstr((char)-10L, str, 5) && str == _T("1441"));
            ASSERT_TRUE(tlib::dig_to_tstr((char)-10L, str, 4) && str == _T("3312"));
            ASSERT_TRUE(tlib::dig_to_tstr((char)-10L, str, 3) && str == _T("100010"));
            ASSERT_TRUE(tlib::dig_to_tstr((char)-10L, str, 2) && str == _T("11110110"));

            ASSERT_TRUE(tlib::dig_to_tstr((unsigned char)-10L, str, 16) && str == _T("f6"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned char)-10L, str, 15) && str == _T("116"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned char)-10L, str, 14) && str == _T("138"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned char)-10L, str, 13) && str == _T("15c"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned char)-10L, str, 12) && str == _T("186"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned char)-10L, str, 11) && str == _T("204"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned char)-10L, str, 10) && str == _T("246"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned char)-10L, str, 9) && str == _T("303"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned char)-10L, str, 8) && str == _T("366"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned char)-10L, str, 7) && str == _T("501"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned char)-10L, str, 6) && str == _T("1050"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned char)-10L, str, 5) && str == _T("1441"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned char)-10L, str, 4) && str == _T("3312"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned char)-10L, str, 3) && str == _T("100010"));
            ASSERT_TRUE(tlib::dig_to_tstr((unsigned char)-10L, str, 2) && str == _T("11110110"));
      }


      TEST(Tiostream, SetIostreamLocale)
      {
            // Инициализируем I/O streams для вывода текста в кодировке консоли, 
            // со всеми необходимыми фасетами

            InitConsolIO();

            ASSERT_TRUE(std::locale().name() == "*");
            ASSERT_TRUE(setlocale(LC_ALL, NULL) != std::string("C"));

            auto print_out = [&](std::string_view io_name, std::ostream& os, std::wostream& wos, tlib::u16ostream& uos, tlib::tostream& tos)
            {
                  os << "[          ] :  " << io_name << " : 'Мама мыла раму'" << std::endl;
                  wos << L"[          ] : w" << cstr_wstr(io_name) << L" : 'Мама мыла раму'" << std::endl;
                  uos << u"[          ] : u" << cstr_u16str(io_name) << u" : 'Мама мыла раму'" << std::endl;                  
                  tos << _T("[          ] : t") << str2tstr(io_name) << _T(" : 'Мама мыла раму'") << std::endl;                  
            };

            print_out("cout", std::cout, std::wcout, tlib::ucout, tlib::tcout);
            print_out("cerr", std::cerr, std::wcerr, tlib::ucerr, tlib::tcerr);
            print_out("clog", std::clog, std::wclog, tlib::uclog, tlib::tclog);

            int pipePair[2];
            int pipePair_stderr[2];
#ifdef _WIN32
            auto saved_stdout = _dup(_fileno(stdout));  // сохраним связь терминала и stdout
            _pipe(pipePair, 256, _O_BINARY);
            _dup2(pipePair[1], _fileno(stdout));        // свяжем stdout с pipe

            auto saved_stderr = _dup(_fileno(stderr));  // сохраним связь терминала и stdout
            _pipe(pipePair_stderr, 256, _O_BINARY);
            _dup2(pipePair_stderr[1], _fileno(stderr)); // свяжем stderr с pipe
#elif __linux__
            auto saved_stdout = dup(STDOUT_FILENO);  // сохраним связь терминала и stdout
            pipe(pipePair);
            dup2(pipePair[1], STDOUT_FILENO);        // свяжем stdout с pipe

            auto saved_stderr = dup(STDERR_FILENO);  // сохраним связь терминала и stderr
            pipe(pipePair_stderr);
            dup2(pipePair_stderr[1], STDERR_FILENO); // свяжем stderr с pipe
#define _read read
#endif

            auto get_stdout = [&](int* pipePair) {
                  fflush(NULL);
                  std::string buf(100, '\0');
                  _read(pipePair[0], &buf[0], 99);
                  return std::string(buf.c_str());
            };

            auto test_out = [&](int* pipePair, std::ostream& os, std::wostream& wos, tlib::u16ostream& uos, tlib::tostream& tos)
            {
                  InitConsolIO();

                  os << "Мама мыла раму" << std::endl;
                  ASSERT_TRUE(get_stdout(pipePair) == "Мама мыла раму\n");

                  wos << L"мама Мыла раму" << std::endl;
                  ASSERT_TRUE(get_stdout(pipePair) == "мама Мыла раму\n");

                  uos << u"мама мыла Раму" << std::endl;
                  ASSERT_TRUE(get_stdout(pipePair) == "мама мыла Раму\n");

                  tos << _T("мама мыла раму") << std::endl;
                  ASSERT_TRUE(get_stdout(pipePair) == "мама мыла раму\n");
            };

            // test std streams in main thread
            test_out(pipePair, std::cout, std::wcout, tlib::ucout, tlib::tcout);
            test_out(pipePair_stderr, std::cerr, std::wcerr, tlib::ucerr, tlib::tcerr);
            test_out(pipePair_stderr, std::clog, std::wclog, tlib::uclog, tlib::tclog);

            // test std streams in other thread
            std::thread test_thread_out(test_out, pipePair, std::ref(std::cout), std::ref(std::wcout), std::ref(tlib::ucout), std::ref(tlib::tcout));
            test_thread_out.join();
            std::thread test_thread_err(test_out, pipePair_stderr, std::ref(std::cerr), std::ref(std::wcerr), std::ref(tlib::ucerr), std::ref(tlib::tcerr));
            test_thread_err.join();
            std::thread test_thread_log(test_out, pipePair_stderr, std::ref(std::clog), std::ref(std::wclog), std::ref(tlib::uclog), std::ref(tlib::tclog));
            test_thread_log.join();


#ifdef _WIN32
            _dup2(saved_stdout, _fileno(stdout)); // восстановим связь терминала и stdout
            _close(saved_stdout);                 // закроем копию дискрептора
            _close(pipePair[0]);                  // закроем дискрепторы pipe
            _close(pipePair[1]);

            _dup2(saved_stderr, _fileno(stderr)); // восстановим связь терминала и stderr
            _close(saved_stderr);                 // закроем копию дискрептора
            _close(pipePair_stderr[1]);           // закроем дискрепторы pipe
            _close(pipePair_stderr[0]);
#elif __linux__
            dup2(saved_stdout, STDOUT_FILENO); // восстановим связь терминала и stdout
            close(saved_stdout);               // закроем копию дискрептора
            close(pipePair[1]);                // закроем дискрепторы pipe
            close(pipePair[0]);

            dup2(saved_stderr, STDERR_FILENO); // восстановим связь терминала и stderr
            close(saved_stderr);               // закроем копию дискрептора
            close(pipePair_stderr[1]);         // закроем дискрепторы pipe
            close(pipePair_stderr[0]);
#undef _read
#endif
      }

      TEST(Tiomanip, put_intger_by_radix)
      {
            std::cout << "tertwe\n";
      }

}
