#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include "src/nfa.cpp"

int main() {
  std::cout << "Input regex: ";
  std::string alpha;
  std::cin >> alpha;
  std::cout << "Input char: ";
  std::string x;
  std::cin >> x;
  std::cout << "Input k: ";
  int k;
  std::cin >> k;


  DFA nfa(alpha);
  nfa.RemoveEps();
  nfa.InitializeThompsonsAlgorithm();
  // nfa.MakeComplete();
  // nfa.Minimize();
  // nfa.OutputGraph();
  
  if (nfa.ConditionTrue(x[0], k)) {
    std::cout << "Yes\n";
  } else {
    std::cout << "No\n";
  }
}