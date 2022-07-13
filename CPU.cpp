#include "CPU.hpp"
#include "registers.hpp"

#include <iostream>
#include <iomanip>

#define HEX( x ) std::setw(2) << std::setfill('0') << std::hex << (0xFF & x)
#define HEX16( x ) std::setw(4) << std::setfill('0') << std::hex << (0xFFFF & x)


CPU::CPU() {
	ic = 0;
	gpu = std::make_unique<GPU>(m);
	gpu->createScreen("Tetris");
	
}

CPU::~CPU() {
	
}

int CPU::loadROM(std::string path)
{
	inStream.open(path, std::ios::binary);
	std::cout << "file opened\n";
	//readInstruction();
	
	//for (uint16_t addr = 0x0000; addr <= 0x7FFF; addr++) {
	inStream.read((char*)m.mem, 0x7FFF);
	//}
		
	for (uint16_t stack = 0x100; stack <= 0x108; stack++) {
		std::cout << HEX(m.get8(stack)) << " ";
		if (stack % 16 == 0) {
			std::cout << "\n";	
		}
	}
	
	return 0;
}

int CPU::readInstruction() {
	std::string rKey[8] = {"b", "c", "d", "e", "h", "l", "hl", "a"};
	std::string rpKey[4] = {"bc", "de", "hl", "sp"};
	std::string rpKey2[4] = {"bc", "de", "hl", "af"};


	ic++;
	if (ic > 500000) {
		executing = false;
	}
	
	if (inStream.eof()) {
		std::cout << "instructions executed: " << std::dec << ic << std::endl;
		executing = false;
	}
	
	//TEMPORARY
	if (ic % 20 == 0) {
		gpu->scanLine();
	}
	
	uint8_t c;
	
	
	//inStream.seekg(r.get16Reg("pc"), std::ios::beg);
	//std::cout << "\nPosition: " << std::hex << inStream.tellg() << " | ";

	std::cout << "\nPosition: " << std::hex << r.getPC() << " | ";
	
	c = getByte();//m.get8(r.getPC());

	
	//inStream.read(&c, 1);
	//std::cout << "Value: " << HEX(c) << "\n";	
	//r.set16Reg("pc", r.get16Reg("pc") + 1);
	
	//OPCODE decoding from www.z80.info/decoding.htm
	
	uint8_t x = (c >> 6) & 0x3;
	uint8_t z = c & 0x7;
	uint8_t y = (c >> 3) & 0x7;
	uint8_t p = (c >> 4) & 0x3;
	uint8_t q = (c >> 3) & 0x1;
	
	std::cout << "c: " << HEX(c) << ", x: " << HEX(x) << ", z: " << HEX(z) << ", y: " << HEX(y);
	std::cout << ", sp: " << HEX16(r.get16Reg("sp")) << ", af: " << HEX16(r.get16Reg("af")) << ", hl: " << HEX16(r.get16Reg("hl"))  << "\n";
	
	//uint16_t bp[100] = [0x]
	
	switch(x) {
		case(0x0):
			switch(z) {
				case(0x0):
					switch(y) {
						case(0x0):
							std::cout << "NOP\n";
							break;
						case(0x1):
							LDSP_to_16i();
							break;
						case(0x2):
							STOP();
							break;
						case(0x3):
							JR_n();
							break;
						case(0x4):
							JR(y);
							break;
						case(0x5):
							JR(y);
							break;
						case(0x6):
							JR(y);
							break;
						case(0x7):
							JR(y);
							break;
						default:
							exit(0);
					}
				break;
				case(0x1):
					//q is the bottom bit of Y
					switch(q) {
						case(0x0):
							LD16i_to_reg(rpKey[p]);
						break;
						case(0x1):
							ADD_HL(rpKey[p]);
						break;
						default:
							exit(0);
					}
				break;
				case(0x2):
					//q is the bottom bit of Y
					std::cout << "q = " << HEX(q) << " p = " << HEX(p) << "\n";
					switch(q) {
						case(0x0):
							switch(p) {
								case(0x0):
									LD_BC_a();
								break;
								case(0x1):
									//LD8i_to_reg("c");
									exit(0);
									break;
								case(0x2):
									LDI_HL_A();
									break;
								case(0x3):
									LDD_HL_A();
									break;
								default:
									exit(0);
							}
							break;
						case(0x1):
							switch(p) {
								case(0x01):
									//HERE======================================================================
									LD_a_mem("de");
									//LD_mem_a("de");
									break;
								case(0x02):
									LDI_a_hl();
									break;
								default:
									exit(0);
									break;
							}
						//default:
							//exit(0);
					}
				break;
				case(0x3):
					switch(q) {
						case(0x0):
							INC_nn(rpKey[p]);
							break;
						case(0x1):
							DEC_nn(rpKey[2]);
							break;
						default:
							exit(0);
					}
				break;
				case(0x4):
					INC(rKey[y]);
				break;
				case(0x5):
					DEC(rKey[y]);
				break;
				case(0x6):
					LD8i_to_reg(rKey[y]);
				break;
				case(0x7):
					switch(y) {
						case(0x0):
							RLCA();
							break;
						case(0x1):
							RRCA();
							break;
						case(0x3):
							RRA();
							break;
						case(0x5):
							CPL();
							break;
						case(0x7):
							CCF();
							break;
						default:
							exit(0);
					}
				break;
			}
		break;
		case(0x1): //XOR A & A = 0, set F to 0x80
			if (z == 6 && y == 6) {
				exit(0);
			} else {
				LDreg_to_reg(rKey[y], rKey[z]);
			}
		break;
		case(0x2): //Always gonna be an ALU op
			switch(y) {
				case(0x0):
					ADD(rKey[z]);
					break;
				case(0x1):
					ADC(rKey[z]);
					break;
				case(0x2):
					SUB(rKey[z]);
					break;
				case(0x3):
					SBC(rKey[z]);
					break;
				case(0x4):
					AND(rKey[z]);
					break;
				case(0x5):
					XOR(rKey[z]);
					break;
				case(0x6):
					OR(rKey[z]);
					break;
				case(0x7):
					CP(rKey[0]);
					break;
				default:
					exit(0);
			}
		break;
		case(0x3): //XOR A & A = 0, set F to 0x80
			switch(z) {
				case(0x0):
					switch(y) {
						case(0x00):
							RET(y);
						break;
						case(0x01):
							RET(y);
						break;
						case(0x02):
							RET(y);
						break;
						case(0x03):
							RET(y);
						break;
						case(0x04):
							LDH(0);
						break;
						case(0x06):
							LDH(1);
						break;
						case(0x07):
							LDHL();
						break;
						default:
							exit(0);
					}
				break;
				case(0x1):
					switch(q) {
						case(0x0):
							POP(rpKey2[p]);
						break;
						case(0x1):
							switch(p) {
								case(0x0):
									RET();
									break;
								case(0x3):
									LDHL_to_SP();
									break;
								default:
									exit(0);
							}
						break;
					}
				break;
				case(0x2):
					switch(y) {
						case(0x5):
							LD_n_a("imm");
							break;
						case(0x7):
							LD_a_mem_nn();
							break;
						default:
							exit(0);
							JP_flag(y);
							break;
					}
				break;
				case(0x3):
					switch(y) {
						case(0x0):
							JP();
						break;
						case(0x1):
							//CB Prefix OPCODE
						break;
						case(0x6):
							DI();
						break;
						default:
							exit(0);
					}
				break;
				case(0x4):
					//No a call????? 3, 4, 0 - was a call statement
					CALL(y);
				break;
				case(0x5):
					switch(q) {
						case(0x0):
							PUSH(rpKey2[p]);
							break;
						case(0x1):
							switch(p) {
							case(0x0):
								CALL();
								break;
							default:
								exit(0);	
							}
						break;
						default:
							exit(0);
					}
				break;
				case(0x6):
					switch(y) {
						case(0x0):
							ADD("imm");
							break;
						case(0x1):
							ADC("imm");
							break;
						case(0x2):
							SUB("imm");
							break;
						case(0x4):
							AND("imm");
							break;
						case(0x7):
							CP("imm");
							break;
						default:
							exit(0);
					}
				break;
				case(0x7):
					RST(y);
				break;
			}
		break;
		default:
			exit(0);
		break;
			
	}

	if (interruptChange) {
		interruptChange--;
		if (!interruptChange) {
			interruptEnabled = !interruptEnabled;
		}
	}
	
	return 0;
}

uint8_t CPU::getByte() {
	//std::cout << "Reading byte: " << HEX16(r.getPC()) << "\n";
	
	uint8_t byte = m.get8(r.getPC());
	r.incPC();
	
	return(byte);
}

void CPU::JP() {
	uint8_t lBit = getByte();
	uint8_t hBit = getByte();

	uint16_t jumpLocation;

	//std::cout << "JP: " << (c1 << 8) << " | " << HEX(c2) <<"\n";
	jumpLocation = ((hBit << 8) | (uint8_t)lBit);
	std::cout << "JP: " << jumpLocation <<"\n";

	r.set16Reg("pc", jumpLocation);
}

void CPU::JP_flag(uint8_t condition) {
	bool jumping = false;
	uint8_t flags = r.get8Reg("f");
	uint8_t z = flags & 0x80;
	uint8_t c = flags & 0x10;
	
	std::cout << "JR " << HEX(condition) << "\n";
	std::cout << "z = " << HEX(z) << " c = " << HEX(c) << "\n";

	//Maybe bad logic? check when this function is implenented
	switch (condition) {
			case(0x4):
				if (z == 0) {
					jumping = true;
				}
			break;
			case(0x5):
				if (z) {
					jumping = true;
				}
			break;
			case(0x6):
				if (c == 0) {
					jumping = true;
				}
			break;
			case(0x7):
				if (c) {
					jumping = true;
				}
			break;
	}
	
					
					
	if (jumping) {
		uint8_t c1 = getByte();
		uint8_t c2 = getByte();

		uint16_t jumpLocation;

		//std::cout << "JP: " << (c1 << 8) << " | " << HEX(c2) <<"\n";
		jumpLocation = ((c1 << 8) | c2);
		std::cout << "JP: " << jumpLocation <<"\n";

		r.set16Reg("pc", jumpLocation);
	}
}

void CPU::JR_n() {
	char c = getByte();
	//r.set16Reg("pc", r.get16Reg("pc") + 1);

	uint16_t pc = r.get16Reg("pc");
	
	pc += (uint8_t)c;
	
	r.set16Reg("pc", pc);
	
	std::cout << "jr, " << HEX(c) << "\n";
}

void CPU::JR(uint8_t condition) {
	bool jumping = false;
	uint8_t flags = r.get8Reg("f");
	uint8_t z = flags & 0x80;
	uint8_t c = flags & 0x10;
	
	std::cout << "JR condition is: " << HEX(condition) << "\n";

	
	switch (condition) {
			case(0x4):
				if (z == 0) {
					jumping = true;
				}
			break;
			case(0x5):
				if (z) {
					jumping = true;
				}
			break;
			case(0x6):
				if (c == 0) {
					jumping = true;
				}
			break;
			case(0x7):
				if (c) {
					jumping = true;
				}
			break;
	}
	
					
					
	if (jumping) {
		int8_t n = getByte();

		r.set16Reg("pc", r.get16Reg("pc") + n);
		std::cout << " immediate is: " << HEX(n) << "\n";
	} else {
		r.incPC();
	}

	std::cout << "hl = " << HEX16(r.get16Reg("hl")) << " |  bc = " << HEX16(r.get16Reg("bc")) << "\n";
}

void CPU::ADD(std::string regName) {
	uint8_t op1;
	uint8_t op2 = r.get8Reg("a");

	uint8_t resultFlags = 0x00;
	
	if (regName == "imm") {
		op1 = getByte();
	}
	else
		op1 = r.get8Reg(regName);
	
	uint8_t result = op1 + op2;
	uint16_t carry = op1 + op2;
		
	if (result == 0x00) {
		resultFlags = 0x80;
	}
	if (((op1 & 0x0f) + (op2 & 0x0f)) & 0x10) {
		resultFlags = resultFlags | 0x10;
	}
	if (carry > 0xff) {
		resultFlags = resultFlags | 0x20;
	}
	
	r.set8Reg("a", result);
	r.set8Reg("f", resultFlags);
	
	std::cout << "ADD a, " << regName << "\n";
}

void CPU::ADC(std::string regName) {
	uint8_t op1;
	uint8_t op2 = (r.get8Reg("f") & 0x10) >> 4;
	uint8_t op3 = r.get8Reg("a");

	uint8_t resultFlags = 0x00;
	
	if (regName == "imm") {
		op1 = getByte();
	}
	else
		op1 = r.get8Reg(regName);
	
	uint8_t result = op1 + op2 + op3;
	uint32_t carry = op1 + op2 + op3;
		
	if (result == 0x00) {
		resultFlags = 0x80;
	}
	if (((op1 & 0x0f) + (op2 & 0x0f) + (op3 & 0x0F)) & 0x10) {
		resultFlags = resultFlags | 0x10;
	}
	if (carry > 0xff) {
		resultFlags = resultFlags | 0x20;
	}
	
	r.set8Reg("a", result);
	r.set8Reg("f", resultFlags);
	
	std::cout << "ADC " << regName << "\n";
}

void CPU::ADD_HL(std::string regName) {
	r.set16Reg("hl", r.get16Reg("hl") + r.get16Reg(regName));
	std::cout << "ADD HL, " << regName << "\n";
}

void CPU::SUB(std::string regName) {
	uint8_t op1;
	if (regName == "imm") {
		op1 = getByte();
	} else {
		op1 = r.get8Reg(regName);
	}
	uint8_t op2 = r.get8Reg("a");
	uint8_t result = op2 - op1;
	uint16_t carry = op2 - op1;

	uint8_t resultFlags = 0x40;
		
	if (result == 0x00) {
		resultFlags |= 0x80;
	}
	//if ((op1 & 0x0f) + (op2 & 0x0f) > 0x0f) {
	//	resultFlags &= 0x10;
	//}

	if (carry > 0xFF) {
		resultFlags |= 0x10;
	}
	
	r.set8Reg("a", result);
	r.set8Reg("f", resultFlags);
	
	std::cout << "SUB a, " << regName << "\n";
}

void CPU::AND(std::string regName) {
	uint8_t op1;
	if (regName == "imm")
		op1 = getByte();		
	else
		op1 = r.get8Reg(regName);
	
	uint8_t op2 = r.get8Reg("a");
	uint8_t result = op2 & op1;
	
	uint8_t resultFlags = 0x20;
		
	if (result == 0x00) {
		resultFlags = 0x80;
	}
	
	r.set8Reg("f", resultFlags);
	
	std::cout << "AND	A, " << regName << "\n";
}

void CPU::CPL() {
	uint8_t flags = r.get8Reg("f");
	
	flags |= 0x60;
	
	uint8_t newA = r.get8Reg("a");
	
	r.set8Reg("a", !newA);
	
	std::cout << "CPL\n";
}

void CPU::SBC(std::string regName) {
	uint8_t op1 = r.get8Reg(regName);
	uint8_t op2 = r.get8Reg("a");
	uint8_t flags = r.get8Reg("f");
	uint8_t c = (flags & 0x10) >> 4;
	
	uint8_t result = op2 - op1 + c;

	uint8_t resultFlags = 0x40;
		
	if (result == 0x00) {
		resultFlags |= 0x80;
	}
	if ((op1 & 0x08) && !(op2 & 0x08)) {
		resultFlags = resultFlags & 0x20;
	}
	if ((op1 & 0x80) && !(op2 & 0x80)) {
		resultFlags = resultFlags & 0x10;
	}
}


void CPU::DEC(std::string regName) {
	uint8_t result = r.get8Reg(regName) - 1;
	uint8_t flags = 0x40;
	
	if (result == 0x00) {
		flags = flags | 0x80;
	}
	
	uint8_t half_carry = (r.get8Reg(regName) ^ (-1) ^ result) & 0x10;
	if (half_carry) {
		flags = flags | (0x20);
	}
	
	flags = flags | (r.get8Reg("f") & 0x10);
	
	r.set8Reg("f", flags);
	r.set8Reg(regName, result);
	std::cout << "DEC " << regName << "\n";
	
}

void CPU::INC(std::string regName) {
	uint8_t result = r.get8Reg(regName) + 1;
	uint8_t flags = 0x40;
	
	if (result == 0x00) {
		flags = flags | 0x80;
	}
	
	uint8_t half_carry = (r.get8Reg(regName) ^ (-1) ^ result) & 0x10;
	if (half_carry) {
		flags = flags | (0x20);
	}
	
	flags = flags | (r.get8Reg("f") & 0x10);
	
	r.set8Reg("f", flags);
	r.set8Reg(regName, result);
	std::cout << "INC " << regName << "\n";
}

void CPU::INC_nn(std::string regName) {

	r.set16Reg(regName, r.get16Reg(regName) + 1);

	std::cout << "INC " << regName << "\n";
}

void CPU::DEC_nn(std::string regName) {
		
	r.set16Reg(regName, r.get16Reg(regName) - 1);

	std::cout << "DEC " << regName << "\n";
}

void CPU::RLCA() {
	uint8_t newCarry = r.get8Reg("a") & 0x80;
	uint8_t result = r.get8Reg("a") << 1;
	
	
	result = result | (newCarry >> 8);
	
	uint8_t newFlag = 0x00;
	if (result == 0x00) {
		newFlag = 0x80;
	}
	
	newFlag = newFlag | (newCarry << 4);
	
	r.set8Reg("f", newFlag);
	r.set8Reg("a", result);
	
	std::cout << "RLCA\n";
	
}

void CPU::RRA() {
	uint8_t newCarry = r.get8Reg("a") & 0x1;
	uint8_t result = r.get8Reg("a") >> 1;
	uint8_t newTopBit = (r.get8Reg("f") & 0x10) << 3;
	
	result = result | newTopBit ;
	
	uint8_t newFlag = 0x00;
	if (result == 0x00) {
		newFlag = 0x80;
	}
	
	newFlag = newFlag | (newCarry << 5);
	
	r.set8Reg("f", newFlag);
	r.set8Reg("a", result);
	
	std::cout << "RRA\n";
}

void CPU::RRCA() {
	uint8_t newCarry = r.get8Reg("a") & 0x1;
	uint8_t result = r.get8Reg("a") >> 1;
	
	uint8_t newFlag = 0x00;
	if (result == 0x00) {
		newFlag = 0x80;
	}
	
	newFlag = newFlag | (newCarry << 5);
	
	r.set8Reg("f", newFlag);
	r.set8Reg("a", result);
	
	std::cout << "RRCA\n";
}

void CPU::RST(uint8_t offset) {
	r.set16Reg("sp", r.get16Reg("sp")-2);
	m.save16(r.get16Reg("sp"), r.get16Reg("pc"));
	r.set16Reg("pc", offset*8);

	exit(0);
	std::cout << "RST\n";	
}

void CPU::RET() {
	std::cout << "Stack before ret:\n";
	
	for (uint16_t stack = r.get16Reg("sp"); stack <= 0xDFFF; stack++) {
		std::cout << HEX(m.get8(stack)) << " ";
		if (stack % 16 == 0) {
			std::cout << "\n";	
		}
	}
	
	
	//NEEDS TO BE FIXED?
	uint16_t newPC;
	newPC = m.get8(r.getSP());
	newPC |= (m.get8(r.incSP()) << 8);
	r.incSP();

	r.set16Reg("pc", newPC);
	
	std::cout << "RET, new PC = " << HEX16(newPC) << "\n";
}

void CPU::RET(uint8_t condition) {
	bool jumping = false;
	uint8_t flags = r.get8Reg("f");
	uint8_t z = flags & 0x80;
	uint8_t c = flags & 0x10;
		
	switch (condition) {
			case(0x0):
				if (z == 0) {
					jumping = true;
				}
			break;
			case(0x1):
				std::cout << "Checking if Z is set\n";
				if (z) {
					std::cout << "Z is set, returning";
					jumping = true;
				}
			break;
			case(0x2):
				if (c == 0) {
					jumping = true;
				}
			break;
			case(0x3):
				if (c == 1) {
					jumping = true;
				}
			break;
	}
	
	if (jumping == true) {
		//r.set16Reg("pc", r.get16Reg("pc") + 1);
		RET();
	} else {
		std::cout << "RET NO JUMP\n";
	}
}

void CPU::CALL() {
	//TODO
	uint16_t pc = r.get16Reg("pc") + 2;
	uint16_t sp = r.decSP();
	m.save8(sp, pc >> 8);
	sp = r.decSP();
	m.save8(sp,  pc & 0xFF);	
	
	JP();
	
	std::cout << "CALL\n";	
	
	
		
	std::cout << "Stack after call:\n";
	
	for (uint16_t stack = r.get16Reg("sp"); stack <= 0xDFFF; stack++) {
		std::cout << HEX(m.get8(stack)) << " ";
		if (stack % 16 == 0) {
			std::cout << "\n";	
		}
	}
	std::cout << "\n";
	
}

void CPU::CALL(uint8_t condition) {
	bool jumping = false;
	uint8_t flags = r.get8Reg("f");
	uint8_t z = flags & 0x80;
	uint8_t c = flags & 0x10;
		
	switch (condition) {
			case(0x0):
				if (z == 0) {
					jumping = true;
				}
			break;
			case(0x1):
				if (z) {
					jumping = true;
				}
			break;
			case(0x2):
				if (c == 0) {
					jumping = true;
				}
			break;
			case(0x3):
				if (c) {
					jumping = true;
				}
			break;
	}
	
	if (jumping == true) {
		//r.set16Reg("pc", r.get16Reg("pc") + 1);
		CALL();
	} else {
		std::cout << "CALL NO JUMP\n";
	}
}

void CPU::CP(std::string regName) {
	uint8_t op1;
	if (regName == "imm") {
		op1 = getByte();		
	} else {
		op1 = r.get8Reg(regName);
	}
	
	uint8_t op2 = r.get8Reg("a");
	
	uint8_t result = op2 - op1;
	uint8_t flags = 0x40;
	
	if (result == 0x00) {
		flags = flags | 0x80;
	}
	
	uint8_t half_carry = (op2 ^ op1 ^ result) & 0x10;
	if (half_carry) {
		flags = flags | (0x20);
	}
	
	if (op2 < op1) {
		flags = flags | 0x10;
	}
	
	
	r.set8Reg("f", flags);
	std::cout << "CP A, " << regName << "\n";
	
}

void CPU::LD8i_to_reg(std::string regName) {
	uint8_t b = getByte();
	
	r.set8Reg(regName, b);
		
	std::cout << "LD: " << regName << ", " << HEX(b) << "\n";
}

void CPU::LDreg_to_reg(std::string reg1Name, std::string reg2Name) {
	if (reg1Name == "HL" || reg1Name == "hl") {
		m.save8(r.get16Reg(reg1Name), r.get8Reg(reg2Name));
		std::cout << "LD (" << HEX16(m.get8(r.get16Reg(reg1Name))) << "), " << reg2Name << "\n";
	} else {
		r.set8Reg(reg1Name, r.get8Reg(reg2Name));
		std::cout << "LD " << reg1Name << ", " << reg2Name << "\n";

	}
}

void CPU::LD16i_to_reg(std::string regName) {
	uint8_t lByte = getByte();
	uint8_t hByte = getByte();
	uint16_t val = (hByte << 8) | lByte;
	
	std::cout << "LD: " << regName  << ", " << HEX16(val) << "\n";
	r.set16Reg(regName, val);
}

void CPU::LD_n_a(std::string regName) {
	uint16_t addr;
	std::cout << "LD ";
	if (regName.length() == 1) {
		r.set8Reg(regName, r.get8Reg("a"));
	} else {
		if (regName == "imm") { 		
				uint8_t lByte = getByte();
				uint8_t hByte = getByte();
				addr = (hByte << 8) | lByte;
		} else {
			addr = m.get8(r.get16Reg(regName));
		}
			
		m.save8(addr, r.get8Reg("a"));
		std::cout << "(" << addr << ")";
	}
	std::cout << ", a\n";

}

void CPU::LDSP_to_16i() {
	uint8_t lByte = getByte();
	uint8_t hByte = getByte();
	uint16_t address = (hByte << 8) | lByte;
	
	m.save16(address, r.get16Reg("sp"));
	
	std::cout << "LD: sp to " << std::hex << address <<"\n";
}

void CPU::LDHL_to_SP() {
	r.set16Reg("sp", r.get16Reg("hl"));
	
	std::cout << "LD SP, HL\n";
}

void CPU::LD_a_mem(std::string regName) {
	uint8_t val = m.get8(r.get16Reg(regName));
	
	r.set8Reg("a", val);
	
	std::cout << "LD a, (" << HEX16(val) << ")\n";
}

///??? IS THIS FUNCTION DOING?? 1a is ld a, (de) but that opcode calls this???
void CPU::LD_mem_a(std::string regName) {
	uint8_t val = r.get8Reg("a");
	uint16_t location;
	if (regName == "imm") {
		uint8_t lByte = getByte();
		uint8_t hByte = getByte();
		location = (hByte << 8) | lByte;
	}
	else
		location = r.get16Reg(regName);
	
	m.save8(location, val);

	std::cout << "LD (" << HEX16(m.get8(r.get16Reg(regName))) << "), a\n";
}

void CPU::LD_a_mem_nn() {
	uint8_t lByte = getByte();
	uint8_t hByte = getByte();
	uint16_t location = (hByte << 8) | lByte;
	
	uint8_t val = m.get8(location);
	
	r.set8Reg("a", val);
	
	std::cout << "LD a, (imm)\n";
}

void CPU::LD_BC_a() {
	uint16_t address = r.get16Reg("bc");
	
	m.save8(address, r.get16Reg("a"));
	
	std::cout << "LD: (BC), a | bc = " << std::hex << address <<"\n";
}

void CPU::POP(std::string regName) {
	uint16_t val;
	val = m.get8(r.getSP());
	val |= (m.get8(r.incSP()) << 8);
	r.incSP();
	
	r.set16Reg(regName, val);
	

	std::cout << "pop " << regName << "\n";
}

void CPU::PUSH(std::string regName) {
	
	
	uint16_t val = r.get16Reg(regName);
	uint16_t sp = r.decSP();
	m.save8(sp, val >> 8);
	sp = r.decSP();
	m.save8(sp, val & 0xFF);
	
	std::cout << "PUSH " << regName << "\n";

}

void CPU::LDD_HL_A() {
	//TODO
	std::cout << "LDD (HL), A\n";
}

void CPU::LDI_HL_A() {
	m.save8(r.get16Reg("hl"), r.get8Reg("a"));
	r.set16Reg("hl", r.get16Reg("hl") + 1);
	std::cout << "LDI (HL), A\n";
}

void CPU::LDI_a_hl() {
	uint8_t newVal;
	
	newVal = m.get8(r.get16Reg("hl"));
	
	r.set8Reg("a", newVal);
	
	r.set16Reg("hl", r.get16Reg("hl") + 1);
	
	std::cout << "LDI a, (HL)\n";
	
}
	
void CPU::LDHL() {
	//TODO flags
	uint16_t result = r.get16Reg("sp");
	
	uint8_t n = getByte();
	
	result += n;
	
	r.set16Reg("hl", result);
	
	std::cout << "LDHL\n";
}

void CPU::LDH(int mode) {
	uint8_t n = getByte();
	
	uint16_t add = 0xFF00 + n;

	std::cout << "LDH: ";
	//TODO flags
	if (mode == 0) {
		std::cout << "\n";
		uint8_t value = r.get8Reg("a");
		
		m.save8(add ,value);
	}
	if (mode == 1) {
		std::cout << "storing " << std::hex << add << " into a\n";
		uint8_t result = m.get8(add);
		r.set8Reg("a", result);
		//std::cout << "a contains" << HEX(r.get8Reg("a"))<< "\n";
	}
}

void CPU::XOR(std::string regName) {
	uint8_t result = r.get8Reg("a") ^ r.get8Reg(regName);
	uint8_t flags = 0x00;
	if (result == 0x00) {
		flags = 0x80;
	}
	std::cout << "XOR: A^" << regName << "\n";
	
	r.set8Reg("a", result);
	r.set8Reg("f", flags);
}

void CPU::OR(std::string regName) {
	uint8_t result = r.get8Reg("a") | r.get8Reg(regName);
	//std::cout << "res = " << HEX(result) << "\n";
	uint8_t flags = 0x00;
	if (result == 0x00) {
		flags = 0x80;
	}
	//result = (result << 8) & flags;
	//std::cout << "flags = " << HEX(flags) << "\n";

	std::cout << "OR: A|" << regName << "\n";
	
	r.set8Reg("a", result);
	r.set8Reg("f", flags);
}

void CPU::LD_SP() {
		uint32_t a = 0x0000;
		uint16_t b = 0x00;
		m.save16(a, b);
}

void CPU::CCF() {
	uint8_t flags = r.get8Reg("f");

	flags &= 0x90;
	flags ^= 1UL << 5;
	
	r.set8Reg("f", flags);
}

void CPU::DI() {
	interruptChange = 2;
	std::cout << "disabling interrupts\n"; 
}

void CPU::STOP() {
		
}