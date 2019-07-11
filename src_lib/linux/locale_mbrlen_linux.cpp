#include <Tlocale.h>
#include <linux/locale_mbrlen_linux.h>
#include <gconv.h>
#include <sys/cdefs.h>
#include <stdio.h>
#include <cstdint>
#include <locale.h>
#include <dlfcn.h>
#include <cassert>
#include <cwchar>
#include <errno.h>
#include <cstring>

using namespace std;

struct gconv_fcts
{
      struct __gconv_step *towc;
      size_t towc_nsteps;
      struct __gconv_step *tomb;
      size_t tomb_nsteps;
};

/* Structure describing locale data in core for a category.  */
struct __locale_data
{
      const char *name;
      const char *filedata; /* Region mapping the file data.  */
      off_t filesize;       /* Size of the file (and the region).  */
      enum                  /* Flavor of storage used for those.  */
      {
            ld_malloced, /* Both are malloc'd.  */
            ld_mapped,   /* name is malloc'd, filedata mmap'd */
            ld_archive   /* Both point into mmap'd archive regions.  */
      } alloc;
      /* This provides a slot for category-specific code to cache data computed
     about this locale.  That code can set a cleanup function to deallocate
     the data.  */
      struct
      {
            void (*cleanup)(struct __locale_data *);
            union {
                  void *data;
                  struct lc_time_data *time;
                  const struct gconv_fcts *ctype;
            };
      } priv;
      unsigned int usage_count; /* Counter for users.  */
      int use_translit;         /* Nonzero if the mb*towv*() and wc*tomb()
                                   functions should use transliteration.  */
      unsigned int nstrings;    /* Number of strings below.  */
      union locale_data_value {
            const uint32_t *wstr;
            const char *string;
            unsigned int word; /* Note endian issues vs 64-bit pointers.  */
      } values __flexarr;      /* Items, usually pointers into `filedata'.  */
};

namespace tlib
{
      size_t
      __mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps, locale_t loc)
      {
            wchar_t buf[1];
            struct __gconv_step_data data;
            int status;
            size_t result;
            size_t dummy = 0;
            const unsigned char *inbuf, *endbuf;
            unsigned char *outbuf = (unsigned char *)(pwc ? pwc : buf);
            const struct gconv_fcts *fcts;
            /* Set information for this step.  */
            data.__invocation_counter = 0;
            data.__internal_use = 1;
            data.__flags = __GCONV_IS_LAST;
            data.__statep = ps;
            /* A first special case is if S is NULL.  This means put PS in the initial state.  */

            if (s == NULL)
            {
                  outbuf = (unsigned char *)buf;
                  s = "";
                  n = 1;
            }
            if (n == 0)
                  return (size_t)-2;
            /* Tell where we want the result.  */
            data.__outbuf = outbuf;
            data.__outbufend = outbuf + sizeof(wchar_t);
            /* Get the conversion functions.  */
           
            fcts = loc->__locales[LC_CTYPE]->priv.ctype;

            /* Do a normal conversion.  */
            inbuf = (const unsigned char *)s;
            endbuf = inbuf + n;
            if (__glibc_unlikely(endbuf < inbuf))
            {
                  endbuf = (const unsigned char *)~(uintptr_t)0;
                  if (endbuf == inbuf)
                  {
                        result = (size_t)-1;
                        errno = EILSEQ;
                        return result;
                  }                  
            }
            __gconv_fct fct = fcts->towc->__fct;
#ifdef PTR_DEMANGLE
            if (fcts->towc->__shlib_handle != NULL)
                  PTR_DEMANGLE(fct);
#endif
            status = DL_CALL_FCT(fct, (fcts->towc, &data, &inbuf, endbuf, NULL, &dummy, 0, 1));

            /* There must not be any problems with the conversion but illegal input
     characters.  The output buffer must be large enough, otherwise the
     definition of MB_CUR_MAX is not correct.  All the other possible
     errors also must not happen.  */
            assert(status == __GCONV_OK || status == __GCONV_EMPTY_INPUT || status == __GCONV_ILLEGAL_INPUT || status == __GCONV_INCOMPLETE_INPUT || status == __GCONV_FULL_OUTPUT);
            if (status == __GCONV_OK || status == __GCONV_EMPTY_INPUT || status == __GCONV_FULL_OUTPUT)
            {
                  if (data.__outbuf != (unsigned char *)outbuf && *(wchar_t *)outbuf == L'\0')
                  {
                        /* The converted character is the NUL character.  */
                        assert(mbsinit(data.__statep));
                        result = 0;
                  }
                  else
                        result = inbuf - (const unsigned char *)s;
            }
            else if (status == __GCONV_INCOMPLETE_INPUT)
                  result = (size_t)-2;
            else
            {
                  result = (size_t)-1;
                  errno = EILSEQ;
            }
            return result;
      }


      int mbrlen_l(const char *s, size_t n, std::mbstate_t *state, const tlib::locale &loc)
      {
            locale_t c_loc = newlocale(LC_ALL_MASK, loc.name().c_str(), (locale_t)0);

            int result;
            /* If S is NULL the function has to return null or not null
     depending on the encoding having a state depending encoding or
     not.  */
            if (s == NULL)
            {
                  const struct gconv_fcts *fcts;
                  /* Get the conversion functions.  */
                  fcts = c_loc->__locales[LC_CTYPE]->priv.ctype;
                  /* Reset the state.  */
                  memset(state, '\0', sizeof *state);
                  result = fcts->towc->__stateful;
            }
            else if (*s == '\0')
                  /* According to the ISO C 89 standard this is the expected behaviour.  */
                  result = 0;
            else
            {
                  memset(state, '\0', sizeof *state);
                  result = __mbrtowc(NULL, s, n, state, c_loc);
                  /* The `mbrtowc' functions tell us more than we need.  Fold the -1 and -2 result into -1.  */
                  if (result < 0)
                        result = -1;
            }
            return result;
      }

}
