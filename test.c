/*__          __
 / /____ ___ / /_ ____
/ __/ -_|_-</ __// __/
\__/\__/___/\__(_)__/
 *
 * Drew O'Malley
 */

#include <qmisc/macros.h>
#include <qmisc/hashtbl.h>

#include <stdio.h>
#include <stdint.h> /* uintptr_t */

#define REQUIRE(e) \
if(!(e)){fprintf(stderr,"%s|%d| failed %s\n",__FILE__,__LINE__,#e);++error;}

C_LINK_START static int
test_macros () {
	int error = 0;
	REQUIRE(qmax(-1,0)==0)
	REQUIRE(qmax( 2,1)==2)
	REQUIRE(qmin(-1,0)==-1)
	REQUIRE(qmin( 2,1)==1)
	REQUIRE(qclamp(-100.f, -200.f, 100.f)==-100.f)
	REQUIRE(qclamp(-100.f,   50.f, 100.f)== 50.f)
	REQUIRE(qclamp(-100.f,  200.f, 100.f)== 100.f)
	REQUIRE(qis_pow2(-1)==0)
	REQUIRE(qis_pow2(0)==0)
	REQUIRE(qis_pow2(1)==1)
	REQUIRE(qis_pow2(2)==1)
	REQUIRE(qis_pow2(3)==0)
	REQUIRE(qis_pow2(4)==1)
	REQUIRE(qis_pow2(7)==0)
	REQUIRE(qis_pow2(8)==1)

	REQUIRE(qalign_down(0xFFFFFFFF, Q_CACHE_LINE) == 0xFFFFFFC0);
	REQUIRE(qalign_down(0x0000003F, Q_CACHE_LINE) == 0x00000000);
	REQUIRE(  qalign_up(0x00000001, Q_CACHE_LINE) == 0x00000040);
	REQUIRE(  qalign_up(0xFFFFFFC1, Q_CACHE_LINE) == 0x00000000);
	REQUIRE(  qalign_up(0xFFFFFFC0, Q_CACHE_LINE) == 0xFFFFFFC0);

	REQUIRE(qalign_ptr_down((void *)0xFFFFFFFFFFFFFFFF, Q_CACHE_LINE) == (void *)0xFFFFFFFFFFFFFFC0);
	REQUIRE(qalign_ptr_down((void *)0x3F, Q_CACHE_LINE) == (void *)0x0);
	REQUIRE(  qalign_ptr_up((void *)0x1, Q_CACHE_LINE) == (void *)0x40);
	REQUIRE(  qalign_ptr_up((void *)0xFFFFFFFFFFFFFFC1, Q_CACHE_LINE) == (void *)0x0);
	REQUIRE(  qalign_ptr_up((void *)0xFFFFFFFFFFFFFFC0, Q_CACHE_LINE) == (void *)0xFFFFFFFFFFFFFFC0);

	qassert(1234==1234);

	return error;
}

static int
test_hashtbl() {
	int error = 0;
	hashtbl_t tbl = {0};
	REQUIRE(hashtbl_get(&tbl, 100) == 0);
	hashtbl_set(&tbl, 100,  10000000000);
	hashtbl_set(&tbl, 200,  10000000002);
	hashtbl_set(&tbl, 300,  10000000004);
	hashtbl_set(&tbl, 400,  10000000008);

	for (int i=500; i<1008; ++i) {
		REQUIRE(hashtbl_get(&tbl, i) == 0);
	}
	for (int i=500; i<1008; ++i) {
		hashtbl_set(&tbl, i, i*i);
	}
	for (int i=500; i<1008; ++i) {
		REQUIRE(hashtbl_get(&tbl, i) == i*i);
	}

	REQUIRE(hashtbl_get(&tbl, 100) == 10000000000);
	REQUIRE(hashtbl_get(&tbl, 200) == 10000000002);
	REQUIRE(hashtbl_get(&tbl, 300) == 10000000004);
	REQUIRE(hashtbl_get(&tbl, 400) == 10000000008);
	REQUIRE(hashtbl_get(&tbl, 401) == 0);
	REQUIRE(tbl.len == 512);

	hashtbl_remove(&tbl, 200);
	REQUIRE(hashtbl_get(&tbl, 200) == 0);
	REQUIRE(tbl.len == 511);
	hashtbl_remove(&tbl, 401);
	REQUIRE(tbl.len == 511);

	for (int i=10000; i<20000; i+=1) {
		hashtbl_set(&tbl, i, i+1);
	}
	for (int i=10000; i<20000; i+=1) {
		REQUIRE(hashtbl_get(&tbl, i) == i+1);
	}

	for (int i=10000; i<20000; i+=2) {
		REQUIRE(hashtbl_remove(&tbl, i));
	}

	for (int i=20000; i<30000; i+=1) {
		hashtbl_set(&tbl, i, i+1);
	}

	for (int i=10000; i<20000; i+=2) {
		REQUIRE(hashtbl_get(&tbl, i) == 0);
		REQUIRE(hashtbl_get(&tbl, i+1) == i+2);
	}

	hashtbl_free(&tbl);
	REQUIRE(hashtbl_get(&tbl, 100) == 0);
	REQUIRE(tbl.len == 0);
	REQUIRE(tbl.cap == 0);

	hashtbl_set(&tbl, 100, 1);
	REQUIRE(hashtbl_get(&tbl, 100) == 1);
	REQUIRE(tbl.len == 1);
	REQUIRE(tbl.cap >= 1);

	return error;
}

int
main (int argc, char *argv[]) {
	int error = 0;
	error += test_macros();
	error += test_hashtbl();
	return !!error;
}
C_LINK_END
