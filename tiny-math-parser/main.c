#include <stdio.h>
#include <ctype.h>

int parse_int(const char **expr) {
    int result = 0;
    while (isdigit(**expr)) {
        result = result * 10 + (**expr - '0');
        (*expr)++;
    }
    return result;
}

int parse_expr(const char **expr) {
    int result = parse_int(expr);
    while (**expr) {
        char op = **expr;
        if (op != '+' && op != '-') {
            break;
        }
        (*expr)++; // Move past the operator

        int value = parse_int(expr);
        if (op == '+') {
            result += value;
        } else if (op == '-') {
            result -= value;
        } else {
            continue;;
        }
    }

    return result;
}

int main() {
    const char *expression = "20+3-(1+1)";
    const char *expr_ptr = expression;

    int result = parse_expr(&expr_ptr);
    printf("Result of '%s' is %d\n", expression, result);
    return 0;
}