This CPU implements a 32-bit RISC architecture. Each instruction is 4 bytes long.
Register set:
    R0:     zero register
    R1-31:  general purpose
    PC:     program counter
    EPC:    (add desc later)
    CAUSE:  (add desc later)
Privileged modes: Kernel / User
Address: 32-bit virtual addresses
Interrupts: Vector-based, 32 vectors.
MMU: 4KB Pages, 32-entry TLB, R/W/X/U bits.
Timer: MITME + MITMECMP
Peripherals:
