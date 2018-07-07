#include <fstream>
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main() {
	srand(time(NULL));
	int N = 1000000;
	int E = N * 4 + rand() % 10000;
	ofstream fout("/home/amadeuschan/original_version/GeminiGraph/simple_graph.biedgelist", ios::binary);
	ofstream ftxtout("/home/amadeuschan/original_version/GeminiGraph/simple_graph.txt");

	ftxtout << N << endl;

	for (int i = 0; i < E; ++ i) {
		int x, y;
		do {
			x = rand() % N;
			y = rand() % N;
			fout.write(reinterpret_cast <const char*> (&x), sizeof(x));
			fout.write(reinterpret_cast <const char*> (&y), sizeof(y));
			ftxtout << x << " " << y << endl;
		} while (x == y);
		/*
		fout.write(reinterpret_cast <const char*> (&i), sizeof(i));
		int j = i + 1;
		fout.write(reinterpret_cast <const char*> (&j), sizeof(j));
		fout.write(reinterpret_cast <const char*> (&j), sizeof(j));
		fout.write(reinterpret_cast <const char*> (&i), sizeof(i));
		ftxtout << i << " " << j << endl;
		ftxtout << j << " " << i << endl;
		*/
	}

	fout.close();
	ftxtout.close();
	return 0;
}
