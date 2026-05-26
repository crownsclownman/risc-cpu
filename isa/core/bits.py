def get_bits(value, offset, width):
    mask = (1 << width) - 1
    return (value >> offset) & mask

def set_bits(instr, value, offset, width):
    mask = (1 << width) - 1
    instr &= ~(mask << offset)
    instr |= (value & mask) << offset
    return instr
