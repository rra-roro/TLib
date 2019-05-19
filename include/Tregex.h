#ifndef TREGEX
#define TREGEX

#include <regex>
#include <string>

#ifdef _WIN32

// CLASS regex_traits<char16_t>
template<>
class std::regex_traits<char16_t>
	: public std::_Regex_traits<char16_t>
{	// specialization for char16_t
public:
	int value(char16_t _Ch, int _Base) const
	{	// map character value to numeric value
		if ((_Base != 8 && u'0' <= _Ch && _Ch <= u'9')
			|| (_Base == 8 && u'0' <= _Ch && _Ch <= u'7'))
			return (_Ch - u'0');
		else if (_Base != 16)
			;
		else if (u'a' <= _Ch && _Ch <= u'f')
			return (_Ch - u'a' + 10);
		else if (u'A' <= _Ch && _Ch <= u'F')
			return (_Ch - u'A' + 10);
		return (-1);
	}
};

template<> inline
const char16_t *std::_Cl_names::_Get<char16_t>() const
{	// get wide pointer
	return ((char16_t*)_Wide);
};

extern template std::locale::id std::collate<char16_t>::id;

#endif

namespace Tstring 
{
	typedef std::basic_regex<char16_t> uregex;
	typedef std::match_results<const char16_t *> ucmatch;
	typedef std::match_results<std::u16string::const_iterator> usmatch;

	typedef std::regex_token_iterator<const char16_t *> ucregex_token_iterator;
	typedef std::regex_token_iterator<std::u16string::const_iterator> usregex_token_iterator;
	typedef std::regex_iterator<const char16_t *> ucregex_iterator;
	typedef std::regex_iterator<std::u16string::const_iterator> usregex_iterator;

#ifdef _WIN32

#ifdef _UNICODE
	typedef std::basic_regex<wchar_t> tregex;                                       // wregex
	typedef std::match_results<const wchar_t *> tcmatch;                            // wcmatch
	typedef std::match_results<std::wstring::const_iterator> tsmatch;               // wsmatch	

	typedef std::regex_token_iterator<const wchar_t *> tcregex_token_iterator;           // wcregex_token_iterator
	typedef std::regex_token_iterator<std::wstring::const_iterator> tsregex_token_iterator;   // wsregex_token_iterator
	typedef std::regex_iterator<const wchar_t *> tcregex_iterator;
	typedef std::regex_iterator<std::wstring::const_iterator> tsregex_iterator;

#else
	typedef std::basic_regex<char> tregex;                                        // regex
	typedef std::match_results<const char *> tcmatch;                             // cmatch
	typedef std::match_results<std::string::const_iterator> tsmatch;              // smatch

	typedef std::regex_token_iterator<const char *> tcregex_token_iterator;            // cregex_token_iterator
	typedef std::regex_token_iterator<std::string::const_iterator> tsregex_token_iterator;  // sregex_token_iterator
	typedef std::regex_iterator<const char *> tcregex_iterator;
	typedef std::regex_iterator<std::string::const_iterator> tsregex_iterator;
	
#endif  // _UNICODE

#elif __linux__

	typedef std::basic_regex<char16_t> tregex;                                       // uregex
	typedef std::match_results<const char16_t *> tcmatch;                            // ucmatch
	typedef std::match_results<std::u16string::const_iterator> tsmatch;              // usmatch

	typedef std::regex_token_iterator<const char16_t *> tcregex_token_iterator;           // ucregex_token_iterator
	typedef std::regex_token_iterator<std::u16string::const_iterator> tsregex_token_iterator;  // usregex_token_iterator
	typedef std::regex_iterator<const char16_t *> tcregex_iterator;
	typedef std::regex_iterator<std::u16string::const_iterator> tsregex_iterator;

#endif

}


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


#endif // TREGEX
