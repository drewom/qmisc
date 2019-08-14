/*__          __
 / /____ ___ / /_ ____
/ __/ -_|_-</ __// __/
\__/\__/___/\__(_)__/
 *
 * Drew O'Malley
 */

#include "qmisc/macros.h"
#include <stdio.h>

#define REQUIRE(expr) if(!(expr)) { fprintf(stderr, "%s|%d| %s\n", __FILE__, __LINE__, #expr); return 1; }
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
	return 0;
}

int
main (int argc, char *argv[]) {
	int is_error = 0;
	is_error &= test_macros();
	return is_error;
}
