#include <bits/stdc++.h>
using namespace std;

const std::string kAlph = "`abc";

using ll = long long;

std::random_device rd;
std::mt19937 gen(rd());

// returns random value from [low, high]
int random(long long low, long long high) {
  std::uniform_int_distribution<> dist(low, high);
  return dist(gen);
}

std::string CreateString(int size) {
  std::string ans = "";

  for (int i = 0; i < size; ++i) {
    ans += kAlph[random(0, kAlph.size() - 1)];
  }

  return ans;
}

int main() {
  std::mt19937 mt(time(nullptr)); 
  std::vector<std::string> regexes = {
    "ab+c.aba.*.bac.+.+*",
    "acb..bab.c.*.ab.ba.+.+*a.",
  };

  std::cout << regexes[random(0, 1)] << "\n";
  std::cout << kAlph[random(1, 3)] << "\n";
  std::cout << random(1, 5) << "\n";

  return 0;
}