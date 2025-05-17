#include "support.h"
#include <stack>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <iostream>
using namespace std;
const char SQRT_SYMBOL = 'r';


string scanNum(int &i, const string &str) {
    string value;
    while(i < str.length() && str[i] == ' ')i++;
    if (i < str.length() && str[i] == '-' && (i == 0 || str[i-1] == '(' || isOperator(str[i-1]))) {
        value += str[i++];
    }
    while(i < str.length() && str[i] == ' ') i++;
    bool Dot = false;
    while(i < str.length() && (isdigit(str[i]) || str[i] == '.')) {
        if(str[i] == '.') {
            if(Dot)
                break;
            Dot = true;
            if(value.empty() || value == "-")
                value += '0';
        }
        value += str[i++];
    }
    return value;
}


int prec(char c) {
    if (c == SQRT_SYMBOL) return 4;
    if (c == '^') return 3;
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return -1;
}


string infixToPostfix(string s) {
    stack<char> st;
    string result;
    for(int i = 0; i < s.length(); i++) {
        char c = s[i];
        if(c == ' ')
            continue;
        if(isdigit(c) || c == '.' || (c == '-' && (i == 0 || s[i-1] == '(' || isOperator(s[i-1])))) {
            string num = scanNum(i, s);
            result += num + " ";
            i--;
            continue;
        }
        if(c == '(') {
            st.push(c);
        } else if(c == ')') {
            while(!st.empty() && st.top() != '(') {
                result += st.top();
                result += " ";
                st.pop();
            }
            if(st.empty()) throw runtime_error("Несбалансированные скобки");
            st.pop();
        }
        else if(isOperator(c)) {
            while(!st.empty() && prec(c) <= prec(st.top())) {
                result += st.top();
                result += " ";
                st.pop();
            }
            st.push(c);
        }
    }
    while(!st.empty()) {
        if(st.top() == '(') throw runtime_error("Несбалансированные скобки");
        result += st.top();
        result += " ";
        st.pop();
    }
    return result;
}


int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == SQRT_SYMBOL);
}


double operation(double a, double b, char op) {
    switch(op) {
    case '+': return b + a;
    case '-': return b - a;
    case '*': return b * a;
    case '/':
        if(a == 0) throw runtime_error("Деление на ноль");
        return b / a;
    case '^':
        if(b < 0 && a != floor(a)) {
            throw runtime_error("Отрицательное число в дробной степени");
        }
        return pow(b, a);
    case SQRT_SYMBOL:
        if(a < 0) throw runtime_error("Корень из отрицательного числа");
        return sqrt(a);
    default:
        throw runtime_error("Неизвестная операция");
    }
}


double postfixEval(const string &postfix) {
    stack<double> stk;
    istringstream iss(postfix);
    string token;

    while(iss >> token) {
        if(token.size() == 1 && isOperator(token[0])) {
            if(stk.size() < 1) throw runtime_error("Ошибка");
            double a = stk.top(); stk.pop();
            if(token[0] == SQRT_SYMBOL) {
                stk.push(operation(a, 0, SQRT_SYMBOL));
            } else {
                if(stk.size() < 1) throw runtime_error("Ошибка");
                double b = stk.top(); stk.pop();
                stk.push(operation(a, b, token[0]));
            }
        } else {
            try {
                stk.push(stod(token));
            } catch(...) {
                throw runtime_error("Некорректное число: " + token);
            }
        }
    }

    if(stk.size() != 1) throw runtime_error("Некорректное выражение");
    return stk.top();
}


double evaluate(const string &str) {
    try {
        string postfix = infixToPostfix(str);
        return postfixEval(postfix);
    } catch(const exception &e) {
        cerr << "Ошибка: " << e.what() << endl;
        throw;
    }
}
