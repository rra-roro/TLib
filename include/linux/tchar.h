
#ifndef __TCHAR_H
#define __TCHAR_H

typedef char16_t     TCHAR;

#define __T(x)      u ## x
#define _T(x)       __T(x)
#define _TEXT(x)    __T(x)

#endif // __TCHAR_H