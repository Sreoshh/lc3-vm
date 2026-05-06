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
    /* Value to store */
    reg[R_R0] = 99;

/*
   ST R0, #0
*/
   memory[0x3000] = 0x3000;

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