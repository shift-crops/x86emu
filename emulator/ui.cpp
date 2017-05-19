#include <thread>
#include <GLFW/glfw3.h>
#include "emulator/ui.hpp"

UI::UI(Memory *m, UISetting s){
	std::thread main_th;

	vga = new VGA();
	keyboard = new Keyboard(m);

	set = s;
	working = true;
	capture = false;

	size_x = 320;
	size_y = 200;
	image = new uint8_t[size_x*size_y*3];

	X = size_x/2;
	Y = size_y/2;
	click[0] = click[1] = false;

	if(set.enable){
		main_th = std::thread(&UI::ui_main, this);
		main_th.detach();
	}
}

UI::~UI(void){
	delete[] image;
	delete vga;
	delete keyboard;
}

void UI::ui_main(void){
	GLFWwindow* window;

	window = glfwCreateWindow(size_x*set.zoom, size_y*set.zoom, "x86emu", set.full ? glfwGetPrimaryMonitor() : NULL, NULL);

	glfwSetWindowUserPointer(window, this);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyboard_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetCursorPosCallback(window, cursorpos_callback);
	//glfwSetCursorEnterCallback(window, cursorenter_callback);

	glPixelZoom(set.zoom, -set.zoom);
	glRasterPos2i(-1, 1);

	while (!glfwWindowShouldClose(window)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
		glfwPollEvents();

		if(vga->need_refresh()){
			uint16_t x, y;

			vga->get_windowsize(&x, &y);
			if(x && y && ((size_x^x) || (size_y*y))){
				size_x = x;
				size_y = y;
				glfwSetWindowSize(window, x*set.zoom, y*set.zoom);

				delete[] image;
				image = new uint8_t[x*y*3];
			}
			vga->rgb_image(image, x*y);
			glDrawPixels(x, y, GL_RGB, GL_UNSIGNED_BYTE, image);

			glfwSwapBuffers(window);
		}
	}

	glfwDestroyWindow(window);
	working = false;
}

void UI::keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
	UI *ui = static_cast<UI*>(glfwGetWindowUserPointer(window));
	Keyboard *kb = ui->keyboard;

	if(!ui->capture)
		return;

	DEBUG_MSG(1, "key : 0x%02x, scancode : 0x%02x, action : %d, mods : %d\n", key, scancode, action, mods);
	switch(key){
		case 0x159:	// right CTRL
			ui->capture = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			return;
	}

	switch(action){
		case GLFW_RELEASE:
			kb->send_code(scancode + 0x80);
			break;
		case GLFW_PRESS:
		case GLFW_REPEAT:
			kb->send_code(scancode);
			break;
	}
}

void UI::mouse_callback(GLFWwindow *window, int button, int action, int mods){
	UI *ui = static_cast<UI*>(glfwGetWindowUserPointer(window));
	Mouse *mouse = ui->keyboard->get_mouse();

	if(ui->capture){
		ui->click[button%2] = action;
		mouse->send_code((1<<3) + (ui->click[1]<<1) + ui->click[0]);
		mouse->send_code(0);
		mouse->send_code(0);

		DEBUG_MSG(1, "[%02x %02x %02x] button : %d, action : %d, mods : %d\n"
				, (1<<3) + (ui->click[1]<<1) + ui->click[0], 0 ,0, button, action, mods);
	}
	else{
		ui->capture = true;
		glfwSetInputMode(window, GLFW_CURSOR, ui->set.cursor ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_HIDDEN);
		MSG("To cancel the input capture, press the right control key.\n");
	}
}

void UI::cursorpos_callback(GLFWwindow *window, double xpos, double ypos){
	UI *ui = static_cast<UI*>(glfwGetWindowUserPointer(window));
	Mouse *mouse = ui->keyboard->get_mouse();
	int32_t _xpos = xpos, _ypos = ypos;
	bool sx, sy;
	static int count = 0;

	if(!ui->capture || count++ % 6)
		return;

	sx = _xpos < ui->X;
	sy = _ypos > ui->Y;

	mouse->send_code((sy<<5) + (sx<<4) + (1<<3) + (ui->click[1]<<1) + ui->click[0]);
	std::this_thread::sleep_for(std::chrono::microseconds(100));
	mouse->send_code(_xpos-ui->X);
	std::this_thread::sleep_for(std::chrono::microseconds(100));
	mouse->send_code(ui->Y-_ypos);

	DEBUG_MSG(1, "[%02x %02x %02x] _xpos : %d, _ypos : %d\n"
			, (sy<<5) + (sx<<4) + (1<<3) + (ui->click[1]<<1) + ui->click[0]
			, (uint8_t)(_xpos-ui->X), (uint8_t)(ui->Y-_ypos), _xpos, _ypos);
	ui->X = _xpos;
	ui->Y = _ypos;
}
/*
void UI::cursorenter_callback(GLFWwindow *window, int entered){
}
*/
