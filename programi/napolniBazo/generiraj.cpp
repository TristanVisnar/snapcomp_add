
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <stdlib.h>

using namespace std;

const int width = 600, height = 400;

int main(int argc, char ** argv) {
	if (argc != 2) {
		cout << "Napaka klicu aplikacije" << endl;
		return -1;
	}

	for (int i = 0; i < atoi(argv[1]); i++) {
		srand(i*(1484011024*(18+i))% 4294967295);
		stringstream ss;
		ss << "./ppm/picture" << i << ".ppm";
		string ime = ss.str();
		ofstream img;
		img.open(ime, ios::out | ios::trunc);
		img << "P3" << endl;
		img << width << " " << height << endl;
		img << "255" << endl;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int r = rand() % 255;
				int g = rand() % 255;
				int b = rand() % 255;

				img << r << " " << g << " " << b << endl;
			}
		}
		img.close();
	}
	return 0;
}
