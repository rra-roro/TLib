#pragma once
#include <locale>
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

      template <typename CharT>
      struct locale_cfg_numbers
      {
            using char_type = CharT;

            char_type decimal_point = TemplateTypeOfCh(',', char_type);
            char_type thousands_sep = TemplateTypeOfCh('.', char_type);
            std::string grouping = "\3";
            const char_type* truename = TemplateTypeOfStr("true", char_type);
            const char_type* falsename = TemplateTypeOfStr("false", char_type);
      };

      template <typename CharT>
      struct locale_cfg_moneys
      {
            using char_type = CharT;
            using string_type = std::basic_string<char_type>;
            using mb = std::money_base;
            using pattern = mb::pattern;            

            char_type decimal_point = TemplateTypeOfCh(',', char_type);
            char_type thousands_sep = TemplateTypeOfCh('.', char_type);
            std::string grouping = "\3";
            string_type curr_symbol = TemplateTypeOfStr("руб.", char_type);
            string_type positive_sign = TemplateTypeOfStr("+", char_type);
            string_type negative_sign = TemplateTypeOfStr("-", char_type);
            int frac_digits = 2;
            pattern pos_format = { mb::sign, mb::value, mb::space, mb::symbol };
            pattern neg_format = { mb::sign, mb::value, mb::space, mb::symbol };
      };

}
