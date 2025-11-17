#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define MAX 100

// -------------------- STACK FOR OPERATORS --------------------
typedef struct {
    char data[MAX];
    int top;
} CharStack;

void pushChar(CharStack *s, char c) {
    s->data[++s->top] = c;
}

char popChar(CharStack *s) {
    return s->data[s->top--];
}

char peekChar(CharStack *s) {
    return s->data[s->top];
}

int isEmptyChar(CharStack *s) {
    return s->top == -1;
}

// -------------------- STACK FOR NUMBERS --------------------
typedef struct {
    double data[MAX];
    int top;
} NumStack;

void pushNum(NumStack *s, double val) {
    s->data[++s->top] = val;
}

double popNum(NumStack *s) {
    return s->data[s->top--];
}

// -------------------- UTILITY --------------------
int precedence(char c) {
    if (c == '+' || c == '-') return 1;
    if (c == '*' || c == '/') return 2;
    if (c == '^') return 3;
    return 0;
}

// -------------------- INFIX TO POSTFIX --------------------
void infixToPostfix(char infix[], char postfix[]) {
    CharStack s;
    s.top = -1;
    int j = 0;

    for (int i = 0; infix[i] != '\0'; i++) {

        // Number (multi-digit)
        if (isdigit(infix[i]) || infix[i] == '.') {
            postfix[j++] = infix[i];
        }

        // Space after number
        else if (infix[i] == ' ') continue;

        // Add space to separate numbers
        else {
            postfix[j++] = ' ';

            if (infix[i] == '(') {
                pushChar(&s, '(');
            }

            else if (infix[i] == ')') {
                while (!isEmptyChar(&s) && peekChar(&s) != '(')
                    postfix[j++] = popChar(&s), postfix[j++] = ' ';

                popChar(&s); // remove '('
            }

            else { // Operator
                while (!isEmptyChar(&s) &&
                       precedence(peekChar(&s)) >= precedence(infix[i])) {
                    postfix[j++] = popChar(&s);
                    postfix[j++] = ' ';
                }
                pushChar(&s, infix[i]);
            }
        }
    }

    // Pop remaining
    while (!isEmptyChar(&s)) {
        postfix[j++] = ' ';
        postfix[j++] = popChar(&s);
    }

    postfix[j] = '\0';
}

// -------------------- EVALUATE POSTFIX --------------------
double evaluatePostfix(char postfix[]) {
    NumStack s;
    s.top = -1;

    for (int i = 0; postfix[i] != '\0'; i++) {

        if (isdigit(postfix[i]) || postfix[i] == '.') {
            double num = 0;
            double decimal = 0;
            int decimalFlag = 0;

            while (isdigit(postfix[i]) || postfix[i] == '.') {
                if (postfix[i] == '.') {
                    decimalFlag = 1;
                    i++;
                    continue;
                }
                if (!decimalFlag)
                    num = num * 10 + (postfix[i] - '0');
                else {
                    decimal = decimal * 10 + (postfix[i] - '0');
                }
                i++;
            }

            num = num + decimal / pow(10, (int)log10(decimal)+1);
            pushNum(&s, num);
        }

        else if (postfix[i] == ' ')
            continue;

        else {
            double b = popNum(&s);
            double a = popNum(&s);

            switch (postfix[i]) {
                case '+': pushNum(&s, a + b); break;
                case '-': pushNum(&s, a - b); break;
                case '*': pushNum(&s, a * b); break;
                case '/': pushNum(&s, a / b); break;
                case '^': pushNum(&s, pow(a, b)); break;
            }
        }
    }

    return popNum(&s);
}

// -------------------- MAIN --------------------
int main() {
    char infix[100], postfix[200];

    printf("Enter expression: ");
    fgets(infix, sizeof(infix), stdin);

    infixToPostfix(infix, postfix);

    printf("Postfix: %s\n", postfix);
    printf("Result: %.4lf\n", evaluatePostfix(postfix));

    return 0;
}
