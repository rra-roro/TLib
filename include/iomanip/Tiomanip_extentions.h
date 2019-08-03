#pragma once

#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <linux/linux.h>
#endif

#include <iomanip>
#include <Tstdlib.h>
#include <Ttype.h>


namespace tlib
{
      //========================================================================================
      // Манипулятор     put_intger_by_radix()
      // для вывода в поток числа в произвольной системе счисления
      // value   - конвертируемое число
      // radix   - требуемое основание системы счисления
      // group   - разбивать ли число на групы символов (скажем по 8 бит в 2-ой сист счисл)
      //           и задаем количество символов в группе.
      // После того, как будет произведена запись в поток, поток сбросит ширину поля
      // (поток вызовет iostr.width(0)), таким образом ширину поля нужно выставлять
      // перед каждой записью числа в поток, в нашем случае перед каждым вызовом put_intger_by_radix().
      // Например:
      //    1)      MemBuffer << setw(2)<< setfill('0');
      //            MemBuffer << put_intger_by_radix(0xf,16) << "\t"
      // 	                  << put_intger_by_radix(0xf,16) << "\t";      Ошибка!!!
      //
      //    2)      MemBuffer << setfill('0');
      //            MemBuffer << setw(2) << put_intger_by_radix(0xf,16) << "\t"
      // 	                  << setw(2) << put_intger_by_radix(0xf,16) << "\t";      Ok!!!

      template <class T>
      class put_intger_by_radix
      {
            T value;
            int radix;
            int group;
            bool IsSigned;

        public:
            template <typename = std::enable_if_t<std::is_integral_v<T>>>
            put_intger_by_radix(T value, int radix, int group = 0) : value(value), radix(radix), group(group),
                                                                 IsSigned(std::is_signed_v<T> && (value < 0))
            {
            }

            template <class _Elem>
            friend inline std::basic_ostream<_Elem>& operator<<(std::basic_ostream<_Elem>& os, const put_intger_by_radix& cr)
            {
                  return cr.C_Radix(os);
            }

        private:
            template <typename _Elem>
            std::basic_ostream<_Elem>& C_Radix(std::basic_ostream<_Elem>& iostr) const
            {
                  if (radix < 2 || radix > 36)
                  {
                        iostr << TemplateTypeOfStr("Error Radix", _Elem);
                        return iostr;
                  }

                  int FlagShowbase = iostr.flags() & std::ios::showbase;
                  int FlagUppercase = iostr.flags() & std::ios::uppercase;

                  std::basic_string<_Elem> IntegerStr = get_str_from<_Elem>(value, radix);

                  if (radix > 10 && FlagUppercase)
                        IntegerStr = get_upper_str(IntegerStr);

                  // вычислим количество нулей необходимых для дополнения числа до ширины поля
                  if (IsSigned)
                        iostr << setw(0);
                  std::streamsize Width = iostr.width() - IntegerStr.length();

                  if (Width > 0 && !IsSigned)
                  {
                        IntegerStr = std::basic_string<_Elem>((unsigned int)Width, _T('0')) + IntegerStr;
                  }

                  if (group != 0)
                  {
                        size_t i = IntegerStr.length() - group;
                        for (; i < IntegerStr.length() && i != 0; i -= group) //проверяем, чтобы i не переполнилось при вычитани,
                        {                                                     //т.е. не стало меньше или равно 0
                              if (FlagShowbase)
                              {
                                    switch (radix)
                                    {
                                    case 2:
                                          IntegerStr.insert(i, TemplateTypeOfStr("b ", _Elem));
                                          break;
                                    case 8:
                                          IntegerStr.insert(i, TemplateTypeOfStr(" 0", _Elem));
                                          break;
                                    case 16:
                                          IntegerStr.insert(i, TemplateTypeOfStr(" 0x", _Elem));
                                          break;
                                    default:
                                          IntegerStr.insert(i, TemplateTypeOfStr(" ", _Elem));
                                    }
                              }
                              else
                                    IntegerStr.insert(i, TemplateTypeOfStr(" ", _Elem));
                        }
                  }

                  if (FlagShowbase)
                        switch (radix)
                        {
                        case 2:
                              IntegerStr = IntegerStr + TemplateTypeOfStr("b", _Elem);
                              break;
                        case 8:
                              IntegerStr = TemplateTypeOfStr("0", _Elem) + IntegerStr;
                              break;
                        case 16:
                              IntegerStr = TemplateTypeOfStr("0x", _Elem) + IntegerStr;
                        }

                  return iostr << IntegerStr;
            }
      };

      extern template std::basic_ostream<char>& put_intger_by_radix<char>::C_Radix(std::basic_ostream<char>& iostr) const;
      extern template std::basic_ostream<wchar_t>& put_intger_by_radix<wchar_t>::C_Radix(std::basic_ostream<wchar_t>& iostr) const;
      extern template std::basic_ostream<char16_t>& put_intger_by_radix<char16_t>::C_Radix(std::basic_ostream<char16_t>& iostr) const;

      //========================================================================================
      // Манипулятор put_guid() Превращает GUID в строку
      class put_guid
      {
            const GUID& guid;

        public:
            put_guid(const GUID& guid) : guid(guid){};

            template <class _Elem>
            friend inline std::basic_ostream<_Elem>& operator<<(std::basic_ostream<_Elem>& os, const put_guid& oG)
            {
                  return oG.PrintGuid(os);
            }

        private:
            template <typename _Elem>
            std::basic_ostream<_Elem>& PrintGuid(std::basic_ostream<_Elem>& iostr) const
            {
                  std::streamsize W = iostr.width();
                  _Elem F = iostr.fill();
                  iostr << setfill<_Elem>(TemplateTypeOfCh('0', _Elem));

                  iostr << setw(8) << put_intger_by_radix(guid.Data1, 16) << TemplateTypeOfCh('-', _Elem);
                  iostr << setw(4) << put_intger_by_radix(guid.Data2, 16) << TemplateTypeOfCh('-', _Elem);
                  iostr << setw(4) << put_intger_by_radix(guid.Data3, 16) << TemplateTypeOfCh('-', _Elem);
                  for (int i = 0; i < 8; i++)
                  {
                        iostr << setw(2) << put_intger_by_radix(((unsigned char*)&guid.Data4)[i], 16);
                        if (i == 1)
                              iostr << TemplateTypeOfCh('-', _Elem);
                  }                        

                  iostr.fill(F);
                  iostr.width(W);
                  return iostr;
            }
      };

      extern template std::basic_ostream<char>& put_guid::PrintGuid(std::basic_ostream<char>& iostr) const;
      extern template std::basic_ostream<wchar_t>& put_guid::PrintGuid(std::basic_ostream<wchar_t>& iostr) const;
      extern template std::basic_ostream<char16_t>& put_guid::PrintGuid(std::basic_ostream<char16_t>& iostr) const;

}
