#include <stdio.h>
#include <stdint.h>

/*MEMORY*/

/* LC-3 has 65536 memory locations */
uint16_t memory[UINT16_MAX];


/*REGISTERS */

enum
{
    R_R0 = 0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_PC,      /* Program Counter */
    R_COND,    /* Condition Flags */
    R_COUNT
};

/* Array to store register values */
uint16_t reg[R_COUNT];


/* OPCODES */

enum
{
    OP_BR = 0,   /* Branch */
    OP_ADD,      /* Add */
    OP_LD,       /* Load */
    OP_ST,       /* Store */
    OP_JSR,      /* Jump Register */
    OP_AND,      /* Bitwise AND */
    OP_LDR,      /* Load Register */
    OP_STR,      /* Store Register */
    OP_RTI,      /* Unused */
    OP_NOT,      /* Bitwise NOT */
    OP_LDI,      /* Load Indirect */
    OP_STI,      /* Store Indirect */
    OP_JMP,      /* Jump */
    OP_RES,      /* Reserved */
    OP_LEA,      /* Load Effective Address */
    OP_TRAP      /* Execute Trap */
};

// Add TRAP vectors enum
enum
{
    TRAP_GETC  = 0x20, /* Get character */
    TRAP_OUT   = 0x21, /* Output character */
    TRAP_PUTS  = 0x22, /* Output string */
    TRAP_IN    = 0x23, /* Input character */
    TRAP_PUTSP = 0x24, /* Output byte string */
    TRAP_HALT  = 0x25  /* Halt program */
};

/* CONDITION FLAGS */

enum
{
    FL_POS = 1 << 0,   /* Positive */
    FL_ZRO = 1 << 1,   /* Zero */
    FL_NEG = 1 << 2    /* Negative */
};


/* UTILITY FUNCTIONS */

/*Sign extend immediate value */
uint16_t sign_extend(uint16_t x, int bit_count)
{
    if ((x >> (bit_count - 1)) & 1)
    {
        x |= (0xFFFF << bit_count);
    }

    return x;
}


/*
   Update condition flags after operations.
*/
void update_flags(uint16_t r)
{
    if (reg[r] == 0)
    {
        reg[R_COND] = FL_ZRO;
    }
    else if (reg[r] >> 15)
    {
        reg[R_COND] = FL_NEG;
    }
    else
    {
        reg[R_COND] = FL_POS;
    }
}

int main(int argc, char* argv[])
{
    printf("LC-3 VM starting...\n");

    /* Program starts at x3000 in LC-3 */
    reg[R_PC] = 0x3000;

    /* TEST PROGRAM */

/* Store string starting at x4000 */

memory[0x4000] = 'H';
memory[0x4001] = 'E';
memory[0x4002] = 'L';
memory[0x4003] = 'L';
memory[0x4004] = 'O';
memory[0x4005] = 0;

/* R0 points to string */
reg[R_R0] = 0x4000;

/* TRAP x22 (PUTS) */
memory[0x3000] = 0xF022;

/* TRAP x25 (HALT) */
memory[0x3001] = 0xF025;

    /*FETCH-DECODE-EXECUTE LOOP*/

    int running = 1;

    while (running)
    {
        /* Fetch instruction */
        uint16_t instr = memory[reg[R_PC]++];

        /* Extract opcode (top 4 bits) */
        uint16_t op = instr >> 12;

        switch (op)
        {

            case OP_ADD:
            {
                /* Destination register */
                uint16_t r0 = (instr >> 9) & 0x7;

                /* First source register */
                uint16_t r1 = (instr >> 6) & 0x7;

                /* Immediate mode flag */
                uint16_t imm_flag = (instr >> 5) & 0x1;

                if (imm_flag)
                {
                    /* Immediate value mode */
                    uint16_t imm5 = sign_extend(instr & 0x1F, 5);

                    reg[r0] = reg[r1] + imm5;
                }
                else
                {
                    /* Register mode */
                    uint16_t r2 = instr & 0x7;

                    reg[r0] = reg[r1] + reg[r2];
                }

                /* Update condition flags */
                update_flags(r0);

                /* Print result for testing */
                printf("R0 = %d\n", reg[r0]);

                running = 0;

                break;
            }

            case OP_AND:
{
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t r1 = (instr >> 6) & 0x7;
    uint16_t imm_flag = (instr >> 5) & 0x1;

    if (imm_flag)
    {
        uint16_t imm5 = sign_extend(instr & 0x1F, 5);
        reg[r0] = reg[r1] & imm5;
    }
    else
    {
        uint16_t r2 = instr & 0x7;
        reg[r0] = reg[r1] & reg[r2];
    }

    update_flags(r0);

    printf("AND Result = %d\n", reg[r0]);

    running = 0;

    break;
}


case OP_NOT:
{
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t r1 = (instr >> 6) & 0x7;

    reg[r0] = ~reg[r1];

    update_flags(r0);

    printf("NOT Result = %d\n", reg[r0]);

    running = 0;

    break;
}

/*LD = Load from memory into a register.
What LD does
Format:
LD DR, PCoffset9
Meaning:
DR = memory[PC + offset]*/

case OP_LD:
{
    /* Destination register */
    uint16_t r0 = (instr >> 9) & 0x7;

    /* PC offset (9-bit signed value) */
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);

    /* Load value from memory */
    reg[r0] = memory[reg[R_PC] + pc_offset];

    /* Update condition flags */
    update_flags(r0);

    printf("LD Result = %d\n", reg[r0]);

    running = 0;

    break;
}

/*Format:
ST SR, PCoffset9
Meaning:
memory[PC+offset]=SR
So instead of loading FROM memory,
you STORE INTO memory.*/

case OP_ST:
{
    /* Source register */
    uint16_t r0 = (instr >> 9) & 0x7;

    /* PC-relative offset */
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);

    /* Store register value into memory */
    memory[reg[R_PC] + pc_offset] = reg[r0];

    printf("Stored value = %d\n", reg[r0]);

    running = 0;

    break;
}
    printf("Memory[0x3001] = %d\n", memory[0x3001]);


/* LDR format
LDR DR, BaseR, offset6
Meaning:
DR = memory[BaseR + offset] */

case OP_LDR:
{
    /* Destination register */
    uint16_t r0 = (instr >> 9) & 0x7;

    /* Base register */
    uint16_t r1 = (instr >> 6) & 0x7;

    /* 6-bit signed offset */
    uint16_t offset = sign_extend(instr & 0x3F, 6);

    /* Load value from memory */
    reg[r0] = memory[reg[r1] + offset];

    update_flags(r0);

    printf("LDR Result = %d\n", reg[r0]);

    running = 0;

    break;
}

/*
STR SR, BaseR, offset6
Meaning:
memory[BaseR + offset] = SR */

case OP_STR:
{
    /* Source register */
    uint16_t r0 = (instr >> 9) & 0x7;

    /* Base register */
    uint16_t r1 = (instr >> 6) & 0x7;

    /* 6-bit signed offset */
    uint16_t offset = sign_extend(instr & 0x3F, 6);

    /* Store value into memory */
    memory[reg[r1] + offset] = reg[r0];

    printf("Stored value = %d\n", reg[r0]);

    printf("Memory value = %d\n",
           memory[reg[r1] + offset]);

    running = 0;

    break;
}

/*
What BR does?
Format:
BR[n][z][p] PCoffset9
Meaning:
if condition flag matches:
PC = PC + offset
this is important because this is the first instruction using:
condition flags (N, Z,P) */

case OP_BR:
{
    /* Extract condition flag bits */
    uint16_t cond_flag = (instr >> 9) & 0x7;

    /* PC-relative signed offset */
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);

    /* Check if condition matches */
    if (cond_flag & reg[R_COND])
    {
        reg[R_PC] += pc_offset;
    }

    printf("PC = 0x%X\n", reg[R_PC]);

    running = 0;

    break;
}

/*
What JMP does
Format: JMP BaseR
Meaning: PC = BaseR

Why JMP matters

This is the first instruction where: a register directly controls execution flow
That concept is foundational for:
functions
returns
interpreters
operating systems
*/
case OP_JMP:
{
    /* Base register */
    uint16_t r1 = (instr >> 6) & 0x7;

    /* Jump to address in register */
    reg[R_PC] = reg[r1];

    printf("Jumped to PC = 0x%X\n", reg[R_PC]);

    running = 0;

    break;
}

/*What JSR does
Format: JSR PCoffset11

Meaning: R7 = current PC
         PC = PC + offset*/

case OP_JSR:
{
    /* Save current PC in R7 */
    reg[R_R7] = reg[R_PC];

    /* Check long flag */
    uint16_t long_flag = (instr >> 11) & 1;

    if (long_flag)
    {
        /* PC-relative offset */
        uint16_t long_pc_offset =
            sign_extend(instr & 0x7FF, 11);

        /* Jump to subroutine */
        reg[R_PC] += long_pc_offset;
    }
    else
    {
        /* Base register */
        uint16_t r1 = (instr >> 6) & 0x7;

        reg[R_PC] = reg[r1];
    }

    printf("JSR Jumped to PC = 0x%X\n", reg[R_PC]);
    printf("Return Address stored in R7 = 0x%X\n",
           reg[R_R7]);

    running = 0;

    break;
}

/*What LEA does

Format: LEA DR, PCoffset9
Meaning: DR = address
NOT:
DR = memory[address]
That’s the key distinction.*/

case OP_LEA:
{
    /* Destination register */
    uint16_t r0 = (instr >> 9) & 0x7;

    /* PC-relative offset */
    uint16_t pc_offset =
        sign_extend(instr & 0x1FF, 9);

    /* Load effective address */
    reg[r0] = reg[R_PC] + pc_offset;

    update_flags(r0);

    printf("LEA Result (Address) = 0x%X\n",
           reg[r0]);

    running = 0;

    break;
}

case OP_TRAP:
{
    /* Save current PC in R7 */
    reg[R_R7] = reg[R_PC];

    /* Extract trap vector */
    uint16_t trap_vect = instr & 0xFF;

    switch (trap_vect)
    {
        case TRAP_HALT:
        {
            printf("HALT\n");

            running = 0;

            break;
        }

        /*
        PUTS lets the VM print an entire string.

         This is one of the most commonly used LC-3 trap routines.
          What TRAP x22 does

          Meaning: print a null-terminated string

          The string address is stored in: R0
         How LC-3 strings work
         Each memory location stores: one ASCII character
         String ends when: 0x0000 is encountered. */

         case TRAP_PUTS:
{
         /* Pointer to characters */
         uint16_t* c = memory + reg[R_R0];

         /* Print characters until null terminator */
        while (*c)
        {
            putc((char)*c, stdout);
            ++c;}
            fflush(stdout);
            break;
        }
    }
    default:
    {
                printf("Unknown instruction\n");
                running = 0;
                break;
            }
        }
    }

    return 0;
}
    }