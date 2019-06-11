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

