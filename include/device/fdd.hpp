#ifndef _FDD_H
#define _FDD_H

#include <thread>
#include <map>
#include <queue>
#include <mutex>
#include "common.hpp"
#include "dev_io.hpp"
#include "dev_irq.hpp"

#define MAX_FDD			4
#define SIZE_SECTOR		0x200
#define N_SpH			18
#define N_HpC			2

#define FDD_READ_TRACK		0x02
#define FDD_SENSE_DRIVE_STATUS	0x04
#define FDD_WRITE_DATA		0x05
#define FDD_READ_DATA		0x06
#define FDD_RECALIBRATE		0x07
#define FDD_READ_ID		0x0a
#define FDD_FORMAT_TRACK	0x0d
#define FDD_SEEK		0x0f
#define FDD_CONFIGURE		0x13

struct DRIVE {
	FILE *disk;
	uint8_t cylinder : 7;	// 0 ~ 79
	uint8_t head : 1;	// 0 or 1
	uint8_t sector : 5;	// 1 ~ 18
	bool write;
	uint16_t progress;
};

struct QUEUE {
	std::queue<uint8_t> queue;
	std::mutex mtx;
	uint16_t max;
};

class FDD : public IRQ, public PortIO {
	private:
		typedef void (FDD::*fddfunc_t)(void);
		std::map<uint8_t, fddfunc_t> fddfuncs;
		DRIVE *drive[MAX_FDD];

		union {
			uint8_t raw;
			struct {
				uint8_t FIFOTHR : 4;
				uint8_t POLL : 1;
				uint8_t EFIFO : 1;
				uint8_t EIS : 1;
			};
		} conf;

		union {
			uint8_t raw;
			struct {
				uint8_t DIR : 1;
				uint8_t nWP : 1;
				uint8_t nINDX : 1;
				uint8_t HDSEL : 1;
				uint8_t nTRK0 : 1;
				uint8_t STEP : 1;
				uint8_t nDRV2 : 1;
				uint8_t INT : 1;
			};
		} sra;

		union {
			uint8_t raw;
			struct {
				uint8_t MOT0 : 1;
				uint8_t MOT1 : 1;
				uint8_t WE : 1;
				uint8_t RD : 1;
				uint8_t WR : 1;
				uint8_t SEL0 : 1;
			};
		} srb;

		union {
			uint8_t raw;
			struct {
				uint8_t SEL0 : 1;
				uint8_t SEL1 : 1;
				uint8_t nRESET : 1;
				uint8_t nDMA : 1;
				uint8_t MOT : 4;
			};
		} dor;

		union {
			uint8_t raw;
		} tdr;

		union {
			uint8_t raw;
			struct {
				uint8_t DRV_BSY : 4;
				uint8_t CMD_BSY : 1;
				uint8_t NON_DMA : 1;
				uint8_t DIO : 1;
				uint8_t RQM : 1;
			};
		} msr;

		union {
			uint8_t raw;
		} dsr;

		uint8_t data;
		QUEUE data_q;

		union {
			uint8_t raw;
		} ccr;

		union {
			uint8_t raw;
		} dir;

		union {
			uint8_t raw;
			struct {
				uint8_t DS0 : 1;
				uint8_t DS1 : 1;
				uint8_t H : 1;
				uint8_t : 1;
				uint8_t EC : 1;
				uint8_t SE : 1;
				uint8_t IC0 : 1;
				uint8_t IC1 : 1;
			};
		} st0;

		union {
			uint8_t raw;
			struct {
				uint8_t MA : 1;
				uint8_t NW : 1;
				uint8_t ND : 1;
				uint8_t : 1;
				uint8_t OR : 1;
				uint8_t DE : 1;
				uint8_t : 1;
				uint8_t EN : 1;
			};
		} st1;

		union {
			uint8_t raw;
			struct {
				uint8_t MD : 1;
				uint8_t BC : 1;
				uint8_t SN : 1;
				uint8_t SH : 1;
				uint8_t WC : 1;
				uint8_t DD : 1;
				uint8_t CM : 1;
			};
		} st2;

		union {
			uint8_t raw;
			struct {
				uint8_t DS0 : 1;
				uint8_t DS1 : 1;
				uint8_t HD : 1;
				uint8_t : 1;
				uint8_t T0 : 1;
				uint8_t : 1;
				uint8_t WP : 1;
			};
		} str3;

		std::thread th;

	public:
		FDD();
		~FDD();
		void insert_disk(uint8_t slot, const char *fname, bool write);
		void remove_disk(uint8_t slot);
		uint8_t in8(uint16_t addr);
		void out8(uint16_t addr, uint8_t v);
		//bool chk_intreq(void) { if(sra.INT){ sra.INT = false; return true; } return false; };
	private:
		void worker(void);
		void fdd_read_track(void);
		void fdd_write_data(void);
		void fdd_read_data(void);
		void fdd_configure(void);

		int32_t seek(uint8_t slot, uint8_t c, uint8_t h, uint8_t s);
		uint8_t read(uint8_t slot);
		void write(uint8_t slot, uint8_t v);
		void sync_position(uint8_t slot);

		void write_datareg(uint8_t v);
		uint8_t read_datareg(void);
		void enqueue(QUEUE *q, uint8_t v);
		uint8_t dequeue(QUEUE *q);
};

#endif
