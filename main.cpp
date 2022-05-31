#include <cstdint>
#include <iostream>

using word = int32_t;

enum REGISTER : word { r1, r2, r3, r4, r5, r6, r7, r8, REG_COUNT };
enum INSTRUCTION { hlt, movl, out, add, mul, cmp, jigt, jilt, jiet };

class VirtualMachine {
   public:
    VirtualMachine(word *memory) { m_memory = memory; }
    void run() {
        word instruction;
        while ((instruction = fetch())) {  // not a hlt instruction
            execute(instruction);
        }
        std::cout << "\n";
    }

   private:
    word fetch() { return m_memory[m_program_counter++]; }
    void execute(word instruction) {
        word dest, src;
        switch (instruction) {
            // move literal into register (src -> dest)
            case movl:
                dest = fetch();  // register
                src = fetch();   // literal
                m_registers[dest] = src;
                break;

                // output register contents
            case out:
                src = fetch();
                std::cout << m_registers[src];
                break;

            // add two registers and store in first ((src + dest) -> dest)
            case add:
                dest = fetch();
                src = fetch();
                m_registers[dest] += m_registers[src];
                break;

            // multiply two registers and store in first
            // ((src + dest) -> dest)
            case mul:
                dest = fetch();
                src = fetch();
                m_registers[dest] *= m_registers[src];
                break;

            // take the difference of two registers and store result in m_diff
            case cmp:
                dest = fetch();
                src = fetch();
                m_diff = m_registers[dest] - m_registers[src];
                break;

            // jump if greater than
            case jigt:
                dest = fetch();
                if (m_diff > 0) m_program_counter = dest;
                break;

            // jump if less than
            case jilt:
                dest = fetch();
                if (m_diff < 0) m_program_counter = dest;
                break;

            // jump if equal to
            case jiet:
                dest = fetch();
                if (m_diff == 0) m_program_counter = dest;
                break;
        }
    }

   private:
    word *m_memory;
    word m_program_counter = 0;
    word m_registers[REG_COUNT] = {};
    word m_diff;
};

int main() {  
    word program[] = {
//      Count to 10!
//      inst dest src
        movl, r1, 1, 
        movl, r2, 0, 
        movl, r3, 9, 
        add,  r2, r1,
        cmp,  r2, r3,
        out,  r2,    
        jilt, 9,     
        hlt,         
    };

    VirtualMachine vm = VirtualMachine(program);
    vm.run();
}
