genetic.out: genetic.cpp
	$(CXX) --std=c++11 $< -o $@

.PHONY: clean bench

bench: genetic.out
	hyperfine --warmup 3 ./genetic.out

clean:
	rm -f *.out
