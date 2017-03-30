#include "hardware/eflags.hpp"

Eflags::Eflags(){
	eflags.reg32 = 0x00000202;
}

template uint32_t Eflags::update_eflags_add(uint32_t v1, uint32_t v2);
template uint32_t Eflags::update_eflags_add(uint16_t v1, uint32_t v2);
template uint32_t Eflags::update_eflags_add(uint8_t v1, uint32_t v2);
template <class T> uint32_t Eflags::update_eflags_add(T v1, uint32_t v2){
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

	return eflags.reg32;
}

template uint32_t Eflags::update_eflags_or(uint32_t v1, uint32_t v2);
template uint32_t Eflags::update_eflags_or(uint16_t v1, uint32_t v2);
template uint32_t Eflags::update_eflags_or(uint8_t v1, uint32_t v2);
template <class T> uint32_t Eflags::update_eflags_or(T v1, uint32_t v2){
	uint64_t result;
	uint8_t size;

	result = v1 | v2;
	size = sizeof(T)*8;

	set_carry(0);
	set_parity(chk_parity(result & 0xff));
	set_zero(!result);
	set_sign((result >> (size-1)) & 1);
	set_overflow(0);

	return eflags.reg32;
}

template uint32_t Eflags::update_eflags_and(uint32_t v1, uint32_t v2);
template uint32_t Eflags::update_eflags_and(uint16_t v1, uint32_t v2);
template uint32_t Eflags::update_eflags_and(uint8_t v1, uint32_t v2);
template <class T> uint32_t Eflags::update_eflags_and(T v1, uint32_t v2){
	uint64_t result;
	uint8_t size;

	result = v1 & v2;
	size = sizeof(T)*8;

	set_carry(0);
	set_parity(chk_parity(result & 0xff));
	set_zero(!result);
	set_sign((result >> (size-1)) & 1);
	set_overflow(0);

	return eflags.reg32;
}

template uint32_t Eflags::update_eflags_sub(uint32_t v1, uint32_t v2);
template uint32_t Eflags::update_eflags_sub(uint16_t v1, uint32_t v2);
template uint32_t Eflags::update_eflags_sub(uint8_t v1, uint32_t v2);
template <class T> uint32_t Eflags::update_eflags_sub(T v1, uint32_t v2){
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

	return eflags.reg32;
}

template uint32_t Eflags::update_eflags_mul(uint32_t v1, uint32_t v2);
template uint32_t Eflags::update_eflags_mul(uint16_t v1, uint32_t v2);
template uint32_t Eflags::update_eflags_mul(uint8_t v1, uint32_t v2);
template <class T> uint32_t Eflags::update_eflags_mul(T v1, uint32_t v2){
	uint64_t result;
	uint8_t size;

	result = v1 * v2;
	size = sizeof(T)*8;

	set_carry(result >> size);
	set_overflow(result >> size);

	return eflags.reg32;
}

template uint32_t Eflags::update_eflags_shl(uint32_t v, uint8_t c);
template uint32_t Eflags::update_eflags_shl(uint16_t v, uint8_t c);
template uint32_t Eflags::update_eflags_shl(uint8_t v, uint8_t c);
template <class T> uint32_t Eflags::update_eflags_shl(T v, uint8_t c){
	T result;
	uint8_t size;

	result = v << c;
	size = sizeof(T)*8;

	set_carry((v >> (size-c)) & 1);
	set_parity(chk_parity(result & 0xff));
	set_zero(!result);
	set_sign((result >> (size-1)) & 1);
	if(c==1)
		set_overflow(((v >> (size-1)) & 1) ^ ((v >> (size-2)) & 1));

	return eflags.reg32;
}

template uint32_t Eflags::update_eflags_shr(uint32_t v, uint8_t c);
template uint32_t Eflags::update_eflags_shr(uint16_t v, uint8_t c);
template uint32_t Eflags::update_eflags_shr(uint8_t v, uint8_t c);
template <class T> uint32_t Eflags::update_eflags_shr(T v, uint8_t c){
	T result;
	uint8_t size;

	result = v >> c;
	size = sizeof(T)*8;

	set_carry((v >> (c-1)) & 1);
	set_parity(chk_parity(result & 0xff));
	set_zero(!result);
	set_sign((result >> (size-1)) & 1);
	if(c==1)
		set_overflow((v >> (size-1)) & 1);

	return eflags.reg32;
}

bool Eflags::chk_parity(uint8_t v){
	bool p = true;

	for(int i=0; i<8; i++)
		p ^= (v>>i) & 1;

	return p;
}
