#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

float fx = 0.0, fy = 0.0, cx = 0.0, cy = 0.0,
k1 = 0.0, k2 = 0.0, p1 = 0.0, p2 = 0.0;

void init() {
	vector<float>paramlist;
	float param = 0.0;
	ifstream infile("param.txt");
	while (infile >> param) {
		paramlist.push_back(param);
	}
	fx = paramlist[0];
	fy = paramlist[1];
	cx = paramlist[2];
	cy = paramlist[3];
	k1 = paramlist[4];
	k2 = paramlist[5];
	p1 = paramlist[6];
	p2 = paramlist[7];
}

int main() {
	init();

	return 0;
}