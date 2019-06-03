/*
*
*/
#include "stdafx.h"
#include <Tlocale.h>
#ifdef __linux__
//#include <linux/stdio_sync_filebuf.h>
#include <ext/stdio_sync_filebuf.h>
#endif


using namespace std;

namespace tlib
{
      namespace internal
      {
            template <class _Codecvt,
                      class _Elem = wchar_t,
                      class _Traits = char_traits<_Elem>>
            struct wbuffer_sync_convert : public wbuffer_convert<_Codecvt, _Elem, _Traits>
            {
                  using base = wbuffer_convert<_Codecvt, _Elem, _Traits>;

                  template <class... Args>
                  wbuffer_sync_convert(Args&&... args) : wbuffer_convert<_Codecvt, _Elem, _Traits>(std::forward<Args>(args)...)
                  {
                  }

                  virtual std::streamsize xsputn(const _Elem* s, std::streamsize count)
                  {
                        std::streamsize result = base::xsputn(s, count);
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
      thread_local internal::wbuffer_sync_convert<codecvt_u16_u8, char16_t> _sync_in_bufferconvert_u16_u8(cin.rdbuf(), new codecvt_u16_u8(CodePageUTF8));
      thread_local internal::wbuffer_sync_convert<codecvt_u16_u8, char16_t> _sync_out_bufferconvert_u16_u8(cout.rdbuf(), new codecvt_u16_u8(CodePageUTF8));
      thread_local internal::wbuffer_sync_convert<codecvt_u16_u8, char16_t> _sync_err_bufferconvert_u16_u8(cerr.rdbuf(), new codecvt_u16_u8(CodePageUTF8));
      thread_local internal::wbuffer_sync_convert<codecvt_u16_u8, char16_t> _sync_log_bufferconvert_u16_u8(clog.rdbuf(), new codecvt_u16_u8(CodePageUTF8));


      thread_local std::wbuffer_convert<codecvt_u16_u8, char16_t>& in_bufferconvert_u16_u8 = _sync_in_bufferconvert_u16_u8;
      thread_local std::wbuffer_convert<codecvt_u16_u8, char16_t>& out_bufferconvert_u16_u8 = _sync_out_bufferconvert_u16_u8;
      thread_local std::wbuffer_convert<codecvt_u16_u8, char16_t>& err_bufferconvert_u16_u8 = _sync_err_bufferconvert_u16_u8;
      thread_local std::wbuffer_convert<codecvt_u16_u8, char16_t>& log_bufferconvert_u16_u8 = _sync_log_bufferconvert_u16_u8;

      //   Convertering buffer wchar_t --> char8_t  (UTF-16  --> UTF-8)
      thread_local internal::wbuffer_sync_convert<codecvt_w_u8, wchar_t> _sync_in_bufferconvert_w_u8(cin.rdbuf(), new codecvt_w_u8(CodePageUTF8));
      thread_local internal::wbuffer_sync_convert<codecvt_w_u8, wchar_t> _sync_out_bufferconvert_w_u8(cout.rdbuf(), new codecvt_w_u8(CodePageUTF8));
      thread_local internal::wbuffer_sync_convert<codecvt_w_u8, wchar_t> _sync_err_bufferconvert_w_u8(cerr.rdbuf(), new codecvt_w_u8(CodePageUTF8));
      thread_local internal::wbuffer_sync_convert<codecvt_w_u8, wchar_t> _sync_log_bufferconvert_w_u8(clog.rdbuf(), new codecvt_w_u8(CodePageUTF8));


      thread_local std::wbuffer_convert<codecvt_w_u8, wchar_t>& in_bufferconvert_w_u8 = _sync_in_bufferconvert_w_u8;
      thread_local std::wbuffer_convert<codecvt_w_u8, wchar_t>& out_bufferconvert_w_u8 = _sync_out_bufferconvert_w_u8;
      thread_local std::wbuffer_convert<codecvt_w_u8, wchar_t>& err_bufferconvert_w_u8 = _sync_err_bufferconvert_w_u8;
      thread_local std::wbuffer_convert<codecvt_w_u8, wchar_t>& log_bufferconvert_w_u8 = _sync_log_bufferconvert_w_u8;

}
