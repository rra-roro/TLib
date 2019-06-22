/*
*   Файл TException.h - содержит макроссы и классы для создания инфраструктуры исключений
*
*/
#pragma once

#include <system_error>

#include <exception/Texception_class.h>
#include <exception/Texception_hendler.h>

#ifdef _WIN32
#include <win/exception_win.h>
#endif


#if !(defined(__linux__) || defined(_LIB) || defined(UNDER_CE) || defined(WINCE))

#define _LIB_NAME "tlib"

#include <win/select_lib.h>

#undef _LIB_NAME

#endif // !(defined(_LIB) || defined(UNDER_CE) || defined(WINCE))
