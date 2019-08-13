/*__          __
 / /____ ___ / /_ ____
/ __/ -_|_-</ __// __/
\__/\__/___/\__(_)__/
 *
 * Drew O'Malley
 */

#include "qmisc/macros.h"
#include <stdio.h>

int main (int argc, char *argv[]) {
	int is_error = 0;
	if (qmax(999,1000) != 1000) { is_error&=1; }
	printf("error = %d\n", is_error);
	return is_error;
}
