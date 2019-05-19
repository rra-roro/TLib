/*
*   Файл TException.h - содержит макроссы и классы для создания инфраструктуры исключений
*
*/

#ifndef TEXCEPTION
#define TEXCEPTION

#include <exception>
#include <Tstring.h>

////////////////////////////////////////////////////////////////////////
//
//   GetNTStatusMessage -  получает текст сообщения	NTStatus, читая его из NTDLL.DLL
//                         если текст сообщения имеет параметры, их можно заполнить, передав их в
//                         GetNTStatusMessage в порядке их следования в тексте сообщения.
Tstring::tstring GetNTStatusMessage(DWORD NTStatusMessage, ...);


////////////////////////////////////////////////////////////////////////
//
//   GetLastErrorMessage -  получает текст сообщения возвращаемого GetLastError, если текст сообщения 
//                          имеет параметры, их можно заполнить, передав их в GetLastErrorMessage 
//                          в порядке их следования в тексте сообщения.
Tstring::tstring GetLastErrorMessage(DWORD LastError, ...);


////////////////////////////////////////////////////////////////////////
//
//   _E(str) -  Макрос принимающий текст исключения и дополняющий его строкой и именем исходного файла
//

#define _E(str) _T(##__FILE__##)##,##__LINE__##,##str


////////////////////////////////////////////////////////////////////////
//
//   EExceptionCPP -  Базовый класс всех исключений этой библиотеки
//

class EException: public std::exception
{		
public:
	virtual DWORD GetErrorCode(void) const =0;                          // Возвращает код ошибки, если он существует      
	virtual const Tstring::tstring GetMsgOfErrorCode() const=0;         // Возвращает сообщение, связанное с кодом ошибки 
	
	virtual const Tstring::tstring GetExceptionClassName(void) const=0; // Возвращает истинное имя класса исключения      

	virtual const Tstring::tstring GetUserErrorText() const=0;          // Возвращает текст сообщения, которое было задано 
	                                                                    // пользователем при возбуждении исключения       
    
	virtual const Tstring::tstring GetExceptionAllInfoMsg() const=0;    // Возвращает всю информацию об исключении        
	virtual const char* what() const=0;                                 // Унаследованная ф-ия, которая в нашем случае будет 
	                                                                    // возвращать, то же что и GetExceptionAllInfoMsg()
};

#if !(defined(_LIB))

#define _LIB_NAME "TException"

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

#endif // !(defined(_LIB))

#endif  //TEXCEPTION