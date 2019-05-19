/*
*   Файл TExceptionCPP.h - содержит макроссы и классы для работы с С++ исключениями
*
*/

#ifndef TEXCEPTION_CPP
#define TEXCEPTION_CPP

#include <TException.h>
#include <Tstring.h>
#include <Tiomanip.h>
#include <Tstdlib.h>

using namespace Tstring;

////////////////////////////////////////////////////////////////////////
//
//   EExceptionCPP -  Базовый класс для всех С++ исключений. 
//                    Описывает простое текстовое пользовательское сообщение об ошибке,
//                    а также Имя Файла исходника и Номер строки, где исключение произошло,
//                    так же возможна передача кода ошибки.
//                    Код ошибки - это код возвращаемый ф-ией GetLastError().
//
//                    Используется совместно с макросом #define _E(str)
//
//                    Например так:  
//                                   throw EExceptionCPP(_E("А тут у нас ошибка"));
//                                   throw EExceptionCPP(_E("А тут у нас ошибка. Номер:"), 10);

class EExceptionCPP: public EException
{	    
        const Tstring::tstring m_ErrMsg;
        const TCHAR* m_FileName;
        int m_lineNumber;
        DWORD m_LastError;

        std::string m_WhatMsg;

public:
	// Конструкторы
        EExceptionCPP(const TCHAR* FileName, int lineNumber, const Tstring::tstring& ErrMsg)
                     : m_ErrMsg(ErrMsg),m_FileName(FileName),m_lineNumber(lineNumber),m_LastError(0){};

        EExceptionCPP(const TCHAR* FileName, int lineNumber, const Tstring::tstring& ErrMsg, DWORD LastError)
                     : m_ErrMsg(ErrMsg),m_FileName(FileName),m_lineNumber(lineNumber),m_LastError(LastError){};

        EExceptionCPP(const TCHAR* FileName, int lineNumber, const TCHAR* ErrMsg)
                     : m_ErrMsg(ErrMsg),m_FileName(FileName),m_lineNumber(lineNumber),m_LastError(0){};

        EExceptionCPP(const TCHAR* FileName, int lineNumber, const TCHAR* ErrMsg, DWORD LastError)
                     : m_ErrMsg(ErrMsg),m_FileName(FileName),m_lineNumber(lineNumber),m_LastError(LastError){};

	// Замещение виртуальных ф-ий базового класса
	   virtual DWORD GetErrorCode(void) const
	   {
		   return m_LastError;
	   }

	   virtual const Tstring::tstring GetMsgOfErrorCode() const
	   {
		   if(GetErrorCode()!=0) return GetLastErrorMessage(GetErrorCode());
           return Tstring::tstring();
	   }

	   virtual const Tstring::tstring GetUserErrorText() const
	   {
		   return m_ErrMsg;
	   }

	   virtual const Tstring::tstring GetExceptionClassName(void) const
	   {
		   return Tstring::tstring(_T("EExceptionCPP"));
	   }

	   virtual const Tstring::tstring GetExceptionAllInfoMsg() const
	   {
		   Tstring::tstringstream Msg;
		   Msg << Tstring::showbase << Tstring::uppercase << Tstring::tsetfill(_T('0'));
		   		
		   Msg << _T("Last Error: ");
		   Msg << Tstring::setw(8) << Tstring::ConversionRadix(GetErrorCode(), 16) << _T("\n");
		   
		   Msg << _T("Source FileName: ");
		   Msg << m_FileName << _T(":(") << std::dec << m_lineNumber << _T(")") <<_T("\n");
		   
		   Msg << _T("Error message: ");
		   Msg <<  GetUserErrorText();
		   Msg << _T(" ") << GetMsgOfErrorCode();
		   Msg << _T("\n");
		   
		   return Msg.str();
	   }
	   
	   virtual const char* what() const
	   {
		   // Снимим константность и изменим m_WhatMsg
		   const_cast<EExceptionCPP *>(this )->m_WhatMsg = tstr2str(GetExceptionAllInfoMsg());
		   return m_WhatMsg.c_str();
	   }
};

////////////////////////////////////////////////////////////////////////
//
//   Объявим исключения производные от EExceptionCPP.
//   Эти исключения описывают общие ошибки, которые могут случиться в пользовательском коде
//                                   

class EBadLoadDLL:public EExceptionCPP
{
public:
	EBadLoadDLL(const TCHAR* FileName, int lineNumber, const Tstring::tstring& ModuleName, DWORD LastError=GetLastError()):
		    EExceptionCPP(FileName,lineNumber,_T("Module \"") + ModuleName + _T("\" can't be loaded."),LastError){};

    virtual const Tstring::tstring GetExceptionClassName(void) const
	{
		return Tstring::tstring(_T("EBadLoadDLL"));
	}
};


class EBadPtrFnDLL:public EExceptionCPP
{
public:
	EBadPtrFnDLL(const TCHAR* FileName, int lineNumber, const Tstring::tstring& ModuleName, const Tstring::tstring& FnName):
		     EExceptionCPP(FileName,lineNumber, 
                           _T("Function \"") + FnName + _T("\" is absent in \"") + ModuleName + _T("\". Possibly this module has an incorrect version."),
			               ERROR_PROC_NOT_FOUND){};

	virtual const Tstring::tstring GetExceptionClassName(void) const
	{
		return Tstring::tstring(_T("EBadPtrFnDLL"));
	}
};

class EBadRefArg:public EExceptionCPP
{
public:
	EBadRefArg(const TCHAR* FileName, int lineNumber, const Tstring::tstring& FnName, const Tstring::tstring& ArgName):
		     EExceptionCPP(FileName,lineNumber,
                                   _T("Argument \"") + ArgName + _T("\" of the function \"") + FnName + _T("\" refers to a zero address."), 
			           ERROR_BAD_ARGUMENTS){};

	virtual const Tstring::tstring GetExceptionClassName(void) const
	{
		return Tstring::tstring(_T("EBadRefArg"));
	}
};

class EBadPtrArg:public EExceptionCPP
{
public:
	EBadPtrArg(const TCHAR* FileName, int lineNumber, const Tstring::tstring& FnName, const Tstring::tstring& ArgName):
		     EExceptionCPP(FileName,lineNumber,
                                   _T("Argument \"") + ArgName + _T("\" of the function \"") + FnName + _T("\" is the null pointer."), 
			           ERROR_BAD_ARGUMENTS){};

	virtual const Tstring::tstring GetExceptionClassName(void) const
	{
		return Tstring::tstring(_T("EBadPtrArg"));
	}
};

class EFileNotFound:public EExceptionCPP
{
public:
	EFileNotFound(const TCHAR* FileName, int lineNumber, const Tstring::tstring& ModuleName):
		      EExceptionCPP(FileName,lineNumber, _T("\"") + ModuleName + _T("\" is not found."),ERROR_MOD_NOT_FOUND){};

	virtual const Tstring::tstring GetExceptionClassName(void) const
	{
		return Tstring::tstring(_T("EFileNotFound"));
	}
};

////////////////////////////////////////////////////////////////////////
//
//   SafeInitMemberByRef(Ref,Fn,TypeArg) -  Макрос позволяющий безопасно инициализировать член класса в списке 
//                                          инициализации конструктора через ссылку. Если у нас нулевая ссылка,
//                                          то мы генерируем исключение EBadRefArg
//

template <typename T>
T& TSafeInitMemberByRef(T& RefArg,const TCHAR* FileName, int lineNumber, const TCHAR* FnName, const TCHAR* ArgName)
{
  if(&RefArg==0) throw EBadRefArg(FileName, lineNumber, FnName, ArgName);
  return RefArg;
}

#define SafeInitMemberByRef(Ref,Fn,TypeArg)  TSafeInitMemberByRef(##Ref##,_E(_T(##Fn##)), _T(##TypeArg##))


////////////////////////////////////////////////////////////////////////
//
//   EWinApiError -  класс исключений для WinApi 
//

class EWinApiError:public EExceptionCPP
{
public:
     EWinApiError(const TCHAR* FileName, 
                  int lineNumber, 
                  const Tstring::tstring& FnName, 
                  DWORD RetValue, 
                  DWORD LastError,
                  const Tstring::tstring& Msg):
              EExceptionCPP(FileName,lineNumber, Msg + _T(" Function \"") + FnName + _T("\" has returned \"") + Tstring::GetStrOfDig(RetValue,16) + _T("\"."), LastError){};

     EWinApiError(const TCHAR* FileName, 
                  int lineNumber, 
                  const Tstring::tstring& FnName, 
                  PVOID RetValue, 
                  DWORD LastError,
                  const Tstring::tstring& Msg):
		      EExceptionCPP(FileName,lineNumber, Msg + _T(" Function \"") + FnName + _T("\" has returned \"") + Tstring::GetStrOfDig(RetValue,16) + _T("\"."), LastError){};

	EWinApiError(const TCHAR* FileName, 
                 int lineNumber, 
                 const Tstring::tstring& FnName, 
                 const TCHAR* RetValue, 
                 DWORD LastError,
                 const Tstring::tstring& Msg):
		      EExceptionCPP(FileName,lineNumber, Msg + _T(" Function \"") + FnName + _T("\" has returned \"") + RetValue + _T("\"."), LastError){};

    EWinApiError(const TCHAR* FileName, 
                 int lineNumber, 
                 const Tstring::tstring& FnName, 
                 DWORD RetValue, 
                 DWORD LastError):
              EExceptionCPP(FileName,lineNumber, _T("Function \"") + FnName + _T("\" has returned \"") + Tstring::GetStrOfDig(RetValue,16) + _T("\"."), LastError){};

    EWinApiError(const TCHAR* FileName, 
                 int lineNumber, 
                 const Tstring::tstring& FnName, 
                 PVOID RetValue, 
                 DWORD LastError):
		      EExceptionCPP(FileName,lineNumber, _T("Function \"") + FnName + _T("\" has returned \"") + Tstring::GetStrOfDig(RetValue,16) + _T("\"."), LastError){};

	EWinApiError(const TCHAR* FileName, 
                 int lineNumber, 
                 const Tstring::tstring& FnName, 
                 const TCHAR* RetValue, 
                 DWORD LastError):
		      EExceptionCPP(FileName,lineNumber, _T("Function \"") + FnName + _T("\" has returned \"") + RetValue + _T("\"."), LastError){};

	virtual const Tstring::tstring GetExceptionClassName(void) const
	{
		return Tstring::tstring(_T("EWinApiError"));
	}
};



//
//    checkEWinApiError - класс функтор, преобразующий результат работы WinApi ф-ии, 
//                        возвращающей тип BOOL,  в исключение EWinApiError
//
class checkEWinApiError
{
        const TCHAR* m_FileName;
        int m_lineNumber;
        const TCHAR* m_FnName;
        const TCHAR* m_Msg;
public:
        // В конструкторе нельзя использовать tstring так, как строки будут создаваться даже, когда 
        // Исключение не происходит. Только const TCHAR*.
        checkEWinApiError(const TCHAR* FileName, 
                       int lineNumber, 
                       const TCHAR* FnName, 
                       const TCHAR* Msg=NULL
                       ):m_Msg(Msg),m_FnName(FnName),m_lineNumber(lineNumber),m_FileName(FileName){};                 

        void operator=(BOOL bResult)
        {
                if(bResult==FALSE)
                {   
                        Tstring::tstring FnName;
                        if (m_FnName!=0)FnName.assign(m_FnName);
                        if(m_Msg==0)
                           throw EWinApiError(m_FileName, m_lineNumber, FnName, _T("FALSE"), GetLastError());
                        else 
                           throw EWinApiError(m_FileName, m_lineNumber, FnName, _T("FALSE"), GetLastError(), m_Msg);
                }
        }
};

#endif  //TEXCEPTION_CPP