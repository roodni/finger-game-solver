cxx = g++
cxxflags = -O2 -std=c++17 -Wall

graphviz = dot
graphviz_flags = -Tpdf

.PHONY: test
test: test_state_trans test_graphviz

test_state_trans_out = ./test/state_trans/test.out
test_state_trans_src = ./test/state_trans/test.cpp
test_state_trans_dep = ./obj/GameState.o
test_state_trans_in = ./test/state_trans/in.txt
test_state_trans_log = ./test/state_trans/log.txt
.PHONY: test_state_trans
test_state_trans: $(test_state_trans_out)
	$(test_state_trans_out) < $(test_state_trans_in) 2> $(test_state_trans_log)

$(test_state_trans_out): $(test_state_trans_src) $(test_state_trans_dep)
	$(cxx) $(cxxflags) $< $(test_state_trans_dep) -o $@


test_graphviz_out = ./test/graphviz/test.out
test_graphviz_src = ./test/graphviz/test.cpp
test_graphviz_dep = ./obj/GraphVisualizer.o
test_graphviz_dot = ./dot/test.dot
test_graphviz_pdf = ./dot/test.pdf
.PHONY: test_graphviz
test_graphviz: $(test_graphviz_dot)
	$(graphviz) $(graphviz_flags) -o $(test_graphviz_pdf) $<

$(test_graphviz_out): $(test_graphviz_src) $(test_graphviz_dep)
	$(cxx) $(cxxflags) $< $(test_graphviz_dep) -o $@
$(test_graphviz_dot): $(test_graphviz_out)
	$(test_graphviz_out) $@


./obj/GameState.o: ./src/GameState.cpp ./src/GameState.hpp
	$(cxx) $(cxxflags) -c  $< -o $@
./obj/GraphVisualizer.o: ./src/GraphVisualizer.cpp ./src/GraphVisualizer.hpp
	$(cxx) $(cxxflags) -c  $< -o $@