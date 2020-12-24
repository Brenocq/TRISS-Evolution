run:
	g++ main.cpp -O3 -march=native -o ag
	./ag

debug:
	g++ -g main.cpp -o ag
	gdb ./ag
