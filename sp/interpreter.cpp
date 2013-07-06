// interpreter.cpp

// Exit codes:
//	0	-	halt
//	1	-	losi atgumenti
//	2	-	ne postojeci fajl
//	3	-	ne postojeci opcode
//	4	-	destinacija nije validna

#include <stdio.h>

#include "common.h"
#include "log.h"
#include "debug.h"
#include "exit.h"

void EXIT() {
  closeLog();
}

//---------------
//--entry point--
//---------------
int main(int argc, char* argv[]) {
	initLog();

	log("Interpreter pokrenut");

	if(argc != 2)
		WRONG_ARGUMENT_COUNT();
	
	if(!loadExecutableFile(argv[1]))
		FILE_CAN_NOT_BE_LOADED();

	log("Izvrsni fajl ucitan");

	for(;;) {
		logCPUInfo();
		logMemory(IP, 20);
		logStackTrace();

		ubyte opcode = read_next_ubyte();

		log("opcode = 0x%02hx", opcode);

		// Testiramo prvi bit instrukcije
		if(opcode & 0x80) { // prvi bit 1
			// odredjujemo dest i src
			ubyte dst = read_next_ubyte(), src;
			
			src = dst & 0x0F;
			dst >>= 4;

			//if(src == 0xf &&  dst == 0xf)
			//	ADRESSING_ERROR();

			switch(opcode) {
 				case 0x80: // JMP
					if(dst != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					IP = read_next_word() + (src == 0xf ? 0 : REGS[src]);
				break;
				case 0x81: // JZ
					if(dst != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();
					{
						word c = read_next_word();

						if(TEST_FLAG_Z())
							IP +=  c + (src == 0xf ? 0 : REGS[src]);
					}
				break;
				case 0x82: // JGT
					if(dst != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					{
						word c = read_next_word();
				
						if(!TEST_FLAG_O() && !TEST_FLAG_Z())
							IP += c + (src == 0xf ? 0 : REGS[src]);
					}
				break;
				case 0x83: // MOV
					if(src == 0x0f || dst == 0x0f)
						ADRESSING_ERROR();

					REGS[dst] = REGS[src];

					SET_OR_CLEAR_FLAG_N(REGS[dst] < 0);
					SET_OR_CLEAR_FLAG_Z(REGS[dst] == 0);
				break;
				case 0x84: // LDR					
					if(dst == 0x0f)
						ADRESSING_ERROR();

					REGS[dst] = read_word(read_next_word() + (src == 0xf ? 0 : REGS[src]));
					
					SET_OR_CLEAR_FLAG_N(REGS[dst] < 0);
					SET_OR_CLEAR_FLAG_Z(REGS[dst] == 0);
				break;
				case 0x85: // STR
					if(src == 0x0f)
						ADRESSING_ERROR();

					write_word(read_next_word() + (dst == 0xf ? 0 : REGS[dst]), REGS[src]);
				break;
				case 0x86: // IN
					if(src != 0x00 || dst == 0x0f) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					scanf("%d", &REGS[dst]);
					
					SET_OR_CLEAR_FLAG_N(REGS[dst] < 0);
					SET_OR_CLEAR_FLAG_Z(REGS[dst] == 0);
				break;
				case 0x87: // OUT
					if(dst != 0x00 || src == 0x0f) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();
					
					printf("%d\n", REGS[src]);
				break;
				case 0x88: // CLC
					if(dst != 0x00 || src != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();
					
					CLEAR_FLAG_C();
				break;
				case 0x89: // STC
					if(dst != 0x00 || src != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					SET_FLAG_C();
				break;
				case 0x8A: // NC
					if(dst != 0x00 || src != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					if(TEST_FLAG_C()) {
						CLEAR_FLAG_C();
					} else {
						SET_FLAG_C();
					}
				break;
				case 0x8B: // MOVF
					if(src != 0x00 || dst == 0x0f) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					REGS[dst] = PSW;
				break;
				case 0x8C: // MOVTSP
					if(dst != 0x00 || src == 0x0f) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					SP = REGS[src];
				break;
				case 0x8D: // MOVFSP
					if(src != 0x00 || dst == 0x0f) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					REGS[dst] = SP;
				break;
				case 0x8E: // CALL
					if(dst != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();
					{
						word c = read_next_word();
						push(IP);
						IP = c + (src == 0xf ? 0 : REGS[src]);
					}
				break;
				case 0x8F: // RET
					if(dst != 0x00 || src != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					IP = pop();
				break;
				case 0x90: // HLT
					if(dst != 0x00 || src != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					HALT();
				break;
				default:
					WRONG_OP_CODE();
			}
		} else { // prvi bit 0
			// odredjujemo dest i src1, src2 
			ubyte dst = opcode & 0x0F;
			
			ubyte src1 = read_next_ubyte(), src2;
			
			src2 = src1 & 0x0F;
			src1 >>= 4;
			
			if(dst == 0x0F || (src1 == 0x0F && src2 == 0x0F))
				ADRESSING_ERROR();
			
			// korisitimo dword aritmetiku da bi mogli da odredimo C flag
			dword val1 = src1 == 0xf ? read_next_word() : REGS[src1];
			dword val2 = src2 == 0xf ? read_next_word() : REGS[src2];
			
			opcode >>= 4;
			
			dword result;
			
			switch(opcode) {
				case 0: // ADD
					REGS[dst] = result = val1 + val2 + TEST_FLAG_C();
				break;
				case 1: // SUB
					REGS[dst] = result = val1 - val2 - TEST_FLAG_C();
				break;
				case 2: // CMP
					result = val1 - val2;
				break;
				case 3: // SAR
					REGS[dst] = result = val1 >> val2;
				break;
				case 4: // SAL
					REGS[dst] = result = val1 << val2;
				break;
				case 5: // AND
					REGS[dst] = result = val1 & val2;
				break;
				case 6: // OR
					REGS[dst] = result = val1 | val2;
				break;
				case 7: // NOT
					if(src2 != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();
					
					REGS[dst] = result = ~val1;
				break;
			}
			
			if(opcode <= 2)
				SET_OR_CLEAR_FLAG_O(
														((val1 & 0x8000) 		&& (val2 & 0x8000) 		&& ((~result) & 0x8000))
															||
														(((~val1) & 0x8000) && ((~val2) & 0x8000) && (result & 0x8000))
													 );
			
			if(opcode <= 4)
				SET_OR_CLEAR_FLAG_C((result >> 16) ^ (result >> 17));
			
			SET_OR_CLEAR_FLAG_N((word)result < 0);
			SET_OR_CLEAR_FLAG_Z(result == 0);
		}
	}
}
