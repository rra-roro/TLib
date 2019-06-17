#pragma once

#include <locale>
#include <stdexcept>

//
//TODO: Check if the GNU libstdc++ std::codecvt template is fixed.
//
// The GNU libstdc++ std::codecvt template is broken for custom types
// but we have to derive from std::codecvt because std::basic_filebuf
// uses has_facet<codecvt<InternT>> and this is using a dynamic_cast.
// Therefore we have to add all custom types at the end of this file.
//

#define CODECVT_DEFINE_PURE(InternT, ExternT, StateT)                                                           \
      namespace std                                                                                             \
      {                                                                                                         \
            template <>                                                                                         \
            bool                                                                                                \
            codecvt<InternT, ExternT, StateT>::do_always_noconv(void) const noexcept                            \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::codecvt<" #InternT ", " #ExternT ", " #StateT ">::do_always_noconv"); \
            }                                                                                                   \
            template <>                                                                                         \
            int                                                                                                 \
            codecvt<InternT, ExternT, StateT>::do_encoding(void) const noexcept                                 \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::codecvt<" #InternT ", " #ExternT ", " #StateT ">::do_encoding");      \
            }                                                                                                   \
            template <>                                                                                         \
            codecvt_base::result                                                                                \
            codecvt<InternT, ExternT, StateT>::do_in(state_type&,                                               \
                extern_type const*, extern_type const*, extern_type const*&,                                    \
                intern_type*, intern_type*, intern_type*&) const                                                \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::codecvt<" #InternT ", " #ExternT ", " #StateT ">::do_in");            \
            }                                                                                                   \
            template <>                                                                                         \
            int                                                                                                 \
            codecvt<InternT, ExternT, StateT>::do_length(state_type&,                                           \
                extern_type const*, extern_type const*, size_t) const                                           \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::codecvt<" #InternT ", " #ExternT ", " #StateT ">::do_length");        \
            }                                                                                                   \
            template <>                                                                                         \
            int                                                                                                 \
            codecvt<InternT, ExternT, StateT>::do_max_length(void) const noexcept                               \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::codecvt<" #InternT ", " #ExternT ", " #StateT ">::do_max_length");    \
            }                                                                                                   \
            template <>                                                                                         \
            codecvt_base::result                                                                                \
            codecvt<InternT, ExternT, StateT>::do_out(state_type&,                                              \
                intern_type const*, intern_type const*, intern_type const*&,                                    \
                extern_type*, extern_type*, extern_type*&) const                                                \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::codecvt<" #InternT ", " #ExternT ", " #StateT ">::do_out");           \
            }                                                                                                   \
            template <>                                                                                         \
            codecvt_base::result                                                                                \
            codecvt<InternT, ExternT, StateT>::do_unshift(state_type&,                                          \
                extern_type*, extern_type*, extern_type*&) const                                                \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::codecvt<" #InternT ", " #ExternT ", " #StateT ">::do_unshift");       \
            }                                                                                                   \
      } /* namespace std */


#define CTYPE_DEFINE_PURE(CharT)                                                                                \
      namespace std                                                                                             \
      {                                                                                                         \
            template <>                                                                                         \
            bool                                                                                                \
            ctype<CharT>::do_is([[maybe_unused]] mask __m, [[maybe_unused]] CharT __c) const                    \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::ctype<" #CharT ">::do_is with 2 args");                               \
            }                                                                                                   \
            template <>                                                                                         \
            const CharT*                                                                                        \
            ctype<CharT>::do_is([[maybe_unused]] const CharT* __lo, [[maybe_unused]] const CharT* __hi,         \
                                [[maybe_unused]] mask* __vec) const                                             \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::ctype<" #CharT ">::do_is with 3 args");                               \
            }                                                                                                   \
            template <>                                                                                         \
            const CharT*                                                                                        \
            ctype<CharT>::do_scan_is([[maybe_unused]] mask __m, [[maybe_unused]] const CharT* __lo,             \
                                     [[maybe_unused]] const CharT* __hi) const                                  \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::ctype<" #CharT ">::do_scan_is");                                      \
            }                                                                                                   \
            template <>                                                                                         \
            const CharT*                                                                                        \
            ctype<CharT>::do_scan_not([[maybe_unused]] mask __m, [[maybe_unused]] const CharT* __lo,            \
                                      [[maybe_unused]] const CharT* __hi) const                                 \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::ctype<" #CharT ">::do_scan_not");                                     \
            }                                                                                                   \
            template <>                                                                                         \
            CharT                                                                                               \
            ctype<CharT>::do_toupper([[maybe_unused]] CharT __c) const                                          \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::ctype<" #CharT ">::do_toupper with 1 arg");                           \
            }                                                                                                   \
            template <>                                                                                         \
            const CharT*                                                                                        \
            ctype<CharT>::do_toupper([[maybe_unused]] CharT* __lo, [[maybe_unused]] const CharT* __hi) const    \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::ctype<" #CharT ">::do_toupper with 2 args");                          \
            }                                                                                                   \
            template <>                                                                                         \
            CharT                                                                                               \
            ctype<CharT>::do_tolower([[maybe_unused]] CharT __c) const                                          \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::ctype<" #CharT ">::do_tolower with 1 arg");                           \
            }                                                                                                   \
            template <>                                                                                         \
            const CharT*                                                                                        \
            ctype<CharT>::do_tolower([[maybe_unused]] CharT* __lo, [[maybe_unused]] const CharT* __hi) const    \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::ctype<" #CharT ">::do_tolower with 2 args");                          \
            }                                                                                                   \
            template <>                                                                                         \
            CharT                                                                                               \
            ctype<CharT>::do_widen([[maybe_unused]] char __c) const                                             \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::ctype<" #CharT ">::do_widen with 1 arg");                             \
            }                                                                                                   \
            template <>                                                                                         \
            const char*                                                                                         \
            ctype<CharT>::do_widen([[maybe_unused]] const char* __lo, [[maybe_unused]] const char* __hi,        \
                                   [[maybe_unused]] CharT* __to) const                                          \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::ctype<" #CharT ">::do_widen with 3 args");                            \
            }                                                                                                   \
            template <>                                                                                         \
            char                                                                                                \
            ctype<CharT>::do_narrow([[maybe_unused]] CharT __c, [[maybe_unused]] char __dfault) const           \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::ctype<" #CharT ">::do_narrow with 2 args");                           \
            }                                                                                                   \
            template <>                                                                                         \
            const CharT*                                                                                        \
            ctype<CharT>::do_narrow([[maybe_unused]] const CharT* __lo, [[maybe_unused]] const CharT* __hi,     \
                                    [[maybe_unused]] char __dfault, [[maybe_unused]] char* __to) const          \
            {                                                                                                   \
                  throw logic_error("pure virtual method called"                                                \
                                    "std::ctype<" #CharT ">::do_narrow with 4 args");                           \
            }                                                                                                   \
            template <>                                                                                         \
            std::ctype<char16_t>::~ctype()                                                                      \
            {                                                                                                   \
            }                                                                                                   \
      } /* namespace std */
