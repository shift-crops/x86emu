#include <GL/freeglut.h>
#include <pthread.h>
#include "device/display.hpp"

Display::Display(){
	int argc = 0;
	char* argv[1] = {NULL};

	glutInit(&argc, argv);

	glutCreateWindow("x86emu");

	glutInitDisplayMode(GLUT_RGBA);
	glClearColor(0, 0, 0, 0);

	glutDisplayFunc(display);

	pthread_create(&thread , NULL , disp_thread_func , NULL);
	sleep(5);
}

void* Display::disp_thread_func(void *p){
	glutMainLoop();
}

void Display::display(void){
}
