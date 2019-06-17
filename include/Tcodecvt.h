/*
*
*/

#ifndef TCODECVT
#define TCODECVT

#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#endif

#include <locale>
#include <codecvt>
#include <char8_t.h>

//#ifdef _WIN32
//extern template std::locale::id std::codecvt<char16_t, char, struct _Mbstatet>::id;
//#endif

namespace tlib
{

      //=======================================================================
      //  Вводим функции, которые позволяют осуществлять преобразования между различными типами строк.
      //  Например, преоразовать string и wstring друг в друга

      template <class Facet>
      struct deletable_facet : Facet
      {
            template <class... Args>
            deletable_facet(Args&&... args) : Facet(std::forward<Args>(args)...)
            {
            }
            // note: создается неявный public деструктор
      };


      struct tlib_mbstate_t
      {
            std::mbstate_t state;
      };

      // предполагается, что
      //    первый параметр задает тип символов для строки с фиксированной шириной символов
      //    второй параметр задает тип символов для мультибайтовой или фиксированной строки
      // соответственно конструктор принимает имен двух локалей:
      //    первая локаль должна задавать кодировку с фикированной шириной символов
      //    вторая локаль может задавать кодировку, как с фикированной шириной символов, так и мультибайтовые кодировки

      template <class InternT, class ExternT, class State>
      class codecvt_bynames : public std::codecvt_byname<InternT, ExternT, State>
      {
      };

      template <>
      class codecvt_bynames<char, char, tlib_mbstate_t> : public std::codecvt_byname<char, char, tlib_mbstate_t>
      {            
            using base = std::codecvt_byname<char, char, tlib_mbstate_t>;
            using codecvt_w_o = deletable_facet<std::codecvt_byname<wchar_t, char, std::mbstate_t>>;

            codecvt_w_o codepage1_wide;
            codecvt_w_o wide_codepage2;

        public:
            using result = std::codecvt_base::result;
            using state_type = tlib_mbstate_t;
            using intern_type = char;
            using extern_type = char;

            explicit codecvt_bynames(
                const char* locname1,
                const char* locname2,
                size_t _refs = 0) : base(locname2, _refs),
                                    codepage1_wide(locname1, _refs),
                                    wide_codepage2(locname2, _refs)
            {
            }

            explicit codecvt_bynames(
                const std::string& locname1,
                const std::string& locname2,
                size_t _refs = 0) : base(locname2, _refs),
                                    codepage1_wide(locname1, _refs),
                                    wide_codepage2(locname2, _refs)
            {
            }

        protected:
            virtual result
            do_out(
                tlib_mbstate_t& state,
                const char* from_cp1, const char* from_end_cp1, const char*& from_next_cp1,
                char* to_cp2, char* to_end_cp2, char*& to_next_cp2) const;

            virtual result
            do_in(
                tlib_mbstate_t& state,
                const char* from_cp2, const char* from_end_cp2, const char*& from_next_cp2,
                char* to_cp1, char* to_end_cp1, char*& to_next_cp1) const;

            virtual result do_unshift(tlib_mbstate_t& state, char* to_cp2, char* to_end_cp2, char*& to_next_cp2) const;

            virtual int do_length(tlib_mbstate_t& state, const char* from_cp2, const char* from_end_cp2, std::size_t max);
            virtual int do_max_length() const noexcept;

            virtual int do_encoding() const noexcept;
            virtual bool do_always_noconv() const noexcept;

            virtual ~codecvt_bynames() noexcept
            { // destroy the object
            }
      };

      /*template <>
      class codecvt_bynames<char16_t, char8_t, std::mbstate_t> : public std::codecvt_byname<char16_t, char8_t, std::mbstate_t>
      {
            using result = std::codecvt_base::result;

        public:
            using state_type = std::mbstate_t;
            using intern_type = char16_t;
            using extern_type = char;

            explicit codecvt_bynames(
                const char* locname1,
                size_t _refs = 0) : std::codecvt_byname<char16_t, char8_t, std::mbstate_t>(locname1, _refs)
            {
            }

            explicit codecvt_bynames(
                const std::string& locname1,
                size_t _refs = 0) : std::codecvt_byname<char16_t, char8_t, std::mbstate_t>(locname1, _refs)
            {
            }

        protected:
            virtual result do_out(state_type& state,
                const intern_type* __from,
                const intern_type* __from_end, const intern_type*& __from_next,
                extern_type* __to, extern_type* __to_end,
                extern_type*& __to_next) const
            {
                  std::cout << "QQQQ -> " << *__to << "\n";                  
                  result res = std::codecvt_byname<char16_t, char8_t, std::mbstate_t>::do_out(state, __from, __from_end, __from_next, __to, __to_end, __to_next);
                  return res;
            }
      };*/


      template <>
      class codecvt_bynames<char16_t, char, tlib_mbstate_t> : public std::codecvt_byname<char16_t, char, tlib_mbstate_t>
      {
            using base = std::codecvt_byname<char16_t, char, tlib_mbstate_t>;
            using codecvt_u16_o = deletable_facet<std::codecvt_byname<wchar_t, char, std::mbstate_t>>;
            using codecvt_u16_u8 = deletable_facet<std::codecvt_byname<char16_t, char8_t, std::mbstate_t>>;

            codecvt_u16_o utf16_char;
            codecvt_u16_u8 utf16_utf8;

        public:
            using result = std::codecvt_base::result;
            using state_type = tlib_mbstate_t;
            using intern_type = char16_t;
            using extern_type = char;

            explicit codecvt_bynames(
                const char* locname1,
                size_t _refs = 0) : base(locname1, _refs),
                                    utf16_char(locname1, _refs),
                                    utf16_utf8(locname1, _refs)
            {
            }

            explicit codecvt_bynames(
                const std::string& locname1,
                size_t _refs = 0) : base(locname1, _refs),
                                    utf16_char(locname1, _refs),
                                    utf16_utf8(locname1, _refs)
            {
            }

        protected:
            virtual result
            do_out(
                tlib_mbstate_t& state,
                const char16_t* from, const char16_t* from_end, const char16_t*& from_next,
                char* to, char* to_end, char*& to_next) const;

            virtual result
            do_in(
                tlib_mbstate_t& state,
                const char* from, const char* from_end, const char*& from_next,
                char16_t* to, char16_t* to_end, char16_t*& to_next) const;

            virtual result do_unshift(tlib_mbstate_t& state, char* to, char* to_end, char*& to_next) const;

            virtual int do_length(tlib_mbstate_t& state, const char* from, const char* from_end, std::size_t max);
            virtual int do_max_length() const noexcept;

            virtual int do_encoding() const noexcept;
            virtual bool do_always_noconv() const noexcept;


            virtual ~codecvt_bynames() noexcept
            { // destroy the object
            }
      };

      // Converter character set Wide <--> Оrdinary
      using codecvt_w_o = deletable_facet<std::codecvt_byname<wchar_t, char, std::mbstate_t>>;

      // Converter character set UTF-16 <--> UTF-8
      using codecvt_u16_u8 = deletable_facet<std::codecvt_byname<char16_t, char8_t, std::mbstate_t>>;

      // Converter character set Wide <--> UTF-8
      using codecvt_w_u8 = deletable_facet<std::codecvt_byname<wchar_t, char8_t, std::mbstate_t>>;

      // Converter character set UTF-16 <--> Ordinary
      using codecvt_u16_o = deletable_facet<codecvt_bynames<char16_t, char, tlib_mbstate_t>>;

      // Converter character set Оrdinary <--> Narrow
      using codecvt_o_n = deletable_facet<codecvt_bynames<char, char, tlib_mbstate_t>>;


      //   String converter wchar_t <--> char  (UTF-16  <--> ProgramCodePage )
      extern thread_local std::wstring_convert<codecvt_w_o> strconvert_w_codepage;

      //   String converter char16_t <--> char8_t  (UTF-16  <--> UTF-8)
      extern thread_local std::wstring_convert<codecvt_u16_u8, char16_t> strconvert_u16_u8;

      //   String converter wchar_t <--> char8_t   (Wide  <--> UTF-8)
      extern thread_local std::wstring_convert<codecvt_w_u8, wchar_t> strconvert_w_u8;


      //   Convertering buffer char16_t <--> char8_t  (UTF-16  <--> UTF-8) need for Linux
      extern std::wbuffer_convert<codecvt_u16_u8, char16_t>& in_bufferconvert_u16_u8;
      extern std::wbuffer_convert<codecvt_u16_u8, char16_t>& out_bufferconvert_u16_u8;
      extern std::wbuffer_convert<codecvt_u16_u8, char16_t>& err_bufferconvert_u16_u8;
      extern std::wbuffer_convert<codecvt_u16_u8, char16_t>& log_bufferconvert_u16_u8;

      //   Convertering buffer wchar_t <--> char8_t  (UTF-16  <--> UTF-8) need for Linux
      extern std::wbuffer_convert<codecvt_w_u8, wchar_t>& in_bufferconvert_w_u8;
      extern std::wbuffer_convert<codecvt_w_u8, wchar_t>& out_bufferconvert_w_u8;
      extern std::wbuffer_convert<codecvt_w_u8, wchar_t>& err_bufferconvert_w_u8;
      extern std::wbuffer_convert<codecvt_w_u8, wchar_t>& log_bufferconvert_w_u8;

      //   Convertering buffer char <--> char  (Program code page <--> Console code page) need for Windows
      extern std::wbuffer_convert<codecvt_o_n, char>& in_bufferconvert_programCP_consoleCP;
      extern std::wbuffer_convert<codecvt_o_n, char>& out_bufferconvert_programCP_consoleCP;
      extern std::wbuffer_convert<codecvt_o_n, char>& err_bufferconvert_programCP_consoleCP;
      extern std::wbuffer_convert<codecvt_o_n, char>& log_bufferconvert_programCP_consoleCP;

      //   Convertering buffer char16_t <--> char (UTF-16  <-->  Console code page) need for Windows
      extern std::wbuffer_convert<codecvt_u16_o, char16_t>& in_bufferconvert_u16_o;
      extern std::wbuffer_convert<codecvt_u16_o, char16_t>& out_bufferconvert_u16_o;
      extern std::wbuffer_convert<codecvt_u16_o, char16_t>& err_bufferconvert_u16_o;
      extern std::wbuffer_convert<codecvt_u16_o, char16_t>& log_bufferconvert_u16_o;

}

#if !(defined(__linux__) || defined(_LIB))

#define _LIB_NAME "tlib_char16"

#include <win/select_lib.h>

#undef _LIB_NAME

#endif // !(defined(__linux__) || defined(_LIB))


#endif //TCODECVT
