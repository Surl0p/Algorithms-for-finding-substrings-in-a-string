#include "aho_corasick.h"
#include <queue>
#include <iostream>

Node* AhoCorasickMachine::getOrCreateChild(Node* parent, char key) {
    if (parent->children.find(key) == parent->children.end()) {
        nodes.push_back(std::make_unique<Node>());
        Node* newNode = nodes.back().get();
        parent->children[key] = newNode;
        return newNode;
    }
    return parent->children.at(key);
}

void AhoCorasickMachine::addOutputToNode(Node* node, const std::string& pattern) {
    node->outputs.insert(pattern);
}

void AhoCorasickMachine::copyOutputsFrom(Node* source, Node* destination) {
    destination->outputs.insert(source->outputs.begin(), source->outputs.end());
}


AhoCorasickMachine::AhoCorasickMachine(const std::vector<std::string>& patterns) {
    nodes.push_back(std::make_unique<Node>());
    root = nodes.back().get();

    if (patterns.empty()) {
        root->failureLink = root;
        return;
    }

    for (const std::string& pattern : patterns) {
        Node* currNode = root;
        for (char key : pattern) {
            currNode = getOrCreateChild(currNode, key);
        }
        addOutputToNode(currNode, pattern);
    }

    std::queue<Node*> q;

    root->failureLink = root;

    for (auto const& [key, child] : root->children) {
        child->failureLink = root;
        q.push(child);
    }

    while (!q.empty()) {
        Node* currNode = q.front();
        q.pop();

        for (auto const& [key, child] : currNode->children) {
            q.push(child);


            Node* linkNode = currNode->failureLink;

            while (linkNode != root && linkNode->children.find(key) == linkNode->children.end()) {
                linkNode = linkNode->failureLink;
            }
           if (linkNode->children.count(key)) {
                child->failureLink = linkNode->children.at(key);
            } else {
                child->failureLink = root;
            }

            copyOutputsFrom(child->failureLink, child);
        }
    }
}

Node* AhoCorasickMachine::getTransition(Node* state, char c) {
    while (state != root && state->children.find(c) == state->children.end()) {
        state = state->failureLink;
    }

    if (state->children.count(c)) {
        return state->children.at(c);
    } else {
        return root;
    }
}

std::vector<std::pair<std::string, size_t>> AhoCorasickMachine::search(const std::string& text) {
    std::vector<std::pair<std::string, size_t>> results;
    Node* currentState = root;

    for (size_t i = 0; i < text.length(); ++i) {
        char c = text[i];
        currentState = getTransition(currentState, c);

        for (const std::string& pattern : currentState->outputs) {

            results.emplace_back(pattern, i - pattern.length() + 1);
        }
    }

    return results;
}


std::string::size_type ahoCorasickSearch(const std::vector<std::string>& patterns, const std::string& text) {
    if (patterns.empty()) {
        std::cout << "Warning: Pattern set is empty." << std::endl;
        return 0;
    }

    AhoCorasickMachine machine(patterns);
    auto results = machine.search(text);

    return results.size();
}
