#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include "img.h"
#include "cmplx.h"

/*  INCLUDE PATHS TO CMPLX.LIB  AND  IMAGELIB.LIB  LIBRARIES */

#define ITERATION_STEPS 100

complex comp_quadratic(complex z, complex c);
std::vector<double> linspace(double start, double stop, int steps);

int main() {
	double dt = 0.1;
	img::image test_image = img::image(256, 256, img::color(255, 255, 255));
	auto map = test_image.get_map();
	auto x = linspace(-1.5, 1.5, 1 / dt);
	auto y = linspace(-1.5, 1.5, 1 / dt);


	for (auto i = x.begin(); i != x.end(); i++) {
		for (auto j = y.begin(); j != y.end(); j++) {
			complex c = complex{ *i,*j };
			complex temp = complex{ 0,0 };
			for (int k = 0; k < ITERATION_STEPS; k++) {
				temp = comp_quadratic(temp, c);
			}
			if (temp.mag() < 2) {
				//series converges
				map[y.end() - j][x.end() - i] = img::color(0, 0, 0);
			}

		}
	}

	test_image.save_ppm("output");

}

complex comp_quadratic(complex z, complex c) {
	complex zsqr = cpow(z, 2);
	complex ret = zsqr + c;
	return ret;
}

std::vector<double> linspace(double start, double stop, int steps) {
	std::vector<double> ret;
	if (stop > start) {

		double step = (stop - start) / steps;
		for (double i = start; i <= stop; i += step) {
			std::cout << i << " ";
			ret.push_back(i);
		}

	}
	else {
		return ret;
	}

}

