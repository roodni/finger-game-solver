#pragma once
#include <ostream>
#include <vector>
#include <map>

enum class OptionType {
    string,
    noQuote // ダブルクォーテーションを出力しない文字列
};

namespace {
    // ノードに付与するオプション
    class GraphOption {
    public:
        void set(const std::string &key, const std::string &value, OptionType type);   // オプション設定
        void toStream(std::ostream &out) const;    // ostreamに設定を吐き出す
    private:
        std::map<std::string, std::string> options;
    };

    // 辺
    class GraphEdge {
    public:
        GraphEdge(int fromNodeId, int toNodeId);
        void setOption(const std::string &key, const std::string &value, OptionType type);
        void toStream(std::ostream &out) const;
    private:
        int from, to;
        GraphOption option;
    };


    // ノード
    class GraphNode {
    public:
        GraphNode(int nodeId);
        void setOption(const std::string &key, const std::string &value, OptionType type);
        void toStream(std::ostream &out) const;
    private:
        int id;
        GraphOption option;
    };
}


// 有向グラフをdot言語として吐く
class GraphVisualizer {
public:
    GraphVisualizer();
    int createNode();   // ノードを作成し、ノードidを返す
    int createNode(const std::string &label);
    void setNodeOption(int nodeId, const std::string &key, const std::string &value, OptionType type = OptionType::string); // 特定のノードにオプションを設定する
    void setAllNodesOption(const std::string &key, const std::string &value, OptionType type = OptionType::string); // 全てのノードにオプションを設定する
    int createEdge(int fromId, int toId);    // 枝を設定する
    void setEdgeOption(int edgeId, const std::string &key, const std::string &value, OptionType type = OptionType::string);
    void toDotLang(std::ostream &out) const;  // グラフをostreamにdot言語で吐く
private:
    int nodeCount;  // 存在するノードの数
    int edgeCount;  // 存在する辺の数
    std::vector<GraphNode> nodes;
    std::vector<GraphEdge> edges;
    GraphOption nodeOption;  // 全てのノードに適用されるオプション
};