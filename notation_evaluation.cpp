#include <iostream>
#include <stack>
#include <string>
#include <cctype>  // For isdigit
#include <algorithm>  // For reverse
using namespace std;

// Function to perform binary operations
int performOperation(char op, int operand1, int operand2) {
    switch (op) {
        case '+':
            return operand1 + operand2;
        case '-':
            return operand1 - operand2;
        case '*':
            return operand1 * operand2;
        case '/':
            if (operand2 != 0)
                return operand1 / operand2;
            else {
                cerr << "Error: Division by zero." << endl;
                exit(1);
            }
        case '%':
            if (operand2 != 0)
                return operand1 % operand2;
            else {
                cerr << "Error: Modulo by zero." << endl;
                exit(1);
            }
        default:
            cerr << "Error: Invalid operator." << endl;
            exit(1);
    }
}

// Function to get precedence of an operator
int getPrecedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/' || op == '%')
        return 2;
    return 0;  // For parentheses
}

// Function to evaluate infix expression
int evaluateInfixExpression(const string &expression) {
    stack<int> operands;
    stack<char> operators;

    for (char ch : expression) {
        if (isdigit(ch)) {
            operands.push(ch - '0');
        } else if (ch == '(') {
            operators.push(ch);
        } else if (ch == ')') {
            while (!operators.empty() && operators.top() != '(') {
                char op = operators.top();
                operators.pop();
                int operand2 = operands.top();
                operands.pop();
                int operand1 = operands.top();
                operands.pop();
                operands.push(performOperation(op, operand1, operand2));
            }
            operators.pop();  // Pop the opening parenthesis
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%') {
            while (!operators.empty() && getPrecedence(ch) <= getPrecedence(operators.top())) {
                char op = operators.top();
                operators.pop();
                int operand2 = operands.top();
                operands.pop();
                int operand1 = operands.top();
                operands.pop();
                operands.push(performOperation(op, operand1, operand2));
            }
            operators.push(ch);
        }
    }

    while (!operators.empty()) {
        char op = operators.top();
        operators.pop();
        int operand2 = operands.top();
        operands.pop();
        int operand1 = operands.top();
        operands.pop();
        operands.push(performOperation(op, operand1, operand2));
    }

    return operands.top();
}

// Function to evaluate postfix expression
int evaluatePostfixExpression(const string &expression) {
    stack<int> operands;

    for (char ch : expression) {
        if (isdigit(ch)) {
            operands.push(ch - '0');
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%') {
            int operand2 = operands.top();
            operands.pop();
            int operand1 = operands.top();
            operands.pop();
            operands.push(performOperation(ch, operand1, operand2));
        }
    }

    return operands.top();
}

// Function to evaluate prefix expression
int evaluatePrefixExpression(const string &expression) {
    stack<int> operands;

    // Reverse the prefix expression to process it from right to left
    string reversedExpression = expression;
    reverse(reversedExpression.begin(), reversedExpression.end());

    for (char ch : reversedExpression) {
        if (isdigit(ch)) {
            operands.push(ch - '0');
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%') {
            int operand1 = operands.top();
            operands.pop();
            int operand2 = operands.top();
            operands.pop();
            operands.push(performOperation(ch, operand1, operand2));
        }
    }

    return operands.top();
}

int main() {
    string expression;
    char notation;

    cout << "Enter notation type(i for infix,p for prefix,o for postfix): ";
    cin >> notation;
    cin.ignore();
    cout << "Enter an expression: ";
    getline(cin, expression);

    int result;

    if (notation == 'i') {
        // Infix notation
        result = evaluateInfixExpression(expression);
    } else if (notation == 'p') {
        // Prefix notation
        result = evaluatePrefixExpression(expression);
    } else if (notation == 'o') {
        // Postfix notation
        result = evaluatePostfixExpression(expression);
    } else {
        cerr << "Error: Invalid notation type." << endl;
        return 1;
    }

    cout << "Result: " << result << endl;

    return 0;
}
