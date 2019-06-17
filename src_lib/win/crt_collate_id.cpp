// crt_collate_id.cpp: определяет экспортированные функции для приложения DLL.
//

#if !(defined(_DLL))                 // статическая линковка
#define LIB_CRT
#else                                   // динамическая линковка
#define LIB_CRT __declspec(dllexport)
#endif

#include "win/xlocnum"
#include "win/xlocmon"
#include "win/locale"

std::locale::id std::collate<char16_t>::id;
std::locale::id std::numpunct<char16_t>::id;
std::locale::id std::moneypunct<char16_t, false>::id;
std::locale::id std::moneypunct<char16_t, true>::id;
std::locale::id std::money_get<char16_t>::id;
std::locale::id std::money_put<char16_t>::id;
