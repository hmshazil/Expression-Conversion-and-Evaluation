# Infix Expression Converter and Evaluator (DSA Assignment 1)

This C++ program processes arithmetic expressions written in **Infix notation**, validates their syntax, transforms them into **Postfix notation** (Reverse Polish Notation) using a Stack data structure, and then evaluates the expression.

## Features Implemented
- **Tokenization:** Breaks down the input string into logical tokens (Variables, Integers/Decimals, Operators, and Brackets).
- **Comprehensive Syntax Validation:** - Checks for unmatched, mismatched, or misplaced grouping symbols `()`, `[]`, `{}`.
  - Detects consecutive operators, illegal starting/ending operators, and missing operators.
- **Infix to Postfix Conversion:** Implements the Shunting-Yard algorithm using a stack, respecting standard precedence: `%` > `*`, `/` > `+`, `-`.
- **Variable Acquisition:** Scans the expression for unique identifiers/variables and prompts the user to input their numeric values dynamically via `stderr`.
- **Expression Evaluation:** Computes the final numeric result using a secondary evaluation stack.
- **Robust Error Handling:** Employs precise system exit codes for automated testing environments (`1` for syntax errors, `2` for runtime errors like Division or Modulus by Zero).

## Error Codes
- `0`: Execution completed successfully.
- `1`: Syntax or validation error in the expression.
- `2`: Runtime error during evaluation (e.g., Division/Modulus by zero, or float modulus type mismatch).

## How to Compile
Compile the program using any standard C++ compiler supporting C++11 or higher (like GCC):
```bash
g++ -std=c++11 ass1_BCSF24A009.cpp -o expression_evaluator

./expression_evaluator
