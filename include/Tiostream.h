/*
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

      using u16istream = std::basic_istream<char16_t>;
      using u16ostream = std::basic_ostream<char16_t>;

#ifdef _WIN32
      extern std::wostream& ucout;
      extern std::wistream& ucin;
      extern std::wostream& ucerr;
      extern std::wostream& uclog;

#else
      extern u16ostream ucout;
      extern u16istream ucin;
      extern u16ostream ucerr;
      extern u16ostream uclog;
#endif

      using tistream = std::basic_istream<TCHAR>;
      using tostream = std::basic_ostream<TCHAR>;
      using tifstream = std::basic_ifstream<TCHAR>;
      using tofstream = std::basic_ofstream<TCHAR>;
      using tfstream = std::basic_fstream<TCHAR>;
      using tistringstream = std::basic_istringstream<TCHAR>;
      using tostringstream = std::basic_ostringstream<TCHAR>;
      using tstringstream = std::basic_stringstream<TCHAR>;
      using tstringbuf = std::basic_stringbuf<TCHAR>;

#if (defined(_WIN32) && defined(_UNICODE))
      extern std::wostream& tcout;
      extern std::wistream& tcin;
      extern std::wostream& tcerr;
      extern std::wostream& tclog;
#else
      extern u16ostream& tcout;
      extern u16istream& tcin;
      extern u16ostream& tcerr;
      extern u16ostream& tclog;
#endif


#ifdef _WIN32
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
#endif // Q_OS_WIN
#endif // _WIN32
}

#ifdef _WIN32
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

#endif // _WIN32


#if !(defined(__linux__) || defined(_LIB) || defined(UNDER_CE) || defined(WINCE))

#define _LIB_NAME "tlib"

#include <win/select_lib.h>

#undef _LIB_NAME

#endif // !(defined(_LIB) || defined(UNDER_CE) || defined(WINCE))


#endif //TIOSTREAM
