/*
*
*/

#ifndef TCODECVT
#define TCODECVT

#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#endif

#include <codecvt>

#if !(defined(__linux__) || defined(_LIB))

#define _LIB_NAME "tlib_char16"

#include <win/select_lib.h>

#undef _LIB_NAME

#endif // !(defined(__linux__) || defined(_LIB))
        

#endif  //TCODECVT
