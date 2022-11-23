#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include "img.h"
#include "cmplx.h"

/*  INCLUDE PATHS TO cmplx.lib  AND  imagelib.lib  LIBRARIES */

#define ITERATION_STEPS 35

#define WHITE color(255, 255, 255)
#define BLACK color(0, 0, 0)


#define USER_INPUT 1
#define WIDTH 1920
#define HEIGHT 1080

complex comp_quadratic(complex z, complex c);
std::vector<double> linspace(double start, double stop, int steps);
double colormap(double in);
vector<vector<color>> run_fractal_generator(int steps,const int width,const int height,complex center, double camera_width,double aspect_ratio);

int main() {

	if (USER_INPUT) {

		int width;
		int height;
		double camera_width = 1;
		double center_re, center_im;
		unsigned int it_steps;
		std::string file_name;

		std::cout << "____________________________F R A C T A L    G E N E R A T O R____________________________\n";
		std::cout << "Insert image resolution pixel width: ";
		std::cin >> width;
		std::cout << std::endl;
		std::cout << "Insert image resolution pixel height: ";
		std::cin >> height;
		std::cout << std::endl;
		std::cout << "Insert number of iteration steps (positive integer): ";
		std::cin >> it_steps;
		std::cout << std::endl;
		std::cout << "Insert complex space view width (view height is automatically calculated using aspect ratio): ";
		std::cin >> camera_width;
		std::cout << std::endl;
		std::cout << "View center point x component (real): ";
		std::cin >> center_re;
		std::cout << std::endl;
		std::cout << "View center point y component (imag): ";
		std::cin >> center_im;
		std::cout << std::endl;


		double aspect_ratio = (double)height / width;
		complex center{ center_re,center_im };

		std::cout << "Computing fractal...";
		auto map = run_fractal_generator(it_steps, width, height, center, camera_width, aspect_ratio);
		std::cout << "\nFractal generated.";
		std::cout << "\nSaving image output...";
		image im = image(width, height, WHITE);
		im.set_map(map);
		im.save_ppm("fractal");
		std::cout << "\nImage saved.";
	}
	else {
		double camera_width = 4;
		double aspect_ratio = (double)HEIGHT / WIDTH;
		complex center{ 0,0 };

		std::cout << "Computing fractal...";
		auto map = run_fractal_generator(ITERATION_STEPS, WIDTH, HEIGHT, center, camera_width, aspect_ratio);
		std::cout << "\nFractal generated.";
		std::cout << "\nSaving image output...";
		image im = image(WIDTH, HEIGHT, WHITE);
		im.set_map(map);
		im.save_ppm("fractal");
		std::cout << "\nImage saved.";
	}

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
	return exp(- 1.3 * in);
}

vector<vector<color>> run_fractal_generator(int steps,const int width, const int height, complex center, double camera_width, double aspect_ratio) {

	image im = image(width, height, WHITE);
	auto map = im.get_map();
	auto x = linspace(center.re() - camera_width / 2, center.re() + camera_width / 2, width);
	auto y = linspace(center.im() - (aspect_ratio * camera_width) / 2, center.im() + (aspect_ratio * camera_width) / 2, height);


	for (auto i = map.begin(); i != map.end(); i++) {
		for (auto j = (*i).begin(); j != (*i).end(); j++) {

			complex c = complex{ x[j - (*i).begin()],y[i - map.begin()] };
			complex temp = complex{ 0,0 };

			for (int k = 1; k <= steps; k++) {
				temp = comp_quadratic(temp, c);

				if (temp.mag() > 2) {
					//diverges
					double percentage = (double)k / steps;
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

	return map;
}

