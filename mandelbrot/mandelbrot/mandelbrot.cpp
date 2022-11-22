#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include "img.h"
#include "cmplx.h"

/*  INCLUDE PATHS TO cmplx.lib  AND  imagelib.lib  LIBRARIES */

#define ITERATION_STEPS 200

#define WHITE color(255, 255, 255)
#define BLACK color(0, 0, 0)


#define WIDTH 1920
#define HEIGHT 1080

complex comp_quadratic(complex z, complex c);
std::vector<double> linspace(double start, double stop, int steps);
double colormap(double in);

int main() {
	double camera_width = 1;
	double aspect_ratio = (double)HEIGHT/WIDTH;
	complex center{-0.9,-0.25 };

	image im = image(WIDTH, HEIGHT, WHITE);
	auto map = im.get_map();
	auto x = linspace(center.re() - camera_width/2, center.re() + camera_width / 2, WIDTH);
	auto y = linspace(center.im() - (aspect_ratio* camera_width) / 2, center.im() + (aspect_ratio * camera_width) / 2, HEIGHT);


	for (auto i = map.begin(); i != map.end(); i++) {
		for (auto j = (*i).begin(); j != (*i).end(); j++) {

			complex c = complex{ x[j - (*i).begin()],y[i - map.begin()] };
			complex temp = complex{ 0,0 };

			for (int k = 1; k <= ITERATION_STEPS; k++) {
				temp = comp_quadratic(temp, c);

				if (temp.mag() > 2) {
					//diverges
					double percentage = (double)k / ITERATION_STEPS;
					double white_sat = colormap(percentage);
					auto saturation = white_sat * 255;
					*j = color((uint8_t)saturation, (uint8_t)saturation, (uint8_t)saturation);
					break;
				}
			}

			if (temp.mag() <= 2) {
				//series converges
				*j = BLACK;
			}

		}
	}

	im.set_map(map);
	im.save_ppm("mandelbrot");

}

complex comp_quadratic(complex z, complex c) {
	complex zsqr = cpow(z, 2);
	complex ret = zsqr + c;
	return ret;
}

std::vector<double> linspace(double start, double stop, int steps) {
	std::vector<double> ret;
	if (stop > start) {

		const double step = (stop - start) / (double)steps;
		for (double i = start; i <= stop; i += step) {
			ret.push_back(i);
		}

	}
	else {
		return ret;
	}

}

double colormap(double in)
{
	return exp(- 2 * in);
}

