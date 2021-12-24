#include <stdio.h>
#include <stdlib.h>

#include "gfx/window.h"

int main(void) {
	if (!Window_init(1280, 720, "GLFW/OPENGL PROJ")) {
		printf("window diff\n");
		return 1;
	} else {
		printf("Hello World!\n");
	}

	return 0;
}
