import isa.defs.alu
import isa.defs.branch
#import isa.defs.memory
#import isa.defs.system

from isa.core.registry import INSTRUCTIONS

for ins in INSTRUCTIONS:
    print(ins.mnemonic)
