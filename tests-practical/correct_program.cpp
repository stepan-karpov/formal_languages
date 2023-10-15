#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include "../src/nfa.cpp"

bool ConditionTrue(std::string& current_string, char symb, int k) {
  int cnt = 0;
  for (size_t i = 0; i < current_string.size(); ++i) {
    if (current_string[i] == symb) { ++cnt; }
  }
  return (cnt % k == 0);
}

bool BruteForce(std::string current_string, DFA& nfa, char symb, int k) {
  if (current_string.size() == nfa.nodes.size() + 5) { return false; }
  if (nfa.CanRecognize(current_string) && ConditionTrue(current_string, symb, k)) { return true; }
  if (BruteForce(current_string + 'a', nfa, symb, k)) { return true; }
  if (BruteForce(current_string + 'b', nfa, symb, k)) { return true; }
  if (BruteForce(current_string + 'c', nfa, symb, k)) { return true; }
  return false;
}

bool BruteForceCheck(DFA& nfa, char symb, int k) {
  return BruteForce("", nfa, symb, k);
}

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
  

  if (BruteForceCheck(nfa, x[0], k)) {
    std::cout << "Yes\n";
  } else {
    std::cout << "No\n";
  }
}