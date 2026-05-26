class Operand:
    def __init__(self, name):
        self.name = name


class RegisterOperand(Operand):
    def __init__(self, name):
        super().__init__(name)


class ImmediateOperand(Operand):
    def __init__(
        self,
        name,
        bits,
        signed=False
    ):
        super().__init__(name)

        self.bits = bits
        self.signed = signed


class LabelOperand(Operand):
    def __init__(self, name):
        super().__init__(name)

def reg(name):
    return RegisterOperand(name)


def imm(name, bits, signed=False):
    return ImmediateOperand(
        name,
        bits,
        signed
    )


def label(name):
    return LabelOperand(name)
