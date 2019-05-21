﻿/*
*/

#ifndef TIOSTREAM
#define TIOSTREAM

#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <iterator>

namespace tlib
{

      using std::endl;

#ifdef _WIN32
      extern std::wostream& ucout;
      extern std::wistream& ucin;
#else
      extern std::ostream& ucout;
      extern std::istream& ucin;
#endif

#if (defined(_WIN32) && defined(_UNICODE))

      extern std::wostream& tcout;
      extern std::wistream& tcin;
      extern std::wostream& tcerr;
      extern std::wostream& tclog;

      typedef std::wostream tostream;
      typedef std::wistream tistream;
      typedef std::wifstream tifstream;
      typedef std::wofstream tofstream;
      typedef std::wfstream tfstream;
      typedef std::wostringstream tostringstream;
      typedef std::wistringstream tistringstream;
      typedef std::wstringstream tstringstream;
      typedef std::wstringbuf tstringbuf;

#else

      extern std::ostream& tcout;
      extern std::istream& tcin;
      extern std::ostream& tcerr;
      extern std::ostream& tclog;

      typedef std::ostream tostream;
      typedef std::istream tistream;
      typedef std::ifstream tifstream;
      typedef std::ofstream tofstream;
      typedef std::fstream tfstream;
      typedef std::ostringstream tostringstream;
      typedef std::istringstream tistringstream;
      typedef std::stringstream tstringstream;
      typedef std::stringbuf tstringbuf;

#endif // _UNICODE

#if !(defined(__linux__) || defined(UNDER_CE) || defined(WINCE))
      // Не актуально для не консольных и WINCE программ
      std::ostream& operator<<(std::ostream& s, char x);
      std::ostream& operator<<(std::ostream& s, const char* x);
      std::ostream& operator<<(std::ostream& s, const std::string& x);
      std::ostream& operator<<(std::ostream& s, std::string_view x);
      std::istream& operator>>(std::istream& s, std::string& x);

      std::wostream& operator<<(std::wostream& s, char16_t x);
      std::wostream& operator<<(std::wostream& s, const char16_t* x);
      std::wostream& operator<<(std::wostream& s, const std::u16string& x);
      std::wostream& operator<<(std::wostream& s, std::u16string_view x);
      std::wistream& operator>>(std::wistream& s, std::u16string& x);


#ifdef Q_OS_WIN
      inline std::ostream& operator<<(std::ostream& s, const QString& x)
      {
            return s << static_cast<const char*>(x.toLocal8Bit());
      }

      inline std::wostream& operator<<(std::wostream& s, const QString& x)
      {
            return s << x.toStdWString();
      }
#endif //Q_OS_WIN

#elif __linux__

      std::ostream& operator<<(std::ostream& s, char16_t x);
      std::ostream& operator<<(std::ostream& s, const char16_t* x);
      std::ostream& operator<<(std::ostream& s, const std::u16string& x);
      std::ostream& operator<<(std::ostream& s, std::u16string_view x);
      std::istream& operator>>(std::istream& s, std::u16string& x);

#endif //!(defined(UNDER_CE) || defined(WINCE))
}

template <>
class std::ostream_iterator<std::string, char>
{ // wrap _Ty inserts to output stream as output iterator
  public:
      using iterator_category = std::output_iterator_tag;
      using value_type = void;
      using difference_type = void;
      using pointer = void;
      using reference = void;
      using ostream_type = std::basic_ostream<char>;

      ostream_iterator(ostream_type& _Ostr, const char* const _Delim = 0)
          : _Mydelim(_Delim), _Myostr(addressof(_Ostr))
      { // construct from output stream and delimiter
      }

      ostream_iterator& operator=(const std::string& _Val)
      { // insert value into output stream, followed by delimiter
            using tlib::operator<<;

            *_Myostr << _Val;

            if (_Mydelim != 0)
            {
                  *_Myostr << _Mydelim;
            }

            return (*this);
      }

      ostream_iterator& operator*()
      { // pretend to return designated value
            return (*this);
      }

      ostream_iterator& operator++()
      { // pretend to preincrement
            return (*this);
      }

      ostream_iterator& operator++(int)
      { // pretend to postincrement
            return (*this);
      }

  protected:
      const char* _Mydelim;  // pointer to delimiter string (NB: not freed)
      ostream_type* _Myostr; // pointer to output stream
};

template <>
class std::ostream_iterator<std::u16string, char16_t>
{ // wrap _Ty inserts to output stream as output iterator
  public:
      using iterator_category = std::output_iterator_tag;
      using value_type = void;
      using difference_type = void;
      using pointer = void;
      using reference = void;
#ifdef _WIN32
      using ostream_type = std::basic_ostream<wchar_t>;
#else
      using ostream_type = std::basic_ostream<char>;
#endif

      ostream_iterator(ostream_type& _Ostr, const char16_t* const _Delim = 0)
          : _Mydelim(_Delim), _Myostr(addressof(_Ostr))
      { // construct from output stream and delimiter
      }

      ostream_iterator& operator=(const std::u16string& _Val)
      { // insert value into output stream, followed by delimiter
            using tlib::operator<<;

            *_Myostr << _Val;

            if (_Mydelim != 0)
            {
                  *_Myostr << _Mydelim;
            }

            return (*this);
      }

      ostream_iterator& operator*()
      { // pretend to return designated value
            return (*this);
      }

      ostream_iterator& operator++()
      { // pretend to preincrement
            return (*this);
      }

      ostream_iterator& operator++(int)
      { // pretend to postincrement
            return (*this);
      }

  protected:
      const char16_t* _Mydelim; // pointer to delimiter string (NB: not freed)
      ostream_type* _Myostr;    // pointer to output stream
};

#if !(defined(__linux__) || defined(_LIB) || defined(UNDER_CE) || defined(WINCE))

#define _LIB_NAME "tlib"

#include <win/select_lib.h>

#undef _LIB_NAME

#endif // !(defined(_LIB) || defined(UNDER_CE) || defined(WINCE))


#endif //TIOSTREAM
