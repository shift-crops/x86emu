#ifndef _UI_H
#define _UI_H

#include <GLFW/glfw3.h>
#include "common.hpp"
#include "device/vga.hpp"
#include "device/keyboard.hpp"
#include "device/mouse.hpp"
#include "hardware/memory.hpp" 	// A20 gate

struct UISetting {
	uint8_t zoom;
	bool full;
	bool cursor;
};

class UI {
	private:
		UISetting set;
		VGA *vga;
		Keyboard *keyboard;

		bool enable;
		bool capture;
		uint16_t size_x, size_y;
		uint8_t *image;

		struct {
			int16_t X, Y;
			bool click[2];
		};

	public:
		UI(Memory *m, UISetting set);
		~UI();
		VGA *get_vga(void) const { return vga; };
		Keyboard *get_keyboard(void) const { return keyboard; };
		bool get_status(void) const { return enable; };
	private:
		void ui_main(void);
		static void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
		static void mouse_callback(GLFWwindow *window, int button, int action, int mods);
		static void cursorpos_callback(GLFWwindow *window, double xpos, double ypos);
		//static void cursorenter_callback(GLFWwindow *window, int entered);
};

#endif
