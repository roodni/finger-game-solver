#include "GraphVisualizer.hpp"

void GraphOption::set(const std::string &key, const std::string &value, eOptionType type) {
    switch (type) {
    case eOptionType::string:
        options[key] = "\"" + value + "\"";
        break;
    case eOptionType::noQuote:
        options[key] = value;
        break;
    }
}

void GraphOption::toStream(std::ostream &out) const {
    // if (options.empty()) {
    //     return;
    // }

    out << "[";
    for (auto it = options.begin(); it != options.end(); it++) {
        if (it != options.begin()) {
            out << ", ";
        }
        out << it->first << "=" << it->second;
    }
    out << "]";
}


GraphVisualizer::GraphVisualizer() {
    nodeCount = 0;
}

int GraphVisualizer::createNode() {
    nodes.push_back(GraphNode());
    return nodeCount++;
}

int GraphVisualizer::createNode(const std::string &label) {
    GraphNode newNode;
    newNode.option.set("label", label, eOptionType::string);
    nodes.push_back(newNode);
    return nodeCount++;
}

void GraphVisualizer::setTrans(int fromId, int toId) {
    nodes[fromId].trans.push_back(toId);
}

void GraphVisualizer::setNodeOption(int nodeId, const std::string &key, const std::string &value, eOptionType type) {
    nodes[nodeId].option.set(key, value, type);
}

void GraphVisualizer::setAllNodesOption(const std::string &key, const std::string &value, eOptionType type) {
    nodeOption.set(key, value, type);
}

void GraphVisualizer::toDotLang(std::ostream &out) const {
    out << "digraph {\n";

    // 各ノードのオプション出力
    out << "node ";
    nodeOption.toStream(out);
    out << "\n";

    for (int i = 0; i < nodeCount; i++) {
        out << "n_" << i << " ";
        nodes[i].option.toStream(out);
        out << "\n";

        for (int j : nodes[i].trans) {
            out << "n_" << i << " -> n_" << j << "\n";
        }
    }

    out << "}";
}