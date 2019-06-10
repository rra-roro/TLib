/*
*
*/
#include "stdafx.h"
#include <Tlocale.h>
#include <Tcodecvt.h>
#include <algorithm>

#ifndef NOMINMAX
#define NOMINMAX // На всякий случай
#endif

#undef min // Вот тут все отключаем !!
#undef max

using std::max; // Ну и далее только шаблоны
using std::min;


using namespace std;

namespace tlib
{

      using codecvt_bn = codecvt_bynames<char, char, tlib_mbstate_t>;

      codecvt_bn::result codecvt_bn::do_out(
          tlib_mbstate_t& state,
          const char* from_cp1, const char* from_end_cp1, const char*& from_next_cp1,
          char* to_cp2, char* to_end_cp2, char*& to_next_cp2) const
      {
            std::wstring tmp(from_end_cp1 - from_cp1, '\0');
            wchar_t* tmp_next_to;
            const wchar_t* tmp_next_from;

            result res1 = codepage1_wide.in(state.state, from_cp1, from_end_cp1, from_next_cp1, &tmp[0], &tmp[tmp.size()], tmp_next_to);
            if (res1 != std::codecvt_base::ok && res1 != std::codecvt_base::partial)
                  return res1;

            result res2 = wide_codepage2.out(state.state, &tmp[0], &tmp[tmp.size()], tmp_next_from, to_cp2, to_end_cp2, to_next_cp2);
            if (res2 != std::codecvt_base::ok && res2 != std::codecvt_base::partial)
                  return res2;

            if (res1 == std::codecvt_base::partial || res2 == std::codecvt_base::partial)
                  return std::codecvt_base::partial;

            return std::codecvt_base::ok;
      }

      codecvt_bn::result codecvt_bn::do_in(tlib_mbstate_t& state,
          const char* from_cp2, const char* from_end_cp2, const char*& from_next_cp2,
          char* to_cp1, char* to_end_cp1, char*& to_next_cp1) const
      {
            std::wstring tmp(from_end_cp2 - from_cp2, '\0');
            wchar_t* tmp_next_to;
            const wchar_t* tmp_next_from;

            result res1 = wide_codepage2.in(state.state, from_cp2, from_end_cp2, from_next_cp2, &tmp[0], &tmp[tmp.size()], tmp_next_to);
            if (res1 != std::codecvt_base::ok && res1 != std::codecvt_base::partial)
                  return res1;

            result res2 = codepage1_wide.out(state.state, &tmp[0], &tmp[tmp.size()], tmp_next_from, to_cp1, to_end_cp1, to_next_cp1);
            if (res2 != std::codecvt_base::ok && res2 != std::codecvt_base::partial)
                  return res2;

            if (res1 == std::codecvt_base::partial || res2 == std::codecvt_base::partial)
                  return std::codecvt_base::partial;

            return std::codecvt_base::ok;
      }

      codecvt_bn::result codecvt_bn::do_unshift(tlib_mbstate_t& state,
          char* to_cp2, char* to_end_cp2, char*& to_next_cp2) const
      {
            return wide_codepage2.unshift(state.state, to_cp2, to_end_cp2, to_next_cp2);
      }

      int codecvt_bn::do_length(tlib_mbstate_t& state,
          const char* from_cp2, const char* from_end_cp2, std::size_t max)
      {
            return wide_codepage2.length(state.state, from_cp2, from_end_cp2, max);
      }

      int codecvt_bn::do_encoding() const noexcept
      {
            return wide_codepage2.encoding();
      }

      bool codecvt_bn::do_always_noconv() const noexcept
      {
            return codepage1_wide.always_noconv() || wide_codepage2.always_noconv();
      }

      int codecvt_bn::do_max_length() const noexcept
      {
            return wide_codepage2.max_length();
      }

      //-----------------------------------------------
      using codecvt_bnw = codecvt_bynames<char16_t, char, tlib_mbstate_t>;

      codecvt_bnw::result codecvt_bnw::do_out(
          tlib_mbstate_t& state,
          const char16_t* from, const char16_t* from_end, const char16_t*& from_next,
          char* to, char* to_end, char*& to_next) const
      {
#ifdef _WIN32
            std::cout << "Iroro:";                  
            return utf16_char.out(state.state, (wchar_t*)from, (wchar_t*)from_end, (const wchar_t*&)from_next, to, to_end, to_next);
#elif __linux__
            return utf16_utf8.out(state.state, from, from_end, from_next, to, to_end, to_next);
#endif
      }

      codecvt_bnw::result codecvt_bnw::do_in(
          tlib_mbstate_t& state,
          const char* from, const char* from_end, const char*& from_next,
          char16_t* to, char16_t* to_end, char16_t*& to_next) const
      {
#ifdef _WIN32
            std::cout << "Iroro:";                  
            return utf16_char.in(state.state, from, from_end, from_next, (wchar_t*)to, (wchar_t*)to_end, (wchar_t*&)to_next);
#elif __linux__
            return utf16_utf8.in(state.state, from, from_end, from_next, to, to_end, to_next);
#endif
      }

      codecvt_bnw::result codecvt_bnw::do_unshift(
          tlib_mbstate_t& state,
          char* to, char* to_end, char*& to_next) const
      {
#ifdef _WIN32
            std::cout << "Uroro:";
            return utf16_char.unshift(state.state, to, to_end, to_next);
#elif __linux__
            return utf16_utf8.unshift(state.state, to, to_end, to_next);
#endif
      }

      int codecvt_bnw::do_length(
          tlib_mbstate_t& state,
          const char* from, const char* from_end, std::size_t max)
      {
#ifdef _WIN32
            return utf16_char.length(state.state, from, from_end, max);
#elif __linux__
            return utf16_utf8.length(state.state, from, from_end, max);
#endif
      }

      int codecvt_bnw::do_encoding() const noexcept
      {
#ifdef _WIN32
            return utf16_char.encoding();
#elif __linux__
            return utf16_utf8.encoding();
#endif
      }

      bool codecvt_bnw::do_always_noconv() const noexcept
      {
#ifdef _WIN32
            return utf16_char.always_noconv();
#elif __linux__
            return utf16_utf8.always_noconv();
#endif
      }

      int codecvt_bnw::do_max_length() const noexcept
      {
#ifdef _WIN32
            return utf16_char.max_length();
#elif __linux__
            return utf16_utf8.max_length();
#endif
      }
      //-----------------------------------------------

      namespace libinternal
      {
            template <class _Codecvt,
                class _Elem = wchar_t,
                class _Traits = char_traits<_Elem>>
            struct wbuffer_sync_convert : public wbuffer_convert<_Codecvt, _Elem, _Traits>
            {
                  using base = wbuffer_convert<_Codecvt, _Elem, _Traits>;

                  template <class... Args>
                  wbuffer_sync_convert(Args&&... args) : wbuffer_convert<_Codecvt, _Elem, _Traits>(forward<Args>(args)...)
                  {
                  }

                  virtual streamsize xsputn(const _Elem* s, streamsize count)
                  {
                        streamsize result = base::xsputn(s, count);
                        this->overflow(_Traits::eof());
                        return result;
                  }
            };

      }

      //   String converter wchar_t <--> char  (UTF-16  <--> ProgramCodePage )
      thread_local std::wstring_convert<codecvt_w_o> strconvert_w_codepage(new codecvt_w_o(GetLocaleNameProgram()));

      //   String converter char16_t <--> char8_t  (UTF-16  <--> UTF-8)
      thread_local std::wstring_convert<codecvt_u16_u8, char16_t> strconvert_u16_u8(new codecvt_u16_u8(CodePageUTF8));

      //   String converter wchar_t <--> char8_t   (Wide  <--> UTF-8)
      thread_local std::wstring_convert<codecvt_w_u8> strconvert_w_u8(new codecvt_w_u8(CodePageUTF8));


      //   Convertering buffer char16_t --> char8_t  (UTF-16  --> UTF-8)
      static libinternal::wbuffer_sync_convert<codecvt_u16_u8, char16_t> _sync_in_bufferconvert_u16_u8(cin.rdbuf(), new codecvt_u16_u8(CodePageUTF8));
      static libinternal::wbuffer_sync_convert<codecvt_u16_u8, char16_t> _sync_out_bufferconvert_u16_u8(cout.rdbuf(), new codecvt_u16_u8(CodePageUTF8));
      static libinternal::wbuffer_sync_convert<codecvt_u16_u8, char16_t> _sync_err_bufferconvert_u16_u8(cerr.rdbuf(), new codecvt_u16_u8(CodePageUTF8));
      static libinternal::wbuffer_sync_convert<codecvt_u16_u8, char16_t> _sync_log_bufferconvert_u16_u8(clog.rdbuf(), new codecvt_u16_u8(CodePageUTF8));


      std::wbuffer_convert<codecvt_u16_u8, char16_t>& in_bufferconvert_u16_u8 = _sync_in_bufferconvert_u16_u8;
      std::wbuffer_convert<codecvt_u16_u8, char16_t>& out_bufferconvert_u16_u8 = _sync_out_bufferconvert_u16_u8;
      std::wbuffer_convert<codecvt_u16_u8, char16_t>& err_bufferconvert_u16_u8 = _sync_err_bufferconvert_u16_u8;
      std::wbuffer_convert<codecvt_u16_u8, char16_t>& log_bufferconvert_u16_u8 = _sync_log_bufferconvert_u16_u8;

      //   Convertering buffer wchar_t --> char8_t  (UTF-32  --> UTF-8)
      static libinternal::wbuffer_sync_convert<codecvt_w_u8, wchar_t> _sync_in_bufferconvert_w_u8(cin.rdbuf(), new codecvt_w_u8(CodePageUTF8));
      static libinternal::wbuffer_sync_convert<codecvt_w_u8, wchar_t> _sync_out_bufferconvert_w_u8(cout.rdbuf(), new codecvt_w_u8(CodePageUTF8));
      static libinternal::wbuffer_sync_convert<codecvt_w_u8, wchar_t> _sync_err_bufferconvert_w_u8(cerr.rdbuf(), new codecvt_w_u8(CodePageUTF8));
      static libinternal::wbuffer_sync_convert<codecvt_w_u8, wchar_t> _sync_log_bufferconvert_w_u8(clog.rdbuf(), new codecvt_w_u8(CodePageUTF8));


      std::wbuffer_convert<codecvt_w_u8, wchar_t>& in_bufferconvert_w_u8 = _sync_in_bufferconvert_w_u8;
      std::wbuffer_convert<codecvt_w_u8, wchar_t>& out_bufferconvert_w_u8 = _sync_out_bufferconvert_w_u8;
      std::wbuffer_convert<codecvt_w_u8, wchar_t>& err_bufferconvert_w_u8 = _sync_err_bufferconvert_w_u8;
      std::wbuffer_convert<codecvt_w_u8, wchar_t>& log_bufferconvert_w_u8 = _sync_log_bufferconvert_w_u8;

      //   Convertering buffer char <--> char  (Program code page <--> Console code page)
      static libinternal::wbuffer_sync_convert<codecvt_o_n, char>
          _sync_in_bufferconvert_programCP_consoleCP(cin.rdbuf(), new codecvt_o_n(GetLocaleNameProgram(), GetLocaleNameConsole()));

      static libinternal::wbuffer_sync_convert<codecvt_o_n, char>
          _sync_out_bufferconvert_programCP_consoleCP(cout.rdbuf(), new codecvt_o_n(GetLocaleNameProgram(), GetLocaleNameConsole()));

      static libinternal::wbuffer_sync_convert<codecvt_o_n, char>
          _sync_err_bufferconvert_programCP_consoleCP(cerr.rdbuf(), new codecvt_o_n(GetLocaleNameProgram(), GetLocaleNameConsole()));

      static libinternal::wbuffer_sync_convert<codecvt_o_n, char>
          _sync_log_bufferconvert_programCP_consoleCP(clog.rdbuf(), new codecvt_o_n(GetLocaleNameProgram(), GetLocaleNameConsole()));

      std::wbuffer_convert<codecvt_o_n, char>& in_bufferconvert_programCP_consoleCP = _sync_in_bufferconvert_programCP_consoleCP;
      std::wbuffer_convert<codecvt_o_n, char>& out_bufferconvert_programCP_consoleCP = _sync_out_bufferconvert_programCP_consoleCP;
      std::wbuffer_convert<codecvt_o_n, char>& err_bufferconvert_programCP_consoleCP = _sync_err_bufferconvert_programCP_consoleCP;
      std::wbuffer_convert<codecvt_o_n, char>& log_bufferconvert_programCP_consoleCP = _sync_log_bufferconvert_programCP_consoleCP;

      //   Convertering buffer char16_t <--> char (UTF-16  <-->  Console code page/UTF-8) need for Windows and Linux
      static libinternal::wbuffer_sync_convert<codecvt_u16_o, char16_t> _sync_in_bufferconvert_u16_o(cin.rdbuf(), new codecvt_u16_o(GetLocaleNameConsole()));
      static libinternal::wbuffer_sync_convert<codecvt_u16_o, char16_t> _sync_out_bufferconvert_u16_o(cout.rdbuf(), new codecvt_u16_o(GetLocaleNameConsole()));
      static libinternal::wbuffer_sync_convert<codecvt_u16_o, char16_t> _sync_err_bufferconvert_u16_o(cerr.rdbuf(), new codecvt_u16_o(GetLocaleNameConsole()));
      static libinternal::wbuffer_sync_convert<codecvt_u16_o, char16_t> _sync_log_bufferconvert_u16_o(clog.rdbuf(), new codecvt_u16_o(GetLocaleNameConsole()));


      std::wbuffer_convert<codecvt_u16_o, char16_t>& in_bufferconvert_u16_o = _sync_in_bufferconvert_u16_o;
      std::wbuffer_convert<codecvt_u16_o, char16_t>& out_bufferconvert_u16_o = _sync_out_bufferconvert_u16_o;
      std::wbuffer_convert<codecvt_u16_o, char16_t>& err_bufferconvert_u16_o = _sync_err_bufferconvert_u16_o;
      std::wbuffer_convert<codecvt_u16_o, char16_t>& log_bufferconvert_u16_o = _sync_log_bufferconvert_u16_o;


}
