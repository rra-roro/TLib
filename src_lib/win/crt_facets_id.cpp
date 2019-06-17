// crt_collate_id.cpp: определяет экспортированные функции для приложения DLL.
//

#if !(defined(_DLL)) // статическая линковка
#define LIB_CRT
#else // динамическая линковка
#define LIB_CRT __declspec(dllexport)
#endif

#include <locale>

namespace std
{
#ifdef _DLL
      template <>
      class collate<char16_t> : public locale::facet
      { // facet for ordering sequences of elements
        public:
            __PURE_APPDOMAIN_GLOBAL LIB_CRT static locale::id id; // unique facet id
      };

      template <>
      class numpunct<char16_t> : public locale::facet
      { // facet for defining numeric punctuation text
        public:
            __PURE_APPDOMAIN_GLOBAL LIB_CRT static locale::id id; // unique facet id
      };

      template <>
      class moneypunct<char16_t, false> : public std::_Mpunct<char16_t>
      { // facet for defining monetary punctuation text
        public:
            __PURE_APPDOMAIN_GLOBAL LIB_CRT static locale::id id; // unique facet id
      };

      template <>
      class moneypunct<char16_t, true> : public std::_Mpunct<char16_t>
      { // facet for defining monetary punctuation text
        public:
            __PURE_APPDOMAIN_GLOBAL LIB_CRT static locale::id id; // unique facet id
      };

      template <>
      class money_get<char16_t, std::istreambuf_iterator<char16_t>> : public locale::facet
      { // facet for converting text to encoded monetary amounts
        public:
            __PURE_APPDOMAIN_GLOBAL LIB_CRT static locale::id id; // unique facet id
      };

      template <>
      class money_put<char16_t, std::ostreambuf_iterator<char16_t>> : public locale::facet
      { // facet for converting encoded monetary amounts to text
        public:
            __PURE_APPDOMAIN_GLOBAL LIB_CRT static locale::id id; // unique facet id
      };

      template <>
      class ctype<char16_t>: public ctype_base
      { // facet for classifying elements, converting cases
        public:          
            __PURE_APPDOMAIN_GLOBAL LIB_CRT static locale::id id;
      };
#endif
}

std::locale::id std::collate<char16_t>::id;
std::locale::id std::numpunct<char16_t>::id;
std::locale::id std::moneypunct<char16_t, false>::id;
std::locale::id std::moneypunct<char16_t, true>::id;
std::locale::id std::money_get<char16_t>::id;
std::locale::id std::money_put<char16_t>::id;
std::locale::id std::ctype<char16_t>::id;
