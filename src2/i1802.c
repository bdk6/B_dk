/////////////////////////////////////////////////////////////////////////////
/// i1802.c
/// copyright 2024 William R Cooke
/// Interpreter / simulator for CDP1802 microprocessor
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

#define RAM_SIZE 65536
#define NUM_REGS 16

enum opcodes
{
  OP_IDL      = 0x00,
  OP_LDN      = 0x00,  // 0x01 to 0x0f
  OP_INC      = 0x10,  // 0x10 to 0x1f
  OP_DEC      = 0x20,  // 0x20 to 0x2f
  OP_BR       = 0x30, 
  OP_BQ       = 0x31,
  OP_BZ       = 0x32,
  OP_BDF      = 0x33,
  OP_BPZ      = 0x33,
  OP_BGE      = 0x33,  // Three mnemonics, same instruction
  OP_B1       = 0x34,
  OP_B2       = 0x35,
  OP_B3       = 0x36,
  OP_B4       = 0x37,
  OP_NBR      = 0x38,
  OP_SKP      = 0x38,  // Two mnemonics, same instruction
  OP_BNQ      = 0x39,
  OP_BNZ      = 0x3a,
  OP_BNF      = 0x3b,
  OP_BM       = 0x3b,
  OP_BL       = 0x3b,  // Three mnemonics, same instruction
  OP_BN1      = 0x3c,
  OP_BN2      = 0x3d,
  OP_BN3      = 0x3e,
  OP_BN4      = 0x3f,
  OP_LDA      = 0x40,  // 0x40 to 0x4f
  OP_STR      = 0x50,  // 0x50 to 0x5f
  OP_IRX      = 0x60,
  OP_OUT1     = 0x61,
  OP_OUT2     = 0x62,
  OP_OUT3     = 0x63,
  OP_OUT4     = 0x64,
  OP_OUT5     = 0x65,
  OP_OUT6     = 0x66,
  OP_OUT7     = 0x67,
  OP_RESVD    = 0x68,
  OP_INP1     = 0x69,
  OP_INP2     = 0x6a,
  OP_INP3     = 0x6b,
  OP_INP4     = 0x6c,
  OP_INP5     = 0x6d,
  OP_INP6     = 0x6e,
  OP_INP7     = 0x6f,
  OP_RET      = 0x70,
  OP_DIS      = 0x71,
  OP_LDXA     = 0x72,
  OP_STXD     = 0x73,
  OP_ADC      = 0x74,
  OP_SDB      = 0x75,
  OP_SHRC     = 0x76,
  OP_RSHR     = 0x76, // Two mnemonics, same instruction
  OP_SMB      = 0x77,
  OP_SAV      = 0x78,
  OP_MARK     = 0x79,
  OP_SEQ      = 0x7a,
  OP_REQ      = 0x7b,
  OP_ADDI     = 0x7c,
  OP_SDBI     = 0x7d,
  OP_SHLC     = 0x7e,
  OP_RSHL     = 0x7e,  // Two mnemonics, same instruction
  OP_SMBI     = 0x7f,
  OP_GLO      = 0x80,  // 80 to 8f
  OP_GHI      = 0x90,  // 90 to 9f
  OP_PLO      = 0xa0,  // a0 to af
  OP_PHI      = 0xb0,  // b0 to bf
  OP_LBR      = 0xc0,
  OP_LBQ      = 0xc1,
  OP_LBZ      = 0xc2,
  OP_LBDF     = 0xc3,
  OP_NOP      = 0xc4,
  OP_LSNQ     = 0xc5,
  OP_LSNZ     = 0xc6,
  OP_LSNF     = 0xc7,
  OP_LSKP     = 0xc8,
  OP_NLBR     = 0xc8,  // Two mnemonics, same instruction
  OP_LBNQ     = 0xc9,
  OP_LBNZ     = 0xca,
  OP_LBNF     = 0xcb,
  OP_LSIE     = 0xcc,
  OP_LSQ      = 0xcd,
  OP_LSZ      = 0xce,
  OP_LSDF     = 0xcf,
  OP_SEP      = 0xd0, // d0 to df
  OP_SEX      = 0xe0,  // e0 to ef
  OP_LDX      = 0xf0,
  OP_OR       = 0xf1,
  OP_AND      = 0xf2,
  OP_XOR      = 0xf3,
  OP_ADD      = 0xf4,
  OP_SD       = 0xf5,
  OP_SHR      = 0xf6,
  OP_SM       = 0xf7,
  OP_LDI      = 0xf8,
  OP_ORI      = 0xf9,
  OP_ANI      = 0xfa,
  OP_XRI      = 0xfb,
  OP_ADI      = 0xfc,
  OP_SDI      = 0xfd,
  OP_SHL      = 0xfe,
  OP_SMI      = 0xff

};

// The working store
uint8_t mem[RAM_SIZE];

// The registers
uint16_t reg[NUM_REGS];

// The other regs
uint8_t reg_P;  // pc reg
uint8_t reg_X;  // pointer reg
uint8_t reg_I;  // instruction reg  (I in high nibble)
uint8_t reg_N;  // reg ptr reg  (N in low nibble)
uint8_t reg_D;  // accumulator
uint8_t reg_DF; // Carry
uint8_t reg_Q;  //  Q output
uint8_t reg_EF; // External Flags 1 to 4
uint8_t reg_T;  // X,P after interrupt
uint8_t reg_IE; // Interrupt enable

uint8_t flag_EF1; // external flags
uint8_t flag_EF2;
uint8_t flag_EF3;
uint8_t flag_EF4;

int initMachine(void)
{
  int rtn = 0;
  for(int i = 0; i < RAM_SIZE; i++)
  {
    mem[i] = 0;
  }
  for(int i = 0; i < NUM_REGS; i++)
  {
    reg[i] = 0;
  }
  reg_I = 0;
  reg_N = 0;
  reg_X = 0;
  reg_P = 0;
  reg_Q = 0;
  reg_IE = 0;
  reg_T = 0;
  flag_EF1 = 0;
  flag_EF2 = 0;
  flag_EF3 = 0;
  flag_EF4 = 0;

  return rtn;
}

/////////////////////////////////////////////////////////////////////////////
/// @fn showState
/// @brief Display current CPU State
/////////////////////////////////////////////////////////////////////////////
static void showState(void)
{
  printf("R0   R1   R2   R3   R4   R5   R6   R7   R8   R9   RA   RB   RC   RD   RE   RF\n");
  printf("%04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X\n",
  reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7],
  reg[8], reg[9], reg[10], reg[11], reg[12], reg[13], reg[14], reg[15]);
  
  // TODO more
  // X, P, T, IE, D, DF, ...
  // mem[pc], mem[x], ...
  

}

// Short branches 3N
// If the short branch is in last two bytes of page
// then if branch is taken it stays on page
// else it goes to next page
int run_3n(void)
{
  int rtn = 0;
  // Get the branch byte
  uint8_t addr = mem[reg[reg_P]];
  // Get the new pc with branch dest
  uint16_t dest_PC = reg[reg_P] & 0xff00 | addr;
  // UPdate the old PC if we don't branch
  reg[reg_P]++;
  // The above order of ops should work correctly at page boundaries.

  switch(reg_N)
  {
    case 0:  // BR unconditional short branch
      reg[reg_P] = dest_PC;
      break;

    case 1:  // BQ  (Q == 1)
      if(reg_Q == 1) reg[reg_P] = dest_PC;
      break;

    case 2:  // BZ  (D == 0)
      if(reg_D == 0) reg[reg_P] = dest_PC;
      break;

    case 3:  // BDF, BPZ, BGE (DF == 1)
      if(reg_DF == 1) reg[reg_P] = dest_PC;
      break;

    case 4:  // B1  (EF1 == 1)
      if(flag_EF1) reg[reg_P] = dest_PC;
      break;

    case 5:  // B2  (EF2 == 1)
      if(flag_EF2) reg[reg_P] = dest_PC;
      break;

    case 6:  // B3  (EF3 == 1)
      if(flag_EF3) reg[reg_P] = dest_PC;
      break;

    case 7:  // B4  (EF4 == 1)
      if(flag_EF4) reg[reg_P] = dest_PC;
      break;

    case 8:  // NBR, SKP (NEVER)
      // nothing left to do, skipped byte already
      break;

    case 9:  // BNQ  (Q == 0)
      if(reg_Q == 0) reg[reg_P] = dest_PC;
      break;

    case 0x0a:  // BNZ  (D != 0)
      if(reg_D != 0) reg[reg_P] = dest_PC;
      break;

    case 0x0b:  // BNF, BM, BL (DF == 0)
      if(reg_DF == 0) reg[reg_P] = dest_PC;
      break;

    case 0x0c:  // BN1  (EF1 == 0)
      // TODO
      break;

    case 0x0d:  // BN2  (EF2 == 0)
      // TODO
      break;

    case 0x0e:  // BN3  (EF3 == 0)
      // TODO
      break;

    case 0x0f:  // BN4  (EF4 == 0)
      // TODO
      break;

    default:
    {
      // ERROR
      break;
    }
  }

  return rtn;
}

int run_6n(void)
{
  int rtn = 0;
  if(reg_N == 0) // INX
  {
    reg[reg_X]++;
  }
  else if(reg_N == 8)  // OP_RSVD 1804/5/6
  {
    // TODO reserved
  }
  else 
  {
    // TODO I/O
  }
  return rtn;
}

int run_7n(void)
{
  int rtn = 0;
  switch(reg_N)
  {
    case  0:        // RET M(R(X)) -> X,P; R(X)++; 1 -> IE
    {
      uint8_t byt = mem[reg[reg_X]];
      reg_X++;
      reg_X = (uint8_t) (byt >> 4);
      reg_P = (uint8_t) (byt & 0x0f);
      reg_IE = 1;
      break;
    }

    case  1:        // DIS M(R(X)) -> X,P; R(X)++; 0 -> IE
    {
      uint8_t byt = mem[reg[reg_X]];
      reg_X++;
      reg_X = (uint8_t) (byt >> 4);
      reg_P = (uint8_t) (byt & 0x0f);
      reg_IE = 0;
    }

    case  2:        // LDXA  M(R(X)) -> D; R(X)++
      reg_D = mem[reg[reg_X]++];
      break;

    case  3:        // STXD  D -> M(R(X)); R(X)--
      mem[reg[reg_X]--] = reg_D;
      break;
      
    case  4:        // ADC  M(R(X)) + D + DF -> DF, D
      {
        int res = mem[reg[reg_X]] + reg_D + reg_DF;
        if(res > 255) reg_DF = 1; else reg_DF = 0;
        reg_D = (uint8_t) (res & 0xff);
        break;
      }

    case  5:        // SDB M(R(X)) - D - (!DF) -> DF, D
      {
        int res = mem[reg[reg_X]] - reg_D;
        if(!reg_DF) res -=1;
        if(res < 0) reg_DF = 0; else reg_DF = 1;
        reg_D = (uint8_t) (res & 0xff);
        break;
      }
    
    case  6:        // SHRC RSHR  D >> 1, D0 -> DF, DF -> D7
      {
        int temp_df = reg_DF;
        reg_DF = reg_D & 0x01;
        reg_D >>= 1;
        if(temp_df) reg_D |= 0x80;
        break;
      }
      
    case  7:        // SMB D - M(R(X)) - (NOT DF) -> DF,D
    {
      int res = reg_D - mem[reg[reg_X]];
      if(res < 0) reg_DF = 0; else reg_DF = 1;
      reg_D = (uint8_t) (res & 0xff);
      break;
    }

    case  8:        // SAV    T -> M(R(X))
      mem[reg[reg_X]] = reg_T;
      break;

    case  9:        // MARK
      reg_T = (uint8_t) (reg_X << 4 | reg_P);
      mem[reg[2]] = reg_T;
      reg_X = reg_P;
      reg[2]--;
      break;

    case  0x0a:     // REQ  0 -> Q
      reg_Q = 0;
      // TODO output
      break;

    case  0x0b:     // SEQ  1-> Q
      reg_Q = 1;
      // TODO output
      break;

    case  0x0c:     // ADDI M(R(P)) + D + DF -> DF, D; R(P)++
      {
        int res = mem[reg[reg_P]++] + reg_D + reg_DF;
        if(res > 255) reg_DF = 1; else reg_DF = 0;
        reg_D = (uint8_t) (res & 0xff);
        break;
      }

    case  0x0d:      // SDBI  M(R(P)) - D - (NOT DF) -> DF, D; R(P)++
      {
        int res = mem[reg[reg_P]++] - reg_D;
        if(!reg_DF) res -= 1;
        if(res < 0) reg_DF = 0; else reg_DF = 1;
        reg_D = (uint8_t) (res & 0xff);
        break;
      }

    case  0x0e:      // SHLC  D <<= 1, D7 -> DF, DF -> D0
      {
        int temp_df = reg_DF;
        reg_DF = reg_D >> 7;
        reg_D <<= 1;
        if(temp_df) reg_D |= 0x01;
        break;
      }

    case 0x0f:       // SMBI D - M(R(P)) - (NOT DF) -> DF, D; p++
    {
      int res = reg_D - mem[reg[reg_P]++];
      if(!reg_DF) res--;
      if(res < 0) reg_DF = 0; else reg_DF = 1;
      reg_D = (uint8_t) (res & 0xff);
      break;
    }

    default:
      // TODO something wrong
      break;
  }
// TODO 
/*
  OP_RET      = 0x70,
  OP_DIS      = 0x71,
  OP_LDXA     = 0x72,
  OP_STXD     = 0x73,
  OP_ADC      = 0x74,
  OP_SDB      = 0x75,
  OP_SHRC     = 0x76,
  OP_RSHR     = 0x76, // Two mnemonics, same instruction
  OP_SMB      = 0x77,
  OP_SAV      = 0x78,
  OP_MARK     = 0x79,
  OP_SEQ      = 0x7a,  // BACKWARD TODO
  OP_REQ      = 0x7b,
  OP_ADDI     = 0x7c,
  OP_SDBI     = 0x7d,
  OP_SHLC     = 0x7e,
  OP_RSHL     = 0x7e,  // Two mnemonics, same instruction
  OP_SMBI     = 0x7f,
  */
  return rtn;
}

int run(void)
{
  int rtn = 0;

  do
  {
    /* code */
    uint8_t byt = mem[reg[reg_P]++];
    reg_I = byt & 0xf0;
    reg_N = byt & 0x0f;
    switch(reg_I)
    {
      case OP_IDL:   // 0x0n  IDL / LDN
        if(reg_N == 0)
        {
          // TODO wait for int or DMA
        }
        else
        {
          reg_D = mem[reg[reg_N]];
          //reg[reg_P]++;
        }
        // TODO wait for int or dma
        // or LD
        break;

      case OP_INC:   // 0x1n
        reg[reg_N]++;
        //reg[reg_P]++;
        break;

      case OP_DEC:   // 0x2n
        reg[reg_N]--;
        //reg[reg_P]++;
        break;

      case OP_BR:    // 0x3n
        run_3n();
        break;

      case OP_LDA:   // 0x4n
        // TODO load adv
        reg_D = mem[reg[reg_N]];
        reg[reg_N]++;
        //reg[reg_P]++;
        break;

      case OP_STR:   // 0x5n
        mem[reg[reg_N]] = reg_D;
        break;

      case OP_IRX:   // 0x6n
        run_6n();
        break;

      case OP_RET:   // 0x7n
        run_7n();
        break;

      case OP_GLO:   // 0x8n
        reg_D = (uint8_t) (reg[reg_N] & 0x7f);
        reg[reg_P]++;
        break;

      case OP_GHI:   // 0x9n
      reg_D = (uint8_t) ((reg[reg_N] >> 8) & 0x7f);
      reg[reg_P]++;
      break;

      case OP_PLO:   // 0xan
        reg[reg_N] = reg[reg_N] & 0xff00 | reg_D;
        reg[reg_P]++;
        break;

      case OP_PHI:   // 0xbn
      reg[reg_N] = reg[reg_N] & 0x00ff | (reg_D << 8);
      reg[reg_P]++;
      break;

      case OP_LBR:   // 0xcn
      case OP_SEP:   // 0xdn
      case OP_SEX:   // 0xen
      case OP_LDX:   // 0xfn
      default:


        
    }

  } while (1);
  


  return rtn;
}

int main(int argc, char* argv[])
{
  int rtn = 0;
  initMachine();


  return rtn;
}

