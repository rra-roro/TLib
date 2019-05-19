#ifndef TTYPE
#define TTYPE

#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#endif

#include <string>
#include <type_traits>

// Зададим общие шаблоны для определения символьных типов,
// которые на запрос о принадлежнасти типу, будут возвращать истину в случае совподения типов

// determine whether _Ty is wchar_t
template<class _Ty> 
  struct is_wchar : std::false_type {};

template<>
  struct is_wchar<wchar_t> : std::true_type {};

template<class _Ty>
  constexpr bool is_wchar_v = is_wchar<_Ty>::value;

// determine whether _Ty is char16_t
template<class _Ty> 
  struct is_char16 : std::false_type {};

template<>
  struct is_char16<char16_t> : std::true_type {};

template<class _Ty>
  constexpr bool is_char16_v = is_char16<_Ty>::value;


// determine whether _Ty is char
template<class _Ty> 
  struct is_char : std::false_type {};

template<>
  struct is_char<char> : std::true_type {};

template<class _Ty>
  constexpr bool is_char_v = is_char<_Ty>::value;

// determine whether _Ty is symbol
template<class _Ty> 
  struct is_symbol : std::false_type {};

template<>
  struct is_symbol<char> : std::true_type {};

template<>
  struct is_symbol<wchar_t> : std::true_type {};

template<>
  struct is_symbol<char16_t> : std::true_type {};

template<>
  struct is_symbol<char32_t> : std::true_type {};

template<class _Ty>
  constexpr bool is_symbol_v = is_symbol<_Ty>::value;


#ifdef _WIN32
// CallConvention - макрос который позволяет конвертировать соглашение о вызовах
//                  __cdecl, __stdcall, __fastcall, __thiscall
//                  в объявление указателя на ф-ию, который уже является типом, и это уже
//                  можно передовать в качестве параметра шаблона

#ifdef __BORLANDC__
  #define CallConvention(CallConv) void CallConv (*)()
#else
  #define CallConvention(CallConv) void (CallConv *)()
#endif

#define _CC(CallConv)    CallConvention(CallConv)  

// Введем механизм определения в шаблонах соглашения о вызовах
template <typename T>
	class IsCDecl
	{
	public:
		enum { Yes = 0, No = 1 };
	};

template <typename T>
	class IsStdcall
	{
	public:
		enum { Yes = 0, No = 1 };
	};

template <typename T>
	class IsFastcall
	{
	public:
		enum { Yes = 0, No = 1 };
	};

#ifndef __BORLANDC__
template <typename T>
	class IsThiscall
	{
	public:
		enum { Yes = 0, No = 1 };
	};
#endif

// Введем механизм определения в шаблонах соглашения о вызовах
#define MK_CDECL_TYPE(T)				\
	template<> class IsCDecl<T>			\
	{						\
		public:					\
			enum { Yes = 1, No = 0 };	\
	};

#define MK_STDCALL_TYPE(T)				\
	template<> class IsStdcall<T>			\
	{						\
		public:					\
			enum { Yes = 1, No = 0 };	\
	};

#define MK_FASTCALL_TYPE(T)				\
	template<> class IsFastcall<T>			\
	{						\
		public:					\
			enum { Yes = 1, No = 0 };	\
	};

#ifndef __BORLANDC__
#define MK_THISCALL_TYPE(T)				\
	template<> class IsThiscall<T>			\
	{						\
		public:					\
			enum { Yes = 1, No = 0 };	\
	};
#endif

// Определим для каких типов шаблоны должны возвращать истину
MK_CDECL_TYPE(CallConvention(__cdecl));
MK_STDCALL_TYPE(CallConvention(__stdcall));
MK_FASTCALL_TYPE(CallConvention(__fastcall));
#ifndef __BORLANDC__
MK_THISCALL_TYPE(CallConvention(__thiscall));
#endif

#endif // #ifdef _WIN32

// Определим какой тип wchar_t, char, или char16_t лежит в основе строки

template<class _Ty>
struct Type_Str
{	
};

template<class _Ty>
struct Type_Str<_Ty *>
{	// determine whether _Ty is a pointer	
	using type = std::remove_cv_t<_Ty>;
};

template<class _Ty>
struct Type_Str<std::basic_string<_Ty>>
{	// determine whether _Ty is a pointer
	using type = std::remove_cv_t<_Ty>;
};

template<class _Ty, size_t N>
struct Type_Str<_Ty[N]>
{	// determine whether _Ty is a pointer	
	using type = std::remove_cv_t<_Ty>;
};


#endif  //TTYPE