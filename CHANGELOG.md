# Changelog

## [v0.1] - Initial VM Setup
- Added LC-3 memory array
- Added registers
- Added opcode definitions
- Implemented fetch-decode-execute loop

---

## [v0.2] - ADD Instruction
- Implemented OP_ADD
- Added sign extension
- Added condition flag updates
- Added manual instruction testing
<p align="center">
  <img src="https://github.com/user-attachments/assets/9decf426-7d2e-468d-934a-60cdd3d56e37" />
</p>

Purpose
- Performs addition between: two registers
- OR
- register + immediate value
- Example
- ADD R0, R1, R2
- Suppose:
- R1 = 5
- R2 = 10
---

## [v0.3] - Logical Instructions
- Implemented OP_AND
- Implemented OP_NOT
<p align="center">
  <img src="https://github.com/user-attachments/assets/8f932865-dc35-4d61-aabd-22d22bb5c8f0" />
</p>
---

## [v0.4] - Memory Operations
- Implemented OP_LD
- Added PC-relative addressing
<p align="center">
  <img src="https://github.com/user-attachments/assets/af0855cd-a75b-43f6-b8e0-d8e49228bae5" />
</p>

- What happens?
- Instruction:
- LDR R0, R1, #1
- means: R0 = memory[R1 + 1]

- Since:
- R1 = 0x4000
- VM loads: memory[0x4001]
- which contains: 55
---

## [v0.5] - Memory Instructions
- Implemented OP_ST
- Added PC-relative memory access
- Added memory load/store testing
<p align="center">
  <img src="https://github.com/user-attachments/assets/056b4646-6854-49e3-9021-61d82c1d5138" />
</p>

- Purpose
- Stores register value into memory.
- Example: ST R0, #0

- Suppose:
- R0 = 99
- PC = 0x3001

- Internal Steps
1. Decode source register R0
- contains: 99
2. Extract PC-relative offset
- Offset: #0
3. Calculate memory address- Address = PC + offset
- So: 0x3001 + 0
- Result: 0x3001
4. Store value into memory
- memory[0x3001] = 99
---

## [v0.6] - Base Register Addressing

- Implemented OP_LDR
- Implemented OP_STR
- Added base register + offset memory addressing
- Added memory pointer style access
- Added testing for register-relative load/store operations
<p align="center">
  <img src="https://github.com/user-attachments/assets/13256f06-9d17-42d7-9ebd-ca6f5883da32" />
</p>

- Purpose: Loads data from memory using:
- base register + offset
- Example: LDR R0, R1, #1
- Suppose: R1 = 0x4000
- memory[0x4001] = 55

- Internal Steps
1. Extract registers
- Destination = R0
- Base Register = R1
2. Extract offset
- offset = 1
3. Calculate effective address
- Address = R1 + offset
- So: 0x4000 + 1
- Result: 0x4001

5. Load from memory
- R0 = memory[0x4001]
- which is: 55
6. Update flags

- Since: 55 > 0
- VM sets: FL_POS

<p align="center">
  <img src="https://github.com/user-attachments/assets/7c7a595a-c400-4aa0-9d6a-70df97a5315b" />
</p>

- Purpose: Stores register value into memory using:
- base register + offset
- Example: STR R0, R1, #1

- Suppose:
- R0 = 77
- R1 = 0x4000

- Internal Steps
1. Extract registers
- Source = R0
- Base Register = R1
2. Extract offset
- offset = 1
3. Calculate effective address
- Address = R1 + offset
- So: 0x4000 + 1 becomes: 0x4001
4. Store value into memory
- memory[0x4001] = 77

---

<<<<<<< HEAD
## [v0.8] - Jump Instruction

- Implemented OP_JMP
- Added register-based control flow
- Added unconditional jump support
- Added JMP instruction testing



## [v0.9] - Subroutine Support

- Implemented OP_JSR
- Added PC-relative subroutine calls
- Added return address storage in R7
- Added subroutine jump testing

1. Fetch instruction
- After fetch:
- PC = 0x3001
2. Save return address
R7 = 0x3001
3. Apply offset
- Offset:
- +1
- So:
- PC = 0x3002
=======
## [v0.7] - Branch Instruction

- Implemented OP_BR
- Added conditional branching support
- Added PC-relative branching
- Connected branch logic with condition flags
- Added branch instruction testing
<p align="center">
  <img src="https://github.com/user-attachments/assets/a7a2668f-fb6d-43b1-aa8c-cdb8d7b18f8b" />
</p>

- What actually happens :
- Instruction: BRp +1 means
- if positive: PC = PC + 1
- After fetch:
- PC already became 0x3001
- offset = 1
- So final: PC = 0x3002

>>>>>>> 3efff29948aa49b0a126bddcccd1b06c13453425
