#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>

using namespace std;



class Calculator {
private:
    string expression;

public:
    Calculator() {}
    Calculator(const string& _expression) { this->expression = _expression; }
    double calculate();
    bool isBooleanExpr(const string& op);
    vector<string> infixToPostfix(const string& infix);
    double calculateNumberExpression(const vector<string>& postfix);
    bool calculateBooleanExpression(const vector<string>& postfix);
    string calculateBooleanExpr(const string& infix, int& i);

};

int precedence(const string op) {

    if (op.size() == 1) {
        if (op == "(" || op == ")" || op == ":" || op == "#") { return 0; }
        else if (op == "?") { return 1; }
        else if (op == "<" || op == ">") { return 5; }
        else if (op == "+" || op == "-") { return 6; }
        else if (op == "*" || op == "/") { return 7; }
        else if (op == "!") { return 8; }
        else { std::cout << "Invalid operator!" << std::endl; }
    }
    else if (op.size() == 2) {
        if (op == "||") { return 2; }
        else if (op == "&&") { return 3; }
        else if (op == "==" || op == "!=") { return 4; }
        else { std::cout << "Invalid operator!" << std::endl; }
    }
    else { std::cout << "Invalid operator!" << std::endl; }
}

string isOperator(const char& symbol, char nextSymbol, int& it) {

    if (symbol == '&' && nextSymbol == '&') { it++; return "&&"; }
    else if (symbol == '|' && nextSymbol == '|') { it++; return "||"; }
    else if (symbol == '=' && nextSymbol == '=') { it++; return "=="; }
    else if (symbol == '!' && nextSymbol == '=') { it++; return "!="; }
    else if (symbol == '+') { return  "+"; }
    else if (symbol == '-') { return  "-"; }
    else if (symbol == '*') { return  "*"; }
    else if (symbol == '/') { return  "/"; }
    else if (symbol == '<') { return  "<"; }
    else if (symbol == '>') { return  ">"; }
    else if (symbol == '!') { return  "!"; }
    else { return""; }
}

bool Calculator::isBooleanExpr(const string& op) {
    if (op == "==" || op == "!=" || op == "<" || op == ">" || op == "||") {
        return true;
    }
    return false;
}

string addNumber(const string& infix, int& it) {
    string number;
    while ((infix[it] >= '0' && infix[it] <= '9') || infix[it] == '.') {
        number.push_back(infix[it]);
        it++;
        if (it == infix.size()) { break; }
    }
    return number;
}

string addNegativeNumber(const string& infix, int& it) {
    it++;
    string number;
    number.push_back(infix[it]);
    it++;
    while (infix[it] != ')') {
        number.push_back(infix[it]);
        it++;
        if (it == infix.size()) { break; }
    }
    return number;
}

double Calculator::calculate() {
    vector<string> postfix = infixToPostfix(this->expression);
    if (isBooleanExpr(postfix[postfix.size() - 1])) {
        if (calculateBooleanExpression(postfix) == 1) {
            return 1;
        }
        return 0;
    }
/*    std::cout.precision(2);
    std::cout.setf(std::ios::fixed);   */             // floatfield not set
    
    return (calculateNumberExpression(postfix));
}

vector<string> Calculator::infixToPostfix(const string& infix) {
    stack<string> operators;
    operators.push("#");
    vector<string> postfix;

    for (int it = 0; it < infix.size(); it++) {

        if (infix[it] != ' ') {
            if (infix[it] == 'i' && infix[it + 1] == 'f') {
                it += 2;
                postfix.push_back(calculateBooleanExpr(infix, it));
                if (it == infix.size()) { break; }
            }
            if (infix[it] >= '0' && infix[it] <= '9') {
                string number = addNumber(infix, it);
                postfix.push_back(number);
            }
            if (infix[it] == '(') {
                if (infix[it] == '(' && infix[it + 1] == '!') {
                    it += 2;
                    /*  if (infix[it] == '(' && infix[it+1] == '-') {
                          it += 2;
                          string number = addNumber(infix, it);
                          postfix.push_back(number);
                          it++;
                      }*/
                    if (infix[it] == '(') {
                        if (infix[it + 1] == '0') {
                            postfix.push_back("1");
                        }
                        else { postfix.push_back("0"); }
                        while (infix[it] != ')') { it++; }
                        it++;

                    }
                    else {
                        it++;
                        string number;
                        number.push_back('-');
                        number += addNumber(infix, it);
                        postfix.push_back(number);
                        it++;
                    }
                }
                else if (infix[it] == '(' && infix[it + 1] == '-') {
                    string number = addNegativeNumber(infix, it);
                    postfix.push_back(number);
                    it++;
                }
                else {
                    string str(1, infix[it]);
                    operators.push(str);
                }
            }
            if (infix[it] == ')') {
                while (operators.top() != "#" && operators.top() != "(") {
                    postfix.push_back(operators.top());
                    operators.pop();
                }
                operators.pop();
            }
            string op = (it + 1 < infix.size()) ? (isOperator(infix[it], infix[it + 1], it)) : (isOperator(infix[it], '\\', it));
            if (op != "") {
                if (!operators.empty() && precedence(op) > precedence(operators.top()))
                    operators.push(op);
                else {
                    while (operators.top() != "#" && precedence(op) <= precedence(operators.top())) {
                        postfix.push_back(operators.top());
                        operators.pop();
                    }
                    operators.push(op);
                }
            }
        }
    }
    while (!operators.empty() && operators.top() != "#") {
        postfix.push_back(operators.top());
        operators.pop();
    }
    return postfix;
}

double Calculator::calculateNumberExpression(const vector<string>& postfix) {

    stack<double> st;
    string op = "+-*/!";
    if (postfix.size() == 0) { return 0; }
    for (int i = 0; i < postfix.size(); i++) {
        string tkn = postfix[i];
        int opIndx = op.find(tkn);
        if (opIndx != -1) {
            if (st.size() < 2) {
                return -1;   //Invalid 
            }
            else {
                double a = st.top();
                st.pop();
                double b = st.top();
                st.pop();
                if (opIndx == 0) { st.push(b + a); }
                if (opIndx == 1) { st.push(b - a); }
                if (opIndx == 2) { st.push(b * a); }
                if (opIndx == 3) { st.push(b / a); }
            }
        }
        else {
            st.push(stod(tkn.c_str()));
        }
    }
    cout.precision(2);
    cout.setf(ios::fixed);
    return st.top();
}

bool Calculator::calculateBooleanExpression(const vector<string>& postfix) {

    stack<double> st;
    vector<string> op = { "+", "-", "*", "/", "==","!=", "<", ">" };
    if (postfix.size() == 0) { return 0; }

    for (int i = 0; i < postfix.size(); i++) {
        string tok = postfix[i];
        int o = -1;
        for (int i = 0; i < op.size(); i++) {
            if (tok == op[i]) {
                o = i;
            }
        }

        if (o != -1) {
            if (st.size() < 2) { return -1; } //invalid 
            else {
                double a = st.top();
                st.pop();
                double b = st.top();
                st.pop();
                if (o == 0) { st.push(b + a); }
                if (o == 1) { st.push(b - a); }
                if (o == 2) { st.push(b * a); }
                if (o == 3) { st.push(b / a); }
                if (o == 4) { return b == a; }
                if (o == 5) { return b != a; }
                if (o == 6) { return b < a; }
                if (o == 7) { return b > a; }
            }
        }
        else {
            st.push(stod(tok.c_str()));
        }
    }
}

string Calculator::calculateBooleanExpr(const string& infix, int& i) {

    string booleanCondition;
    while (infix[i] != '?') {
        booleanCondition.push_back(infix[i]);
        i++;
    }
    i++;

    vector<string> postfix = infixToPostfix(booleanCondition);
    bool result = calculateBooleanExpression(postfix);

    if (result) {
        string isTrue;
        while (infix[i] != ':') {
            isTrue.push_back(infix[i]);
            i++;
        }
        vector<string> isTruePostfix = infixToPostfix(isTrue);
        double finalResult = calculateNumberExpression(isTruePostfix);

        while (infix[i] != ')' && i < infix.size()) { i++; }
        return to_string(finalResult);

    }
    else {
        string isFalse;
        while (infix[i] != ':') { i++; }
        while (infix[i] != ')' && i == infix.size() - 1) {
            isFalse.push_back(infix[i]);
            i++;
        }
        vector<string> isFalsePostfix = infixToPostfix(isFalse);
        double finalResult = calculateNumberExpression(isFalsePostfix);
        return to_string(finalResult);
    }
}
