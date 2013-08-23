#include <stdio.h>
#include <stdlib.h>

#include "disassemble.h"

void disassemble(inst_t inst)
{
  /* you could use itype or type instead of rtype */
  /* remember that opcode field is at the same place for all types */
  switch (inst.rtype.opcode)
  {
  case 0x0: // opcode == 0x0 (SPECIAL)

    switch (inst.rtype.funct)
    {
    case 0x0:
      printf("sll\t$%d,$%d,%d\n",inst.rtype.rd,\
          inst.rtype.rt,inst.rtype.shamt);
      break;
    case 0x2:
      printf("srl\t$%d,$%d,%d\n",inst.rtype.rd,\
          inst.rtype.rt,inst.rtype.shamt);
      break;
    case 0x3:
      printf("sra\t$%d,$%d,%d\n",inst.rtype.rd,\
          inst.rtype.rt,inst.rtype.shamt);
      break;  
    case 0x8:
      printf("jr\t$%d\n",inst.rtype.rs);
      break;   
    case 0x9:
      printf("jalr\t$%d\n",inst.rtype.rs);
      break;// I code to here June 9 2013   
    case 0x20:          
      printf("add\t$%d,$%d,$%d\n",inst.rtype.rd,\
          inst.rtype.rs,inst.rtype.rt);
      break;
    case 0x21:
      printf("addu\t$%d,$%d,$%d\n",inst.rtype.rd,\
          inst.rtype.rs,inst.rtype.rt);
      break;
     case 0x26:
      printf("xor\t$%d,$%d,$%d\n",inst.rtype.rd,\
          inst.rtype.rs,inst.rtype.rt);
      break;
 
    case 0x2a:  // let's see if it works in the end
   
      printf("slt\t$%d,$%d,$%d\n",inst.rtype.rd,\
          inst.rtype.rs,inst.rtype.rt);
      break;
    case 0xc: // funct == 0xc (SYSCALL)
      printf("syscall\n");
      break;
    
    case 0x24: // funct == 0x24 (AND)
      printf("and\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
      break;      

    case 0x25: // funct == 0x25 (OR)
      printf("or\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
      break;

    default: // undefined funct
      fprintf(stderr, "%s: illegal instruction: %08x\n", __FUNCTION__, inst.bits);
      exit(-1);
      break;
    }
    break;
  case 0x4:
    printf("beq\t$%d,$%d,%d\n",inst.itype.rs,inst.itype.rt,\
        ((int32_t)inst.itype.imm)*4);
    break;
  case 0x5:
    printf("bne\t$%d,$%d,%d\n",inst.itype.rs,inst.itype.rt,\
        ((int32_t)inst.itype.imm)*4);
    break;
  case 0x9:
    printf("addiu\t$%d,$%d,%d\n",inst.itype.rt,inst.itype.rs,\
        inst.itype.imm);
    break;
  case 0xd: // opcode == 0xd (ORI)
    printf("ori\t$%d,$%d,0x%x\n", inst.itype.rt, inst.itype.rs, inst.itype.imm);
    break;

  case 0xe:
    printf("xori\t$%d,$%d,0x%x\n",inst.itype.rt,inst.itype.rs,inst.itype.imm);
    break;
  case 0xf: 
    printf("lui\t$%d,0x%x\n",inst.itype.rt,(uint32_t)inst.itype.imm);
    break;
  case 0x20:
    printf("lb\t$%d,%d($%d)\n",inst.itype.rt,(int32_t)inst.itype.imm,inst.itype.rs);
    break;
  case 0x23:
    printf("lw\t$%d,%d($%d)\n",inst.itype.rt,(int32_t)inst.itype.imm,inst.itype.rs);
    break;
  case 0x24:
    printf("lbu\t$%d,%d($%d)\n",inst.itype.rt,(int32_t)inst.itype.imm,inst.itype.rs);
    break;
  case 0x28:
    printf("sb\t$%d,%d($%d)\n",inst.itype.rt,(int32_t)inst.itype.imm,inst.itype.rs);
    break;
  case 0x2b:
    printf("sw\t$%d,%d($%d)\n",inst.itype.rt,(int32_t)inst.itype.imm,inst.itype.rs);
    break;
  case 0x2: // opcode == 0x2 (J)
    printf("j\t0x%x\n", inst.jtype.addr*4);
    break;
  /* add j code here
   *
   * */
  case 0x3:
    
    printf("jal\t0x%x\n",inst.jtype.addr*4);
    break;
  default: // undefined opcode
    fprintf(stderr, "%s: illegal instruction: %08x\n", __FUNCTION__, inst.bits);
    exit(-1);
    break;
  }
} 
