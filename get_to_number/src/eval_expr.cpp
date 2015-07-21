/*
** Simple eval_expr implementation w/o operator precedence
** by dasblinkenlight on SO: http://stackoverflow.com/a/11172637
*/

#include <iostream>
#include <cctype>

using namespace std;

bool eval_expr(const char **pe, int &lhs, bool inside = false);
bool eval_expr(const std::string& str, int &lhs, bool inside = false);

// gets the next char after skipping optional whitespace
char skip_ws(const char **pe) {
    while (**pe == ' ') ++(*pe);
    return **pe;
}

// evaluates a parenthesized expression or a number
bool eval_prim(const char **pe, int &res) {
    char c = skip_ws(pe);
    if (c == '(') {
        ++(*pe);
        if (!eval_expr(pe, res, true)) return false;
        ++(*pe);
        return true;
    }
    if (isdigit(c)) {
        res = 0;
        while (isdigit(c)) {
            res = 10*res + c - '0';
            c = *(++(*pe));
        }
        return true;
    }
    return false;
}

// evaluates a chain of + - * / operations
bool eval_expr(const char **pe, int &lhs, bool inside) {
    if (!eval_prim(pe, lhs)) return false;
    char op;
    while ((op = skip_ws(pe)) && (op == '+' || op == '-' || op == '*' || op == '/')) {
        ++(*pe);
        int rhs;
        if (!eval_prim(pe, rhs)) return false;
        switch (op) {
            case '+': lhs += rhs; break;
            case '-': lhs -= rhs; break;
            case '*': lhs *= rhs; break;
            case '/': lhs /= rhs; break;
        }
    }
    return inside ? op == ')' : !op;
}

bool eval_expr(const std::string& str, int &lhs, bool inside) {
    const char *cstr = strdup(str.c_str());
    
    bool result = eval_expr(&cstr, lhs, inside);
    // delete cstr;
    return result;
}

// wrapper API to hide an extra level of indirection
bool evaluate(const char *e, int &result) {
    return eval_expr(&e, result);
}
