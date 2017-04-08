#ifndef _UI_H
#define _UI_H

#include <GLFW/glfw3.h>
#include "common.hpp"
#include "device/display.hpp"
#include "device/keyboard.hpp"
#include "device/mouse.hpp"

class UI {
	private:
		Display *disp;
		Keyboard *keyboard;
		Mouse *mouse;
		bool enable;

	public:
		UI();
		~UI();
		Display *get_display(void) const { return disp; };
		Keyboard *get_keyboard(void) const { return keyboard; };
		Mouse *get_mouse(void) const { return mouse; };
		bool get_status(void) const { return enable; };
	private:
		void ui_main(void);
		static void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
		static void mouse_callback(GLFWwindow *window, int button, int action, int mods);
		static void cursor_callback(GLFWwindow *window, double xpos, double ypos);
};

#endif
