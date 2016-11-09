#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

float fx = 0.0, fy = 0.0, cx = 0.0, cy = 0.0,
k1 = 0.0, k2 = 0.0, p1 = 0.0, p2 = 0.0;

float p_degree = 0.0, t_degree = 0.0;
float R[3][3] = { 0, }, RT[3][3] = { 0, };
float X1 = 0, Y1 = 0, Z1 = 0;
vector<Point2f>pos;
Mat img = Mat::zeros(480, 680, CV_8UC3);
Scalar color(255, 255, 255);
int thickness = 1;

void cal(float Xw, float Yw, float Zw) {
	float p = p_degree * 3.1415926 / 180.0, t = t_degree * 3.1415926 / 180.0;
	/*
	R[0][0] = sin(p);
	R[0][1] = sin(t)*cos(p);
	R[0][2] = cos(t)*cos(p);
	R[1][0] = -cos(p);
	R[1][1] = sin(t)*sin(p);
	R[1][2] = cos(t)*sin(p);
	R[2][0] = 0;
	R[2][1] = -cos(t);
	R[2][2] = sin(t);
	*/

	R[0][0] = cos(p);
	R[0][1] = sin(p);
	R[0][2] = 0;
	R[1][0] = -sin(t)*sin(p);
	R[1][1] = sin(t)*cos(p);
	R[1][2] = -cos(t);
	R[2][0] = -cos(t)*sin(p);
	R[2][1] = cos(t)*cos(p);
	R[2][2] = sin(t);
	/*
	RT[0][0] = sin(p);
	RT[0][1] = -cos(p);
	RT[0][2] = 0;
	RT[1][0] = sin(t)*cos(p);
	RT[1][1] = sin(t)*sin(p);
	RT[1][2] = -cos(t);
	RT[2][0] = cos(t)*cos(p);
	RT[2][1] = cos(t)*sin(p);
	RT[2][2] = sin(t);
	*/
	float Xc = 0, Yc = 0, Zc = 0;
	Xc = R[0][0] * (Xw - X1) + R[0][1] * (Yw - Y1) + R[0][2] * (Zw - Z1);
	Yc = R[1][0] * (Xw - X1) + R[1][1] * (Yw - Y1) + R[1][2] * (Zw - Z1);
	Zc = R[2][0] * (Xw - X1) + R[2][1] * (Yw - Y1) + R[2][2] * (Zw - Z1);
	//cout << "Xc = " << Xc << " Yc = " << Yc << " Zc = " << Zc << endl;

	float u, v, r2, u_d, v_d, x, y;
	u = Xc / Zc;
	v = Yc / Zc;
	r2 = u*u + v*v;
	u_d = (1 + k1*r2 + k2*r2*r2)*u + 2 * p1*u*v + p2*(r2 + 2 * u*u);
	v_d = (1 + k1*r2 + k2*r2*r2)*v + p1*(r2 + 2 * v*v) + 2 * p2*u*v;
	x = u_d*fx + cx;
	y = v_d*fy + cy;
	//cout << "Pixel X = " << x << ", " << "Pixel Y = " << y << endl;
	pos.push_back(Point2f(x, y));
}

void drawing_plane() {
	for (int i = 0; i < pos.size(); i++) {
		if (i + 1 != 4)
			line(img, Point(pos[i].x, pos[i].y), Point(pos[i + 1].x, pos[i + 1].y), color, thickness);
		else if (i + 1 == 4)
			line(img, Point(pos[i].x, pos[i].y), Point(pos[0].x, pos[0].y), color, thickness);
	}
	pos.clear();
}

void drawing_text(float Xw, float Yw, float Zw) {
	float Xc = 0, Yc = 0, Zc = 0;
	Xc = R[0][0] * (Xw - X1) + R[0][1] * (Yw - Y1) + R[0][2] * (Zw - Z1);
	Yc = R[1][0] * (Xw - X1) + R[1][1] * (Yw - Y1) + R[1][2] * (Zw - Z1);
	Zc = R[2][0] * (Xw - X1) + R[2][1] * (Yw - Y1) + R[2][2] * (Zw - Z1);

	float u, v, r2, u_d, v_d, x, y;
	u = Xc / Zc;
	v = Yc / Zc;
	r2 = u*u + v*v;
	u_d = (1 + k1*r2 + k2*r2*r2)*u + 2 * p1*u*v + p2*(r2 + 2 * u*u);
	v_d = (1 + k1*r2 + k2*r2*r2)*v + p1*(r2 + 2 * v*v) + 2 * p2*u*v;
	x = u_d*fx + cx;
	y = v_d*fy + cy;
	String _x = format("%.1f", x);
	String _y = format("%.1f", y);
	String str = "(" + _x + "," + _y + ")";
	putText(img, str, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, color, thickness);
}

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

	//ī�޶� ����
	X1 = 0;
	Y1 = -26;
	Z1 = 1;
}

void draw_disp() {
	img = Mat::zeros(480, 680, CV_8UC3);
	//1�� ���
	cal(0, 0, 0);
	cal(7.5, 0, 0);
	cal(7.5, 7, 0);
	cal(0, 7, 0);
	drawing_plane();

	//2�� ���
	cal(0, 0, 3);
	cal(0, 0, 0);
	cal(7.5, 0, 0);
	cal(7.5, 0, 3);
	drawing_plane();

	//3�� ���
	cal(0, 0, 0);
	cal(0, 7, 0);
	cal(0, 7, 3);
	cal(0, 0, 3);
	drawing_plane();

	//4�� ���
	cal(0, 7, 3);
	cal(0, 7, 0);
	cal(7.5, 7, 0);
	cal(7.5, 7, 3);
	drawing_plane();

	//5�� ���
	cal(7.5, 0, 3);
	cal(7.5, 0, 0);
	cal(7.5, 7, 0);
	cal(7.5, 7, 3);
	drawing_plane();

	//6�� ���
	cal(0, 0, 3);
	cal(7.5, 0, 3);
	cal(7.5, 7, 3);
	cal(0, 7, 3);
	drawing_plane();

	drawing_text(0, 0, 0);
	drawing_text(7.5, 0, 0);
	drawing_text(7.5, 7, 0);
	drawing_text(7.5, 7, 3);
	drawing_text(0, 7, 3);
	drawing_text(0, 0, 3);
	drawing_text(7.5, 0, 3);
}

int main() {
	init();
	draw_disp();
	cout << "Camera Control Key List" << endl;

	while (1) {
		imshow("img", img);
		int keycode = waitKey(0);
		//Z
		if (keycode == 105) { //I
			Z1 += 0.1;
			draw_disp();
		}
		else if (keycode == 107) { //K
			Z1 -= 0.1;
			draw_disp();
		}
		//X
		else if (keycode == 106) { //J
			X1 -= 0.1;
			draw_disp();
		}
		else if (keycode == 108) { //L
			X1 += 0.1;
			draw_disp();
		}
		//Y
		else if (keycode == 73) { //shift+I
			Y1 += 0.1;
			draw_disp();
		}
		else if (keycode == 75) { //shift+K
			Y1 -= 0.1;
			draw_disp();
		}
		//tilt
		else if (keycode == 9) { //con+I
			t_degree += 0.1;
			draw_disp();
		}
		else if (keycode == 11) { //con+K
			t_degree -= 0.1;
			draw_disp();
		}
		//pan
		else if (keycode == 10) { //con+J
			p_degree += 0.1;
			draw_disp();
		}
		else if (keycode == 12) { //con+L
			p_degree -= 0.1;
			draw_disp();
		}
		else if (keycode == 27) { //esc
			break;
		}
	}
	destroyAllWindows();
	/*
	VideoCapture vc(0);
	if (!vc.isOpened()) return 0;

	Mat input;
	while (1) {
		vc >> input;
		if (input.empty()) break;
		int col = input.cols;
		int row = input.rows;

		Point center(col / 2, row / 2);
		Scalar color(0, 0, 255);
		circle(input, center, 5, color, CV_FILLED);

		imshow("cam", input);
		if (waitKey(33) == 27) break;
	}
	destroyAllWindows();
	*/
	return 0;
}