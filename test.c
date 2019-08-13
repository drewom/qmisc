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
	REQUIRE(qmax(-1,0)==-1)
	REQUIRE(qmax( 2,1)==1)

	REQUIRE(qis_pow2(-1)==1)
	REQUIRE(qis_pow2(0)==0)
	REQUIRE(qis_pow2(1)==1)
	REQUIRE(qis_pow2(2)==1)
	REQUIRE(qis_pow2(3)==0)
	REQUIRE(qis_pow2(4)==1)
	return 0;
}

int
main (int argc, char *argv[]) {
	int is_error = 0;
	printf("Starting tests...\n");
	is_error &= test_macros();
	printf("error = %d\n", is_error);
	return is_error;
}
