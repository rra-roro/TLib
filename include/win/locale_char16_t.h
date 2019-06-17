//////////////////////////////////////////////////////////////////////////////////////////////////
//
//   Файл locale_char16_t.h  - объявление новый фасет ctype<char16_t>
//
#pragma once

//namespace tlib
//{
//      template <class CharT>
//      class ctype : public std::ctype<CharT>
//      {
//            static_assert(true, "implementation of this class is missing");
//      };
//
//      template <>
//      class ctype<char16_t> : public std::ctype<char16_t> 
//
//      {
//        public:
//            
//
//            explicit ctype(size_t _Refs = 0) : ctype_base(_Refs)
//            { // construct from current locale
//                  _Locinfo _Lobj;
//                  _Init(_Lobj);
//            }
//
//            ctype(const std::_Locinfo &_Lobj, size_t _Refs = 0) : ctype_base(_Refs)
//            { // construct from specified locale
//                  _Init(_Lobj);
//            }
//
//            ctype(const char *_Pch, size_t _Refs = 0) : ctype_base(_Refs)
//            { // construct from specified locale
//                  _Locinfo _Lobj(_Pch);
//                  _Init(_Lobj);
//            }
//
//
//            static size_t _Getcat(const std::locale::facet **_Ppf = 0, const std::locale *_Ploc = 0)
//            {
//                  // return locale category mask and construct standard facet
//                  if (_Ppf != 0 && *_Ppf == 0)
//                        *_Ppf = new ctype<char16_t>(std::_Locinfo(_Ploc->c_str()));
//
//                  return (_X_CTYPE);
//            }
//
//        protected:
//            virtual ~ctype() noexcept
//            { // destroy the object
//                  if (_Ctype._Delfl)
//                        _CSTD free((void *)_Ctype._Table);
//
//                  _CSTD free(_Ctype._LocaleName);
//            }
//
//            void _Init(const std::_Locinfo &_Lobj)
//            { // initialize from _Lobj
//                  _Ctype = _Lobj._Getctype();
//                  _Cvt = _Lobj._Getcvt();
//            }
//
//            virtual bool do_is(mask _Maskval, char16_t _Ch) const
//            { // test if element fits any mask classifications
//                  return ((_CSTD _Getwctype(_Ch, &_Ctype) & _Maskval) != 0);
//            }
//
//            virtual const char16_t *do_is(const char16_t *_First, const char16_t *_Last, mask *_Dest) const
//            { // get mask sequence for elements in [_First, _Last)
//                  _Adl_verify_range(_First, _Last);
//                  return (const char16_t *)(_CSTD _Getwctypes((const wchar_t *)_First, (const wchar_t *)_Last, _Dest, &_Ctype));
//            }
//
//            virtual const char16_t *do_scan_is(mask _Maskval, const char16_t *_First, const char16_t *_Last) const
//            { // find first in [_First, _Last) that fits mask classification
//                  _Adl_verify_range(_First, _Last);
//                  for (; _First != _Last && !is(_Maskval, *_First); ++_First)
//                        ;
//                  return (_First);
//            }
//
//            virtual const char16_t *do_scan_not(mask _Maskval, const char16_t *_First, const char16_t *_Last) const
//            { // find first in [_First, _Last) not fitting mask classification
//                  _Adl_verify_range(_First, _Last);
//                  for (; _First != _Last && is(_Maskval, *_First); ++_First)
//                        ;
//                  return (_First);
//            }
//
//            virtual char16_t do_tolower(char16_t _Ch) const
//            { // convert element to lower case
//                  return (_Towlower(_Ch, &_Ctype));
//            }
//
//            virtual const char16_t *do_tolower(char16_t *_First, const char16_t *_Last) const
//            { // convert [_First, _Last) in place to lower case
//                  _Adl_verify_range((const char16_t *)_First, _Last);
//                  for (; _First != _Last; ++_First)
//                        *_First = _Towlower(*_First, &_Ctype);
//                  return ((const char16_t *)_First);
//            }
//
//            virtual char16_t do_toupper(char16_t _Ch) const
//            { // convert element to upper case
//                  return (_Towupper(_Ch, &_Ctype));
//            }
//
//            virtual const char16_t *do_toupper(char16_t *_First, const char16_t *_Last) const
//            { // convert [_First, _Last) in place to upper case
//                  _Adl_verify_range((const char16_t *)_First, _Last);
//                  for (; _First != _Last; ++_First)
//                        *_First = _Towupper(*_First, &_Ctype);
//                  return ((const char16_t *)_First);
//            }
//
//            char16_t _Dowiden(char _Byte) const
//            { // widen char
//                  mbstate_t(_Mbst);
//                  wchar_t _Wc;
//                  return (_Mbrtowc(&_Wc, &_Byte, 1, &_Mbst, &_Cvt) < 0
//                              ? (wchar_t)WEOF
//                              : _Wc);
//            }
//
//            virtual char16_t do_widen(char _Byte) const
//            { // widen char
//                  return (_Dowiden(_Byte));
//            }
//
//            /**
//		*  Конвертируем из текущей кодировки локали в UTF-16
//		*
//		*  Эта ф-ия ковертирует [_First, _Last) в UTF-16 и
//		*  записывает результат в буффер _Dest.
//		*  Размер буфера _Dest должен быть не меньше  чем (_Last - _First) * sizeof(wchar_t)
//		*
//		*  _First   указатель на начало исходного буффера
//		*  _Last    указатель на символ следующий за концом исходного буффера
//		*  _Dest    указатель на начало конечного буффера
//		*   return  указатель на начало исходного буффера
//		*/
//            virtual const char *do_widen(const char *_First, const char *_Last, char16_t *_Dest) const
//            { // widen chars in [_First, _Last)
//                  _Adl_verify_range(_First, _Last);
//                  for (; _First != _Last; ++_First, ++_Dest)
//                        *_Dest = _Dowiden(*_First);
//                  return (_First);
//            }
//
//            char _Donarrow(char16_t _Ch, char _Dflt) const
//            { // narrow element to char
//                  char _Buf[MB_LEN_MAX];
//                  mbstate_t(_Mbst);
//                  return (_Wcrtomb(_Buf, _Ch, &_Mbst, &_Cvt) != 1
//                              ? _Dflt
//                              : _Buf[0]);
//            }
//
//            virtual char do_narrow(char16_t _Ch, char _Dflt) const
//            { // narrow element to char
//                  return (_Donarrow(_Ch, _Dflt));
//            }
//
//            /**
//		*  Конвертируем из UTF-16 в текущую кодировку локали
//		*
//		*  Эта ф-ия ковертирует [_First, _Last) в текущую кодировку локали и
//		*  записывает результат в буффер _Dest.
//		*  Размер буфера _Dest должен быть не меньше, чем (_Last - _First) / sizeof(wchar_t)
//		*
//		*  _First   указатель на начало исходного буффера
//		*  _Last    указатель на символ следующий за концом исходного буффера
//		*  _Dest    указатель на начало конечного буффера
//		*   return  указатель на начало исходного буффера
//		*/
//            virtual const char16_t *do_narrow(const char16_t *_First, const char16_t *_Last, char _Dflt, char *_Dest) const
//            { // narrow elements in [_First, _Last) to chars
//                  _Adl_verify_range(_First, _Last);
//                  for (; _First != _Last; ++_First, ++_Dest)
//                        *_Dest = _Donarrow(*_First, _Dflt);
//                  return (_First);
//            }
//
//        private:
//            std::_Locinfo::_Ctypevec _Ctype; // locale info passed to _Tolower, etc.
//            std::_Locinfo::_Cvtvec _Cvt;     // conversion information
//      };
//
//
//
//}
//





//
////#include <locale>
//
/////
////  Фасет ctype<char16_t> фактически эквивалентен ctype<wchar_t>,
////  только для типа char16_t
////  Особенностью фасета является использование WinAPI для ф-ий
////  конвертации строк .widen() и .narrow():
////    WideCharToMultiByte() и MultiByteToWideChar()
////  По результатам тестов это повысило производительность ф-ий .widen() и .narrow()
/////
//
template<>
	class std::ctype<char16_t> : public std::ctype_base
	{
	public:

		bool is(mask _Maskval, char16_t _Ch) const
		{	// test if element fits any mask classifications
			return (do_is(_Maskval, _Ch));
		}

		const char16_t* is(const char16_t *_First, const char16_t *_Last, mask *_Dest) const
		{	// get mask sequence for elements in [_First, _Last)
			return (do_is(_First, _Last, _Dest));
		}

		const char16_t* scan_is(mask _Maskval, const char16_t *_First, const char16_t *_Last) const
		{	// find first in [_First, _Last) that fits mask classification
			return (do_scan_is(_Maskval, _First, _Last));
		}

		const char16_t* scan_not(mask _Maskval, const char16_t *_First, const char16_t *_Last) const
		{	// find first in [_First, _Last) not fitting mask classification
			return (do_scan_not(_Maskval, _First, _Last));
		}

		char16_t tolower(char16_t _Ch) const
		{	// convert element to lower case
			return (do_tolower(_Ch));
		}

		const char16_t* tolower(char16_t *_First, const char16_t *_Last) const
		{	// convert [_First, _Last) in place to lower case
			return (do_tolower(_First, _Last));
		}

		char16_t toupper(char16_t _Ch) const
		{	// convert element to upper case
			return (do_toupper(_Ch));
		}

		const char16_t* toupper(char16_t *_First, const char16_t *_Last) const
		{	// convert [_First, _Last) in place to upper case
			return (do_toupper(_First, _Last));
		}

		char16_t widen(char _Byte) const
		{	// widen char
			return (do_widen(_Byte));
		}

		const char* widen(const char *_First, const char *_Last, char16_t *_Dest) const
		{	// widen chars in [_First, _Last)
			return (do_widen(_First, _Last, _Dest));
		}

		char  narrow(char16_t _Ch, char _Dflt = '\0') const
		{	// narrow element to char
			return (do_narrow(_Ch, _Dflt));
		}

		const char16_t* narrow(const char16_t *_First, const char16_t *_Last, char _Dflt, char *_Dest) const
		{	// narrow elements in [_First, _Last) to chars
			return (do_narrow(_First, _Last, _Dflt, _Dest));
		}

		static std::locale::id id;

		explicit ctype(size_t _Refs = 0) : ctype_base(_Refs)
		{	// construct from current locale
			_Locinfo _Lobj;
			_Init(_Lobj);
		}

		ctype(const std::_Locinfo& _Lobj, size_t _Refs = 0) : ctype_base(_Refs)
		{	// construct from specified locale
			_Init(_Lobj);
		}

		ctype(const char *_Pch, size_t _Refs = 0) : ctype_base(_Refs)
		{	// construct from specified locale
			_Locinfo _Lobj(_Pch);
			_Init(_Lobj);
		}


		static size_t _Getcat(const std::locale::facet **_Ppf = 0, const std::locale *_Ploc = 0)
		{
			// return locale category mask and construct standard facet
			if (_Ppf != 0 && *_Ppf == 0)
				*_Ppf = new ctype<char16_t>(std::_Locinfo(_Ploc->c_str()));

			return (_X_CTYPE);
		}

	protected:
		virtual ~ctype() noexcept
		{	// destroy the object
			if (_Ctype._Delfl)
				_CSTD free((void *)_Ctype._Table);

			_CSTD free(_Ctype._LocaleName);
		}

		void _Init(const std::_Locinfo& _Lobj)
		{	// initialize from _Lobj
			_Ctype = _Lobj._Getctype();
			_Cvt = _Lobj._Getcvt();
		}

		virtual bool do_is(mask _Maskval, char16_t _Ch) const
		{	// test if element fits any mask classifications
			return ((_CSTD _Getwctype(_Ch, &_Ctype) & _Maskval) != 0);
		}

		virtual const char16_t* do_is(const char16_t *_First, const char16_t *_Last, mask *_Dest) const
		{	// get mask sequence for elements in [_First, _Last)
			_Adl_verify_range(_First, _Last);
			return (const char16_t*)(_CSTD _Getwctypes((const wchar_t*)_First, (const wchar_t*)_Last, _Dest, &_Ctype));
		}

		virtual const char16_t* do_scan_is(mask _Maskval, const char16_t *_First, const char16_t *_Last) const
		{	// find first in [_First, _Last) that fits mask classification
			_Adl_verify_range(_First, _Last);
			for (; _First != _Last && !is(_Maskval, *_First); ++_First)
				;
			return (_First);
		}

		virtual const char16_t* do_scan_not(mask _Maskval, const char16_t *_First, const char16_t *_Last) const
		{	// find first in [_First, _Last) not fitting mask classification
			_Adl_verify_range(_First, _Last);
			for (; _First != _Last && is(_Maskval, *_First); ++_First)
				;
			return (_First);
		}

		virtual char16_t do_tolower(char16_t _Ch) const
		{	// convert element to lower case
			return (_Towlower(_Ch, &_Ctype));
		}

		virtual const char16_t* do_tolower(char16_t *_First, const char16_t *_Last) const
		{	// convert [_First, _Last) in place to lower case
			_Adl_verify_range((const char16_t *)_First, _Last);
			for (; _First != _Last; ++_First)
				*_First = _Towlower(*_First, &_Ctype);
			return ((const char16_t *)_First);
		}

		virtual char16_t do_toupper(char16_t _Ch) const
		{	// convert element to upper case
			return (_Towupper(_Ch, &_Ctype));
		}

		virtual const char16_t* do_toupper(char16_t *_First, const char16_t *_Last) const
		{	// convert [_First, _Last) in place to upper case
			_Adl_verify_range((const char16_t *)_First, _Last);
			for (; _First != _Last; ++_First)
				*_First = _Towupper(*_First, &_Ctype);
			return ((const char16_t *)_First);
		}

		char16_t  _Dowiden(char _Byte) const
		{	// widen char
			mbstate_t(_Mbst);
			wchar_t _Wc;
			return (_Mbrtowc(&_Wc, &_Byte, 1, &_Mbst, &_Cvt) < 0
				? (wchar_t)WEOF : _Wc);
		}

		virtual char16_t  do_widen(char _Byte) const
		{	// widen char
			return (_Dowiden(_Byte));
		}

		/**
		*  Конвертируем из текущей кодировки локали в UTF-16
		*
		*  Эта ф-ия ковертирует [_First, _Last) в UTF-16 и
		*  записывает результат в буффер _Dest.
		*  Размер буфера _Dest должен быть не меньше  чем (_Last - _First) * sizeof(wchar_t)
		*
		*  _First   указатель на начало исходного буффера
		*  _Last    указатель на символ следующий за концом исходного буффера
		*  _Dest    указатель на начало конечного буффера
		*   return  указатель на начало исходного буффера
		*/
		virtual const char* do_widen(const char *_First, const char *_Last, char16_t *_Dest) const
		{        // widen chars in [_First, _Last)
			_Adl_verify_range(_First, _Last);
			for (; _First != _Last; ++_First, ++_Dest)
				*_Dest = _Dowiden(*_First);
			return (_First);
		}

		char _Donarrow(char16_t _Ch, char _Dflt) const
		{	// narrow element to char
			char _Buf[MB_LEN_MAX];
			mbstate_t(_Mbst);
			return (_Wcrtomb(_Buf, _Ch, &_Mbst, &_Cvt) != 1
				? _Dflt : _Buf[0]);
		}

		virtual char do_narrow(char16_t _Ch, char _Dflt) const
		{	// narrow element to char
			return (_Donarrow(_Ch, _Dflt));
		}

		/**
		*  Конвертируем из UTF-16 в текущую кодировку локали
		*
		*  Эта ф-ия ковертирует [_First, _Last) в текущую кодировку локали и
		*  записывает результат в буффер _Dest.
		*  Размер буфера _Dest должен быть не меньше, чем (_Last - _First) / sizeof(wchar_t)
		*
		*  _First   указатель на начало исходного буффера
		*  _Last    указатель на символ следующий за концом исходного буффера
		*  _Dest    указатель на начало конечного буффера
		*   return  указатель на начало исходного буффера
		*/
		virtual const char16_t* do_narrow(const char16_t *_First, const char16_t *_Last, char _Dflt, char *_Dest) const
		{        // narrow elements in [_First, _Last) to chars
			_Adl_verify_range(_First, _Last);
			for (; _First != _Last; ++_First, ++_Dest)
				*_Dest = _Donarrow(*_First, _Dflt);
			return (_First);
		}

	private:
		std::_Locinfo::_Ctypevec _Ctype;	// locale info passed to _Tolower, etc.
		std::_Locinfo::_Cvtvec _Cvt;		// conversion information
	};


