#include "Parser.cpp"
#include "Tree.cpp"
#include "Counter.cpp"

using namespace std;

class Calculator {
public:
    string name;
    Calculator(string str) {
        this->name = str;
    }

    void run() {
        Parser* parser = new Parser();
        parser->parse(name);
        Tree* tree = parser->getTree();
        parser->printTree();
        Counter counter;
        counter.calculate(tree);
        cout << "Answer: " << counter.getAnswer();
    }
};
