/*
*   Файл TException.h - содержит макроссы и классы для создания инфраструктуры исключений
*
*   Версия файла 1.0
*
*/

#include "stdafx.h"
#include <TException.h>
#include <TExceptionCPP.h>
#include <TExceptionSEH.h>
#include <objectnativeapi.h>
#include <vector>
#include <Tstring.h>
#include <strsafe.h>


using namespace tlib;
using namespace std;
using NT::NTSTATUS;

////////////////////////////////////////////////////////////////////////
//
//   GetNTStatusMessage -  получает текст сообщения	NTStatus, читая его из NTDLL.DLL
//                         если текст сообщения имеет параметры, их можно заполнить, передав их в
//                         GetNTStatusMessage в порядке их следования в тексте сообщения.

tstring GetNTStatusMessage(DWORD NTStatusMessage, ...)
{
     va_list args = NULL;
    va_start(args, NTStatusMessage);

	HMODULE Hand = GetModuleHandle(_T("NTDLL.DLL"));

	NT::PMESSAGE_RESOURCE_ENTRY mre = 0;
 // RT_MESSAGETABLE - это MAKEINTRESOURCE(11), значит нужно подставить 11
	NT::RtlFindMessage(Hand, 11, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  NTStatusMessage, &mre);
	if(mre!=0)
	{
		USHORT Length = mre->Length/sizeof(wchar_t);
		NT::UnicodeString uMsg((wchar_t*)mre->Text,Length);

		std::vector<wchar_t> ptrMsgArg(Length*2);
		HRESULT hr = StringCbVPrintfW(&ptrMsgArg[0],Length*2*sizeof(wchar_t),uMsg.GetWstring().c_str(),args);

		va_end(args);
		return tstring(wstr2tstr(&ptrMsgArg[0]));
	}else
	{
		va_end(args);
		return tstring();
	}
}

////////////////////////////////////////////////////////////////////////
//
//   GetLastErrorMessage -  получает текст сообщения возвращаемого GetLastError, если текст сообщения 
//                          имеет параметры, их можно заполнить, передав их в GetLastErrorMessage 
//                          в порядке их следования в тексте сообщения.

tstring GetLastErrorMessage(DWORD LastError, ...)
{
  LPWSTR lpMsgBuf = NULL;
  va_list args = NULL;
  va_start(args, LastError);

  FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER |  FORMAT_MESSAGE_FROM_SYSTEM,
                 NULL,LastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                 (LPWSTR)&lpMsgBuf,0,&args);
  tstring errMsg(wstr2tstr(lpMsgBuf));

  LocalFree(lpMsgBuf);
  va_end(args);
  return  errMsg;
}


////////////////////////////////////////////////////////////////////////
//
//   TExceptionSEH::GetExceptionMsg() -  получает текст сообщения для SEH исключения, формируя сообщение 
//                                       с учетом параметров исключения из PEXCEPTION_POINTERS

tstring EExceptionSEH::GetExceptionMsg(DWORD NTStatusMessage, const PEXCEPTION_POINTERS pExcPointers) const
{
	tstring ErrorMsgStr;
	TCHAR* ptrOperationType=0;

	if(pExcPointers->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION ||
	   pExcPointers->ExceptionRecord->ExceptionCode == EXCEPTION_IN_PAGE_ERROR)
	{
		switch (pExcPointers->ExceptionRecord->ExceptionInformation[0])
		{
			case 0:
				ptrOperationType = _T("read");
				break;
			case 1:
				ptrOperationType = _T("write");
				break;
			case 8:
				ptrOperationType = _T("execute (a user-mode data execution prevention (DEP))");
				break;
           default:
                ptrOperationType = _T("");
                break;
		}
	}
     
	switch (pExcPointers->ExceptionRecord->ExceptionCode)
    {
		case EXCEPTION_ACCESS_VIOLATION:
		{
			tstring wNtStMsg = GetNTStatusMessage(NTStatusMessage, pExcPointers->ExceptionRecord->ExceptionAddress,
					                                   pExcPointers->ExceptionRecord->ExceptionInformation[1],
													   ptrOperationType);							                                   
			ErrorMsgStr.assign(wNtStMsg);
		}
		break;

		case EXCEPTION_IN_PAGE_ERROR:
		{

			tstring wNtStMsg = GetNTStatusMessage(NTStatusMessage, pExcPointers->ExceptionRecord->ExceptionAddress,
					                                   pExcPointers->ExceptionRecord->ExceptionInformation[1],
													   pExcPointers->ExceptionRecord->ExceptionInformation[2]);
			wNtStMsg = wNtStMsg + _T(" The memory could not be\"") + ptrOperationType  + _T("\"");
			ErrorMsgStr.assign(wNtStMsg);
		}
		break;

		default:
		{
			tstring wNtStMsg = GetNTStatusMessage(NTStatusMessage);
			ErrorMsgStr.assign(wNtStMsg);
		}
		break;
	}
	return ErrorMsgStr;
}

tstring EExceptionSEH::GetNameSEHCode() const
{
	switch (m_nSeCode)
	{
			case EXCEPTION_ACCESS_VIOLATION:
				return tstring(_T("EXCEPTION_ACCESS_VIOLATION"));
				break;
			case EXCEPTION_DATATYPE_MISALIGNMENT:
				return tstring(_T("EXCEPTION_DATATYPE_MISALIGNMENT"));
				break;
			case EXCEPTION_BREAKPOINT:
				return tstring(_T("EXCEPTION_BREAKPOINT"));
				break;
			case EXCEPTION_SINGLE_STEP:
				return tstring(_T("EXCEPTION_SINGLE_STEP"));				
				break;
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
				return tstring(_T("EXCEPTION_ARRAY_BOUNDS_EXCEEDED"));				
				break;
			case EXCEPTION_FLT_DENORMAL_OPERAND:
				return tstring(_T("EXCEPTION_FLT_DENORMAL_OPERAND"));
				break;
			case EXCEPTION_FLT_DIVIDE_BY_ZERO:
				return tstring(_T("EXCEPTION_FLT_DIVIDE_BY_ZERO"));
				break;
			case EXCEPTION_FLT_INEXACT_RESULT:
				return tstring(_T("EXCEPTION_FLT_INEXACT_RESULT"));
				break;
			case EXCEPTION_FLT_INVALID_OPERATION:
				return tstring(_T("EXCEPTION_FLT_INVALID_OPERATION"));
				break;
			case EXCEPTION_FLT_OVERFLOW:
				return tstring(_T("EXCEPTION_FLT_OVERFLOW"));
				break;
			case EXCEPTION_FLT_STACK_CHECK:
				return tstring(_T("EXCEPTION_FLT_STACK_CHECK"));
				break;
			case EXCEPTION_INT_DIVIDE_BY_ZERO:
				return tstring(_T("EXCEPTION_INT_DIVIDE_BY_ZERO"));
				break;
			case EXCEPTION_INT_OVERFLOW:
				return tstring(_T("EXCEPTION_INT_OVERFLOW"));
				break;
			case EXCEPTION_PRIV_INSTRUCTION:
				return tstring(_T("EXCEPTION_PRIV_INSTRUCTION"));
				break;
			case EXCEPTION_IN_PAGE_ERROR:
				return tstring(_T("EXCEPTION_IN_PAGE_ERROR"));
				break;
			case EXCEPTION_ILLEGAL_INSTRUCTION:
				return tstring(_T("EXCEPTION_ILLEGAL_INSTRUCTION"));
				break;
			case EXCEPTION_NONCONTINUABLE_EXCEPTION:
				return tstring(_T("EXCEPTION_NONCONTINUABLE_EXCEPTION"));
				break;
			case EXCEPTION_STACK_OVERFLOW:
				return tstring(_T("EXCEPTION_STACK_OVERFLOW"));
				break;
			case EXCEPTION_INVALID_DISPOSITION:
				return tstring(_T("EXCEPTION_INVALID_DISPOSITION"));
				break;
			case EXCEPTION_GUARD_PAGE:
				return tstring(_T("EXCEPTION_GUARD_PAGE"));
				break;
			case EXCEPTION_INVALID_HANDLE:
				return tstring(_T("EXCEPTION_INVALID_HANDLE"));
				break;
			case EXCEPTION_POSSIBLE_DEADLOCK:
				return tstring(_T("EXCEPTION_POSSIBLE_DEADLOCK"));
				break;

           default:
                return tstring(_T(""));
                break;
	}
};

tstring EExceptionSEH::GetContextRecordMsg() const
{
	tstringstream ContextRecordMsg;
	ContextRecordMsg << showbase << uppercase << setfill(_T('0'));

	ContextRecordMsg << _T("ContextFlags=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->ContextFlags,16) << _T("\n");
	if(m_pExcPointers->ContextRecord->ContextFlags & CONTEXT_DEBUG_REGISTERS)
	{
			ContextRecordMsg << _T("Dr0=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Dr0,16) << _T("\n");
			ContextRecordMsg << _T("Dr1=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Dr1,16) << _T("\n");
			ContextRecordMsg << _T("Dr2=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Dr2,16) << _T("\n");
			ContextRecordMsg << _T("Dr3=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Dr3,16) << _T("\n");
			ContextRecordMsg << _T("Dr6=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Dr6,16) << _T("\n");
			ContextRecordMsg << _T("Dr7=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Dr7,16) << _T("\n");
	}	
#ifdef _M_IX86	
	ContextRecordMsg << _T("Eax=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Eax,16) << _T("\n");
	ContextRecordMsg << _T("Ebx=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Ebx,16) << _T("\n");
	ContextRecordMsg << _T("Ecx=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Ecx,16) << _T("\n");
	ContextRecordMsg << _T("Edx=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Edx,16) << _T("\n");
	ContextRecordMsg << _T("Edi=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Edi,16) << _T("\n");
	ContextRecordMsg << _T("Esi=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Esi,16) << _T("\n");
	ContextRecordMsg << _T("Ebp=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Ebp,16) << _T("\n");
	ContextRecordMsg << _T("Esp=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Esp,16) << _T("\n");
	ContextRecordMsg << _T("Eip=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Eip,16) << _T("\n");
	ContextRecordMsg << _T("EFlags=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->EFlags,16) << _T("\n");
	ContextRecordMsg << _T("SegCs=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->SegCs,16) << _T("\n");				
	ContextRecordMsg << _T("SegDs=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->SegDs,16) << _T("\n");				
	ContextRecordMsg << _T("SegEs=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->SegEs,16) << _T("\n");
	ContextRecordMsg << _T("SegFs=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->SegFs,16) << _T("\n");
	ContextRecordMsg << _T("SegGs=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->SegGs,16) << _T("\n");
	ContextRecordMsg << _T("SegSs=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->SegSs,16) << _T("\n");

	if(m_pExcPointers->ContextRecord->ContextFlags & CONTEXT_EXTENDED_REGISTERS)
			ContextRecordMsg << _T("ExtendedRegisters1=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->ExtendedRegisters[0],16) << _T("\n");

#elif defined(_M_AMD64)
	ContextRecordMsg << _T("Rax=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Rax,16) << _T("\n");
	ContextRecordMsg << _T("Rbx=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Rbx,16) << _T("\n");
	ContextRecordMsg << _T("Rcx=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Rcx,16) << _T("\n");
	ContextRecordMsg << _T("Rdx=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Rdx,16) << _T("\n");
	ContextRecordMsg << _T("Rdi=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Rdi,16) << _T("\n");
	ContextRecordMsg << _T("Rsi=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Rsi,16) << _T("\n");
	ContextRecordMsg << _T("Rbp=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Rbp,16) << _T("\n");
	ContextRecordMsg << _T("Rsp=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Rsp,16) << _T("\n");
	ContextRecordMsg << _T("Rip=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->Rip,16) << _T("\n");

	ContextRecordMsg << _T("R8=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->R8,16) << _T("\n");
	ContextRecordMsg << _T("R9=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->R9,16) << _T("\n");
	ContextRecordMsg << _T("R10=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->R10,16) << _T("\n");
	ContextRecordMsg << _T("R11=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->R11,16) << _T("\n");
	ContextRecordMsg << _T("R12=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->R12,16) << _T("\n");
	ContextRecordMsg << _T("R13=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->R13,16) << _T("\n");
	ContextRecordMsg << _T("R14=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->R14,16) << _T("\n");
	ContextRecordMsg << _T("R15=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->R15,16) << _T("\n");

	ContextRecordMsg << _T("EFlags=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->EFlags,16) << _T("\n");
	ContextRecordMsg << _T("SegCs=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->SegCs,16) << _T("\n");				
	ContextRecordMsg << _T("SegDs=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->SegDs,16) << _T("\n");				
	ContextRecordMsg << _T("SegEs=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->SegEs,16) << _T("\n");
	ContextRecordMsg << _T("SegFs=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->SegFs,16) << _T("\n");
	ContextRecordMsg << _T("SegGs=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->SegGs,16) << _T("\n");
	ContextRecordMsg << _T("SegSs=") << setw(8) << ConversionRadix(m_pExcPointers->ContextRecord->SegSs,16) << _T("\n");
#else
	#error It is necessary ContextRecord for this CPU 
#endif

	return ContextRecordMsg.str();
}
