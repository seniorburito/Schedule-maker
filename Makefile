genetic.out: genetic.cpp
	$(CXX) -O3 --std=c++11 $< -o $@

.PHONY: clean bench

bench: genetic.out
	hyperfine --warmup 3 ./genetic.out

clean:
	rm -f *.out

genetic:	genetic.cpp
	g++ -std=c++11 -o genetic genetic.cpp
