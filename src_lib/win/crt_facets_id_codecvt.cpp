// crt_codecvt_id.cpp: определяет экспортированные функции для приложения DLL.
//

#if !(defined(_DLL)) // статическая линковка
#define LIB_CRT
#else // динамическая линковка
#define LIB_CRT __declspec(dllexport)
#endif

#include "./xlocale_codecvt"

namespace std
{
       locale::id codecvt<char16_t, char, _Mbstatet>::id;
}


