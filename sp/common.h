// common.h

#ifndef _COMMON_H_
#define _COMMON_H_

typedef char 	byte; 	// 1B - 8b
typedef short	word; 	// 2B - 16b
typedef long 	dword;	// 4B - 32b

typedef unsigned char 	ubyte; 	// 1B - 8b
typedef unsigned short	uword; 	// 2B - 16b
typedef unsigned long 	udword;	// 4B - 32b

#define MEMORY_SIZE 0x10000

// memorija emulirane masine
extern ubyte memory[];

typedef word reg;
typedef uword ureg;

// lokacije flagov-a u psw-u
#define FLAG_Z_MASK 0x0001
#define FLAG_O_MASK 0x0002
#define FLAG_C_MASK 0x0004
#define FLAG_N_MASK 0x0008

#define FLAG_Z_POS 0
#define FLAG_O_POS 1
#define FLAG_C_POS 2
#define FLAG_N_POS 3

#define REGS_COUNT 15

struct CPU {
	ureg ip;
	ureg sp;
	reg psw;
	reg regs[REGS_COUNT];

	#ifdef __cplusplus
	CPU() {
		ip = 0;
		sp = 0xffff;
	}
	#endif
} extern cpu;

#define TEST_FLAG(mask) ((PSW & (mask)) != 0)
#define SET_FLAG(mask) PSW |= (mask)
#define CLEAR_FLAG(mask) PSW &= ~(mask)
// sets or clears flag u zavisnosto od val ( val je 0 ili 1)
#define SET_OR_CLEAR_FLAG(mask, val)\
	if(val)\
		SET_FLAG(mask);\
	else\
		CLEAR_FLAG(mask);

#define TEST_FLAG_Z() TEST_FLAG(FLAG_Z_MASK)
#define TEST_FLAG_O() TEST_FLAG(FLAG_O_MASK)
#define TEST_FLAG_C() TEST_FLAG(FLAG_C_MASK)
#define TEST_FLAG_N() TEST_FLAG(FLAG_N_MASK)

#define SET_FLAG_Z() SET_FLAG(FLAG_Z_MASK)
#define SET_FLAG_O() SET_FLAG(FLAG_O_MASK)
#define SET_FLAG_C() SET_FLAG(FLAG_C_MASK)
#define SET_FLAxG_N() SET_FLAG(FLAG_N_MASK)

#define CLEAR_FLAG_Z() CLEAR_FLAG(FLAG_Z_MASK)
#define CLEAR_FLAG_O() CLEAR_FLAG(FLAG_O_MASK)
#define CLEAR_FLAG_C() CLEAR_FLAG(FLAG_C_MASK)
#define CLEAR_FLAG_N() CLEAR_FLAG(FLAG_N_MASK)

#define SET_OR_CLEAR_FLAG_Z(val) SET_OR_CLEAR_FLAG(FLAG_Z_MASK, val)
#define SET_OR_CLEAR_FLAG_O(val) SET_OR_CLEAR_FLAG(FLAG_O_MASK, val)
#define SET_OR_CLEAR_FLAG_C(val) SET_OR_CLEAR_FLAG(FLAG_C_MASK, val)
#define SET_OR_CLEAR_FLAG_N(val) SET_OR_CLEAR_FLAG(FLAG_N_MASK, val)

#define IP cpu.ip
#define SP cpu.sp
#define PSW cpu.psw
#define REGS cpu.regs

inline ubyte read_ubyte(uword location) {
	return memory[location];
}

inline word read_word(uword location) {
	return *(word*)(memory + location);
}

inline ubyte read_next_ubyte() {
	return read_ubyte(IP++);
}

inline word read_next_word() {
	word ret = read_word(IP);
	IP += 2;
	return ret;
}

inline void write_word(uword location, word data) {
	*(word*)(memory + location) = data;
}

inline void push(word data) {
	SP -= 2;
	write_word(SP, data);
}

inline word pop() {
	word ret = read_word(SP);
	SP += 2;
	return ret;
}

#ifdef __cplusplus
bool loadExecutableFile(char* fileName);
#endif

#endif // _COMMON_H_
