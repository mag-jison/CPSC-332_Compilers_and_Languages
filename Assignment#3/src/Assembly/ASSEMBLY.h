#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

typedef vector<string> assembly;

class ASSEMBLY {
private:
    assembly code { "PUSHI", "PUSHM", "POPM", "STDOUT", "STDIN", "ADD", "SUB", "MUL", "DIV", "GRT", "LES", "EQU", "NEQ", "GEQ", "LEQ", "JUMPZ", "JUMP", "LABEL" };

public:
    void printAssembly();
};

void ASSEMBLY::printAssembly(){
    cout << "+-----------------------------------------------------------+\n";
    cout << "|" << setw(20) << "#" << setw(20) << "INSTRUCTIONS" << setw(20) << "MEMORY |" << endl;
    cout << "testing only" << endl;
}
