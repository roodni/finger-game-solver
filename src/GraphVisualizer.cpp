#include "GraphVisualizer.hpp"
#include <cassert>

void GraphOption::set(const std::string &key, const std::string &value, OptionType type) {
    switch (type) {
    case OptionType::string:
        options[key] = "\"" + value + "\"";
        break;
    case OptionType::noQuote:
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


GraphEdge::GraphEdge(int fromNodeId, int toNodeId) {
    from = fromNodeId;
    to = toNodeId;
}

void GraphEdge::setOption(const std::string &key, const std::string &value, OptionType type) {
    option.set(key, value, type);
}

void GraphEdge::toStream(std::ostream &out) const {
    out << "n" << from << " -> n" << to << " ";
    option.toStream(out);
}


GraphNode::GraphNode(int nodeId) {
    id = nodeId;
}

void GraphNode::setOption(const std::string &key, const std::string &value, OptionType type) {
    option.set(key, value, type);
}

void GraphNode::toStream(std::ostream &out) const {
    out << "n" << id << " ";
    option.toStream(out);
}


GraphVisualizer::GraphVisualizer() {
    nodeCount = 0;
    edgeCount = 0;
}

int GraphVisualizer::createNode() {
    nodes.push_back(GraphNode(nodeCount));
    return nodeCount++;
}

int GraphVisualizer::createNode(const std::string &label) {
    nodes.push_back(GraphNode(nodeCount));
    nodes.back().setOption("label", label, OptionType::string);
    return nodeCount++;
}

void GraphVisualizer::setNodeOption(int nodeId, const std::string &key, const std::string &value, OptionType type) {
    nodes[nodeId].setOption(key, value, type);
}

void GraphVisualizer::setAllNodesOption(const std::string &key, const std::string &value, OptionType type) {
    nodeOption.set(key, value, type);
}

int GraphVisualizer::createEdge(int fromId, int toId) {
    assert(0 <= fromId && fromId < nodeCount);
    assert(0 <= toId && toId < nodeCount);
    edges.push_back(GraphEdge(fromId, toId));
    return edgeCount++;
}

void GraphVisualizer::setEdgeOption(int edgeId, const std::string &key, const std::string &value, OptionType type) {
    edges[edgeId].setOption(key, value, type);
}

void GraphVisualizer::toDotLang(std::ostream &out) const {
    out << "digraph {\n";

    // 各ノードのオプション出力
    out << "node ";
    nodeOption.toStream(out);
    out << "\n";

    for (const GraphNode &node : nodes) {
        node.toStream(out);
        out << "\n";
    }
    for (const GraphEdge &edge : edges) {
        edge.toStream(out);
        out << "\n";
    }

    out << "}";
}