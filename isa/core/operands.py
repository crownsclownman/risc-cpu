class OperandKind:
    REG     = "REG"
    IMM16S  = "IMM16S"   # signed   16-bit immediate
    IMM16U  = "IMM16U"   # unsigned 16-bit immediate
    IMM26U  = "IMM26U"   # unsigned 26-bit jump target
    SHAMT   = "SHAMT"    # 5-bit shift amount (0..31)
    CSR     = "CSR"      # 5-bit CSR index
    BRANCH  = "BRANCH"   # signed 16-bit pc-relative offset, label_ok
    JTARGET = "JTARGET"  # 26-bit absolute target, label_ok
    LABEL   = "LABEL"    # чистая символьная метка

class Operand:
    def __init__(self, name: str):
        self.name = name

    @property
    def kind(self) -> str:
        raise NotImplementedError

class RegisterOperand(Operand):
    def __init__(self, name: str):
        super().__init__(name)

    @property
    def kind(self) -> str:
        return OperandKind.REG

class ImmediateOperand(Operand):
    def __init__(
        self,
        name:      str,
        bits:      int,
        signed:    bool = False,
        label_ok:  bool = False,   # метка разрешена на этой позиции
        pcrel:     bool = False,   # смещение относительно PC
        sem_kind:  str | None = None,  # переопределить автовывод
    ):
        super().__init__(name)
        self.bits     = bits
        self.signed   = signed
        self.label_ok = label_ok
        self.pcrel    = pcrel
        self._sem_kind = sem_kind

    @property
    def kind(self) -> str:
        if self._sem_kind:
            return self._sem_kind
        if self.bits == 5 and self.name in ("shamt",):
            return OperandKind.SHAMT
        if self.bits == 5 and self.name in ("csr",):
            return OperandKind.CSR
        if self.bits == 26:
            return OperandKind.JTARGET
        if self.bits == 16 and self.signed and self.pcrel:
            return OperandKind.BRANCH
        if self.bits == 16 and self.signed:
            return OperandKind.IMM16S
        if self.bits == 16 and not self.signed:
            return OperandKind.IMM16U
        # fallback
        return OperandKind.IMM16U

    @property
    def imm_min(self) -> int:
        if self.signed:
            return -(1 << (self.bits - 1))
        return 0

    @property
    def imm_max(self) -> int:
        if self.signed:
            return (1 << (self.bits - 1)) - 1
        return (1 << self.bits) - 1


class LabelOperand(Operand):
    def __init__(self, name: str):
        super().__init__(name)

    @property
    def kind(self) -> str:
        return OperandKind.LABEL

def reg(name: str) -> RegisterOperand:
    """Регистровый операнд."""
    return RegisterOperand(name)


def imm(name: str, bits: int, signed: bool = False) -> ImmediateOperand:
    """Числовой immediate. Для специфичных случаев используй branch/shamt/csr."""
    return ImmediateOperand(name, bits, signed=signed)


def branch(name: str = "offset") -> ImmediateOperand:
    """
    16-битное знаковое смещение ветвления, PC-relative.
    Принимает метку - ассемблер разрешит в offset.
    """
    return ImmediateOperand(
        name,
        bits     = 16,
        signed   = True,
        label_ok = True,
        pcrel    = True,
        sem_kind = OperandKind.BRANCH,
    )


def jtarget(name: str = "target") -> ImmediateOperand:
    """
    26-битный абсолютный адрес перехода (J-type).
    Принимает метку.
    """
    return ImmediateOperand(
        name,
        bits     = 26,
        signed   = False,
        label_ok = True,
        pcrel    = False,
        sem_kind = OperandKind.JTARGET,
    )


def shamt(name: str = "shamt") -> ImmediateOperand:
    """5-битная величина сдвига (0..31)."""
    return ImmediateOperand(
        name,
        bits     = 5,
        signed   = False,
        label_ok = False,
        sem_kind = OperandKind.SHAMT,
    )


def csr(name: str = "csr") -> ImmediateOperand:
    """5-битный индекс CSR-регистра."""
    return ImmediateOperand(
        name,
        bits     = 5,
        signed   = False,
        label_ok = False,
        sem_kind = OperandKind.CSR,
    )


def label(name: str) -> LabelOperand:
    """Чистая метка - только для псевдоинструкций."""
    return LabelOperand(name)
