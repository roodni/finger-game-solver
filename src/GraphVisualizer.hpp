#pragma once
#include <ostream>
#include <vector>
#include <map>

enum class eOptionType {
    string,
    noQuote // ダブルクォーテーションを出力しない文字列
};

// ノードに付与するオプション
class GraphOption {
public:
    void set(const std::string &key, const std::string &value, eOptionType type);   // オプション設定
    void toStream(std::ostream &out) const;    // ostreamに設定を吐き出す
private:
    std::map<std::string, std::string> options;
};


// 各ノードのオプションと状態遷移を記録するデータ構造
class GraphNode {
public:
    std::vector<int> trans;
    GraphOption option;
};


// 有向グラフをdot言語として吐く
class GraphVisualizer {
public:
    GraphVisualizer();
    int createNode();   // ノードを作成し、ノードidを返す
    int createNode(const std::string &label);
    void setTrans(int fromId, int toId);    // 枝を設定する
    void setNodeOption(int nodeId, const std::string &key, const std::string &value, eOptionType type = eOptionType::string); // 特定のノードにオプションを設定する
    void setAllNodesOption(const std::string &key, const std::string &value, eOptionType type = eOptionType::string); // 全てのノードにオプションを設定する
    void toDotLang(std::ostream &out) const;  // グラフをostreamにdot言語で吐く
private:
    int nodeCount;  // 存在するノードの数
    std::vector<GraphNode> nodes;
    GraphOption nodeOption;  // 全てのノードに適用されるオプション
};