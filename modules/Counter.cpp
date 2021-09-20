#include <vector>
#include <string>
#include <map>
#include <stack>
#include "Tree.cpp"

using namespace std;

class Counter{
private:
    double answer;
    Tree* tree ;
    map<string, double> calculatedValue;

public:

    void setAnswer(double answer);
    void calculate(Tree* tree);
    double getAnswer();
    bool canCalculate(Node* node);
    void addAllVariables(vector<string>& variables, Node* node);
    double calculateExpression(Node* node);
    double applyOperation(double b, double a, char op);
    bool checkCondition(string condition, Condition* node);
    bool canMakeCondition(string condition);
    int getPrecedence(char operation);
    bool findOperation(string str) ;
    double countExpression(string expression);
};

double Counter::countExpression(string infixExpression){
    stack<char> s;
    string postfix = "";
    for (int i = 0; i < infixExpression.size(); ++i)
    {
        if (isalpha(infixExpression[i])){
            while (isalpha(infixExpression[i]) && i < infixExpression.size()){
                postfix += infixExpression[i++];
            }
            i--;

            postfix += " ";
        }
        else if (isdigit(infixExpression[i])){
            while (isdigit(infixExpression[i]) && i < infixExpression.size()){
                postfix += infixExpression[i++];
            }
            if (infixExpression[i] == '.'){
                postfix += infixExpression[i++];
            }
            while (isdigit(infixExpression[i]) && i < infixExpression.size()){
                postfix += infixExpression[i++];
            }
            i--;

            postfix += " ";
        }
        else if (infixExpression[i] == '('){
            s.push(infixExpression[i]);
        }
        else if (infixExpression[i] == ')'){
            while (!s.empty() && s.top() != '('){
                postfix += s.top();
                postfix += " ";
                s.pop();
            }
            if (s.top() == '(')
                s.pop();
        }
        else if (getPrecedence(infixExpression[i])){
            if (s.empty()){
                s.push(infixExpression[i]);
            }
            else{
                if (getPrecedence(infixExpression[i]) > getPrecedence(s.top())){
                    s.push(infixExpression[i]);
                }
                else if (getPrecedence(infixExpression[i]) == getPrecedence(s.top()) && infixExpression[i] == '^'){
                    s.push(infixExpression[i]);
                }
                else{
                    while (!s.empty() && getPrecedence(infixExpression[i]) <= getPrecedence(s.top())){
                        postfix += s.top();
                        postfix += " ";
                        s.pop();
                    }
                    s.push(infixExpression[i]);
                }
            }
        }
    }
    while (!s.empty()){
        postfix += s.top();
        postfix += " ";
        s.pop();
    }

    postfix = postfix.substr(0, postfix.length() - 1);

    string expression = postfix;
    stack<double> variables;
    for(int i = 0; i < expression.size(); i ++ ){
        if(expression[i] == ' ') continue;

        if(isalpha(expression[i]))
        {
            string number = "" ;
            while(isalpha(expression[i]) && i < expression.size() ){number += expression[i ++];}
            if(expression[i] == '.'){number += expression[i ++ ];}
            while(isalpha(expression[i]) && i < expression.size() ){number += expression[i ++];}
            i--;

            variables.push( calculatedValue[number] );
        } else if(isdigit(expression[i])) {
            string number = "" ;
            while(isdigit(expression[i]) && i < expression.size() ){number += expression[i ++];}
            if(expression[i] == '.'){number += expression[i ++ ];}
            while(isdigit(expression[i]) && i < expression.size() ){number += expression[i ++];}
            i--;

            variables.push(stod(number)) ;
        } else {
            double a = variables.top() ;
            variables.pop();
            double b = variables.top();
            variables.pop();

            variables.push(applyOperation(a, b, expression[i])) ;
        }
    }

    return variables.top() ;
}

bool Counter::findOperation(string str) {
    for(char x: str){
        if(getPrecedence(x) > 0)
            return true;
    }
    return false;
}
int Counter::getPrecedence(char operation)
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

void Counter::setAnswer(double answer)
{
    this->answer = answer;
}

double Counter::getAnswer()
{
    return answer;
}

bool Counter::canMakeCondition(string condition){
    string var = "" ;
    for(int i = 0; i < condition.size();i ++){
        if(condition[i] == ' ') continue;

        if(getPrecedence(condition[i]) > 0 || i == condition.size() - 1 || condition[i] == '=' || condition[i] == '<' || condition[i] == '>' ){
            if(i == condition.size() - 1) var += condition[i];

            if(isdigit(var[0]) || calculatedValue.find(var) != calculatedValue.end() || var.size() == 0 ) {var = "";continue;}
            else return false;
        }
        if(isdigit(condition[i]) || isalpha(condition[i]))var += condition[i];
    }
    return true;
}

bool Counter::checkCondition(string condition, Condition* node)
{
    string firstPart = "";
    string secondPart = "";
    string comparator = "";

    double a = 0;
    double b = 0;

    for(int i = 0; i < condition.size() - 1; i ++){
        if(condition[i] == '='){
            comparator = "=";
            break;
        } else if(condition[i] == '>' || condition[i] == '<'){
            if(condition[i + 1] == '='){
                comparator = condition[i] ;
                comparator += '=';
                break;
            }
        }
    }

    if(comparator != "" ){

        firstPart = condition.substr(0, condition.find(comparator));
        secondPart = condition.substr(condition.find(comparator) + 1 + comparator.size());

        firstPart.erase(remove(firstPart.begin(), firstPart.end(), ' '), firstPart.end());
        secondPart.erase(remove(secondPart.begin(), secondPart.end(), ' '), secondPart.end());

        if(findOperation(firstPart)){
            a = countExpression(firstPart);
        } else {
            if(isdigit(firstPart[0]))
                a = stod(firstPart);
            else a = calculatedValue[firstPart];
        }

        if(findOperation(secondPart)){
            b = countExpression(secondPart);
        } else {
            if(isdigit(secondPart[0]))
                b = stod(secondPart);
            else b = calculatedValue[secondPart];
        }

        if( comparator == "="){
            if(a == b)
                return true;
            else return false;
        } else if( comparator == ">=") {
            if(a >= b)
                return true;
            else return false;
        } else if( comparator == "<=") {
            if(a <= b)
                return true;
            else return false;
        } else if( comparator == "<") {
            if(a < b)
                return true;
            else return false;
        } else if( comparator == ">") {
            if(a < b)
                return true;
            else return false;
        }

    } else {
        double b ;
        condition.erase(remove(condition.begin(), condition.end(), ' '), condition.end());

        if(isdigit(condition[0]))
            b = stod(condition);
        else b = calculatedValue[condition];

        if(b > 0){
            return true;
        } else return false;

    }

    return false;
}

void Counter::calculate(Tree* tree)
{
    bool wasUnknownVariable = false;
    double result;
    int len = tree->getRoot()->getChildren()->size() ;
    vector<bool> found(len, false);
    for (size_t i = 0; i < len; i++)
    {
        if (tree->getRoot()->getChildren()->at(i)->getType() == "Operator")
        {
            Operator* node = static_cast<Operator *>(tree->getRoot()->getChildren()->at(i));
            char operation = node->getOperation();
            if(operation == '=')
            {
                Variable* var = static_cast<Variable*>(node->getLeft());
                if(canCalculate(node->getRight()))
                {
                    found[i] = true;
                    calculatedValue[var->getName()] = calculateExpression(node->getRight());
                }
            } else {
                if(canCalculate(node))
                {
                    found[i] = true;
                    result = calculateExpression(node);
                }
            }
        }
        else
        {
            Condition* node = static_cast<Condition *>(tree->getRoot()->getChildren()->at(i));

            if(canMakeCondition(node->getConditionExpression()) && canCalculate(node->getRight()) && canCalculate(node->getLeft()) )
            {
                if(checkCondition(node->getConditionExpression(), node) )
                {
                    //right
                    calculatedValue[node->getVariable()] = calculateExpression(node->getRight());
                }
                else
                {
                    //left
                    calculatedValue[node->getVariable()] = calculateExpression(node->getLeft());
                }
                found[i] = true;
            }
        }
    }

    for (size_t i = 0; i < tree->getRoot()->getChildren()->size(); i++)
    {
        if(found[i]) continue;

        if (tree->getRoot()->getChildren()->at(i)->getType() == "Operator")
        {
            Operator* node = static_cast<Operator *>(tree->getRoot()->getChildren()->at(i));
            char operation = node->getOperation();
            if(operation == '=')
            {
                Variable* var = static_cast<Variable*>(node->getLeft());
                if(canCalculate(node->getRight()))
                {
                    calculatedValue[var->getName()] = calculateExpression(node->getRight());
                }
                else
                {
                    wasUnknownVariable = true;
                }
            }
            else
            {
                if(canCalculate(node))
                {
                    result = calculateExpression(node);
                }
                else
                {
                    wasUnknownVariable = true;
                }
            }
        } else {
            Condition* node = static_cast<Condition *>(tree->getRoot()->getChildren()->at(i));

            if(canMakeCondition(node->getConditionExpression()))
            {
                if(checkCondition(node->getConditionExpression(), node) && canCalculate(node->getRight()) && canCalculate(node->getLeft()) )
                {
                    //right
                    calculatedValue[node->getVariable()] = calculateExpression(node->getRight());
                }
                else
                {
                    //left
                    calculatedValue[node->getVariable()] = calculateExpression(node->getLeft());
                }
                found[i] = true;
            } else wasUnknownVariable = true;
        }
    }

    if(wasUnknownVariable)
    {
        exit(0);
    }
    else
    {
        setAnswer(result);
    }
}


bool Counter::canCalculate(Node* node)
{
    vector<string> variables;
    addAllVariables(variables, node);
    for(int i = 0; i < variables.size(); i ++)
    {
        if(calculatedValue.find(variables[i]) == calculatedValue.end())
        {
            return false;
        }
    }
    return true;
}

void Counter::addAllVariables(vector<string>& variables, Node* node)
{
    if(node != nullptr){
        if(node->getType() == "Variable")
            variables.push_back(static_cast<Variable*>(node)->getName());

        addAllVariables(variables, node->getLeft());
        addAllVariables(variables, node->getRight());
    }
}

double Counter::calculateExpression(Node* node)
{
    if(!node)
    {
        return 0;
    }
    if(!node->getLeft() && !node->getRight())
    {
        if(node->getType() == "Constant")
        {
            return static_cast<Constant*>(node)->getValue();
        }
        else if(node->getType() == "Variable")
        {
            return calculatedValue[static_cast<Variable*>(node)->getName()] ;
        }
    }

    double leftValue = calculateExpression(node->getLeft()) ;

    double rightValue = calculateExpression(node->getRight()) ;

    return applyOperation(leftValue, rightValue, static_cast<Operator*>(node)->getOperation()) ;
}


double Counter::applyOperation(double b, double a, char op)
{
    double res = 0;
    switch (op) {
        case '+' :
            res = a + b;
            break;
        case '-' :
            res = b - a;
            break;
        case '*' :
            res = a * b;
            break;
        case '/' :
            res = (double) b / a;
            break;
    }
    return res;
}

