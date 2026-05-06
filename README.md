# LC-3 Virtual Machine in C

A simple implementation of the LC-3 (Little Computer 3) virtual machine written in C.

This project simulates the basic components of a CPU:
- memory
- registers
- instruction fetch/decode/execute cycle
- opcode execution

The goal of this project is to understand low-level computer architecture and how virtual machines work internally.
> Kindly refer to the CHANGELOG.md for better understanding of each component "))
---

## Inspiration / Learning Source

This project was developed while learning from the excellent LC-3 VM tutorial by Justin Meiners and Ryan Pendleton:

https://justinmeiners.github.io/lc3-vm/

The implementation in this repository is written manually while following and understanding the concepts explained in the tutorial.

---

## Features Implemented

- LC-3 memory model
- Registers
- Program Counter (PC)
- Instruction fetch/decode loop
- ADD instruction
- Condition flags
- Sign extension

---

## Setup & Run

### Requirements
- GCC compiler
- VS Code (recommended)

### Compile

```bash
gcc main.c -o vm.exe

Run
.\vm.exe