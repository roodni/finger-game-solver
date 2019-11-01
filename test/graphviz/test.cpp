#include "../../src/GraphVisualizer.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <file>" << std::endl;
        return 1;
    }

    GraphVisualizer graph;
    int a = graph.createNode("a");
    int b = graph.createNode("b");
    int c = graph.createNode("c");
    int d = graph.createNode("d");
    int e = graph.createNode("e");

    graph.setNodeOption(a, "fontsize", "30");
    graph.setNodeOption(b, "shape", "ellipse");
    graph.setNodeOption(b, "style", "filled");
    graph.setNodeOption(b, "fillcolor", "red");
    graph.setAllNodesOption("shape", "box");

    graph.setTrans(a, b);
    graph.setTrans(b, c);
    graph.setTrans(c, d);
    graph.setTrans(d, b);
    graph.setTrans(b, e);
    graph.setTrans(d, e);

    std::ofstream out(argv[1]);
    graph.toDotLang(out);
    out.close();

    return 0;
}