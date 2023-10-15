#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include "../src/nfa.cpp"

int main() {
  std::string alpha;
  std::cin >> alpha;
  std::string x;
  std::cin >> x;
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