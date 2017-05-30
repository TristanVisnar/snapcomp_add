
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <stdlib.h>

using namespace std;

const int width = 255, height = 255;

int main(int argc, char ** argv) {
	if (argc != 2) {
		cout << "Napaka klicu aplikacije" << endl;
		return -1;
	}

	srand(time(NULL));
	int tmp = 0;
	int tmp2 = 1;
	int tmp3 = 2;
	for (int i = 0; i < atoi(argv[1]); i++) {
		stringstream ss;
		ss << "./ppm/picture" << i << ".ppm";
		string ime = ss.str();
		ofstream img;
		img.open(ime.c_str(), ios::out | ios::trunc);
		img << "P3" << endl;
		img << width << " " << height << endl;
		img << "255" << endl;
		tmp = rand();
		tmp2 = rand();
		tmp3 = rand();

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {

				int r,g,b;
				if(rand()%2==0){
				r = x*tmp % 255;
				g = y*tmp2 % 255;
				b = x*y*tmp3 % 255;
				}
				else{
				r = x+tmp % 255;
				g = y+tmp2 % 255;
				b = (x+tmp)*(y+tmp2)+tmp3 %255;
				}
				img << r << " " << g << " " << b << endl;
			}
		}
		img.close();
	}
	return 0;
}
