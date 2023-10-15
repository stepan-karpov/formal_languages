#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

// usefull tool: https://csacademy.com/app/graph_editor/

// symbol ` means eps
const std::string kAlph = "`abc";

const bool interface_output = false;

struct NFA;
NFA operator+(const NFA& first, const NFA& second);
NFA operator*(const NFA& first, const NFA& second);
NFA operator*(const NFA& first);

// no bitmasks optimizations
// to support any number of vertices/edges
struct NFA {
  struct Node {
    std::map<char, std::set<int>> edges;
    int output_power = 0;
    bool is_terminal = 0;

    void AddEdge(int v, char symb) {
      if (edges[symb].find(v) == edges[symb].end()) {
        ++output_power;
      }
      edges[symb].insert(v);
    }

    void OutputEdges(int me) {
      for (auto edge : edges) {
        for (int to : edge.second) {
          std::cout << me << " " << to << " " << edge.first << "\n";
        }
      }
    }

    Node() {
      for (size_t i = 0; i < kAlph.size(); ++i) {
        edges[kAlph[i]] = {};
      }
    }

    Node(const Node& old_node,
         const std::map<int, int>& compressed_coords) {
      is_terminal = old_node.is_terminal;
      for (auto edge : old_node.edges) {
        edges[edge.first] = {};
        for (int to : edge.second) {
          if (compressed_coords.contains(to)) {
            edges[edge.first].insert(compressed_coords.at(to));
            ++output_power;
          }
        }
      }
    }

    Node (const Node& other) = default;
    ~Node () = default;
  };

  struct ThompsonNode : public NFA::Node {
    std::set<int> represented_subset;
    ThompsonNode(std::set<int>&& subset)
      : represented_subset(std::move(subset)) {}
    ThompsonNode() = default;
  };

  std::vector<Node> nodes;

  void MarkupDFS(int vertex, std::vector<bool>& used) {
    if (used[vertex]) { return; }
    used[vertex] = true;
    for (auto edge : nodes[vertex].edges) {
      if (edge.second.empty()) { continue; }
      for (int to : edge.second) {
        MarkupDFS(to, used);
      }
    }
  }

  void EpsDFS(int current_vertex, int parent,
              std::set<int>& used) {
    if (nodes[current_vertex].is_terminal) {
      nodes[parent].is_terminal = true;
    }
    if (used.find(current_vertex) != used.end()) {
      return;
    }
    used.insert(current_vertex);
    Node& current_node = nodes[current_vertex];
    for (auto edge : current_node.edges) {
      if (edge.second.empty()) { continue; } // debug usefull
      if (edge.first == '`') {
        for (int to : edge.second) {
          EpsDFS(to, parent, used);
        }
        edge.second.clear();
      } else {
        for (int to : edge.second) {
          nodes[parent].edges[edge.first].insert(to);
        }
      }
    }
    if (current_vertex == parent) {
      current_node.edges['`'].clear();
    }
  }

  void ClearUseless() {
    std::vector<bool> used(nodes.size(), false);
    MarkupDFS(0, used);

    std::map<int, int> compressed_coords;

    int pointer = 0;
    for (int i = 0; i < nodes.size(); ++i) {
      bool skip =
        !used[i] || (!nodes[i].is_terminal && nodes[i].output_power == 0);
      if (!skip) {
        compressed_coords[i] = pointer++;
      }
    }

    std::vector<Node> new_nodes;
    new_nodes.reserve(nodes.size());
    for (size_t i = 0; i < nodes.size(); ++i) {
      bool skip =
        !used[i] || (!nodes[i].is_terminal && nodes[i].output_power == 0);
      if (!skip) {
        new_nodes.push_back(Node(nodes[i], compressed_coords));
      }
    }
    nodes = new_nodes;
  }

  void ReadNFA() {
    if (interface_output) {
      std::cout << "\n";
      std::cout << "Input number of edges: ";
    }
    int edges; std::cin >> edges;
    if (interface_output) {
      std::cout << "Input NFA's edges in the following format:\n";
      std::cout << "$number1$ $number2$ $letter$, where\n";
      std::cout << "$number1$ - number of an outgoing vertex (0-indexation),\n";
      std::cout << "$number2$ - number of an ingoing vertex (0-indexation),\n";
      std::cout << "$letter$ - letter from alphabet (kAlph in code)\n\n";
    }

    for (int i = 0; i < edges; ++i) {
      int u, v; char word;
      std::cin >> u >> v >> word;
      nodes[u].AddEdge(v, word);
    }
    if (interface_output) {
      std::cout << "Input number of terminal vertices: ";
    }
    int terminal; std::cin >> terminal;
    for (int i = 0; i < terminal; ++i) {
      int u; std::cin >> u;
      nodes[u].is_terminal = true;
    }
    ClearUseless(); // legacy code
  }

  void RemoveEps() {
    for (size_t i = 0; i < nodes.size(); ++i) {
      std::set<int> used;
      EpsDFS(i, i, used);
    }
  }

  void OutputGraph() {
    ClearUseless();
    std::cout << "Number of vertices: " << nodes.size() << "\n";
    std::cout << "Edges: " << "\n";
    std::set<int> terminals;
    for (int i = 0; i < nodes.size(); ++i ){
      nodes[i].OutputEdges(i);
      if (nodes[i].is_terminal) {
        terminals.insert(i);
      }
    }

    std::cout << "\nNumber of terminal vertices: ";
    std::cout << terminals.size() << "\n";

    for (auto term_vertex : terminals) {
      std::cout << term_vertex << " ";
    }
    std::cout << "\n";
  }

  bool DfsEps(int current_vertex, std::set<int>& used) {
    if (used.find(current_vertex) != used.end()) { return false; }
    used.insert(current_vertex);
    if (nodes[current_vertex].is_terminal) { return true; }
    for (int to : nodes[current_vertex].edges['`']) {
      if (DfsEps(to, used)) { return true; }
    }
    return false;
  }

  bool DFSRecognize(int current_vertex, int pointer, std::string& word,
                    std::set<std::pair<int, int>>& used) {
    while (pointer < word.size() && word[pointer] == '`') {
      ++pointer;
    }
    if (used.find({pointer, current_vertex}) != used.end()) {
      return false;
    }
    used.insert({pointer, current_vertex});
    if (pointer == word.size()) {
      std::set<int> used;
      return DfsEps(current_vertex, used);
    }
    for (int to : nodes[current_vertex].edges[word[pointer]]) {
      if (DFSRecognize(to, pointer + 1, word, used)) { return true; }
    }
    for (int to : nodes[current_vertex].edges['`']) {
      if (DFSRecognize(to, pointer, word, used)) { return true; }
    }
    return false;
  }

  bool CanRecognize(std::string& word) {
    if (static_cast<int>(nodes.size()) == 0) {
      return false;
    }

    std::set<std::pair<int, int>> used;
    return DFSRecognize(0, 0, word, used);
  }

  NFA(char symb) {
    assert(symb != '`' && "cringe in NFA from symbol constructor");
      nodes.push_back(Node());
    if (symb != '1') { // symb in kAlph \ '`'
      nodes.push_back(Node());
      nodes[0].AddEdge(1, symb);
      nodes[1].is_terminal = true;
    } else {
      nodes[0].is_terminal = true;
    }
  }

  // builds NFA by regex in reverse Polish notation
  NFA(std::string regex) {
    std::stack<NFA> stack;

    for (size_t i = 0; i < regex.size(); ++i) {
      if (regex[i] == '.') {
        NFA second = stack.top(); stack.pop();
        NFA first = stack.top(); stack.pop();
        stack.push(first * second);
      } else if (regex[i] == '*') {
        NFA last = stack.top(); stack.pop();
        stack.push(*last);
      } else if (regex[i] == '+') {
        NFA second = stack.top(); stack.pop();
        NFA first = stack.top(); stack.pop();
        stack.push(first + second);
      } else { // regex[i] == 'a' | 'b' | 'c' | '1'
        stack.push(NFA(regex[i]));
      }
    }
    *this = stack.top();
  }

  NFA(std::vector<ThompsonNode>& new_nodes) {
    nodes.reserve(new_nodes.size());
    for (size_t i = 0; i < new_nodes.size(); ++i) {
      nodes.push_back(new_nodes[i]);
    }
  }
  NFA(int size) { nodes.assign(size, Node()); }
  NFA(const NFA& other) = default;
  // NFA(NFA&& other) = default;
  ~NFA() = default;
};

struct DFA : public NFA {
  void InitializeThompsonsAlgorithm() {
    if (static_cast<int>(nodes.size()) == 0) {
      return;
    }
    std::vector<ThompsonNode> new_nodes;
    new_nodes.push_back(ThompsonNode());
    
    new_nodes[0].represented_subset = {0};
    new_nodes[0].edges = {};
    new_nodes[0].is_terminal = nodes[0].is_terminal;
    new_nodes[0].output_power = nodes[0].output_power;

    std::queue<int> queue;

    queue.push(0);
    std::map<std::set<int>, int> used;
    used[{0}] = 0;

    while (!queue.empty()) {
      int current_number = queue.front();
      queue.pop();

      for (char current_char : kAlph) {
        std::set<int> togo;
        for (int from : new_nodes[current_number].represented_subset) {
          for (int to : nodes[from].edges[current_char]) {
            togo.insert(to); // TODO - write if from == '`'
          }
        }
        if (togo.empty()) { continue; }
        ++new_nodes[current_number].output_power;
        int number;
        if (!used.contains(togo)) {
          used.insert({togo, new_nodes.size()});
          new_nodes.push_back(ThompsonNode(std::move(togo)));
          number = new_nodes.size() - 1;
          queue.push(number);
        } else {
          number = used[togo];
        }
        new_nodes[current_number].edges[current_char].insert(number);
      }
    }

    for (size_t i = 0; i < new_nodes.size(); ++i) {
      ThompsonNode& current_node = new_nodes[i];
      for (auto vertex : current_node.represented_subset) {
        current_node.is_terminal |= nodes[vertex].is_terminal;
      }
    }

    DFA rebuild(new_nodes);
    *this = rebuild;
  }

  void swap(DFA& other) {
    std::swap(other.nodes, nodes);
  }

  bool isComplete() {
    for (auto& node : nodes) {
      for (char current_char : kAlph) {
        if (current_char == '`') { continue; }
        if (node.edges[current_char].empty()) { return false; }
      }
    }
    return true;
  }

  void MakeComplete() {
    if (isComplete()) { return; }
    nodes.push_back(Node());

    for (auto& node : nodes) {
      for (auto current_char : kAlph) {
        if (current_char == '`') { continue; }
        if (node.edges[current_char].empty()) {
          node.AddEdge(nodes.size() - 1, current_char);
        }
      }
    }
  }

  void FindSimularvertices(
    std::set<std::pair<int, int>>& storage,
    std::vector<std::map<char, std::set<int>>>& reveresed_edges) {
    std::queue<std::pair<int, int>> pairs;

    for (size_t i = 0; i < nodes.size(); ++i) {
      for (size_t j = i + 1; j < nodes.size(); ++j) {
        Node& node1 = nodes[i], node2 = nodes[j];
        if (node1.is_terminal != node2.is_terminal) {
          pairs.push({i, j});
          storage.insert({i, j});
        }
      }
    }

    for (size_t i = 0; i < nodes.size(); ++i) {
      Node& node = nodes[i];
      for (auto current_char : kAlph) {
        for (int to : node.edges[current_char]) {
          if (!reveresed_edges[to].contains(current_char)) {
            reveresed_edges[to][current_char] = {};
          }
          reveresed_edges[to][current_char].insert(i);
        }
      }
    }

    while (!pairs.empty()) {
      int x = pairs.front().first, y = pairs.front().second;
      pairs.pop();
      for (auto current_char : kAlph) {

        for (int a : reveresed_edges[x][current_char]) {
          for (int b : reveresed_edges[y][current_char]) {
            std::pair<int, int> temp = {std::min(a, b), std::max(a, b)};
            if (storage.find(temp) == storage.end()) {
              storage.insert(temp);
              pairs.push(temp);
            }
          }
        }
      }
    }
  }

  void Minimize() {
    if (nodes.size() == 0) { return; }
    if (!isComplete()) {
      std::cout << "ERROR ERROR ERROR ERROR ERROR ERROR Not complete!\n";
      return;
    }

    int last_size = 100'500;

    while (last_size > nodes.size()) {
      last_size = nodes.size();
      std::vector<std::map<char, std::set<int>>> reveresed_edges(nodes.size());
      std::set<std::pair<int, int>> storage;
      FindSimularvertices(storage, reveresed_edges);

    
      std::vector<bool> deleted(nodes.size());
      for (int first_vertex = 0; first_vertex < nodes.size(); ++first_vertex) {
        for (int second_vertex = first_vertex + 1; second_vertex < nodes.size(); ++second_vertex) {
          if (storage.find({first_vertex, second_vertex}) != storage.end()) { continue; }
          if (!deleted[first_vertex] && !deleted[second_vertex]) {
            for (auto current_char : kAlph) {
              for (int parent : reveresed_edges[second_vertex][current_char]) {
                reveresed_edges[first_vertex][current_char].insert(parent);
                nodes[parent].edges[current_char].erase(second_vertex);
                nodes[parent].AddEdge(first_vertex, current_char);
              }
            }
          }
        }
      }

      ClearUseless();
    }
  }

  bool DfsCheck(int current_vertex, char symb, int cnt,
                int k, std::set<std::pair<int, int>>& used, int depth) {
    if (used.find({current_vertex, cnt}) != used.end()) { return false; }
    if (depth >= nodes.size() * 10) { return false; }
    if (cnt % k == 0 && nodes[current_vertex].is_terminal) { return true; }
    used.insert({current_vertex, cnt});
    for (auto p : nodes[current_vertex].edges) {
      for (auto to : p.second) {
        if (DfsCheck(to, symb, cnt + int(p.first == symb), k, used, depth + 1)) {
          return true;
        }
      }
    }
    return false;
  }

  bool ConditionTrue(char symb, int k) {
    std::set<std::pair<int, int>> used;
    return DfsCheck(0, symb, 0, k, used, 0);
  }

  DFA(std::string regex) : NFA(regex) {}
  DFA(std::vector<ThompsonNode>& nodes) : NFA(nodes) {}
  DFA(int size) : NFA(size) {}
  ~DFA() = default;
  DFA(const DFA& other) = default;
  DFA& operator=(const DFA& other) {
    DFA copy = other;
    swap(copy);
    return *this;
  }
  DFA(DFA&& other) = default;
};

NFA operator+(const NFA& first, const NFA& second) {
  NFA answer(int(first.nodes.size() + second.nodes.size()));


  for (size_t i = 0; i < first.nodes.size(); ++i) {
    answer.nodes[i] = first.nodes[i];
  }
  answer.nodes[0].AddEdge(first.nodes.size(), '`');
  
  for (int i = 0; i < second.nodes.size(); ++i) {
    answer.nodes[i + first.nodes.size()] = second.nodes[i];
    for (auto& p : answer.nodes[i + first.nodes.size()].edges) {
      std::set<int> new_indexes;
      for (auto el : p.second) {
        new_indexes.insert(el + first.nodes.size());
      }
      p.second = new_indexes;
    }
  }

  return answer;
}

NFA operator*(const NFA& first, const NFA& second) {
  NFA answer(int(first.nodes.size() + second.nodes.size()));

  for (size_t i = 0; i < first.nodes.size(); ++i) {
    answer.nodes[i] = first.nodes[i];
    if (answer.nodes[i].is_terminal) {
      answer.nodes[i].is_terminal = false;
      answer.nodes[i].AddEdge(first.nodes.size(), '`');
    }
  }
  
  for (int i = 0; i < second.nodes.size(); ++i) {
    answer.nodes[i + first.nodes.size()] = second.nodes[i];
    for (auto& p : answer.nodes[i + first.nodes.size()].edges) {
      std::set<int> new_indexes;
      for (auto el : p.second) {
        new_indexes.insert(el + first.nodes.size());
      }
      p.second = new_indexes;
    }
  }

  return answer;
}

NFA operator*(const NFA& first) {
  NFA answer(int(first.nodes.size()));

  for (size_t i = 0; i < first.nodes.size(); ++i) {
    answer.nodes[i] = first.nodes[i];
  }

  for (size_t i = 0; i < first.nodes.size(); ++i) {
    if (answer.nodes[i].is_terminal) {
      answer.nodes[i].AddEdge(0, '`');
      answer.nodes[0].AddEdge(i, '`');
    }
  }

  return answer;
}
