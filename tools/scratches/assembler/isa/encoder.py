def insert_bits(word, value, shift, width):
    mask = ((1 << width) - 1) << shift

    word &= ~mask

    word |= (value << shift) & mask

    return word


def encode_instruction(desc, operands):
    word = 0

    field_map = {}

    for field in desc.fmt.fields:
        field_map[field.name] = field

    #
    # Fixed fields
    #

    for name, value in desc.fixed_fields.items():
        field = field_map[name]

        word = insert_bits(
            word,
            value,
            field.shift,
            field.width
        )

    #
    # Operand fields
    #

    for mapping in desc.operand_map:
        field = field_map[mapping.field_name]

        value = operands[mapping.operand_index]

        word = insert_bits(
            word,
            value,
            field.shift,
            field.width
        )

    return word
