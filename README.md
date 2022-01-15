# TheCalculator

A math expression evaluator written in C++17, with both a command-line REPL and a GTK4 graphical interface.

Parses and evaluates arithmetic expressions using a classic pipeline: **tokenizer** (lexer) → **parser** (shunting-yard, infix to postfix) → **evaluator** (stack-based RPN). Supports variables, parentheses, operator precedence, and exponentiation with right-associativity.

## Building

Requires `cmake` and a C++17 compiler (`g++` or `clang++`). For the GUI, you also need `libgtk-4-dev`.

```bash
cmake -B build
cmake --build build
```

This produces:
- `build/TheCalculator` — the CLI
- `build/TheCalculatorGUI` — the GUI (only if GTK4 is found)

## Usage

### CLI

**Interactive mode** — just run it:

```
$ ./build/TheCalculator
> 2 + 3 * 4
= 14
> (2 + 3) * 4
= 20
> 2^3^2
= 512
> let radius = 6.5
> 3.14 * radius ^ 2
= 132.665
```

**Single expression** — pass it as an argument:

```
$ ./build/TheCalculator "2 + 3 * 4"
14
```

### GUI

```
$ ./build/TheCalculatorGUI
```

A GTK4 window with an expression input field, result display, scrollable history, and a variables panel. Uses the Catppuccin Mocha dark theme.

## What it can do

### Math

| Operator | What it does | Precedence | Associativity |
|----------|-------------|------------|---------------|
| `+` `-`  | Add, subtract | 1 | Left |
| `*` `/`  | Multiply, divide | 2 | Left |
| `^`      | Power | 3 | Right |

- Parentheses `()` and brackets `[]` both work
- Unary minus is handled: `-5 + 3`, `2 * -3`, `(-4)^2`
- Division by zero throws an error

### Variables

- `let x = 5` — assign a variable
- `del x` — delete it
- `vars` — list all variables
- `clear` — delete all variables
- `ans` — always holds the last result

### File I/O

- `load expressions.txt` — evaluate each line from a file
- `save results.txt` — append `ans` to a file
- `save results.txt 2+3` — evaluate and append the result

### Other commands

- `precision 4` — set decimal places (0–20)
- `help` — show all commands
- `exit` / `quit` — leave the REPL

## How it works

The evaluation pipeline has three stages, each a separate class:

```
"2 + 3 * 4"  →  Tokenizer  →  Parser  →  Evaluator  →  14.0
                 (lexing)     (shunting   (stack-based
                               yard)      RPN eval)
```

**Tokenizer** scans the input string character by character, producing a list of tokens (numbers, operators, parentheses, variable names). It detects unary minus by context — if a `-` appears at the start, after an operator, or after `(`, it inserts `0` before it to normalize it as subtraction.

**Parser** implements the [shunting-yard algorithm](https://en.wikipedia.org/wiki/Shunting-yard_algorithm) to reorder infix tokens into postfix (reverse Polish notation). Operator precedence and associativity are defined in a data map, not hard-coded in the logic.

**Evaluator** walks the postfix token list with a stack. Numbers get pushed; operators pop their operands, compute, and push the result. Operations are registered as `std::function` lambdas, making it straightforward to add new ones.

**Calculator** is the facade that wires these three together and plugs in variable resolution from a **VariableStore** (a `map<string, double>`).

The core pipeline is compiled into a static library (`calc_core`) that both the CLI and GUI link against.

## Project structure

```
src/
├── main.cpp                    # CLI entry point
├── pipeline/
│   ├── Token.h                 # Token type definitions
│   ├── Tokenizer.cpp/.h        # Lexer
│   ├── Parser.cpp/.h           # Shunting-yard (infix → postfix)
│   └── Evaluator.cpp/.h        # Stack-based RPN evaluator
├── engine/
│   ├── Calculator.cpp/.h       # Pipeline facade
│   └── VariableStore.cpp/.h    # Named variable storage
├── cli/
│   ├── CLI.cpp/.h              # REPL loop + single-expression mode
│   ├── CommandRouter.cpp/.h    # Command parsing and dispatch
│   └── FileIO.cpp/.h           # Load/save operations
└── gui/
    ├── main_gui.cpp            # GTK4 application entry point
    └── CalculatorWindow.cpp/.h # Window, input, history, variables
```

The legacy Visual Studio project (`TheCalculator/`) is kept for reference but is not the active codebase.

## License

No license specified.
