
#ifndef MYLOG_API_H
#define MYLOG_API_H

#ifdef MYLOG_STATIC
#  define MYLOG_API
#  define MYLOG_NO_EXPORT
#else
#  ifndef MYLOG_API
#    ifdef myLog_EXPORTS
        /* We are building this library */
#      define MYLOG_API 
#    else
        /* We are using this library */
#      define MYLOG_API 
#    endif
#  endif

#  ifndef MYLOG_NO_EXPORT
#    define MYLOG_NO_EXPORT 
#  endif
#endif

#ifndef MYLOG_DEPRECATED
#  define MYLOG_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef MYLOG_DEPRECATED_EXPORT
#  define MYLOG_DEPRECATED_EXPORT MYLOG_API MYLOG_DEPRECATED
#endif

#ifndef MYLOG_DEPRECATED_NO_EXPORT
#  define MYLOG_DEPRECATED_NO_EXPORT MYLOG_NO_EXPORT MYLOG_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef MYLOG_NO_DEPRECATED
#    define MYLOG_NO_DEPRECATED
#  endif
#endif

#endif /* MYLOG_API_H */
