#include "hardware/eflags.hpp"

Eflags::Eflags(Processor *p){
	processor = p;
	p->eflags.raw = 0x00000002;
}

template uint32_t Eflags::update_add(uint32_t v1, uint32_t v2);
template uint32_t Eflags::update_add(uint8_t v1, uint32_t v2);
template <class T> uint32_t Eflags::update_add(T v1, uint32_t v2){
	bool s1, s2, sr;
	uint64_t result;
	uint8_t size;

	result = v1 + v2;
	size = sizeof(T)*8;

	s1 = v1 >> (size-1);
	s2 = v2 >> (size-1);
	sr = (result >> (size-1)) & 1;

	set_carry(result >> size);
	set_parity(chk_parity(result & 0xff));
	set_zero(!result);
	set_sign(sr);
	set_overflow(!(s1^s2) && s1^sr);

	return processor->eflags.raw;
}

template uint32_t Eflags::update_or(uint32_t v1, uint32_t v2);
template uint32_t Eflags::update_or(uint8_t v1, uint32_t v2);
template <class T> uint32_t Eflags::update_or(T v1, uint32_t v2){
	uint64_t result;
	uint8_t size;

	result = v1 | v2;
	size = sizeof(T)*8;

	set_carry(0);
	set_parity(chk_parity(result & 0xff));
	set_zero(!result);
	set_sign((result >> (size-1)) & 1);
	set_overflow(0);

	return processor->eflags.raw;
}

template uint32_t Eflags::update_and(uint32_t v1, uint32_t v2);
template uint32_t Eflags::update_and(uint8_t v1, uint32_t v2);
template <class T> uint32_t Eflags::update_and(T v1, uint32_t v2){
	uint64_t result;
	uint8_t size;

	result = v1 & v2;
	size = sizeof(T)*8;

	set_carry(0);
	set_parity(chk_parity(result & 0xff));
	set_zero(!result);
	set_sign((result >> (size-1)) & 1);
	set_overflow(0);

	return processor->eflags.raw;
}

template uint32_t Eflags::update_sub(uint32_t v1, uint32_t v2);
template uint32_t Eflags::update_sub(uint8_t v1, uint32_t v2);
template <class T> uint32_t Eflags::update_sub(T v1, uint32_t v2){
	bool s1, s2, sr;
	uint64_t result;
	uint8_t size;

	result = v1 - v2;
	size = sizeof(T)*8;

	s1 = v1 >> (size-1);
	s2 = v2 >> (size-1);
	sr = (result >> (size-1)) & 1;

	set_carry(result >> size);
	set_parity(chk_parity(result & 0xff));
	set_zero(!result);
	set_sign(sr);
	set_overflow(s1^s2 && s1^sr);

	return processor->eflags.raw;
}

bool Eflags::chk_parity(uint8_t v){
	bool p = true;

	for(int i=0; i<8; i++)
		p ^= (v>>i) & 1;

	return p;
}
