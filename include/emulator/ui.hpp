#ifndef _UI_H
#define _UI_H

#include <GLFW/glfw3.h>
#include "common.hpp"
#include "device/vga.hpp"
#include "device/keyboard.hpp"
#include "device/mouse.hpp"

class UI {
	private:
		bool enable;
		VGA *vga;
		Keyboard *keyboard;
		uint16_t size_x, size_y;
		uint8_t zoom;
		uint8_t *image;

		struct {
			int16_t X, Y;
			bool click[2];
		};

	public:
		UI(uint8_t z = 2);
		~UI();
		VGA *get_vga(void) const { return vga; };
		Keyboard *get_keyboard(void) const { return keyboard; };
		bool get_status(void) const { return enable; };
	private:
		void ui_main(void);
		static void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
		static void mouse_callback(GLFWwindow *window, int button, int action, int mods);
		static void cursor_callback(GLFWwindow *window, double xpos, double ypos);
};

#endif
