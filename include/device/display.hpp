#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <GL/freeglut.h>
#include <pthread.h>
#include "common.hpp"

class Display {
	private:
		pthread_t thread;

	public:
		Display();
		~Display() { glutLeaveMainLoop(); };
	protected:
		void disp_update(void) { glutPostRedisplay(); };
	private:
		static void* disp_thread_func(void *p);
		static void display(void);
};

#endif
