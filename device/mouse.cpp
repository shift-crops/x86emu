#include "device/mouse.hpp"
#include "device/keyboard.hpp"

void Mouse::command(uint8_t v){
	switch(v){
		case 0xf4:
			enable = true;
			keyboard->out_buf = 0xfa;
			if(keyboard->ccb->MIE)
				intr = true;
			break;
	}
}
void Mouse::send_code(uint8_t code){
	if(keyboard->kcsr.OBF)
		return;

	if(!keyboard->ccb->ME && enable){	// enable
		keyboard->kcsr.OBF = 1;
		keyboard->out_buf = code;
		if(keyboard->ccb->MIE)
			intr = true;
	}
}

