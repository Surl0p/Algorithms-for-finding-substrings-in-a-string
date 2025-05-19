#ifndef AHO_CORASICK_H
#define AHO_CORASICK_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>

struct Node {
    std::map<char, Node*> children;
    Node* failureLink;
    std::set<std::string> outputs;

    Node() : failureLink(nullptr) {}
};


class AhoCorasickMachine {
public:
    explicit AhoCorasickMachine(const std::vector<std::string>& patterns);


    std::vector<std::pair<std::string, size_t>> search(const std::string& text);


private:
    Node* root;
    std::vector<std::unique_ptr<Node>> nodes;
    Node* getOrCreateChild(Node* parent, char key);
    static void addOutputToNode(Node* node, const std::string& pattern);
    static void copyOutputsFrom(Node* source, Node* destination);
    Node* getTransition(Node* state, char c);
};

std::string::size_type ahoCorasickSearch(const std::vector<std::string>& patterns, const std::string& text);


#endif // AHO_CORASICK_H