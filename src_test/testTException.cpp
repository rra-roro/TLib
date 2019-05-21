// testTException.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <Windows.h>
#include <TException.h>
#include <TExceptionCPP.h>
#include <TExceptionSEH.h>

		class Foo
		{
		};

		class TempClass:Foo
		{
		public:
			TempClass()
			{
				throw EExceptionCPP(_E(_T("Ошибка в конструкторе")));
			}
		};


//using namespace std;

#include <exception>
#include <stdio.h>
using namespace std;
using namespace Tstring;
void my_unexpected_handler()
{
	throw bad_exception();
}

#pragma warning( disable : 4297 )
void promise_breaker() throw()
{
	throw 1;
}

int _tmain(int argc, TCHAR* argv[])
{
        TSet_Se_Translator<EExceptionSEH> SEHTranslator;

        try
        {
             // DWORD i=0;
             // DWORD j=1/i;
			DWORD *ptr=(DWORD*)1;
			  *ptr=1;
        }
		catch(EAccessViolationSEH& e)
		{
			MessageBox(0,e.GetExceptionAllInfoMsg().c_str(),_T("yetryer"),MB_OK);
		}
        catch(EExceptionSEH& e)
        {
//                e.m_pExcPointers;
//                UINT qq = e.m_nSeCode;
			MessageBox(0,e.GetExceptionAllInfoMsg().c_str(),_T("yetryer"),MB_OK);
        }
        catch(...)
        {
				MessageBox(0,_T("catch(...)"),_T("yetryer"),MB_OK);
        }
//////////////////////////////
		set_unexpected(my_unexpected_handler);
		try
		{
			promise_breaker();
		}
		catch(bad_exception &)
		{
			printf("Busted!");
		}
		catch(int &i)
		{
			printf("Escaped! %d",i);
		}

		catch(...)
		{
			printf("Escaped!");
		}
//////////////////////////////
		try
		{
			throw 1;
		}
		catch(EExceptionCPP &i)
		{
			(void)i;
			try
			{
				throw 1;
			}
			catch(int &i)
			{
				(void)i;
			}	
		}
		catch(...)
		{
			printf("Escaped!");
		}
//////////////////////////////
		try
		{
			throw TempClass();
		}
		catch(EExceptionCPP &i)
		{
			(void)i;
			try
			{
				throw 1;
			}
			catch(int &i)
			{
				(void)i;
			}	
		}
		catch(...)
		{
			printf("Escaped!");
		}
////////////////////////////////////////////////////////
#pragma warning( disable : 4286 )
		try
		{
			throw TempClass();
		}
		catch(std::exception &i)
		{
			MessageBoxA(0,i.what(), "yetryer",MB_OK);
		}
		catch(EExceptionCPP &i)
		{
			tstring Msg = i.GetExceptionAllInfoMsg();
		}
		catch(...)
		{
			printf("Escaped!");
		}
        return 0;
}


