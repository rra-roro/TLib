#ifdef _MBCS
	#if !(defined(_DLL))                    // статическая линковка
		#ifdef _DEBUG
                #pragma message("Link with " _LIB_NAME "d.lib") 
		    #pragma comment (lib, _LIB_NAME "d.lib")
		#else
                #pragma message("Link with " _LIB_NAME ".lib") 
		    #pragma comment (lib, _LIB_NAME ".lib")w
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
#else
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
#endif // _MBCS
