#include <thread>
#include <string.h>
#include "device/pit.hpp"

PIT::PIT(){
	memset(timer, 0, sizeof(timer));
	for(int i=0; i<3; i++)
		timer[i].count = timer[i].def = 0xffff;
}

PIT::~PIT(){
	for(int i=0; i<3; i++)
		if(timer[i].th.joinable()){
			timer[i].running = false;
			timer[i].th.join();
		}
}

bool rl_fst;

uint8_t PIT::in8(uint16_t addr){
	uint8_t rgn = addr&0x3;

	switch(rgn){
		case 0:
		case 1:
		case 2:
			switch(cwr.RL){
				case 1:	// LSB
					return timer[rgn].count>>8;
				case 2:	// MSB
					return timer[rgn].count&0xff;
				case 3:
					if(!(rl_fst ^= true))	// LSB
						return timer[rgn].count>>8;
					else			// MSB
						return timer[rgn].count&0xff;
			}
		default:
			return 0;
	}
}

void PIT::out8(uint16_t addr, uint8_t v){
	uint8_t rgn = addr&0x3;

	switch(rgn){
		case 0:
		case 1:
		case 2:
			//if(cwr.SC != rgn)
			//	break;
			switch(cwr.RL){
				case 1:	// LSB
					timer[rgn].count = (timer[rgn].count&0xff00) + v;
					break;
				case 2: // MSB
					timer[rgn].count = (v<<8) + (timer[rgn].count&0xff);
					break;
				case 3:
					if(!(rl_fst ^= true)) 	// LSB
						timer[rgn].count = v;
					else			// MSB
						timer[rgn].count = (v<<8) + (timer[rgn].count&0xff);
					break;
			}
			timer[rgn].def = timer[rgn].count;
			INFO(2, "timer[%d].def = 0x%04x", rgn, timer[rgn].def);
			break;
		case 3:
			cwr.raw = v;
			if(cwr.SC < 3){
				timer[cwr.SC].mode = cwr.mode;
				switch(cwr.RL){
					case 0:
						timer[cwr.SC].def = timer[cwr.SC].count;
						break;
					case 3:
						rl_fst = true;
						break;
				}
			}

			if(!timer[cwr.SC].th.joinable()){
				timer[cwr.SC].running = true;
				timer[cwr.SC].th = std::thread(&PIT::counter, this, &timer[cwr.SC]);
			}

			break;
	}
}

/*
bool PIT::chk_intreq(void){
	if(cwr.BCD){
	}
	else	timer[0].count--;

	switch(cwr.mode){
		// TODO
		case 2:
			if(timer[0].count==1){
				timer[0].count = timer[0].def;
				return true;
			}
	}

	return false;
}
*/

void PIT::counter(Timer *t){
	while(t->running){
		switch(t->mode){
			case 2:
				std::this_thread::sleep_for(std::chrono::milliseconds(100*t->def/119318));
				intr = true;
				break;
			default:
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}
