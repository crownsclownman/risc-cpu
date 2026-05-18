#ifndef ENCODING_H
#define ENCODING_H

#define GET_OPCODE(x) (((x) >> 26) & 0x3F)

#define GET_RS(x)     (((x) >> 21) & 0x1F)
#define GET_RT(x)     (((x) >> 16) & 0x1F)
#define GET_RD(x)     (((x) >> 11) & 0x1F)

#define GET_SHAMT(x)  (((x) >> 6) & 0x1F)
#define GET_FUNCT(x)  ((x) & 0x3F)

#define GET_IMM(x)    ((x) & 0xFFFF)

#define GET_TARGET(x) ((x) & 0x03FFFFFF)

#endif
