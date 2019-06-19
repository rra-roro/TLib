// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

//////////////////////////////////////// 
// Common macroses: 

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
  #define _WIN32_WINNT _WIN32_WINNT_WIN8 // Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef WIN32_LEAN_AND_MEAN     
    #define WIN32_LEAN_AND_MEAN   // Это включаем для всех НЕ MFC проектов
#endif

//////////////////////////////////////// 
// Common headers: 

// C++ Standart headers:

#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#endif

#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <locale>
#include <iomanip>

// Windows headers
#include <windows.h>
#include <tchar.h>

// My headers
#include <Tstring.h>

