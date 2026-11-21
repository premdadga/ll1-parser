#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> firstSet, followSet;
map<char, map<char, string>> parsingTable;

set<char> terminals, nonTerminals;

const char EPS = '#';
const char DOLLAR = '$';

// Find FIRST of a symbol
set<char> findFirst(char c) {
    if (!isupper(c))
        return {c};

    if (!firstSet[c].empty())
        return firstSet[c];

    for (string p : grammar[c]) {
        bool flag = true;

        for (char x : p) {
            set<char> temp = findFirst(x);

            for (char ch : temp) {
                if (ch != EPS)
                    firstSet[c].insert(ch);
            }

            if (temp.find(EPS) == temp.end()) {
                flag = false;
                break;
            }
        }

        if (flag)
            firstSet[c].insert(EPS);
    }

    return firstSet[c];
}

// Find FIRST of a string
set<char> findFirstString(string s) {
    set<char> ans;
    bool flag = true;

    for (char c : s) {
        set<char> temp = findFirst(c);

        for (char x : temp) {
            if (x != EPS)
                ans.insert(x);
        }

        if (temp.find(EPS) == temp.end()) {
            flag = false;
            break;
        }
    }

    if (flag)
        ans.insert(EPS);

    return ans;
}

// Calculate FOLLOW sets
void findFollow(char start) {
    followSet[start].insert(DOLLAR);

    bool changed = true;

    while (changed) {
        changed = false;

        for (auto rule : grammar) {
            char lhs = rule.first;

            for (string p : rule.second) {
                for (int i = 0; i < p.size(); i++) {
                    char ch = p[i];

                    if (!isupper(ch))
                        continue;

                    string beta = p.substr(i + 1);
                    set<char> temp = findFirstString(beta);

                    for (char x : temp) {
                        if (x != EPS) {
                            if (followSet[ch].insert(x).second)
                                changed = true;
                        }
                    }

                    if (beta.empty() || temp.count(EPS)) {
                        for (char x : followSet[lhs]) {
                            if (followSet[ch].insert(x).second)
                                changed = true;
                        }
                    }
                }
            }
        }
    }
}

// Print FIRST and FOLLOW
void printSets(string name, map<char, set<char>> &s) {
    cout << "\n" << name << " sets:\n";

    for (char c : nonTerminals) {
        cout << name << "(" << c << ") = { ";

        for (char x : s[c])
            cout << x << " ";

        cout << "}\n";
    }
}

// Create parsing table
bool makeTable() {
    bool flag = true;

    for (auto rule : grammar) {
        char lhs = rule.first;

        for (string p : rule.second) {
            set<char> temp = findFirstString(p);

            for (char x : temp) {
                if (x == EPS)
                    continue;

                if (parsingTable[lhs].count(x)) {
                    cout << "Conflict at [" << lhs << ", " << x << "]\n";
                    flag = false;
                }

                parsingTable[lhs][x] = p;
            }

            if (temp.count(EPS)) {
                for (char x : followSet[lhs]) {
                    if (parsingTable[lhs].count(x)) {
                        cout << "Conflict at [" << lhs << ", " << x << "]\n";
                        flag = false;
                    }

                    parsingTable[lhs][x] = p;
                }
            }
        }
    }

    return flag;
}

// Parse the input string
void parseString(string input, char start) {
    input += DOLLAR;

    stack<char> st;
    st.push(DOLLAR);
    st.push(start);

    int i = 0;

    cout << "\nStack\t\tInput\t\tAction\n";

    while (!st.empty()) {
        char top = st.top();
        char ch = input[i];

        if (top == ch) {
            cout << top << "\t\t"
                 << input.substr(i) << "\t\tMatch\n";

            st.pop();
            i++;

            if (top == DOLLAR) {
                cout << "\nString Accepted\n";
                return;
            }
        }
        else if (!isupper(top)) {
            cout << "\nString Rejected\n";
            return;
        }
        else {
            if (!parsingTable[top].count(ch)) {
                cout << "\nNo production found for ["
                     << top << ", " << ch << "]\n";

                cout << "String Rejected\n";
                return;
            }

            string p = parsingTable[top][ch];

            cout << top << "\t\t"
                 << input.substr(i) << "\t\t"
                 << top << " -> " << p << "\n";

            st.pop();

            if (p != "#") {
                for (int j = p.size() - 1; j >= 0; j--)
                    st.push(p[j]);
            }
        }
    }

    cout << "\nString Rejected\n";
}

int main() {
    int n;
    char start;

    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter start symbol: ";
    cin >> start;

    cout << "Enter productions (Example: E=TR):\n";

    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        char lhs = s[0];
        string rhs = s.substr(2);

        nonTerminals.insert(lhs);

        stringstream ss(rhs);
        string p;

        while (getline(ss, p, '|')) {
            grammar[lhs].push_back(p);
        }
    }

    // Identify terminals
    for (auto rule : grammar) {
        for (string p : rule.second) {
            for (char c : p) {
                if (!isupper(c) && c != EPS)
                    terminals.insert(c);
            }
        }
    }

    terminals.insert(DOLLAR);

    // Calculate FIRST and FOLLOW
    for (char c : nonTerminals)
        findFirst(c);

    findFollow(start);

    printSets("FIRST", firstSet);
    printSets("FOLLOW", followSet);

    // Display parsing table
    cout << "\nLL(1) Parsing Table:\n";

    if (!makeTable()) {
        cout << "\nGrammar is not LL(1).\n";
        return 0;
    }

    for (char nt : nonTerminals) {
        for (char t : terminals) {
            if (parsingTable[nt].count(t)) {
                cout << "M[" << nt << ", " << t << "] = "
                     << nt << " -> " << parsingTable[nt][t] << "\n";
            }
        }
    }

    string input;

    cout << "\nEnter input string: ";
    cin >> input;

    parseString(input, start);

    return 0;
}
