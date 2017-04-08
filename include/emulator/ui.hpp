#ifndef _UI_H
#define _UI_H

#include "common.hpp"
#include "device/display.hpp"
#include "device/keyboard.hpp"
#include "device/mouse.hpp"

class UI {
	private:
		static UI *ui;
		Display *disp;
		Keyboard *keyboard;
		Mouse *mouse;

	public:
		UI(Display *d, Keyboard *k, Mouse *m);
		static UI *current_ui(void) { return ui; };
		//Display *get_display(void) const { return disp; };
		Keyboard *get_keyboard(void) const { return keyboard; };
		Mouse *get_mouse(void) const { return mouse; };
	private:
		void ui_main(void);
		void refresh_event(void);
};

#endif
