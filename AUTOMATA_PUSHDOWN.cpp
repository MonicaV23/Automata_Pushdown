#include <iostream>
#include <stack>
#include <map>
#include <vector>
#include <tuple>
using namespace std;

// Se define la estructura de la transición
struct Transition {
    string nextState;     // Siguiente estado
    string stackOp;       // Tipo de operación: "push" or "pop" or "none"
    char symbolToPush;    // Simbolo para hacer push si stackOp == "push"
};

// Se define la clase de PDA
class PDA {
private:
    string startState;          // Estado incial
    string currentState;        // Estado actual del PDA
    string acceptState;         // Aceptación de estado 
    stack<char> pdaStack;       // Stack del PDA
    char initialStackSymbol;    // Simbolo inicial del stack
    bool acceptByEmptyStack;    // Vandera para un stack vacío
    map<tuple<string, char, char>, Transition> transitions; // Tabla de transición

public:
    PDA(string start, char initStackSymbol, bool emptyStackAcceptance = true) {
        startState = start;
        currentState = start;
        initialStackSymbol = initStackSymbol;
        acceptByEmptyStack = emptyStackAcceptance;
        pdaStack.push(initialStackSymbol); 
    }

    // Se establece el estado de aceptación
    void setAcceptState(string state) {
        acceptState = state;
    }

    // Se añade una transición a la tabla de transición
    void addTransition(string state, char inputSymbol, char stackTop, string nextState, string stackOp, char symbolToPush = '\0') {
        transitions[make_tuple(state, inputSymbol, stackTop)] = {nextState, stackOp, symbolToPush};
    }

    // Simula un PDA con una entrada tipo string
    bool processInput(string input) {
        for (char symbol : input) {
            if (pdaStack.empty()) return false; // Rechaza el stack si se encuentra vacío
            
            char stackTop = pdaStack.top();
            auto key = make_tuple(currentState, symbol, stackTop);

            if (transitions.find(key) == transitions.end()) return false; 
            
            Transition transition = transitions[key];

            // Se aplica la transición
            if (transition.stackOp == "pop") {
                pdaStack.pop();
            } else if (transition.stackOp == "push") {
                pdaStack.push(transition.symbolToPush);
            }

            // Se mueve al siguiente estado
            currentState = transition.nextState;
        }

        // Revisión final, basado en si está vacío o dependiendo del estado
        if (acceptByEmptyStack) {
            return pdaStack.size() == 1 && pdaStack.top() == initialStackSymbol; 
        } else {
            return currentState == acceptState;
        }
    }
};

int main() {
    //  PDA a utlizar para tarea con lenguaje  L = { a^n b^n | n >= 1 }
    
    PDA pda("q0", 'Z'); // Estado inicial: q0, Simbolo incial del stack: Z
    
    // Definimos transiciones
    // q0: Push 'A' por cada 'a', se mantiene en  q0
    pda.addTransition("q0", 'a', 'Z', "q0", "push", 'A');
    pda.addTransition("q0", 'a', 'A', "q0", "push", 'A');
    
    // q0 -> q1: Pop 'A' por cada 'b', se mueve a q1
    pda.addTransition("q0", 'b', 'A', "q1", "pop");

    // q1: Pop 'A' por cada 'b', se mantiene en q1
    pda.addTransition("q1", 'b', 'A', "q1", "pop");
    
    // Realiza la prueba con los datos ingresados
    string input;
    cout << "Ingrese la cadena de texto para un PDA : ";
    cin >> input;

    if (pda.processInput(input)) {
        cout << "La información ingresada ha sido aceptada y correcta!" << endl;
    } else {
        cout << "Lo que se ha ingresado fue denegado, no cumple." << endl;
    }

    return 0;
}