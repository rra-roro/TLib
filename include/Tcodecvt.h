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

#define _LIB_NAME "Tchar16_support"

#ifdef _UNICODE
	#if !(defined(_DLL))                    // статическая линковка
			#ifdef _DEBUG
                #pragma message("Link with " _LIB_NAME "wd.lib") 
				#pragma comment (lib, _LIB_NAME "wd.lib")
			#else
                #pragma message("Link with " _LIB_NAME "w.lib") 
				#pragma comment (lib, _LIB_NAME "w.lib")
			#endif //_DEBUG
	#else                                   // динамическая линковка
			#ifdef _DEBUG
                #pragma message("Link with " _LIB_NAME "wd_crtdll.lib") 
				#pragma comment (lib, _LIB_NAME "wd_crtdll.lib")
			#else
                #pragma message("Link with " _LIB_NAME "w_crtdll.lib") 
				#pragma comment (lib, _LIB_NAME "w_crtdll.lib")
			#endif //_DEBUG
	#endif // !(defined(_DLL)) 
#else
	#if !(defined(_DLL))                    // статическая линковка
			#ifdef _DEBUG
                #pragma message("Link with " _LIB_NAME "d.lib") 
				#pragma comment (lib, _LIB_NAME "d.lib")
			#else
                #pragma message("Link with " _LIB_NAME ".lib") 
				#pragma comment (lib, _LIB_NAME ".lib")
			#endif //_DEBUG
	#else                                  // динамическая линковка
			#ifdef _DEBUG
                #pragma message("Link with " _LIB_NAME "d_crtdll.lib") 
				#pragma comment (lib, _LIB_NAME "d_crtdll.lib")
			#else
                #pragma message("Link with " _LIB_NAME "_crtdll.lib") 
				#pragma comment (lib, _LIB_NAME "_crtdll.lib")
			#endif //_DEBUG
	#endif // !(defined(_DLL)) 
#endif // _UNICODE


#undef _LIB_NAME

#endif // !(defined(__linux__) || defined(_LIB))
        

#endif  //TCODECVT
