# LL(1) Parser in C++

A simple LL(1) predictive parser implemented in C++.
## How it works

1. **Grammar Input:** Takes the grammar and start symbol from the user.
2. **FIRST:** Calculates the FIRST set for each non-terminal.
3. **FOLLOW:** Calculates the FOLLOW set for each non-terminal.
4. **Parsing Table:** Builds the LL(1) parsing table using FIRST and FOLLOW.
5. **Conflict Checking:** Checks whether multiple productions map to the same table entry. If they do, the grammar is not LL(1).
6. **Parsing:** Uses a stack and the parsing table to check the input string.
7. **Result:** Prints whether the input string is accepted or rejected.

## Input Format

Example grammar:

```text
E=TR
R=+TR|#
T=FY
Y=*FY|#
F=(E)|i
```

- Uppercase letters represent non-terminals.
- Lowercase letters and symbols represent terminals.
- `#` represents epsilon (empty production).
- `|` separates alternative productions.
- `$` represents the end of input.

## How to Run

Compile:

```bash
g++ -std=c++17 -O2 main.cpp -o parser
```

Run:

```bash
./parser
```
## Example

For the grammar above, try:
```text
Input: i+i*i
```

Output:
```text
String ACCEPTED
```
