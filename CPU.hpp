#ifndef CPU_H
#define CPU_H

#include "registers.hpp"
#include "GPU.hpp"
#include "memory.hpp"

#include <fstream>
#include <memory>

class CPU
{
	public:
		bool executing = true;
		int insCount = 0;
	
		CPU();
		~CPU();
		int loadROM(std::string);
		int readInstruction();
	private:
	
	//GPU gpu;
		std::unique_ptr<GPU> gpu;

		registers r;
		memory m;
	
		bool interruptEnabled = true;
		int interruptChange = 0;
		std::ifstream inStream;
		uint64_t ic;
	
		uint8_t getByte();
	
		void JP();
		void JP_flag(uint8_t);
		void JR_n();
		void JR(uint8_t);
	
		void ADD(std::string);
		void ADD_HL(std::string);
		void ADC(std::string);


		void SUB(std::string);
	
		void AND(std::string);

		void CPL();
	
		void SBC(std::string);
	
		void DEC(std::string);
		void INC(std::string);
	
		void CP(std::string);

		void RLCA();

		void RRA();
		void RRCA();

		void INC_nn(std::string);
		void DEC_nn(std::string);
	
		void RST(uint8_t);
		void CALL();
		void CALL(uint8_t);


		void RET();
		void RET(uint8_t);
	
		void LD8i_to_reg(std::string);
		void LDreg_to_reg(std::string, std::string);
		void LD16i_to_reg(std::string);
		void LD_n_a(std::string);
		void LDSP_to_16i();
		void LD_BC_a();
		void LDHL_to_SP();
	
		void LD_a_mem(std::string);
		void LD_mem_a(std::string);
		void LD_a_mem_nn();

	
		void POP(std::string);
		void PUSH(std::string);

	
		void LDD_HL_A();
		void LDI_HL_A();
		void LDI_a_hl();
	
		void LDHL();
		void LDH(int);

		void OR(std::string);
		void XOR(std::string);
	
		void LD_SP();
	
		void CCF();
	
		void DI();
		void STOP();
		
};

#endif