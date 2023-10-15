#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include "../src/nfa.cpp"

int main() {
  if (interface_output) {
    std::cout << "!the initial automaton vertex should have number 0\n";
    std::cout << "Input number of vertices: ";
  }
  int vertices; std::cin >> vertices;
  DFA nfa(vertices);
  nfa.ReadNFA();
  nfa.RemoveEps();
  nfa.InitializeThompsonsAlgorithm();

  nfa.MakeComplete();
  nfa.Minimize();

  int testsize; std::cin >> testsize;

  for (int i = 0; i < testsize; ++i) {
    std::string text; std::cin >> text;
    if (nfa.CanRecognize(text)) {
      std::cout << "YES\n";
    } else {
      std::cout << "NO\n";
    }
  }
}