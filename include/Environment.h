#ifndef ENV_DLL
#define ENV_DLL

#include <Tstring.h>
#include <WinAPIStaticWrapper.h>

	template<typename  _Elem>
		inline std::basic_string<_Elem,std::char_traits<_Elem>,std::allocator<_Elem> > TGetEnvironmentVar( const _Elem* lpName)		
		{
			if(lpName==0) return std::basic_string<_Elem,std::char_traits<_Elem>,std::allocator<_Elem> >();
			DWORD SizeStr = TGetEnvironmentVariable(lpName,(_Elem*)0,0);
			if(SizeStr != 0)
			{
              vector<_Elem> bufEnvVar(SizeStr);
			  TGetEnvironmentVariable(lpName,&bufEnvVar[0],SizeStr);			
			  return std::basic_string<_Elem,std::char_traits<_Elem>,std::allocator<_Elem> >(bufEnvVar.begin(),bufEnvVar.end()-1);
		    }
			return std::basic_string<_Elem,std::char_traits<_Elem>,std::allocator<_Elem> >();
		}

	template< typename  _Elem,  typename  _Traits, typename _Ax>
		inline std::basic_string<_Elem,_Traits,_Ax> TGetEnvironmentVar( const std::basic_string<_Elem,_Traits,_Ax>& lpName)
		{
			return TGetEnvironmentVar(lpName.c_str());
		}

#endif //ENV_DLL


