from dataclasses import dataclass

@dataclass
class InstructionNode:
    mnemonic: str
    operands: list

@dataclass
class LabelNode:
    name: str
