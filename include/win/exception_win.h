#pragma once

#include <Windows.h>
#include <Tstring.h>

namespace tlib::ex
{
      ////////////////////////////////////////////////////////////////////////
      //
      //   GetNTStatusMessage -  получает текст сообщения	NTStatus, читая его из NTDLL.DLL
      //                         если текст сообщения имеет параметры, их можно заполнить, передав их в
      //                         GetNTStatusMessage в порядке их следования в тексте сообщения.
      //GetNTStatusMessage(DWORD NTStatusMessage, ...);


      ////////////////////////////////////////////////////////////////////////
      //
      //   GetLastErrorMessage -  получает текст сообщения возвращаемого GetLastError, если текст сообщения
      //                          имеет параметры, их можно заполнить, передав их в GetLastErrorMessage
      //                          в порядке их следования в тексте сообщения.
      tstring get_last_error_msg(DWORD LastError, ...);

      ////////////////////////////////////////////////////////////////////////
      //
      //   get_hresult_error_msg -  получает текст сообщения соответствующего коду HRESULT
      //
      //tstring get_hresult_error_msg(HRESULT LastError);

      struct win32_error_code
      {
            explicit win32_error_code(DWORD e) noexcept : error(e) {}
            DWORD error;
      };

      struct hresult_code
      {
            explicit hresult_code(HRESULT e) noexcept : error(e) {}
            HRESULT error;
      };


      class win32_error_category_t : public std::error_category
      {
        public:
            /// Return a short descriptive name for the category.
            char const* name() const noexcept override final { return "Win32 API Error"; }

            /// Return what each error code means in text.
            std::string message(int c) const override final
            {
                  return tstr2str(get_last_error_msg(static_cast<DWORD>(c)));
            };
      };

      class hresult_category_t : public std::error_category
      {
        public:
            /// Return a short descriptive name for the category.
            char const* name() const noexcept override final { return "HRESULT Error"; }

            /// Return what each error code means in text.
            std::string message(int c) const override final
            {
                  // ToDo нужна специлизированная версия
                  //      tstring get_hresult_error_msg(HRESULT LastError);
                  //      которая дергает _com_error()
                  //      проблема в том, что это потянет за собой все множество библиотек COM
                  return tstr2str(get_last_error_msg(HRESULT_CODE(static_cast<HRESULT>(c))));
            };
      };

      /// Return a static instance of the custom category.
      extern const win32_error_category_t& win32_error_category();
      extern const hresult_category_t& hresult_category();

      // Overload the global make_error_code() free function with our
      // custom error. It will be found via ADL by the compiler if needed.
      inline std::error_code make_error_code(win32_error_code const& we)
      {
            return std::error_code(static_cast<int>(we.error), win32_error_category());
      };

      inline std::error_code make_error_code(hresult_code const& we)
      {
            return std::error_code(static_cast<int>(we.error), hresult_category());
      };

      /// Create an error_code from a Windows error code.
      inline std::error_code make_win32_error_code(DWORD e)
      {
            return make_error_code(win32_error_code(e));
      };

      /// Create an error_code from the last Windows error.
      inline std::error_code make_win32_error_code()
      {
            return make_win32_error_code(GetLastError());
      };

      /// Create an error_code from a Windows error code.
      inline std::error_code make_hresult_code(HRESULT e)
      {
            return make_error_code(hresult_code(e));
      };

      /// Create an error_code from the last Winsock error.
      inline std::error_code make_winsock_error_code()
      {
            return make_win32_error_code(WSAGetLastError());
      };

      class throw_if_winapi_error : public throw_if_api_error
      {
        public:
            throw_if_winapi_error(source_error_position&& err_pos,
                                 const std::error_category& errcat = win32_error_category()) : throw_if_api_error(std::move(err_pos), errcat)
            {
            }

            template <class T>
            throw_if_winapi_error(T&& what_arg, source_error_position&& err_pos,
                                  const std::error_category& errcat = win32_error_category()) : throw_if_api_error(std::forward<T>(what_arg),
                                                                                                                   std::move(err_pos), errcat )
            {
            }

            template <class I>
            void operator=(I state_code)
            {
                  if constexpr (std::is_same_v<I, std::error_code>)
                  {
                        *(throw_if_api_error*)this = state_code;
                  }
                  else if constexpr (std::is_same_v<I, BOOL>)
                  {
                        if (!state_code)
                        {
                              const_cast<std::error_code&>(this->code()).assign(GetLastError(), this->code().category());
                              throw *this;
                        }
                  }
                  else if constexpr (std::is_same_v<I, HANDLE>)
                  {
                        if (state_code == INVALID_HANDLE_VALUE)
                        {
                              const_cast<std::error_code&>(this->code()).assign(GetLastError(), this->code().category());
                              throw *this;
                        }
                  }
                  else if constexpr (std::is_same_v<I, DWORD>)
                  {
                        if (state_code != ERROR_SUCCESS)
                        {
                              const_cast<std::error_code&>(this->code()).assign(state_code, this->code().category());
                              throw *this;
                        }
                  }
                  else if constexpr (std::is_same_v<I, HRESULT>)
                  {
                        if (FAILED(state_code))
                        {
                              const_cast<std::error_code&>(this->code()).assign(state_code, hresult_category());
                              throw *this;
                        }
                  }
            }
      };
}

namespace std
{
      // Tell the C++ 11 STL metaprogramming that win32_error_code
      // is registered with the standard error code system.
      template <>
      struct is_error_code_enum<tlib::ex::win32_error_code> : std::true_type
      {
      };

      template <>
      struct is_error_code_enum<tlib::ex::hresult_code> : std::true_type
      {
      };
      
}
