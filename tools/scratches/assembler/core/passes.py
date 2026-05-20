from parser.ast import *

def first_pass(nodes):
    address = 0
    symbols = {}

    for node in nodes:
        if isinstance(node, LabelNode):
            symbols[node.name] = address

        elif isinstance(node, InstructionNode):
            address += 4

    return symbols
