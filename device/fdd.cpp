#include <thread>
#include <cmath>
#include "device/fdd.hpp"

FDD::FDD(){
	fddfuncs[FDD_READ_TRACK] = &FDD::fdd_read_track;
	fddfuncs[FDD_WRITE_DATA] = &FDD::fdd_write_data;
	fddfuncs[FDD_READ_DATA] = &FDD::fdd_read_data;
	fddfuncs[FDD_CONFIGURE] = &FDD::fdd_configure;

	for(int i=0; i<MAX_FDD; i++)
		drive[i] = NULL;

	conf.EIS = 0;
	conf.EFIFO = 1;
	conf.POLL = 0;

	sra.raw = 0;
	srb.raw = 0;

	data_q.max = 0;

	th = std::thread(&FDD::worker, this);
}

FDD::~FDD(){
	for(int i=0; i<MAX_FDD; i++)
		remove_disk(i);

	th.join();
}

void FDD::insert_disk(uint8_t slot, const char* fname, bool write){
	DRIVE *d;

	if(slot>=MAX_FDD || drive[slot])
		return;

	d = new DRIVE;
	d->disk = fopen(fname, write ? "rb+" : "rb");
	if(!d->disk){
		delete d;
		return;
	}
	d->cylinder = 0;
	d->head = 0;
	d->sector = 1;
	d->write = write;

	drive[slot] = d;
}

void FDD::remove_disk(uint8_t slot){
	if(slot>=MAX_FDD || !drive[slot])
		return;

	fclose(drive[slot]->disk);
	delete drive[slot];
	drive[slot] = NULL;
}

int32_t FDD::seek(uint8_t slot, uint8_t c, uint8_t h, uint8_t s){
	int32_t dc, dh, ds, offset;

	if(!drive[slot] || !drive[slot]->disk)
		ERROR("not ready disk%d", slot);
	
	if(s < 1 || s > N_SpH)	ERROR("");
	if(h < 0 || h >= N_HpC)	ERROR("");
	if(c < 0)		ERROR("");

	msr.DRV_BSY |= 1<<slot;
	ds = (s - drive[slot]->sector) * SIZE_SECTOR;
	dh = (h - drive[slot]->head) * SIZE_SECTOR * N_SpH;
	dc = (c - drive[slot]->cylinder) * SIZE_SECTOR * N_SpH * N_HpC;
	offset = dc+dh+ds;

	INFO("seek : %d, ds : %d(%d->%d), dh : %d(%d->%d), dc : %d(%d->%d)\n"
				, offset, ds, drive[slot]->sector, s, dh, drive[slot]->head, h, dc, drive[slot]->cylinder, c);
	drive[slot]->cylinder = c;
	drive[slot]->head = h;
	drive[slot]->sector = s;
	drive[slot]->progress = 0;

	fseek(drive[slot]->disk, offset, SEEK_CUR);
	msr.DRV_BSY ^= 1<<slot;

	return offset;
}

uint8_t FDD::read(uint8_t slot){
	uint8_t v;

	if(!drive[slot] || !drive[slot]->disk)
		ERROR("not ready disk%d", slot);
	
	if(!fread(&v, 1, 1, drive[slot]->disk))
		v = 0;
	sync_position(slot);
	return v;
}

void FDD::write(uint8_t slot, uint8_t v){
	if(!drive[slot] || !drive[slot]->disk)
		ERROR("not ready disk%d", slot);

	fwrite(&v, 1, 1, drive[slot]->disk);
	sync_position(slot);
}

void FDD::sync_position(uint8_t slot){
	if(++drive[slot]->progress < 0x200)
		return;

	drive[slot]->progress = 0;
	drive[slot]->sector++;
	if(drive[slot]->sector > N_SpH){
		drive[slot]->sector = 1;
		drive[slot]->head++;
	}
	if(drive[slot]->head >= N_HpC){
		drive[slot]->head = 0;
		drive[slot]->cylinder++;
	}
}

uint8_t FDD::in8(uint16_t addr){
	uint8_t v;
	INFO("in %x", addr);
	switch(addr&7){
		case 0:
			v = sra.raw;
			break;
		case 1:
			v = srb.raw;
			break;
		case 3:
			v = tdr.raw;
			break;
		case 4:
			v = msr.raw;
			break;
		case 5:
			v = read_datareg();
			break;
		case 7:
			v = ccr.raw;
			break;
	}
	return v;
}

void FDD::out8(uint16_t addr, uint8_t v){
	INFO("out %x <- %x", addr, v);
	switch(addr&7){
		case 2:
			dor.raw = v;
			break;
		case 3:
			tdr.raw = v;
			break;
		case 4:
			dsr.raw = v;
			break;
		case 5:
			enqueue(&data_q, v);
			break;
		case 7:
			dir.raw = v;
			break;
	}
}

void FDD::worker(void){
	uint8_t mode;

	while(true){
		while(data_q.queue.empty())
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

		mode = dequeue(&data_q);
		if(!fddfuncs.count(mode)){
			data = 0x80;
			continue;
		}

		msr.CMD_BSY = 1;
		(this->*fddfuncs[mode])();
		msr.CMD_BSY = 0;
	}
}

void FDD::fdd_read_track(void){
	uint8_t cmd[8];
	uint8_t slot;

	msr.RQM = 1;
	msr.DIO = 0;

	for(int i=0; i<8; i++)
		cmd[i] = dequeue(&data_q);

	slot = cmd[0]&3;
	if(conf.EIS)
		seek(slot, cmd[1], cmd[2], 1);

	for(int i=0; i<std::pow(2, cmd[4])*128*N_SpH; i++){
		write_datareg(read(slot));
		intr = true;
	}

	write_datareg(st0.raw);
	write_datareg(st1.raw);
	write_datareg(st2.raw);
	write_datareg(drive[slot]->cylinder);
	write_datareg(drive[slot]->head);
	write_datareg(drive[slot]->sector);
	write_datareg(cmd[4]);

	msr.RQM = 0;
}

void FDD::fdd_write_data(void){
	uint8_t cmd[8];
	uint8_t slot;

	msr.RQM = 1;
	msr.DIO = 1;

	for(int i=0; i<8; i++)
		cmd[i] = dequeue(&data_q);

	slot = cmd[0]&3;
	if(conf.EIS)
		seek(slot, cmd[1], cmd[2], cmd[3]);

	for(int i=0; i<std::pow(2, cmd[4])*128; i++){
		write(slot, dequeue(&data_q));
		intr = true;
	}	

	write_datareg(st0.raw);
	write_datareg(st1.raw);
	write_datareg(st2.raw);
	write_datareg(drive[slot]->cylinder);
	write_datareg(drive[slot]->head);
	write_datareg(drive[slot]->sector);
	write_datareg(cmd[4]);

	msr.RQM = 0;
}

void FDD::fdd_read_data(void){
	uint8_t cmd[8];
	uint8_t slot;

	msr.RQM = 1;
	msr.DIO = 0;

	for(int i=0; i<8; i++)
		cmd[i] = dequeue(&data_q);

	slot = cmd[0]&3;
	if(conf.EIS)
		seek(slot, cmd[1], cmd[2], cmd[3]);

	for(int i=0; i<std::pow(2, cmd[4])*128; i++){
		write_datareg(read(slot));
		intr = true;
	}

	write_datareg(st0.raw);
	write_datareg(st1.raw);
	write_datareg(st2.raw);
	write_datareg(drive[slot]->cylinder);
	write_datareg(drive[slot]->head);
	write_datareg(drive[slot]->sector);
	write_datareg(cmd[4]);

	msr.RQM = 0;
}

void FDD::fdd_configure(void){
	uint8_t cmd[3];

	for(int i=0; i<3; i++)
		cmd[i] = dequeue(&data_q);
	conf.raw = cmd[1];
}

uint8_t FDD::read_datareg(void){
	uint8_t v;
	
	while(!sra.INT)
		std::this_thread::sleep_for(std::chrono::microseconds(50));
	v = data;
	sra.INT = 0;

	return v;
}

void FDD::write_datareg(uint8_t v){
	while(sra.INT)
		std::this_thread::sleep_for(std::chrono::microseconds(50));
	data = v;
	sra.INT = 1;
}

void FDD::enqueue(QUEUE *q, uint8_t v){
	while(q->max && q->queue.size() >= q->max)
		std::this_thread::sleep_for(std::chrono::microseconds(50));
	q->mtx.lock();
	q->queue.push(v);
	q->mtx.unlock();
}

uint8_t FDD::dequeue(QUEUE *q){
	uint8_t v;

	q->mtx.lock();
	while(q->queue.empty()){
		q->mtx.unlock();
		std::this_thread::sleep_for(std::chrono::microseconds(50));
		q->mtx.lock();
	}

	v = q->queue.front();
	q->queue.pop();
	q->mtx.unlock();

	return v;
}
