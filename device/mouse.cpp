#include <thread>
#include "device/mouse.hpp"
#include "device/keyboard.hpp"

void Mouse::command(uint8_t v){
	switch(v){
		case 0xf4:
			while(keyboard->kcsr.OBF);		// busy wait

			keyboard->kcsr.OBF = 1;
			keyboard->out_buf = 0xfa;
			if(keyboard->ccb->MIE)
				intr = true;

			enable = true;
			break;
	}
}
void Mouse::send_code(uint8_t code){
	if(keyboard->ccb->ME || !enable)	// disabled
		return;

	while(keyboard->kcsr.OBF)
		std::this_thread::sleep_for(std::chrono::microseconds(10));

	keyboard->kcsr.OBF = 1;
	keyboard->out_buf = code;
	if(keyboard->ccb->MIE)
		intr = true;
}

