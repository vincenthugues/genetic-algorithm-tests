#include <iostream>
#include <cctype>

#ifndef EVAL_EXPR_HPP
# define EVAL_EXPR_HPP

bool eval_expr(const char **pe, int &lhs, bool inside = false);
bool eval_expr(const std::string& str, int &lhs, bool inside = false);

#endif