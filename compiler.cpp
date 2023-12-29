#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include<bits/stdc++.h>
using namespace std;

#define Fast ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

enum Type {
    KEYWORD,
    IDENTIFIER,
    OPERATORMATH,
    OPERATORLOGICAL,
    NUMBER,
    DELIMITER,
    UNKNOWN
};
struct Token {
    Type type;
    string value;
};

class LexicalAnalysisPhase {
private:
    string statement;

public:
    LexicalAnalysisPhase() { }
    LexicalAnalysisPhase(string _statement) {
        statement = _statement;
    }
    Token get_next_token() {
        Token token;
        // if the user send an empty input
        if (statement.empty()) {
            token.type = UNKNOWN;
            return token;
        }

        // space
        if (isspace(statement[0])) {
            statement.erase(0, 1);
        }

        char c = statement[0];
        statement.erase(0, 1);

        // Keywords
        if (isalpha(c)) {
            string identifier;
            identifier += c;
            while (isalnum(statement[0])) {
                identifier += statement[0];
                statement.erase(0, 1);
            }

            const char* keyword = strstr("float int double char if while", identifier.c_str());
            if (keyword) {
                token.type = KEYWORD;
                token.value = identifier;
            }
            else {
                token.type = IDENTIFIER;
                token.value = identifier;
            }
        }
        // Digites
        else if (isdigit(c)) {
            string number;
            number += c;
            while (isdigit(statement[0]) || statement[0] == '.') {
                number += statement[0];
                statement.erase(0, 1);
            }
            token.type = NUMBER;
            token.value = number;
        }
        // Operators and Delimiters
        else if (ispunct(c)) {
            switch (c) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '=':
                token.type = OPERATORMATH;
                token.value += c;
                break;
            case '!':
            case '%':
            case '>':
            case '<':
            case '&':
            case '|':
            case '?':
            case ':':
            case '^':
                token.type = OPERATORLOGICAL;
                token.value += c;
                break;
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            case ';':
            case ',':
            case '\'':
                token.type = DELIMITER;
                token.value += c;
                break;
            default:
                token.type = UNKNOWN;
                break;
            }
        }

        return token;
    }
    
    void Print() {
        cout << "Lexical Analysis For:\n\t"<<statement<<"\n";
        
        Token token;
        while (token.type != UNKNOWN) {
            token = get_next_token();
            if (token.type != UNKNOWN) {

                if (token.type == 0) cout << "KEYWORD:\n";
                else if (token.type == 1) cout << "IDENTIFIER:\n";
                else if (token.type == 2) cout << "Mathematical OPERATOR:\n";
                else if (token.type == 3) cout << "Logical OPERATOR:\n";
                else if (token.type == 4) cout << "NUMBER:\n";
                else if (token.type == 5) cout << "DELIMITER:\n";
                else cout << "UNKNOWN:\n";

                cout << " \t " << token.value << endl;
                
            }
        }

    }
};


struct Pattern {
    string op;
    string arg1;
    string arg2;
    string result;
};

// Intermediate Code Generation
class IntermediateCodeGenerationPhase {
public:
    vector<Pattern> pattern;  // Store pattern instructions
    int count;      // Counter for temporary variables
    string Statement;
    IntermediateCodeGenerationPhase() {
        count = 0;
        
    }
    
    void Arrange(string statement) {// 5 + 6 * 3 /9 - 5 ;
        stack<string> s;
        Statement = statement;
        if (s.empty()) {
            s.push(to_string(statement[0] - '0')); // 5 + 6
        }
        while (s.top()[0] != 'R' ) {
            for (int i = 1; i < statement.length(); i++) {
                if (isspace(statement[i])) {
                    continue;
                }

                if (statement[i] == ';') {
                    while (s.size() > 1) {
                        string arg1 = s.top();
                        s.pop();
                        string op = s.top();
                        s.pop();
                        string arg2 = s.top();
                        s.pop();
                        if (op == "+") {
                        string temp = arithmeticStep(op, arg1, arg2);
                        s.push(temp);
                        }
                        else {
                            string temp = arithmeticStep(op, arg2, arg1);
                        s.push(temp);
                        }
                    }
                    break;
                }

                if (isdigit(statement[i])) {
                    if (s.top() == "*" || s.top() == "/") {
                        string op = s.top();
                        s.pop();
                        string temp = arithmeticStep(op, s.top(), to_string(statement[i] - '0'));
                        s.pop();
                        s.push(temp);
                        continue;
                    }
                    else if (s.top() == "+" || s.top() == "-") {
                        s.push(to_string(statement[i] - '0'));
                        continue;
                    }
                }
                else {
                    switch (statement[i]) {
                    case '+':
                        s.push("+");
                        break;
                    case '-':
                        s.push("-");
                        break;
                    case '*':
                        s.push("*");
                        break;
                    case '/':
                        s.push("/");
                        break;
                    }
                }
            }
        }
        pattern.push_back({ "=","result",s.top(),"" });

    }
   //0 =>48 9=>57 
    // Function to generate a unique temporary variable name
    string genTemp(string ch) {
        
            return ch + to_string(++count);
        
    }

    // Function to generate pattern for an assignment expression
    void assignmentStep(string var, string expr) {
        string temp = genTemp("R");  // Generate temporary for expression result
        pattern.push_back({ "=", expr, "", temp });
        pattern.push_back({ "=", temp, "", var });
    }

    // Function to generate pattern for an arithmetic expression
    string arithmeticStep(string op, string arg1, string arg2) {
        string temp;
        
            temp = genTemp("R");
        
        pattern.push_back({ op, arg1, arg2, temp });
        return temp;
    }
    void print() {
        cout << "\nIntermediate Code Generation:\n\t"<<Statement<<"\n";

        for (auto val : pattern) {
            cout << val.arg1 << " " << val.op << " " << val.arg2 << " " << val.result << endl;
        }
    }

};


int main() {
    Fast
        string code = "int result = 10 * 5 + 19; result>50? print(result):print('hello world') }";
        LexicalAnalysisPhase LAP = LexicalAnalysisPhase(code);
        LAP.Print();
            
        IntermediateCodeGenerationPhase obj = IntermediateCodeGenerationPhase();
        
        obj.Arrange("7 + 6 * 3 / 9 * 2 - 2 ;");
        obj.print();
    return 0;
}
