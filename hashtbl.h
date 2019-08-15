/* __            __   __  __   __  __
  / /  ___ ____ / /  / /_/ /  / / / /
 / _ \/ _ `(_-</ _ \/ __/ _ \/ / / _ \
/_//_/\_,_/___/_//_/\__/_.__/_(_)_//_/
 * Hash table. Zeroed struct is initialised. Automatically resizes at 50%
 * residency to ~25%. Key of zero is the special null case.
 *
 * Drew O'Malley
 */
#ifndef HASHTBL_H
#define HASHTBL_H

#ifndef STRESSTEST
#	define HASHTBL_GROW_TRIGGER(len) (2*(len))
#else
#	define HASHTBL_GROW_TRIGGER(len) ((len)+1)
#endif

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

typedef struct {
    uintptr_t *keys;
    uintptr_t *vals;
    size_t len;
    size_t cap;
} hashtbl_t;

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

static void hashtbl_grow_at_least (hashtbl_t *tbl, size_t cap);

#define HASHTBL_CHECKS                                             \
qassert(tbl); qassert(!tbl->cap||qis_pow2(tbl->cap));              \
qassert(!tbl->cap || tbl->cap > tbl->len);                         \
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


int
hashtbl_set(hashtbl_t *tbl, uintptr_t key, uintptr_t val) { HASHTBL_CHECKS
	qassert(key);

	/* if cap is 0 or >= 50% residency (unless under stress-test) */
	if (!tbl->cap || tbl->cap <= HASHTBL_GROW_TRIGGER(tbl->len)) {
		hashtbl_grow_at_least(tbl, 2*tbl->cap); }

	size_t hash = hash_uintptr(key),
	       mask = tbl->cap-1,
	       slot = hash & mask;
	int result = !tbl->keys[slot];

	for (int i=0;!qassert(i<tbl->cap);++i) { /* check for inf loop */
		uintptr_t test = tbl->keys[slot];
		if (!test || test == key)   {
			tbl->len += (uintptr_t)!test; /* len += (test==0) ? 1 : 0 */
			tbl->keys[slot] = key;
			tbl->vals[slot] = val;
			HASHTBL_CHECKS
			return result;
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
	HASHTBL_CHECKS
}

void
hashtbl_free(hashtbl_t *tbl) {
	qfree(tbl->keys); qfree(tbl->vals);
	tbl->keys = tbl->vals = 0;
	tbl->len  = tbl->cap  = 0;
}

static int
hashtbl_remove (hashtbl_t *tbl, uintptr_t key) { HASHTBL_CHECKS
	qassert(key);

	if (tbl->len == 0) { return 0; } /* .keys could be uninitialised */
	size_t hash = hash_uintptr(key),
	       mask = tbl->cap-1,
	       slot = hash & mask; /* == (hash % tbl->cap) if cap is ^2 */

	for (int i=0;!qassert(i<tbl->cap);++i) { /* check for inf loop */
		if (tbl->keys[slot] == key) { break; }
		if (tbl->keys[slot] == 0)   { return 0; }
		++slot; slot &= mask;
	}
	/* remove this slot */
	tbl->keys[slot] = 0;
	tbl->vals[slot] = 0;
	--tbl->len;

	/* walk the chain down, re-adding as we go */
	++slot;
	for (int i=0;!qassert(i<tbl->cap);++i) { /* check for inf loop */
		if (tbl->keys[slot] == 0) { break; } /* once key's empty, stop */

		/* remove before add, as it may go here again */
		uintptr_t k = tbl->keys[slot], v = tbl->vals[slot];
		tbl->keys[slot] = tbl->vals[slot] = 0; --tbl->len;
		hashtbl_set(tbl, k, v);

		++slot; slot &= mask;
	}
	HASHTBL_CHECKS
	return 1;
}

#undef HASHTBL_CHECKS
C_LINK_END

#endif
