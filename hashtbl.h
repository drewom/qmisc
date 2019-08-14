/* __            __   __  __   __  __
  / /  ___ ____ / /  / /_/ /  / / / /
 / _ \/ _ `(_-</ _ \/ __/ _ \/ / / _ \
/_//_/\_,_/___/_//_/\__/_.__/_(_)_//_/
 * hash table. Zeroed is initialised and automatically resizes at 50%
 * residency. You supply the hash function. Hash of zero is a special
 * case.
 *
 * Drew O'Malley
 */
#ifndef HASHTBL_H
#define HASHTBL_H

#include <qmisc/macros.h>

#include <stdint.h>

#ifndef qmalloc
#	include <stdlib.h>
#	define qmalloc malloc
#endif
#ifndef qcalloc
#	include <stdlib.h>
#	define qcalloc calloc
#endif
#ifndef qfree
#	include <stdlib.h>
#	define qfree free
#endif

C_LINK_START

size_t
hash_uintptr (uintptr_t key) {
#if UINTPTR_MAX == 0xffffffffffffffff
	/* MurmurHash3 64bit mixer */
	key ^= key >> 33;
	key *= 0xff51afd7ed558ccd;
	key ^= key >> 33;
	key *= 0xc4ceb9fe1a85ec53;
	key ^= key >> 33;
#elif UINTPTR_MAX == 0xffffffff
	/* MurmurHash3 32bit mixer */
	key ^= key >> 16;
	key *= 0x85ebca6b;
	key ^= key >> 13;
	key *= 0xc2b2ae35;
	key ^= key >> 16;
#else
#	error UINTPTR_WIDTH not 32bit or 64bit... Wat?
#endif
	return (size_t)key;
}

typedef struct {
    uintptr_t *keys;
    uintptr_t *vals;
    size_t len;
    size_t cap;
} hashtbl_t;

static void hashtbl_grow_at_least (hashtbl_t *tbl, size_t cap);

#define HASHTBL_CHECKS                                             \
qassert(tbl); qassert(!tbl->cap||qis_pow2(tbl->cap));              \
qassert(tbl->cap >= tbl->len);                                     \
qassert((!tbl->vals && !tbl->vals) || tbl->cap);
uintptr_t
hashtbl_get(hashtbl_t *tbl, uintptr_t key) { HASHTBL_CHECKS
	qassert(key);

	if (tbl->len == 0) { return 0; } /* .keys could be uninitialised */
	size_t hash = hash_uintptr(key),
	       mask = tbl->cap-1,
	       slot = hash & mask; /* == (hash % tbl->cap) if cap is ^2 */

	for (int i=0;!qassert(i<tbl->cap);++i) { /* check for inf loop */
		if (tbl->keys[slot] == key) { return tbl->vals[slot]; }
		if (tbl->keys[slot] == 0)   { return 0; }
		++slot; slot &= mask;
	}
	qassert(0&&"Unreachable code");
}

void
hashtbl_set(hashtbl_t *tbl, uintptr_t key, uintptr_t val) { HASHTBL_CHECKS
	qassert(key);

	/* if cap is 0 or >= 50% residency */
	if (tbl->cap <= 2*tbl->len) { hashtbl_grow_at_least(tbl, 2*tbl->cap); }

	size_t hash = hash_uintptr(key),
	       mask = tbl->cap-1,
	       slot = hash & mask;

	for (int i=0;!qassert(i<tbl->cap);++i) { /* check for inf loop */

		if (tbl->keys[slot] == 0)   {
			tbl->keys[slot] = key;
			tbl->vals[slot] = val;
			++tbl->len;
			return;
		}
		++slot; slot &= mask;
	}
	qassert(0&&"Unreachable code");
}

static void
hashtbl_grow_at_least (hashtbl_t *tbl, size_t cap) { HASHTBL_CHECKS
	cap = qmax(8, cap); /* as cap can be zero if table is new */
	hashtbl_t new_tbl = {
		qcalloc(cap,  sizeof(uintptr_t)),
		qmalloc(cap * sizeof(uintptr_t)),
		0, cap
	};

	for (size_t slot=0, N=tbl->cap; slot<N; ++slot) {
		intptr_t key = tbl->keys[slot];
		if(key) { hashtbl_set(&new_tbl, key, tbl->vals[slot]); }
	}
	qassert(tbl->len == new_tbl.len);
	qfree(tbl->keys); qfree(tbl->vals);
	*tbl = new_tbl;
}

#undef HASHTBL_CHECKS
C_LINK_END

#endif
