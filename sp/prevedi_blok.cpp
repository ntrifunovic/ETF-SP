// prevedi_blok.cpp

#include <sstream>

#include "prevedi_blok.h"
#include "prevedi.h"
#include "exit.h"

using namespace std;

komanda prevedi(const uword adresa) {
	static stringstream ss;
	
	IP = adresa;
	
	reset(ss);
	
	bool generate = true;

	while(generate) {
		#ifdef DEBUG
		ss << "  logCPUInfo();" << endl;
		ss << "  logMemory(" << IP << ", 20);" << endl;
		ss << "  logStackTrace();" << endl;
		#endif

		ubyte opcode = read_next_ubyte();

		log("opcode = 0x%02hx", opcode);

		// Testiramo prvi bit instrukcije
		if(opcode & 0x80) { // prvi bit 1
			// odredjujemo dest i src
			ubyte dst = read_next_ubyte(), src;
			
			src = dst & 0x0F;
			dst >>= 4;

			if(src == 0xf &&  dst == 0xf)
				ADRESSING_ERROR();			

			switch(opcode) {
 				case 0x80: // JMP
					if(dst != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					ss << "  IP = " << read_next_word();
					if(src != 0xf)
						ss << " +  REGS[" << src << "]" << endl;
					ss << ";" << endl;

					generate = false;
				break;
				case 0x81: // JZ
					if(dst != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();
					
					ss << "  if(TEST_FLAG_Z())" << endl;

					ss << "    IP = " << read_next_word() + IP;
					if(src != 0xf)
						ss << " +  REGS[" << src << "]" << endl;
					ss << ";" << endl;

					ss << "  else" << endl;
					ss << "    IP = " << IP << ";" << endl;

					generate = false;
				break;
				case 0x82: // JGT
					if(dst != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					ss << "  if(!TEST_FLAG_O() && !TEST_FLAG_Z())" << endl;

					ss << "    IP = " << read_next_word() + IP;
					if(src != 0xf)
						ss << " +  REGS[" << src << "]" << endl;
					ss << ";" << endl;

					ss << "  else" << endl;
					ss << "    IP = " << IP << ";" << endl;
					
					generate = false;
				break;
				case 0x83: // MOV
					if(src == 0x0f || dst == 0x0f)
						ADRESSING_ERROR();

					ss << "  REGS[" << dst << "] = REGS[" << src << "];" << endl;

					ss << "  SET_OR_CLEAR_FLAG_N(REGS[" << dst << "] < 0);" << endl;
					ss << "  SET_OR_CLEAR_FLAG_Z(REGS[" << dst << "] == 0);" << endl;
				break;
				case 0x84: // LDR
					if(dst == 0x0f)
						ADRESSING_ERROR();

					ss << "  REGS[" << dst << "] = read_word(" << read_next_word();
					
					if(src != 0xf)
						ss << " + REGS[" << src << "]";
					
					ss << ");" << endl;
					
					ss << "  SET_OR_CLEAR_FLAG_N(REGS[" << dst << "] < 0);" << endl;
					ss << "  SET_OR_CLEAR_FLAG_Z(REGS[" << dst << "] == 0);" << endl;
				break;
				case 0x85: // STR	
					if(src == 0x0f)
						ADRESSING_ERROR();		
					{
						word c = read_next_word();

						ss << "  write_word(" <<  c;
						if(dst != 0xf)				
							ss << " + REGS[" << dst << "]";
						ss << ", REGS[" << src << "]);" << endl;

            #ifdef SELF_MODIFYING
						ss << "  if(self_modifying(" << c;
						if(dst != 0xf)				
							ss << " + REGS[" << dst << "]";
						ss << ")) {" << endl;
						ss << "    prevedi(" << IP << ")();" << endl;
						ss << "    return;" << endl;
						ss << "  }" << endl;
            #endif
					}
				break;
				case 0x86: // IN
					if(src != 0x00 || dst == 0x0f) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					ss << "  scanf(\"%d\", &REGS[" << dst << "]);" << endl;
					
					ss << "  SET_OR_CLEAR_FLAG_N(REGS[" << dst << "] < 0);" << endl;
					ss << "  SET_OR_CLEAR_FLAG_Z(REGS[" << dst << "] == 0);" << endl;
				break;
				case 0x87: // OUT
					if(dst != 0x00 || src == 0x0f) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();
					
					ss << "  printf(\"%d\\n\", REGS[" << src << "]);" << endl;
				break;
				case 0x88: // CLC
					if(dst != 0x00 || src != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();
					
					ss << "  CLEAR_FLAG_C();" << endl;
				break;
				case 0x89: // STC
					if(dst != 0x00 || src != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					ss << "  SET_FLAG_C();" << endl;
				break;
				case 0x8A: // NC
					if(dst != 0x00 || src != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					ss << "  if(TEST_FLAG_C()) {" << endl;
					ss << "  	CLEAR_FLAG_C();" << endl;
					ss << "  } else {" << endl;
					ss << "  	SET_FLAG_C();" << endl;
					ss << "  }" << endl;
				break;
				case 0x8B: // MOVF
					if(src != 0x00 || dst == 0x0f) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					ss << "  REGS[" << dst << "] = PSW;" << endl;
				break;
				case 0x8C: // MOVTSP
					if(dst != 0x00 || src == 0x0f) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					ss << "  SP = REGS[" << src << "];" << endl;
				break;
				case 0x8D: // MOVFSP
					if(src != 0x00 || dst == 0x0f) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					ss << "  REGS[" << dst << "] = SP;" << endl;
				break;
				case 0x8E: // CALL
					if(dst != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					{
						word c = read_next_word();

						ss << "  push(" << IP << ");" << endl;

						ss << "  IP = " << c;
						if(src != 0xf)
							ss << " +  REGS[" << src << "]" << endl;
						ss << ";" << endl;
					}
					generate = false;
				break;
				case 0x8F: // RET
					if(dst != 0x00 || src != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					ss << "  IP = pop();" << endl;

					generate = false;
				break;
				case 0x90: // HLT
					if(dst != 0x00 || src != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();

					ss << "  HALT();" << endl; 

					generate = false;
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

			ss << "  {" << endl;
			ss << "    dword val1, val2, result;" << endl; 

			// korisitimo dword aritmetiku da bi mogli da odredimo C flag
			if(src1 == 0xf)
				ss << "    val1 = " << read_next_word() << ";" << endl;
			else 
				ss << "    val1 = REGS[" << src1 << "];" << endl;

			if(src2 == 0xf)
				ss << "    val2 = " << read_next_word() << ";" << endl;
			else 
				ss << "    val2 = REGS[" << src2 << "];" << endl;
			
			opcode >>= 4;
			
			switch(opcode) {
				case 0: // ADD
					ss << "    REGS[" << dst << "] = result = val1 + val2 + TEST_FLAG_C();" << endl;
				break;
				case 1: // SUB
					ss << "    REGS[" << dst << "] = result = val1 - val2 - TEST_FLAG_C();" << endl;
				break;
				case 2: // CMP
					ss << "    result = val1 - val2;" << endl;
				break;
				case 3: // SAR
					ss << "    REGS[" << dst << "] = result = val1 >> val2;" << endl;
				break;
				case 4: // SAL
					ss << "    REGS[" << dst << "] = result = val1 << val2;" << endl;
				break;
				case 5: // AND
					ss << "    REGS[" << dst << "] = result = val1 & val2;" << endl;
				break;
				case 6: // OR
					ss << "    REGS[" << dst << "] = result = val1 | val2;" << endl;
				break;
				case 7: // NOT
					if(src2 != 0x00) // polje koje se ne koristi mora biti 0
						ADRESSING_ERROR();
					
					ss << "    REGS[" << dst << "] = result = ~val1;" << endl;
				break;
			}
			
			if(opcode <= 2)
				ss << "    SET_OR_CLEAR_FLAG_O(" << endl
					 <<	"      ((val1 & 0x8000)    && (val2 & 0x8000)    && ((~result) & 0x8000))" << endl
					 <<	"       ||" << endl
					 <<	"      (((~val1) & 0x8000) && ((~val2) & 0x8000) && (result & 0x8000)));" << endl;
			
			if(opcode <= 4)
				ss << "    SET_OR_CLEAR_FLAG_C((result >> 16) ^ (result >> 17));" << endl;
			
			ss << "    SET_OR_CLEAR_FLAG_N((word)result < 0);" << endl;
			ss << "    SET_OR_CLEAR_FLAG_Z(result == 0);" << endl;
			
			ss << "  }" << endl;
		}
		ss << "//------------------>" << endl;
	}

	return prevedi(ss);
}
