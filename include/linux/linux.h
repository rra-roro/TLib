#ifndef __LINUX_H
#define __LINUX_H

#include <locale.h>

inline char* GetConsoleOutputCP(void)
{
	// проинициализируем С run-time, 
	// локалью заданной в системе в переменной окружения LANG=
	setlocale(LC_ALL, "");
	return setlocale(LC_ALL, NULL);
}

typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} GUID;

#endif // __LINUX_H
