#include <thread>
#include <GLFW/glfw3.h>
#include "emulator/ui.hpp"

UI::UI(uint8_t z){
	std::thread main_th;

	vga = new VGA();
	keyboard = new Keyboard();

	zoom = z;
	enable = true;

	main_th = std::thread(&UI::ui_main, this);
	main_th.detach();

	size_x = 320;
	size_y = 200;
	image = new uint8_t[size_x*size_y*3];
}

UI::~UI(void){
	delete[] image;
	delete vga;
	delete keyboard;
}

void UI::ui_main(void){
	GLFWwindow* window;

	window = glfwCreateWindow(size_x*zoom, size_y*zoom, "x86emu", NULL, NULL);

	glfwSetWindowUserPointer(window, this);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, keyboard_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetCursorPosCallback(window, cursor_callback);

	glPixelZoom(zoom, -zoom);
	glRasterPos2i(-1, 1);

	while (!glfwWindowShouldClose(window)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		if(vga->need_refresh()){
			uint16_t x, y;

			vga->get_windowsize(&x, &y);
			if(x && y && ((size_x^x) || (size_y*y))){
				size_x = x;
				size_y = y;
				glfwSetWindowSize(window, x*zoom, y*zoom);

				delete[] image;
				image = new uint8_t[x*y*3];
			}
			vga->rgb_image(image, x*y);
			glDrawPixels(x, y, GL_RGB, GL_UNSIGNED_BYTE, image);
		}

		glfwSwapBuffers(window);
		//glfwWaitEvents();
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	enable = false;
}

void UI::keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
	Keyboard *kb = static_cast<UI*>(glfwGetWindowUserPointer(window))->keyboard;

	INFO("key : %d, scancode : %d, action : %d, mods : %d\n", key, scancode, action, mods);
	switch(action){
		case 0:		// release
			kb->send_code(scancode-8 + 0x80);
			break;
		case 1:		// press
		case 2:		// keep
			kb->send_code(scancode-8);
			break;
	}
}

void UI::mouse_callback(GLFWwindow *window, int button, int action, int mods){
	UI *ui = static_cast<UI*>(glfwGetWindowUserPointer(window));
	Mouse *mouse = ui->keyboard->get_mouse();

	ui->click[button%2] = action;
	mouse->send_code((1<<3) + (ui->click[1]<<1) + ui->click[0]);
	mouse->send_code(0);
	mouse->send_code(0);

	INFO("[%02x %02x %02x] button : %d, action : %d, mods : %d", (1<<3) + (ui->click[1]<<1) + ui->click[0], 0 ,0, button, action, mods);
}

void UI::cursor_callback(GLFWwindow *window, double xpos, double ypos){
	UI *ui = static_cast<UI*>(glfwGetWindowUserPointer(window));
	Mouse *mouse = ui->keyboard->get_mouse();
	int32_t _xpos = xpos, _ypos = ypos;
	bool sx, sy;

	sx = _xpos < ui->X;
	sy = _ypos > ui->Y;

	mouse->send_code((sy<<5) + (sx<<4) + (1<<3) + (ui->click[1]<<1) + ui->click[0]);
	mouse->send_code(_xpos-ui->X);
	mouse->send_code(_ypos-ui->Y);

	INFO("[%02x %02x %02x] xpos : %d, ypos : %d"
			, (sy<<5) + (sx<<4) + (1<<3) + (ui->click[1]<<1) + ui->click[0], _xpos-ui->X, ui->Y-_ypos, _xpos, _ypos);
	ui->X = _xpos;
	ui->Y = _ypos;
}
