#ifndef SUPPORT_H
#define SUPPORT_H
#include <string>
#include <string>
#include <cmath>
using namespace std;

string scanNum(int &i, const string &str);
int prec(char c) ;
string infixToPostfix(string s) ;
int isOperator(char ch);
int isOperand(char ch);
double operation(double a, double b, char op);
double postfixEval(const string &postfix);
double evaluate(const string &str);
double sqrtOperation(double a);
const char INTERNAL_SQRT_SYMBOL = 'r';
bool isValidNumber(const std::string& str);

#endif // SUPPORT_H
