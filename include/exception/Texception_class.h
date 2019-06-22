#pragma once

#pragma once

#include <system_error>
#include <Tstring.h>
#include <Tiomanip.h>
#include <Tiostream.h>
#include <Ttype.h>

#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

namespace tlib
{
      namespace ex
      {
            ////////////////////////////////////////////////////////////////////////
            //
            //   _E(str) -  Макрос принимающий текст исключения и дополняющий его строкой и именем исходного файла
            //

            struct source_error_position
            {
                  const TCHAR* file_name;
                  int line_number;
                  const TCHAR* fn_name;
            };

#define _SRC_LINE_ERR_                                      \
      {                                                     \
            _T(##__FILE__##)                                \
            ##, ##__LINE__##, ##_T(##__PRETTY_FUNCTION__##) \
      }

#define _SLE_ _SRC_LINE_ERR_

#define _E(str) str##, ##_SRC_LINE_ERR_


            ////////////////////////////////////////////////////////////////////////
            //
            //   Базовые классы всех исключений этой библиотеки
            //
            //class application_error_base : public std::logic_error
            //{
            //  public:
            //      virtual const tlib::tstring get_exception_name(void) const = 0; // Возвращает истинное имя класса исключения

            //      virtual const tlib::tstring get_user_error_msg() const = 0;     // Возвращает текст сообщения, которое было задано
            //                                                                      // пользователем при возбуждении исключения

            //      virtual const tlib::tstring get_all_error_msg() const = 0;      // Возвращает всю информацию об исключении

            //      template <class _Elem>
            //      virtual const _Elem* what() const = 0; // Унаследованная ф-ия, которая в нашем случае будет
            //                                                                      // возвращать, то же что и GetExceptionAllInfoMsg()
            //};

            ////////////////////////////////////////////////////////////////////////
            //
            //   EExceptionCPP -  Базовый класс для всех С++ исключений.
            //                    Описывает простое текстовое пользовательское сообщение об ошибке,
            //                    а также Имя Файла исходника и Номер строки, где исключение произошло,
            //                    так же возможна передача кода ошибки.
            //                    Код ошибки - это код возвращаемый ф-ией GetLastError().
            //
            //                    Используется совместно с макросом #define _E(str)
            //
            //                    Например так:
            //                                   throw EExceptionCPP(_E("А тут у нас ошибка"));
            //                                   throw EExceptionCPP(_E("А тут у нас ошибка. Номер:"), 10);

            class api_error : public std::system_error
            {
                  const tlib::tstring err_msg;
                  source_error_position err_pos;

                  std::string tmp_all_msg;

              public:
                  using code_t = unsigned long long;

                  // Конструкторы
                  template <class T,
                      class _Elem = get_underlying_char_t<T>,
                      typename = std::enable_if_t<std::is_same_v<_Elem, TCHAR>>>
                  api_error(T&& what_arg, source_error_position&& err_pos) : std::system_error(0), err_msg(std::forward<T>(what_arg)), err_pos(err_pos)
                  {
                  }

                  api_error(std::error_code ec, source_error_position&& err_pos) : std::system_error(ec), err_msg(), err_pos(err_pos)
                  {
                  }

                  template <class T,
                      class _Elem = get_underlying_char_t<T>,
                      typename = std::enable_if_t<std::is_same_v<_Elem, TCHAR>>>
                  api_error(std::error_code ec, T&& what_arg, source_error_position&& err_pos) : std::system_error(ec), err_msg(std::forward<T>(what_arg)), err_pos(err_pos)
                  {
                  }

                  template <class I,
                      typename = std::enable_if_t<std::is_integral_v<I>>>
                  api_error(I code, const std::error_category& errcat, source_error_position&& err_pos) : std::system_error(code, errcat), err_msg(), err_pos(err_pos)
                  {
                  }

                  template <class I, class T,
                      typename = std::enable_if_t<std::is_integral_v<I>>,
                      class _Elem = get_underlying_char_t<T>,
                      typename = std::enable_if_t<std::is_same_v<_Elem, TCHAR>>>
                  api_error(I code, const std::error_category& errcat, T&& what_arg, source_error_position&& err_pos) : std::system_error(code, errcat), err_msg(std::forward<T>(what_arg)), err_pos(err_pos)
                  {
                  }

                  virtual const tlib::tstring get_code_error_msg() const
                  {
                        return str2tstr(code().message());
                  }

                  virtual const tlib::tstring get_exception_name() const
                  {
                        return str2tstr(code().category().name());
                  }

                  virtual const tlib::tstring get_user_error_msg() const
                  {
                        return err_msg;
                  }

                  virtual const tlib::tstring get_all_error_msg() const
                  {
                        tlib::tstringstream msg;

                        msg << _T("Source file name: ");
                        msg << err_pos.file_name << _T(":(") << std::dec << err_pos.line_number << _T(")")
                            << _T("\n");

                        msg << _T("Function: ");
                        msg << err_pos.fn_name << _T("\n\n");

                        msg << _T("Error domain: ") << get_exception_name() << _T("\n");

                        int error_code = code().value();

                        if (error_code)
                        {
                              msg << _T("Last error: ");
                              msg << _T("0x") << tlib::uppercase << setfill(_T('0')) << internal;
                              msg << setw(8) << hex << error_code << _T("(") << dec << error_code << _T(")\n");

                              msg << _T("Error system message: ");
                              msg << get_code_error_msg() << _T("\n");
                        }

                        if (get_user_error_msg().size())
                        {
                              msg << _T("Error user message: ");
                              msg << get_user_error_msg() << _T("\n");
                        }

                        return msg.str();
                  }

                  template <class _Elem,
                      class string_t = std::basic_string<_Elem>>
                  const string_t what() const noexcept
                  {
                        return tstr_templateStr<_Elem>(get_all_error_msg());
                  }

                  virtual const char* what() const noexcept
                  {
                        const_cast<api_error*>(this)->tmp_all_msg = tstr2str(get_all_error_msg());
                        return tmp_all_msg.c_str();
                  }
            };

            class throw_if_api_error : public api_error
            {
              public:
                  throw_if_api_error(source_error_position&& err_pos,
                      const std::error_category& errcat = std::generic_category()) : api_error(0, errcat, std::move(err_pos))
                  {
                  }

                  template <class T>
                  throw_if_api_error(T&& what_arg, source_error_position&& err_pos,
                      const std::error_category& errcat = std::generic_category()) : api_error(0, errcat,
                                                                                         std::forward<T>(what_arg),
                                                                                         std::move(err_pos))
                  {
                  }

                  void operator=(std::error_code ec)
                  {
                        if (ec)
                        {
                              const_cast<std::error_code&>(this->code()).assign(ec.value(), ec.category());
                              throw *this;
                        }
                  }

                  void operator=(int error)
                  {
                        if (error)
                        {
                              const_cast<std::error_code&>(this->code()).assign(error, this->code().category());
                              throw *this;
                        }
                  }
            };
      }

}
