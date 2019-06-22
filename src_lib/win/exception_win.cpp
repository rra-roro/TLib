#include <Windows.h>
#include <Tstring.h>
#include <Texception.h>

namespace tlib
{
      const ex::win32_error_category_t& ex::win32_error_category()
      {
            static ex::win32_error_category_t cat;
            return cat;
      };

      const ex::hresult_category_t& ex::hresult_category()
      {
            static ex::hresult_category_t cat;
            return cat;
      };

      ////////////////////////////////////////////////////////////////////////
      //
      //   GetLastErrorMessage -  получает текст сообщения возвращаемого GetLastError, если текст сообщения
      //                          имеет параметры, их можно заполнить, передав их в GetLastErrorMessage
      //                          в порядке их следования в тексте сообщения.

      tstring ex::get_last_error_msg(DWORD LastError, ...)
      {
            LPWSTR lpMsgBuf = NULL;
            va_list args = NULL;
            va_start(args, LastError);

            FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL, LastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPWSTR)&lpMsgBuf, 0, &args);
            tstring errMsg(wstr2tstr(lpMsgBuf));

            LocalFree(lpMsgBuf);
            va_end(args);
            return errMsg;
      }

}
