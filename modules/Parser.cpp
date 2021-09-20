#include <iostream>
#include <fstream>
#include <map>
#include <stack>
#include <string>
#include <ctime>
#include <windows.h>
#include <math.h>
#include <algorithm>
#include "Tree.cpp"


using namespace std;

class Parser
{
private:
    Tree *tree = new Tree();

    bool isPrimitive(string expression);
    int getPrecedence(char operation);
    string trim(string line);
    string convertFromInfixToPostfix(string infixExpression);

    void parseVariable(string line);
    Operator *parseExpression(string expression);

    bool findOperation(string line);
    void printTreeLevel(string prefix, Node *node, bool isLeft, int color);
    void parseCondition(string expression, bool isTernar);
public:
    Parser();
    void parse(string fileName);
    void printTree();
    Tree* getTree();
};

/*
** Parser interface
*/

Parser::Parser() {}

bool Parser::findOperation(string line){
    for(char ch: line){
        if(getPrecedence(ch) > 0){
            return true;
        }
    }
    return false;
}

void Parser::parse(string fileName)
{
    ifstream programFile(fileName);
    string currentLine;

    while (!programFile.eof())
    {
        getline(programFile, currentLine, ';');
        if (currentLine != "")
        {
            if(currentLine.find("if") != string::npos)
            {

                string condition = currentLine;
                int leftLines = 1;
                while(leftLines --> 0){
                    getline(programFile, currentLine, ';');

                    condition += currentLine;
                }
                replace( condition.begin(), condition.end(), '\n', ' ');
                parseCondition(condition, false);
            } else if(currentLine.find('?') != string::npos){
                string condition = currentLine;
                parseCondition(condition, true);
            }
            else if (currentLine.find('=') != string::npos)
            {
                currentLine = trim(currentLine);
                parseVariable(currentLine);
            }
            else
            {
                currentLine = trim(currentLine);
                tree->addNode(parseExpression(currentLine));
            }
        }
        else
        {
            break;
        }
    }
}

/*
** Parser private methods
*/

void Parser::parseCondition(string expression, bool isTernar){
    string ifCase = "";
    string elseCase = "";
    string condition = "";

    string variableName = "";
    string variableValue = "";

    if(!isTernar){
        int leftCut = expression.find("if");
        int rightCut = expression.find("then");
        condition = expression.substr(leftCut + 2, rightCut - leftCut - 2);

        leftCut = expression.find("then");
        rightCut = expression.find("else");
        ifCase = trim(expression.substr(leftCut + 4, rightCut - leftCut - 4));

        leftCut = expression.find("else");
        rightCut = expression.size();
        elseCase = trim(expression.substr(leftCut + 4, rightCut - leftCut - 4));

        variableName = ifCase.substr(0, ifCase.find('='));
        variableValue = ifCase.substr(ifCase.find('=') + 1);
    } else {
        int leftCut = expression.find("=");
        int rightCut = expression.find("?");
        condition = expression.substr(leftCut + 1, rightCut - leftCut - 1 );

        leftCut = expression.find("?");
        rightCut = expression.find(":");
        ifCase = trim(expression.substr(leftCut + 1, rightCut - leftCut - 1));

        leftCut = expression.find(":");
        rightCut = expression.size();
        elseCase = trim(expression.substr(leftCut + 1, rightCut - leftCut - 1));

        // k =
        variableName = expression.substr(0, expression.find('='));
        variableValue = ifCase;
    }
    Condition* umova = new Condition();
    umova->setCondExpression(condition);

    variableName = trim(variableName);

    if (variableName.length() > 0 && variableValue.length() > 0)
    {
        umova->setVariable(variableName);

        if (isPrimitive(variableValue) && !findOperation(ifCase))
        {
            Constant *constant = new Constant();
            constant->setValue(stod(variableValue));
            umova->setRight(constant);
        }
        else
        {
            umova->setRight(parseExpression(variableValue));
        }
    }
    if(isTernar) variableValue = elseCase;
    else variableName = elseCase.substr(elseCase.find("="));

    if (variableName.length() > 0 && variableValue.length() > 0)
    {
        if (isPrimitive(variableValue) && !findOperation(elseCase))
        {
            Constant *constant = new Constant();
            constant->setValue(stod(variableValue));
            umova->setLeft(constant);
        }
        else
        {
            umova->setLeft(parseExpression(variableValue));
        }
    }

    tree->addNode(umova);

}

bool Parser::isPrimitive(string expression)
{
    try
    {
        int number;
        number = stod(expression);
        return true;
    }
    catch (exception e)
    {
        return false;
    }
}

int Parser::getPrecedence(char operation)
{
    switch (operation)
    {
    case '+':
    case '-':
        return 2;
    case '*':
    case '/':
        return 3;
    case '^':
        return 4;
    default:
        return 0;
    }
}

string Parser::trim(string line)
{
    string s = "";
    for (char currentChar : line)
    {
        if (currentChar != ' ' && currentChar != '\n')
        {
            s += currentChar;
        }
    }
    return s;
}

string Parser::convertFromInfixToPostfix(string infixExpression)
{
    stack<char> s;
    string postfix = "";
    for (int i = 0; i < infixExpression.size(); ++i)
    {
        if (isalpha(infixExpression[i]))
        {
            while (isalpha(infixExpression[i]) && i < infixExpression.size())
            {
                postfix += infixExpression[i++];
            }
            i--;

            postfix += " ";
        }
        else if (isdigit(infixExpression[i]))
        {
            while (isdigit(infixExpression[i]) && i < infixExpression.size())
            {
                postfix += infixExpression[i++];
            }
            if (infixExpression[i] == '.')
            {
                postfix += infixExpression[i++];
            }
            while (isdigit(infixExpression[i]) && i < infixExpression.size())
            {
                postfix += infixExpression[i++];
            }
            i--;

            postfix += " ";
        }
        else if (infixExpression[i] == '(')
        {
            s.push(infixExpression[i]);
        }
        else if (infixExpression[i] == ')')
        {
            while (!s.empty() && s.top() != '(')
            {
                postfix += s.top();
                postfix += " ";
                s.pop();
            }
            if (s.top() == '(')
                s.pop();
        }
        else if (getPrecedence(infixExpression[i]))
        {
            if (s.empty())
            {
                s.push(infixExpression[i]);
            }
            else
            {
                if (getPrecedence(infixExpression[i]) > getPrecedence(s.top()))
                {
                    s.push(infixExpression[i]);
                }
                else if (getPrecedence(infixExpression[i]) == getPrecedence(s.top()) && infixExpression[i] == '^')
                {
                    s.push(infixExpression[i]);
                }
                else
                {
                    while (!s.empty() && getPrecedence(infixExpression[i]) <= getPrecedence(s.top()))
                    {
                        postfix += s.top();
                        postfix += " ";
                        s.pop();
                    }
                    s.push(infixExpression[i]);
                }
            }
        }
    }
    while (!s.empty())
    {
        postfix += s.top();
        postfix += " ";
        s.pop();
    }

    postfix = postfix.substr(0, postfix.length() - 1);
    return postfix;
}

void Parser::parseVariable(string line)
{
    string variableName = line.substr(0, line.find('='));
    string variableValue = line.substr(line.find('=') + 1);
    if (variableName.length() > 0 && variableValue.length() > 0)
    {
        Operator *assigning = new Operator();
        Variable *variable = new Variable();

        variable->setName(variableName);

        assigning->setLeft(variable);
        assigning->setOperation('=');

        if (isPrimitive(variableValue) && !findOperation(line))
        {
            Constant *constant = new Constant();
            constant->setValue(stod(variableValue));
            assigning->setRight(constant);
        }
        else
        {
            assigning->setRight(parseExpression(variableValue));
        }

        tree->addNode(assigning);
    }
}

Operator *Parser::parseExpression(string expression)
{
    expression = convertFromInfixToPostfix(expression);
    stack<Node*> variables;
    for(int i = 0; i < expression.size(); i ++ ){
        if(expression[i] == ' ') continue;

        if(isalpha(expression[i]))
        {
            string number = "" ;
            while(isalpha(expression[i]) && i < expression.size() ){number += expression[i ++];}
            if(expression[i] == '.'){number += expression[i ++ ];}
            while(isalpha(expression[i]) && i < expression.size() ){number += expression[i ++];}
            i--;

            Variable* node = new Variable();
            node->setName(number);
            variables.push(node);
        } else if(isdigit(expression[i])) {
            string number = "" ;
            while(isdigit(expression[i]) && i < expression.size() ){number += expression[i ++];}
            if(expression[i] == '.'){number += expression[i ++ ];}
            while(isdigit(expression[i]) && i < expression.size() ){number += expression[i ++];}
            i--;

            Constant* node = new Constant();
            node->setValue(stod(number)) ;
            variables.push(node) ;
        } else {
                Node* a = variables.top() ;
                variables.pop();
                Node* b = variables.top();
                variables.pop();

                Operator* result = new Operator();
                result->setOperation(expression[i]) ;
                result->setLeft(b);
                result->setRight(a);
                variables.push(result) ;
            }
        }
        Operator* root = static_cast<Operator*>(variables.top());
        return root;
}

void Parser::printTree()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 15);
    cout << tree->getRoot()->getType() << endl;
    srand(time(NULL));
    for (size_t i = 0; i < tree->getRoot()->getChildren()->size(); i++)
    {
        printTreeLevel("    ", tree->getRoot()->getChildren()->at(i), true, 15);
    }
}


void Parser::printTreeLevel(string prefix, Node *node, bool isLeft, int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (node != nullptr)
    {
        SetConsoleTextAttribute(hConsole, color);
        cout << prefix;
        if (isLeft)
        {
            cout << "|---";
        }
        else
        {
            cout << "^---";
        }

        string nodeType = node->getType();
        cout << nodeType << ", ";
        if (nodeType == "Constant")
        {
            cout << static_cast<Constant *>(node)->getValue() << endl;
        }
        else if (nodeType == "Variable")
        {
            cout << static_cast<Variable *>(node)->getName() << endl;
        }
        else if (nodeType == "Operator")
        {
            cout << static_cast<Operator *>(node)->getOperation() << endl;
        }
        else if(nodeType == "Condition")
        {
            cout << static_cast<Condition *>(node)->getConditionExpression()
                    << " for variable " << static_cast<Condition *>(node)->getVariable() << endl;
        }
        int newColor;
        do
        {
            newColor = rand() % 15 + 1;
        } while (newColor == color);

        printTreeLevel(prefix + (false ? "|   " : "    "), node->getLeft(), true, newColor);
        printTreeLevel(prefix + (false ? "|   " : "    "), node->getRight(), false, newColor);
    }
}
Tree* Parser::getTree(){
    return this->tree;
}
