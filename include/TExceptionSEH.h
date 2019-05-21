/*
*   Файл TExceptionSEH.h - содержит макроссы и классы для работы с SEH исключениями,
*                          как будто они С++ исключения 
*/

#ifndef TEXCEPTION_SEH
#define TEXCEPTION_SEH

#include <windows.h>
#include <eh.h>
#include <TException.h>
#include <Tstring.h>
#include <Tiomanip.h>
#include <Tstdlib.h>

////////////////////////////////////////////////////////////////////////
//
//   class EExceptionSEH  -  Базовый класс для SEH исключений, который содержит всю ф-ость, 
//                           для получения информации о SEH исключении.
//                         
//                           При возбуждении копируется, а оригинал будет удален
//                           Копирующий конструктор по умолчанию должен все корректрно скопировать, так как
//                           у класса всего два POD члена 

class EExceptionSEH : public EException
{
	unsigned int m_nSeCode;
	PEXCEPTION_POINTERS m_pExcPointers;
	std::string m_WhatMsg;
	
	tlib::tstring GetExceptionMsg(DWORD NTStatusMessage, const PEXCEPTION_POINTERS pExcPointers) const;

public:
	// Конструкторы
	   EExceptionSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):m_nSeCode(nSeCode),m_pExcPointers(pExcPointers){};

	// Ф-ии специфичные только для этого класса
	   UINT GetSEHCode(void) const                  // Возвращает номер исключения SEH
 	   {
		   return m_nSeCode;
	   }

	   tlib::tstring GetNameSEHCode() const;            // Возвращает имя исключения SEH
	   
	   const PEXCEPTION_POINTERS GetExceptionPtr(void) const
	   {
		   return m_pExcPointers;
	   }

	   tlib::tstring GetContextRecordMsg() const;


	// Замещение виртуальных ф-ий базового класса
	   virtual DWORD GetErrorCode(void) const        // Возвращает номер исключения SEH
 	   {
		   return m_nSeCode;
	   }

	   virtual const tlib::tstring GetMsgOfErrorCode() const
	   {
		   return GetExceptionMsg(m_nSeCode, m_pExcPointers);
	   }

	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EExceptionSEH"));
	   }

	   virtual const tlib::tstring GetUserErrorText() const
	   {
		   return tlib::tstring();
	   }

	   virtual const tlib::tstring GetExceptionAllInfoMsg() const
	   {
		   tlib::tstringstream Msg;
		   Msg << tlib::showbase << tlib::uppercase << tlib::tsetfill(_T('0'));
		   
		   Msg << _T("Error code: ") << GetNameSEHCode() << tlib::setw(8) << _T(" (") << tlib::ConversionRadix(GetSEHCode(),16) << _T(") \n");
		   Msg << _T("Error message: ") << GetMsgOfErrorCode() << _T("\n");
		   Msg << _T("ContextRecord: ") << GetContextRecordMsg() << _T("\n");
		   return Msg.str();
	   };

	   virtual const char* what() const
	   {
		   // Снимим константность и изменим m_WhatMsg
		   const_cast<EExceptionSEH *>(this )->m_WhatMsg = tstr2str(GetExceptionAllInfoMsg());
		   return m_WhatMsg.c_str();
	   }


	// Ф-ия вызываемая транслятором SEH исключений, реализована ниже, послеобъявления
	// производных классов, определяет какой из производных класов будет создан, для обработки
	// исключения.
	static void sehTransFunction(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers );
};

////////////////////////////////////////////////////////////////////////
//
//   Объявим производные классы на каждый тип SEH исключения, для диференцированной обработки
//   каждого типа исключения в отдельном catch() обработчике. 
//   Также объявим промежуточные базовые классы для групировки SEH исключений по типу.
//

//   ==============================================================================
//   EMemoryAccessSEH   - base class - Все исключения с ошибкой доступа к памяти

class EMemoryAccessSEH :public EExceptionSEH 
{
	public:
		EMemoryAccessSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EExceptionSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EMemoryAccessSEH"));
	   }
};

//   Попытка потока читать или писать по виртуальному адресу, к которому у него нет 
//   подходящего доступа
class EAccessViolationSEH :public EMemoryAccessSEH 
{
	public:
		EAccessViolationSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EMemoryAccessSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EAccessViolationSEH"));
	   }
};

//   Попытка потока иметь доступ к элементам за границей масива в случае хардверной подержки
//   проверки границ массива
class EArrayBoundsExceededSEH :public EMemoryAccessSEH 
{
	public:
		EArrayBoundsExceededSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EMemoryAccessSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EArrayBoundsExceededSEH"));
	   }
};

//   Попытка потока читать и писать данные, что не были выровнены 
//   по границе слова или двойного слова
class EDataMisAlignmentSEH:public EMemoryAccessSEH 
{
	public:
		EDataMisAlignmentSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EMemoryAccessSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EDataMisAlignmentSEH"));
	   }
};

//   Попытка потока обратится к отсутствующей странице, и система не смогла загрузить страницу 
//   Например: Такое может произойти при потере сетевого соединения пока программа запускалась поверх сети.
class EInPageErrorSEH :public EMemoryAccessSEH 
{
	public:
		EInPageErrorSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EMemoryAccessSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EInPageErrorSEH"));
	   }
};

//   Переполнился стек
class EStackOverflowSEH :public EMemoryAccessSEH 
{
	public:
		EStackOverflowSEH (unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EMemoryAccessSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EStackOverflowSEH"));
	   }
};

//    Попытка обратится к странице защищенной PageGuard
class EGuardPageSEH :public EMemoryAccessSEH 
{
	public:
		EGuardPageSEH (unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EMemoryAccessSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EGuardPageSEH"));
	   }
};

//   ==============================================================================
//   ESystemObjectSEH   - base class - Все исключения связанные с Системными объектами

class ESystemObjectSEH :public EExceptionSEH 
{
	public:
		ESystemObjectSEH (unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EExceptionSEH (nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("ESystemObjectSEH"));
	   }
};
//   Поток использовал handle на kernel object который был инвалидный 
//   (Возможно потому что объект быз закрыт. Может генерироваться при создании критической секции.)
class EInvalidHandleSEH :public ESystemObjectSEH 
{
	public:
		EInvalidHandleSEH (unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																ESystemObjectSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EInvalidHandleSEH"));
	   }
};

// Исключение EXCEPTION_POSSIBLE_DEADLOCK может быть сгенерировано, 
// если окончится время ожидания в critical section
class EPossibleDeadlockSEH :public ESystemObjectSEH 
{
	public:
		EPossibleDeadlockSEH (unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																ESystemObjectSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EPossibleDeadlockSEH"));
	   }
};

//   ==============================================================================
//   EInstructionSEH   - base class - Все исключения связанные с выполнением не допустимых инструкций
class EInstructionSEH :public EExceptionSEH 
{
	public:
		EInstructionSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EExceptionSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EInstructionSEH"));
	   }
};

// Попытка выполнить привелигированную инструкцию
class EPrivilegedInstructionSEH :public EInstructionSEH 
{
	public:
		EPrivilegedInstructionSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EInstructionSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EPrivilegedInstructionSEH"));
	   }
};

// Попытка выполнить нелегальную инструкцию
class EIllegalInstructionSEH :public EInstructionSEH
{
	public:
		EIllegalInstructionSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EInstructionSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EIllegalInstructionSEH"));
	   }
};

//   ==============================================================================
//   EDebugExceptionSEH   - base class - Все исключения связанные с отладкой

class EDebugExceptionSEH :public EExceptionSEH 
{
	public:
		EDebugExceptionSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EExceptionSEH (nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EDebugExceptionSEH"));
	   }
};

//   Был встречен Breakpoint
class EBreakpointSEH :public EDebugExceptionSEH 
{
	public:
		EBreakpointSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EDebugExceptionSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EBreakpointSEH"));
	   }
};
//   При по шаговой трасировке сигнализирует, что одна инструкция была выполнена
class ESingleStepSEH :public EDebugExceptionSEH 
{
	public:
		ESingleStepSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EDebugExceptionSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("ESingleStepSEH"));
	   }
};

//   ==============================================================================
//   EInternalExceptionSEH   - base class - Все исключения возбуждаемые системой в неправильных обработчиках
//							                исключений
class EInternalExceptionSEH :public EExceptionSEH 
{
	public:
		EInternalExceptionSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EExceptionSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EInternalExceptionSEH"));
	   }
};

// Поток пытается продолжить выполнение после того как, случилось noncontinuable исключение
class ENonContinuableExceptionSEH :public EInternalExceptionSEH 
{
	public:
		ENonContinuableExceptionSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EInternalExceptionSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("ENonContinuableExceptionSEH"));
	   }
};

// Обработчик исключения вернул инвалидное расположение exception dispatcher. 
// Программа написанная на высокоуровневых языках таких как C никогда не должна получить такое исключение.
class EInvalidDispositionSEH :public EInternalExceptionSEH 
{
	public:
		EInvalidDispositionSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EInternalExceptionSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EInvalidDispositionSEH"));
	   }
};

//   ==============================================================================
//   EIntegerOperationSEH   - base class - Все исключения связанные с целочисленными операциями

class EIntegerOperationSEH :public EExceptionSEH 
{
	public:
		EIntegerOperationSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EExceptionSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EIntegerOperationSEH"));
	   }
};

// Целочисленное деление на ноль
class EIntDivideByZeroSEH :public EIntegerOperationSEH
{
	public:
		EIntDivideByZeroSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EIntegerOperationSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EIntDivideByZeroSEH"));
	   }
};

// Целочисленное переполнение
class EIntOverflowSEH :public EIntegerOperationSEH
{
	public:
		EIntOverflowSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EIntegerOperationSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EIntOverflowSEH"));
	   }
};

//   ==============================================================================
//   EFloatOperationSEH   - base class - Все исключения связанные с отладкой

class EFloatOperationSEH :public EExceptionSEH 
{
	public:
		EFloatOperationSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EExceptionSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EFloatOperationSEH"));
	   }
};

class EFltDenormalOperandSEH :public EFloatOperationSEH 
{
	public:
		EFltDenormalOperandSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EFloatOperationSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EFltDenormalOperandSEH"));
	   }
};

class EFltDivideByZeroSEH :public EFloatOperationSEH 
{
	public:
		EFltDivideByZeroSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EFloatOperationSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EFltDivideByZeroSEH"));
	   }
};

class EFltInexactResultSEH :public EFloatOperationSEH 
{
	public:
		EFltInexactResultSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EFloatOperationSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EFltInexactResultSEH"));
	   }
};

class EFltInvalidOperationSEH :public EFloatOperationSEH 
{
	public:
		EFltInvalidOperationSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EFloatOperationSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EFltInvalidOperationSEH"));
	   }
};

class EFltOverflowSEH :public EFloatOperationSEH 
{
	public:
		EFltOverflowSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EFloatOperationSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EFltOverflowSEH"));
	   }
};

class EFltStackCheckSEH :public EFloatOperationSEH 
{
	public:
		EFltStackCheckSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EFloatOperationSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EFltStackCheckSEH"));
	   }
};

class EFltUnderflowSEH :public EFloatOperationSEH 
{
	public:
		EFltUnderflowSEH(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers):
																EFloatOperationSEH(nSeCode, pExcPointers){};
	   virtual const tlib::tstring GetExceptionClassName(void) const
	   {
		   return tlib::tstring(_T("EFltUnderflowSEH"));
	   }
};


// Реализация ф-ии вызываемой транслятором SEH исключений, реализована 
// Определяет какой из производных класов будет создан, для обработки
// исключения.

inline void EExceptionSEH::sehTransFunction(unsigned int nSeCode, PEXCEPTION_POINTERS pExcPointers )
{
	switch (nSeCode)
	{
			case EXCEPTION_ACCESS_VIOLATION:
				throw EAccessViolationSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_DATATYPE_MISALIGNMENT:
				throw EDataMisAlignmentSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_BREAKPOINT:
				throw EBreakpointSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_SINGLE_STEP:
				throw ESingleStepSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
				throw EArrayBoundsExceededSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_FLT_DENORMAL_OPERAND:
				throw EFltDenormalOperandSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_FLT_DIVIDE_BY_ZERO:
				throw EFltDivideByZeroSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_FLT_INEXACT_RESULT:
				throw EFltInexactResultSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_FLT_INVALID_OPERATION:
				throw EFltInvalidOperationSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_FLT_OVERFLOW:
				throw EFltOverflowSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_FLT_STACK_CHECK:
				throw EFltStackCheckSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_INT_DIVIDE_BY_ZERO:
				throw EIntDivideByZeroSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_INT_OVERFLOW:
				throw EIntOverflowSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_PRIV_INSTRUCTION:
				throw EPrivilegedInstructionSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_IN_PAGE_ERROR:
				throw EInPageErrorSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_ILLEGAL_INSTRUCTION:
				throw EIllegalInstructionSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_NONCONTINUABLE_EXCEPTION:
				throw ENonContinuableExceptionSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_STACK_OVERFLOW:
				throw EStackOverflowSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_INVALID_DISPOSITION:
				throw EInvalidDispositionSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_GUARD_PAGE:
				throw EGuardPageSEH(nSeCode, pExcPointers);
				break;
			case EXCEPTION_INVALID_HANDLE:
				throw EInvalidHandleSEH(nSeCode, pExcPointers);
				break;

#ifndef STATUS_POSSIBLE_DEADLOCK
#define STATUS_POSSIBLE_DEADLOCK         ((DWORD   )0xC0000194L)

			case EXCEPTION_POSSIBLE_DEADLOCK:

#undef STATUS_POSSIBLE_DEADLOCK
#else
			case EXCEPTION_POSSIBLE_DEADLOCK:

#endif
				throw EPossibleDeadlockSEH(nSeCode, pExcPointers);
				break;

           default:
                throw EExceptionSEH(nSeCode, pExcPointers);
                break;
	}
};



// Для установки транслятора в начале ф-ии разместите запись подобну этой
// TSet_Se_Translator<TExceptionSEH> SEHTranslator;

template<class E> class TSet_Se_Translator
{
	_se_translator_function OldTranslatorFn;
	public:
		TSet_Se_Translator(): OldTranslatorFn(0)
		{
			OldTranslatorFn  = _set_se_translator((_se_translator_function)E::sehTransFunction);
		};
		~TSet_Se_Translator()
		{
			_set_se_translator(OldTranslatorFn);
		};
};





#endif  //TEXCEPTION_SEH
