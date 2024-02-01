#pragma once
#include "pch/pch.h"

namespace x64
{
	enum x64Register : uint8_t
	{
		RA = 0,
		RC,
		RB,
		RD,
		SP,
		BP,
		SI,
		DI,
		R8,
		R9,
		R10,
		R11,
		R12,
		R13,
		R14,
		R15,
		IP,
		IMM,
		DIS,
	};

	enum x64AccessType : uint8_t
	{
		ACCESS_64 = 64,
		ACCESS_32 = 32,
		ACCESS_16 = 16,
		ACCESS_8 = 8,
		ACCESS_8_H = 0,
	};
#pragma pack(push, 1)
	struct x64Operand
	{
		x64Register m_reg;

		union
		{
			struct
			{
				//Register
				x64AccessType m_accessType;
				uint8_t m_derefSize;
				int32_t m_derefOffset;
			};

			struct
			{
				//Immediate
				uint64_t m_val;
			};

			struct
			{
				//Displacement
				int64_t m_displacement;
			};
		};

		void reset()
		{
			m_val = 0;
		}

		void decode(bool rex, uint8_t size, uint8_t reg, bool x)
		{
			reg |= x << 3;
			m_reg = static_cast<x64Register>(reg);
			m_accessType = static_cast<x64AccessType>(size);
			if (reg >= SP && reg <= DI && !rex && size != 64)
			{
				m_reg = static_cast<x64Register>(m_reg - 4);
				m_accessType = ACCESS_8_H;
			}
		}
	};
#pragma pack(pop)
	enum x64OperandEncoding : uint16_t
	{
		ZO = NULL,
		O = 0b001,
		M = 0b010,
		R = 0b011,
		I = 0b100,
		D = 0b101,
		A = 0b111,
		OPERAND_MASK = 0b111,
		BITS_PER_OPERAND = 3,
		MR = M | R << BITS_PER_OPERAND,
		RM = R | M << BITS_PER_OPERAND,
		OI = O | I << BITS_PER_OPERAND,
		MI = M | I << BITS_PER_OPERAND,
		AI = A | I << BITS_PER_OPERAND,
		RMI = R | M << BITS_PER_OPERAND | I << BITS_PER_OPERAND * 2,
	};

	struct x64Operation
	{
		const char* m_name;
		uint16_t m_opcode;
		x64OperandEncoding m_operandEncoding;
		uint8_t m_operandSize;
		uint8_t m_distinguish;
		static constexpr uint8_t m_maxOperands = 3;

		x64Operation(const char* name, uint16_t opcode, x64OperandEncoding operandEncoding) : x64Operation(
			name, opcode, operandEncoding, 0, 8)
		{
		}

		x64Operation(const char* name, uint16_t opcode, x64OperandEncoding operandEncoding,
		             uint8_t operandSize) : x64Operation(name, opcode, operandEncoding, operandSize, 8)
		{
		}

		x64Operation(const char* name, uint16_t opcode, x64OperandEncoding operandEncoding, uint8_t operandSize,
		             uint8_t distinguish) : m_name(name), m_opcode(opcode), m_operandEncoding(operandEncoding),
		                                    m_operandSize(operandSize), m_distinguish(distinguish)
		{
		}

		uint16_t get_unique_id()
		{
			return m_distinguish == 8 ? m_opcode : m_opcode << 3 | m_distinguish;
		}

		bool matches(const uint8_t* code)
		{
			uint16_t b = *code;
			if (b == 0xF)
				++code, b <<= 8, b |= *code;
			if (get_opr_encoding(0) == O)
				b &= ~7;
			return m_opcode == b && (m_distinguish == 8 || (code[1] >> 3 & 7) == m_distinguish);
		}

		x64OperandEncoding get_opr_encoding(uint8_t i)
		{
			return static_cast<x64OperandEncoding>(m_operandEncoding >> BITS_PER_OPERAND * i & OPERAND_MASK);
		}

		uint8_t get_num_operands()
		{
			uint8_t i{};
			while (i != m_maxOperands)
			{
				if (get_opr_encoding(i) == ZO)
					break;
				++i;
			}
			return i;
		}

		uint8_t get_num_modrm_operands()
		{
			uint8_t count{};
			for (uint8_t i{}; i != m_maxOperands; ++i)
				if (x64OperandEncoding enc = get_opr_encoding(i); enc == M || enc == R)
					++count;
			return count;
		}
	};

	inline static x64Operation m_operations[] = {
		{"mov", 0x88, MR, 8},
		{"mov", 0x89, MR},
		{"mov", 0x8A, RM, 8},
		{"mov", 0x8B, RM},
		{"mov", 0xB0, OI, 8},
		{"mov", 0xB8, OI},
		{"mov", 0xC6, MI, 8},
		{"mov", 0xC7, MI, 32},
		{"ret", 0xC3, ZO},
		{"push", 0x50, O, 64},
		{"pop", 0x58, O, 64},
		{"push", 0xFF, M, 64, 6},
		{"add", 0x81, MI, 32, 0},
		{"add", 0x83, MI, 8, 0},
		{"sub", 0x81, MI, 32, 5},
		{"sub", 0x83, MI, 8, 5},
		{"cmp", 0x80, MI, 8, 7},
		{"cmp", 0x83, MI, 8, 7},
		{"cmp", 0x3C, AI, 8},
		{"cmp", 0x3D, AI, 32},
		{"cmp", 0x38, MR, 8},
		{"cmp", 0x39, MR},
		{"lea", 0x8D, RM},
		{"jz", 0x74, D, 8},
		{"jnz", 0x75, D, 8},
		{"call", 0xE8, D, 32},
		{"jmp", 0xEB, D, 8},
		{"jmp", 0xE9, D, 32},
		{"test", 0x84, MR, 8},
		{"test", 0x85, MR},
		{"xor", 0x33, RM},
		{"movzx", 0x0FB6, RM, 8},
		{"movzx", 0x0FB7, RM, 16},
		{"imul", 0x69, RMI, 32},
		{"nop", 0x90, ZO},
	};

	struct x64Instruction
	{
		x64Operation* m_operation = nullptr;
		x64Operand m_operands[x64Operation::m_maxOperands];

		bool is_valid()
		{
			return m_operation != nullptr;
		}

		void reset()
		{
			m_operation = nullptr;
			for (auto& opr : m_operands)
				opr.reset();
		}

		std::string to_bytecode()
		{
			std::string res{};
			if (m_operands[0].m_accessType == ACCESS_64)
				res.push_back('\x48'); //REX.W
			if (m_operation->m_opcode >> 8 & 0xFF)
				res.push_back(m_operation->m_opcode >> 8 & 0xFF);
			res.push_back(m_operation->m_opcode & 0xFF);
			if (m_operation->get_num_modrm_operands() != NULL)
			{
				uint8_t modrm{};
				modrm |= 0b11 << 6; // direct
				modrm |= m_operands[1].m_reg << 3; // reg
				modrm |= m_operands[0].m_reg; // r/m
				res.push_back(modrm);
			}
			return res;
		}
	};

	template <typename T = uint64_t>
	static void get_immediate(T& val, uint8_t* code, uint8_t immBytes)
	{
		++code;
		val = 0;
		for (uint8_t i{immBytes}; i; --i)
			val <<= 8, val |= code[i];
		code += immBytes - 1;
	}

	inline x64Instruction disassemble_instruction_code(uint8_t*& code)
	{
		bool operand_size_override = false;
		bool address_size_override = false;
		bool rex = false;
		bool default_operand_size = true;
		bool reg_x = false;
		bool rm_x = false;
		if (*code == 0x66)
			operand_size_override = true, ++code;
		if (*code == 0x67)
			address_size_override = true, ++code;
		if (*code >> 4 == 4)
		{
			rex = true;
			if (*code >> 3 & 1)
				default_operand_size = false;
			reg_x = *code >> 2 & 1;
			rm_x = *code & 1;
			++code;
		}
		//Opcodes
		x64Instruction res{};
		for (auto& op : m_operations)
		{
			if (op.matches(code))
			{
				if (*code == 0x0F)
					++code;
				res.m_operation = &op;
				if (op.m_operandEncoding != ZO)
				{
					uint8_t operand_size;
					uint8_t immediate_size;
					if (operand_size_override)
						operand_size = immediate_size = 16;
					else if (op.m_operandSize == NULL)
						operand_size = immediate_size = (default_operand_size ? 32 : 64);
					else if (default_operand_size)
						operand_size = immediate_size = op.m_operandSize;
					else
						operand_size = 64, immediate_size = op.m_operandSize;
					uint8_t opcode = *code;
					uint8_t oprI{};
					uint8_t num_modrm_oprs = op.get_num_modrm_operands();
					bool modrm_read{false};
					uint8_t modrm{};
					uint8_t sib{};
					for (uint8_t opr_enc; opr_enc = op.m_operandEncoding >> oprI * BITS_PER_OPERAND & OPERAND_MASK,
					     opr_enc != ZO;)
					{
						x64Operand& opr = res.m_operands[oprI];
						if (opr_enc == O)
						{
							opr.decode(rex, operand_size, opcode & 7, rm_x);
						}
						else if (opr_enc == M || opr_enc == R)
						{
							if (!modrm_read)
								modrm_read = true, modrm = *++code;
							const bool direct = modrm >> 6 == 3;
							if (opr_enc == M)
							{
								opr.decode(rex || !direct, operand_size, modrm & 7, rm_x);
								if (!direct)
								{
									opr.m_accessType = address_size_override ? ACCESS_32 : ACCESS_64;
									opr.m_derefSize = num_modrm_oprs == 2 ? 1 : operand_size;
									//Hiding pointer type when other operand makes it apparent
									if (opr.m_reg == SP)
										sib = *++code;
									switch (modrm >> 6)
									{
									case 0:
										{
											//Absolute address
											if (opr.m_reg == SP && sib == 0x25)
												opr.m_reg = IMM, get_immediate(opr.m_val, code, 4);
												//Read as RIP-relative
											else if (opr.m_reg == BP)
												opr.m_reg = IP, get_immediate(opr.m_derefOffset, code, 4);
										}
										break;
									case 1:
										{
											opr.m_derefOffset = static_cast<int8_t>(*++code);
										}
										break;
									case 2:
										{
											get_immediate(opr.m_derefOffset, code, 4);
										}
										break;
									}
								}
							}
							else if (opr_enc == R)
								opr.decode(rex, operand_size, modrm >> 3 & 7, reg_x);
						}
						else if (opr_enc == I)
							opr.m_reg = IMM, get_immediate(opr.m_val, code, immediate_size / 8);
						else if (opr_enc == D)
						{
							opr.m_reg = DIS;
							get_immediate(opr.m_val, code, immediate_size / 8);
							if (immediate_size == 8)
								opr.m_displacement = static_cast<uint8_t>(opr.m_val);
							else if (immediate_size == 32)
								opr.m_displacement = static_cast<uint32_t>(opr.m_val);
						}
						else if (opr_enc == A)
							opr.decode(rex, operand_size, RA, reg_x);
						if (++oprI == COUNT(res.m_operands))
							break;
					}
				}
				++code;
				break;
			}
		}
		return res;
	}
}
