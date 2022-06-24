#include <stdio.h>
#include <stdlib.h>

#include "gfx/window.h"

int main(void) {
	if (!Window_init(1920, 1080, "GLFW/OPENGL PROJ")) {
		printf("window diff\n");
		return 1;
	} else {
		printf("Good Bye!\n");
	}

	return 0;
}
