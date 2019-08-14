/*                               __
  __ _  ___ ____________  ___   / /
 /  ' \/ _ `/ __/ __/ _ \(_-<_ / _ \
/_/_/_/\_,_/\__/_/  \___/___(_)_//_/
 * Macros to allow fast and convenient development
 * Drew O'Malley
 */
#ifndef MACROS_H
#define MACROS_H

#define qmin(l, r) ((l)<=(r) ?(l) :(r))
#define qmax(l, r) ((l)>=(r) ?(l) :(r))
#define qclamp(min, x, max) qmax(qmin(x, max), min)
#define qis_pow2(v) (((v) != 0) && ((v) & ((v)-1)) == 0)

/* Memory alignment */
#define qalign_down(v, a) \
	((v) & ~((a)-1))
#define qalign_up(n, a) \
	qalign_down((n) + (a) - 1, (a))
#define qalign_ptr_down(p, a) \
	((void *)qalign_down((uintptr_t)(p), (a)))
#define qalign_ptr_up(p, a) \
	((void *)qalign_up((uintptr_t)(p), (a)))


#endif
