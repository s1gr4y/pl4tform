#include <stdio.h>
#include <stdlib.h>

#include "include/window.h"

int main(void) {

	if (!Window_init(1920, 1080, "PL4TFORM")) {
		printf("window diff\n");
		return 1;
	} else {
		printf("Good Bye!\n");
	}

	return 0;
}
