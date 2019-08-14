/*                               __
  __ _  ___ ____________  ___   / /
 /  ' \/ _ `/ __/ __/ _ \(_-<_ / _ \
/_/_/_/\_,_/\__/_/  \___/___(_)_//_/
 * Macros to allow fast and convenient development
 * Drew O'Malley
 */
#ifndef MACROS_H
#define MACROS_H

// DEBUG or NDEBUG must be defined, but not both
#if (defined(DEBUG) || defined(_DEBUG)) \
 && (defined(NDEBUG) || defined(_NDEBUG))
#	 error Both DEBUG and NDEBUG are defined.
#elif !defined(DEBUG)  && !defined(_DEBUG) \
   && !defined(NDEBUG) && !defined(_NDEBUG)
#	 error Neither DEBUG or NDEBUG are defined.
#endif

// The only time we switch to debug is when asked. NDEBUG or {nothing} results
// in release build (fewer surprises at runtime).
#if defined(DEBUG) || defined(_DEBUG)
# define BUILD_DEBUG 1
#else
# define BUILD_RELEASE 1
#endif

/* hack: use instead of 0 to avoid w4127: "Cond. expr. is constant" */
#define MACRO_FALSE (__LINE__ == -1)

/* Use to wrap functions that require c linkage */
#ifdef __cplusplus
#	define C_LINK_START extern"c"{
#	define C_LINK_END }
#else
#	define C_LINK_START
#	define C_LINK_END
#endif

/* Math macros */
#define qmin(l, r) ((l)<=(r) ?(l) :(r))
#define qmax(l, r) ((l)>=(r) ?(l) :(r))
#define qclamp(min, x, max) qmax(qmin(x, max), min)
#define qis_pow2(v) (((v) != 0) && ((v) & ((v)-1)) == 0)

/* Memory alignment */
enum { Q_CACHE_LINE = 64 }; /* All modern platforms have 64byte cacheline */
#define qalign_down(v, a) \
	((v) & ~((a)-1))
#define qalign_up(n, a) \
	qalign_down((n) + (a) - 1, (a))
#define qalign_ptr_down(p, a) \
	((void *)qalign_down((uintptr_t)(p), (a)))
#define qalign_ptr_up(p, a) \
	((void *)qalign_up((uintptr_t)(p), (a)))

#endif
