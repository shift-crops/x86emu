#include <thread>
#include <GLFW/glfw3.h>
#include "emulator/ui.hpp"

void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow *window, int button, int action, int mods);
void cursor_callback(GLFWwindow *window, double xpos, double ypos);

UI *UI::ui;

UI::UI(Display *d, Keyboard *k, Mouse *m){
	std::thread main_th, refresh_th;
	ui = this;

	disp = d;
	keyboard = k;
	mouse = m;

	d->resize(320, 200, 2);

	main_th = std::thread(&UI::ui_main, this);
	main_th.detach();
	refresh_th = std::thread(&UI::refresh_event, this);
	refresh_th.detach();
}

void UI::ui_main(void){
	GLFWwindow* window;
	uint16_t x = disp->size_x, y = disp->size_y;
	uint8_t zoom = disp->zoom;

	glfwInit();
	window = glfwCreateWindow(x*zoom, y*zoom, "x86emu", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyboard_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glPixelZoom(zoom, -zoom);
	glRasterPos2i(-1, 1);

	while (!glfwWindowShouldClose(window)) {
		if(disp->need_refresh())
			glDrawPixels(x, y, GL_RGB, GL_UNSIGNED_BYTE, disp->get_image());

		glfwSwapBuffers(window);
		glfwWaitEvents();
	}

	glfwTerminate();
	exit(0);
}

void UI::refresh_event(void){
	while(true){
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if(disp->need_refresh())
			glfwPostEmptyEvent();
	}
}

void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
	Keyboard *kb = UI::current_ui()->get_keyboard();

	INFO("key : %d, scancode : %d, action : %d, mods : %d\n", key, scancode, action, mods);
	if(action < 2)
		kb->send_code(scancode-8 + (1-action)*0x80);
}

void mouse_callback(GLFWwindow *window, int button, int action, int mods){
	Mouse *mouse = UI::current_ui()->get_mouse();
	mouse->test();
	INFO("button : %d, action : %d, mods : %d\n", button, action, mods);
}

void cursor_callback(GLFWwindow *window, double xpos, double ypos){
	Mouse *mouse = UI::current_ui()->get_mouse();
	mouse->test();
	INFO("xpos : %d, ypos : %d\n", (int)xpos, (int)ypos);
}

