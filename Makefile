CXX = g++
CXXFLAGS = -O2 -std=c++17 -Wall

.PHONY: test
test: test_state_trans

test_state_trans_out = ./test/state_trans/test.out
test_state_trans_src = ./test/state_trans/test.cpp
test_state_trans_dep = ./obj/GameState.o
test_state_trans_in = ./test/state_trans/in.txt
test_state_trans_log = ./test/state_trans/log.txt
.PHONY: test_state_trans
test_state_trans: $(test_state_trans_out)
	$(test_state_trans_out) < $(test_state_trans_in) 2> $(test_state_trans_log)
	cat $(test_state_trans_log)

$(test_state_trans_out): $(test_state_trans_src) $(test_state_trans_dep)
	$(CXX) $(CXXFLAGS) $(test_state_trans_src) $(test_state_trans_dep) -o  $(test_state_trans_out)


./obj/GameState.o: ./src/GameState.cpp ./src/GameState.hpp
	$(CXX) $(CXXFLAGS) -c  $< -o $@