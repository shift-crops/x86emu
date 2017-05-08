#include <thread>
#include "device/pit.hpp"

PIT::PIT(){
	count[0] = def[0] = 0xffff;

	std::thread th = std::thread(&PIT::counter, this);
	th.detach();
};

uint8_t PIT::in8(uint16_t addr){
	uint8_t rgn = addr&0x3;

	switch(rgn){
		case 0:
		case 1:
		case 2:
			switch(cwr.RL){
				case 1:
					return count[rgn]>>8;
				case 2:
					return count[rgn]&0xff;
				case 3:
					if(!(first ^= true))
						return count[rgn]>>8;
					else
						return count[rgn]&0xff;
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
				case 1:
					count[rgn] = (count[rgn]&0xff00) + v;
				case 2:
					count[rgn] = (v<<8) + (count[rgn]&0xff);
				case 3:
					if(!(first ^= true))
						count[rgn] = v;
					else
						count[rgn] = (v<<8) + (count[rgn]&0xff);
			}
			def[rgn] = count[rgn];
			INFO(2, "count[%d] = 0x%04x", rgn, count[rgn]);
			break;
		case 3:
			cwr.raw = v;
			first = true;
			if(cwr.RL == 0 && cwr.SC < 3)
				def[cwr.SC] = count[cwr.SC];
			break;
	}
}
/*
bool PIT::chk_intreq(void){
	//INFO("counter = 0x%04x", count[0]);
	if(cwr.BCD){
	}
	else	count[0]--;

	switch(cwr.mode){
		// TODO
		case 2:
			if(count[0]==1){
				count[0] = def[0];
				return true;
			}
	}

	return false;
}
*/

void PIT::counter(void){
	while(true){
		std::this_thread::sleep_for(std::chrono::milliseconds(100*def[0]/119318));
		intr = true;
	}
}
