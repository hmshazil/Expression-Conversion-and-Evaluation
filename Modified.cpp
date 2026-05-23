// ============================================================
  // NAME : H.M.Shazil
  //ROLL NO : BCSF24A009
// ============================================================

#include <iostream>
#include <string>
#include <stack>    
#include <vector>  
#include <map>      
using namespace std;

// ============================================================
// HELPER FUNCTIONS 
// ============================================================

bool isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isOpenBracket(char c) {
    return c == '(' || c == '[' || c == '{';
}

bool isCloseBracket(char c) {
    return c == ')' || c == ']' || c == '}';
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

// Closing bracket ka matching opening bracket
char matchingOpen(char close) {
    if (close == ')') return '(';
    if (close == ']') return '[';
    if (close == '}') return '{';
    return '?';
}

// Operator ki priority:

int precedence(const string& op) {
    if (op == "%")             return 3;  
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

// String ko double mein convert karo
double toDouble(const string& s) {
    double result = 0;
    double dec = 0;       // 0 matlab abhi decimal nahi aaya
    for (char c : s) {
        if (c == '.') {
            dec = 0.1;    // Decimal point aa gaya
        } else {
            int d = c - '0';
            if (dec == 0) result = result * 10 + d;   // Integer part
            else          { result += d * dec; dec *= 0.1; }  // Decimal part
        }
    }
    return result;
}

// ============================================================
// STEP 1: TOKENIZATION
// ============================================================

vector<string> tokenize(const string& expr) {
    vector<string> tokens;
    int i = 0, len = expr.size();

    while (i < len) {
        char c = expr[i];

        // Whitespace skip
        if (c == ' ' || c == '\t') { i++; continue; }

        // Variable naam
        if (isLetter(c) || c == '_') {
            string word;
            while (i < len && (isLetter(expr[i]) || isDigit(expr[i]) || expr[i] == '_'))
                word += expr[i++];
            tokens.push_back(word);
            continue;
        }

        // Number: digits aur optional decimal point
        if (isDigit(c)) {
            string num;
            bool hasDot = false;
            while (i < len && (isDigit(expr[i]) || (expr[i] == '.' && !hasDot))) {
                if (expr[i] == '.') hasDot = true;
                num += expr[i++];
            }
            tokens.push_back(num);
            continue;
        }

        // Operator ya bracket   single character
        if (isOperator(c) || isOpenBracket(c) || isCloseBracket(c)) {
            tokens.push_back(string(1, c));
            i++;
            continue;
        }

        // Koi aur character syntax error
        cerr << "Error: Invalid character '" << c << "' in expression" << endl;
        exit(1);
    }

    return tokens;
}

// ============================================================
// STEP 2: SYNTAX VALIDATION 
// ============================================================

void validateSyntax(const vector<string>& tokens) {
    int n = tokens.size();

    if (n == 0) {
        cerr << "Error: Empty expression" << endl;
        exit(1);
    }

    // Shuru ya end operator se nahi ho sakta
    if (isOperator(tokens[0][0])) {
        cerr << "Error: Expression starts with operator '" << tokens[0] << "'" << endl;
        exit(1);
    }
    if (isOperator(tokens[n-1][0])) {
        cerr << "Error: Expression ends with operator '" << tokens[n-1] << "'" << endl;
        exit(1);
    }

    for (int i = 0; i < n - 1; i++) {
        char curr = tokens[i][0];
        char next = tokens[i+1][0];

        //  token operand hai (variable ya number)
        bool currIsOperand = isLetter(curr) || isDigit(curr) || curr == '_';
        bool nextIsOperand = isLetter(next) || isDigit(next) || next == '_';

        // Do operators saath saath nahi aa sakte: a ** b 
        if (isOperator(curr) && isOperator(next)) {
            cerr << "Error: Two consecutive operators '"
                 << tokens[i] << "' and '" << tokens[i+1] << "'" << endl;
            exit(1);
        }

        // Do operands ke beech operator HONA chahiye
        bool currIsOperandOrClose = currIsOperand || isCloseBracket(curr);
        bool nextIsOperandOrOpen  = nextIsOperand || isOpenBracket(next);
        if (currIsOperandOrClose && nextIsOperandOrOpen) {
            cerr << "Error: Missing operator between '"
                 << tokens[i] << "' and '" << tokens[i+1] << "'" << endl;
            exit(1);
        }

        // Operator ke baad seedha closing bracket: a + )
        if (isOperator(curr) && isCloseBracket(next)) {
            cerr << "Error: Operator '" << tokens[i]
                 << "' before closing bracket '" << tokens[i+1] << "'" << endl;
            exit(1);
        }

        // Opening bracket ke baad seedha operator: ( * a
        if (isOpenBracket(curr) && isOperator(next)) {
            cerr << "Error: Operator '" << tokens[i+1]
                 << "' right after opening bracket '" << tokens[i] << "'" << endl;
            exit(1);
        }
    }
}

// ============================================================
// STEP 3: INFIX → POSTFIX 
// ============================================================

vector<string> infixToPostfix(const vector<string>& tokens) {
    vector<string> postfix;   
    stack<string>  opStack;   

    for (const string& token : tokens) {
        char first = token[0];

        // Operand (number ya variable)  seedha output mein
        if (isDigit(first) || isLetter(first) || first == '_') {
            postfix.push_back(token);
        }

        // Opening bracket  push stack
        else if (isOpenBracket(first)) {
            opStack.push(token);
        }

        // Closing bracket  tk pop
        else if (isCloseBracket(first)) {
            char expectedOpen = matchingOpen(first);
            bool found = false;

            while (!opStack.empty()) {
                string top = opStack.top(); opStack.pop();
                if (top[0] == expectedOpen) { found = true; break; }
                postfix.push_back(top);     // Operator output mein
            }

            if (!found) {
                cerr << "Error: No matching opening bracket for '" << token << "'" << endl;
                exit(1);
            }
        }

        // Operator  high/equal priority
        else if (isOperator(first)) {
            while (!opStack.empty()
                   && !isOpenBracket(opStack.top()[0])
                   && precedence(opStack.top()) >= precedence(token)) {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
        }
    }

    // output
    while (!opStack.empty()) {
        string top = opStack.top(); opStack.pop();
        if (isOpenBracket(top[0])) {
            cerr << "Error: Unmatched opening bracket '" << top << "'" << endl;
            exit(1);
        }
        postfix.push_back(top);
    }

    return postfix;
}

// ============================================================
// STEP 4: VARIABLES KI VALUES LENA
// ============================================================

map<string, double> collectVariables(const vector<string>& postfix) {
    map<string, double> varMap;    
    vector<string> uniqueVars;       // postfix mein jis order mein aaye

    // Postfix se unique variable names nikalo
    for (const string& token : postfix) {
        char first = token[0];
        if ((isLetter(first) || first == '_') &&
            varMap.find(token) == varMap.end()) {  
            varMap[token] = 0;       
            uniqueVars.push_back(token); 
        }
    }

    for (const string& name : uniqueVars) {
        cerr << "Enter value for " << name << ": ";
        double val;
        if (!(cin >> val)) {
            cerr << "Error: Invalid input for '" << name << "'" << endl;
            exit(1);
        }
        varMap[name] = val;  // Map update
    }

    return varMap;
}

// ============================================================
// STEP 5: POSTFIX EVALUATION
// ============================================================

double evaluatePostfix(const vector<string>& postfix, const map<string, double>& varMap) {
    stack<double> numStack;

    for (const string& token : postfix) {
        char first = token[0];


        if (isDigit(first) || first == '.') {
            numStack.push(toDouble(token));
        }

        else if (isLetter(first) || first == '_') {
            if (varMap.find(token) == varMap.end()) {
                cerr << "Error: Variable '" << token << "' not found" << endl;
                exit(2);
            }
            numStack.push(varMap.at(token));
        }

        else if (isOperator(first)) {
            if (numStack.size() < 2) {
                cerr << "Error: Not enough operands for '" << token << "'" << endl;
                exit(2);
            }
            //  right pehle nikalta hai 
            double right = numStack.top(); numStack.pop();
            double left  = numStack.top(); numStack.pop();
            double result = 0;

            if      (token == "+") result = left + right;
            else if (token == "-") result = left - right;
            else if (token == "*") result = left * right;
            else if (token == "/") {
                if (right == 0) {
                    cerr << "Error: Division by zero" << endl;
                    exit(2);
                }
                result = left / right;
            }
            else if (token == "%") {
                
                if (right == 0) {
                    cerr << "Error: Modulus by zero" << endl;
                    exit(2);
                }
                if ((long long)left != left || (long long)right != right) {
                    cerr << "Error: Modulus (%) only works on integers, got decimals" << endl;
                    exit(2);
                }
                result = (long long)left % (long long)right;  //  only Integer modulus
            }

            numStack.push(result);
        }
    }

    // only 1 value hogi
    if (numStack.size() != 1) {
        cerr << "Error: Expression is invalid — extra operands left over" << endl;
        exit(2);
    }

    return numStack.top();
}

// ============================================================
// POSTFIX VECTOR → PRINTABLE STRING
// ============================================================

string postfixToString(const vector<string>& postfix) {
    string s;
    for (int i = 0; i < (int)postfix.size(); i++) {
        if (i > 0) s += " ";
        s += postfix[i];
    }
    return s;
}

// ============================================================
// MAIN
// ============================================================

int main() {
    string expression;
    getline(cin, expression);           

    vector<string> tokens  = tokenize(expression);        // Step 1
    validateSyntax(tokens);                                // Step 2 
    vector<string> postfix = infixToPostfix(tokens);      // Step 3

    cout << postfixToString(postfix) << endl;              // Postfix print 

    map<string, double> varMap = collectVariables(postfix); // Step 4 
    double answer = evaluatePostfix(postfix, varMap);       // Step 5

    // Agar answer integer hai toh bina .0 print karo
    if (answer == (long long)answer)
        cout << (long long)answer << endl;
    else
        cout << answer << endl;

    return 0;  
}