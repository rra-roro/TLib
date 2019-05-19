// crt_collate_id.cpp: определяет экспортированные функции для приложения DLL.
//

#if !(defined(_DLL))                 // статическая линковка
#define LIB_CRT
#else                                   // динамическая линковка
#define LIB_CRT __declspec(dllexport)
#endif

#include "win/locale"


std::locale::id std::collate<char16_t>::id;
