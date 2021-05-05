#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct INSTR {
    size_t addr;
    string op;
    size_t oprnd;
};

typedef vector<string> assembly;
typedef vector<INSTR> instruction;

class Assembly {
private:
    assembly op_code { "PUSHI", "PUSHM", "POPM", "STDOUT", "STDIN", "ADD", "SUB", "MUL", "DIV", "GRT", "LES", "EQU", "NEQ", "GEQ", "LEQ", "JUMPZ", "JUMP", "LABEL" };
    instruction table;


public:
    void gen_instr(const instruction&);
    void printAssembly();
};

void Assembly::gen_instr(const instruction& i){
    cout << "address                  Op             Oprnd" << endl;
    for (instruction::iterator it = table.begin(); it < table.end(); ++it){
        cout << (*it).addr << " " << (*it).op << " " << (*it).oprnd << endl;
    }
}

void Assembly::printAssembly(){
    cout << "+-----------------------------------------------------------+\n";
    cout << "|" << setw(20) << "#" << setw(20) << "INSTRUCTIONS" << setw(20) << "MEMORY |" << endl;
    cout << "testing only" << endl;
}
