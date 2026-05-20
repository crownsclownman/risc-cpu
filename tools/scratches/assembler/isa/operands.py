from dataclasses import dataclass

@dataclass
class OperandType:
    name: str

REG     = OperandType("reg")
IMM16   = OperandType("imm16")
SHAMT5  = OperandType("shamt5")
ADDRESS = OperandType("address")
LABEL   = OperandType("label")
