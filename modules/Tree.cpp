#ifndef SHAPE_CPP
#define SHAPE_CPP

#include <vector>
#include <string>

using namespace std;

class Node
{
private:
    Node *right;
    Node *left;
    string type;

public:
    Node() : right(nullptr), left(nullptr), type("Node") {}
    void setType(string type);
    string getType();
    void setLeft(Node *node);
    Node *getLeft();
    void setRight(Node *node);
    Node *getRight();
};

class Root : public Node
{
private:
    vector<Node *> children;

public:
    Root();
    void addChild(Node *node);
    vector<Node *> *getChildren();
};

class Condition : public Node
{
private:
    string forVariable ;
    string conditionExpression ;
public:
    Condition();
    void setVariable(string variable);
    string getVariable();
    void setCondExpression(string conditionExpression);
    string getConditionExpression();
};

class Variable : public Node
{
private:
    string name;

public:
    Variable();
    void setName(string name);
    string getName();
};
class Operator : public Node
{
private:
    char operation;

public:
    Operator();
    void setOperation(char operation);
    char getOperation();
};

class Constant : public Node
{
private:
    double value;

public:
    Constant();
    void setValue(double value);
    double getValue();
};

class Tree
{
private:
    Root *root;

public:
    Tree();
    Root *getRoot();
    void addNode(Node *node);
    vector<Node *> *getChildren();
};

/*
** Basic Node interface
*/

void Condition::setVariable(string variable){
    this->forVariable = variable;
}
string Condition::getVariable(){
    return this->forVariable;
}
Condition::Condition(){
    forVariable = "";
    conditionExpression = "";
    this->setType("Condition");
}

void Condition::setCondExpression(string conditionExpression){
    this->conditionExpression = conditionExpression;
}
string Condition::getConditionExpression(){
    return this->conditionExpression;
}

void Node::setType(string type)
{
    this->type = type;
}

string Node::getType()
{
    return this->type;
}

void Node::setLeft(Node *node)
{
    this->left = node;
}

Node *Node::getLeft()
{
    return this->left;
}

void Node::setRight(Node *node)
{
    this->right = node;
}

Node *Node::getRight()
{
    return this->right;
}

/*
** Root interface
*/
Root::Root()
{
    this->setType("Root");
}

vector<Node *> *Root::getChildren()
{
    vector<Node *> *a = &children;
    return a;
}

void Root::addChild(Node *node)
{
    this->children.push_back(node);
}

/*
** Variable interface
*/
Variable::Variable()
{
    this->setType("Variable");
}

void Variable::setName(string name)
{
    this->name = name;
}

string Variable::getName()
{
    return this->name;
}

/*
** Operator interface
*/
Operator::Operator()
{
    this->setType("Operator");
}

void Operator::setOperation(char operation)
{
    this->operation = operation;
}

char Operator::getOperation()
{
    return this->operation;
}

/*
** Constant interface
*/
Constant::Constant()
{
    this->setType("Constant");
}

void Constant::setValue(double value)
{
    this->value = value;
}

double Constant::getValue()
{
    return this->value;
}

/*
** Tree interface
*/
Tree::Tree() : root(new Root) {}

Root *Tree::getRoot()
{
    return this->root;
}

void Tree::addNode(Node *node)
{
    this->root->addChild(node);
}
#endif
