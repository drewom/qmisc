/*__          __
 / /____ ___ / /_ ____
/ __/ -_|_-</ __// __/
\__/\__/___/\__(_)__/
 *
 * Drew O'Malley
 */

#include "qmisc/macros.h"
#include <stdio.h>
#include <stdint.h> /* uintptr_t */

#define REQUIRE(e) \
if(!(e)){fprintf(stderr,"%s|%d| %s [%lld]\n",__FILE__,__LINE__,#e,(long long)(e));return 1;}
int
test_macros () {
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

	return 0;
}

int
main (int argc, char *argv[]) {
	int is_error = 0;
	is_error &= test_macros();
	return is_error;
}
