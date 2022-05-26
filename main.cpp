#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <regex>
#include <vector>
#include <algorithm>
#include <numeric>
#include "ColorConversions.h"


class Color {
public:
	int arg_mode, MODE, alpha;
	double R, G, B, hue, saturation, lightness;
	std::vector<std::string> vec_colors;
	std::vector<int> tmpR, tmpG, tmpB, tmpalpha;
	std::string a1, a2, a3, hex;

	void mixMODE(std::vector<std::string> colors) {
		tmpR = extract_RGBA(colors, 0); tmpG = extract_RGBA(colors, 1);
		tmpB = extract_RGBA(colors, 2); tmpalpha = extract_RGBA(colors, 3);
		R = std::accumulate(tmpR.begin(), tmpR.end(), 0.0) / tmpR.size();
		G = std::accumulate(tmpG.begin(), tmpG.end(), 0.0) / tmpG.size();
		B = std::accumulate(tmpB.begin(), tmpB.end(), 0.0) / tmpB.size();
		alpha = std::accumulate(tmpalpha.begin(), tmpalpha.end(), 0.0) / tmpalpha.size();
		hex = convertRGBtoHex(R, G, B, alpha);
		rgb2hsl(R, G, B, hue, saturation, lightness);
	}

	void lowestMODE(std::vector<std::string> colors) {
		tmpR = extract_RGBA(colors, 0); tmpG = extract_RGBA(colors, 1);
		tmpB = extract_RGBA(colors, 2); tmpalpha = extract_RGBA(colors, 3);
		R = *std::min_element(tmpR.begin(), tmpR.end());
		G = *std::min_element(tmpG.begin(), tmpG.end());
		B = *std::min_element(tmpB.begin(), tmpB.end());
		alpha = *std::min_element(tmpalpha.begin(), tmpalpha.end());
		hex = convertRGBtoHex(R, G, B, alpha);
		rgb2hsl(R, G, B, hue, saturation, lightness);
	}

	void highestMODE(std::vector<std::string> colors) {
		tmpR = extract_RGBA(colors, 0); tmpG = extract_RGBA(colors, 1);
		tmpB = extract_RGBA(colors, 2); tmpalpha = extract_RGBA(colors, 3);
		R = *std::max_element(tmpR.begin(), tmpR.end());
		G = *std::max_element(tmpG.begin(), tmpG.end());
		B = *std::max_element(tmpB.begin(), tmpB.end());
		alpha = *std::max_element(tmpalpha.begin(), tmpalpha.end());
		hex = convertRGBtoHex(R, G, B, alpha);
		rgb2hsl(R, G, B, hue, saturation, lightness);
	}

	void mix_saturateMODE(std::vector<std::string> colors) {
		std::vector<double> saturations;
		tmpR = extract_RGBA(colors, 0); tmpG = extract_RGBA(colors, 1);
		tmpB = extract_RGBA(colors, 2); tmpalpha = extract_RGBA(colors, 3);
		R = tmpR[tmpR.size() - 1];
		G = tmpG[tmpR.size() - 1];
		B = tmpB[tmpR.size() - 1];
		alpha = tmpalpha[tmpalpha.size() - 1];
		hex = convertRGBtoHex(R, G, B, alpha);
		for (int i = 0; i < colors.size(); i++) {
			rgb2hsl(tmpR[i], tmpG[i], tmpB[i], hue, saturation, lightness);
			saturations.push_back(saturation);
		}
		saturation = std::accumulate(saturations.begin(), saturations.end(), 0.0) / saturations.size();
		alpha = tmpalpha[tmpalpha.size() - 1];
		rgb2hsl(R, G, B, hue, saturation, lightness);
	}
};


//read file colors.txt
void read_file(std::vector<std::string> &v_c) {

	std::string data_line;
	std::ifstream color_file("colors.txt");
	while (std::getline(color_file, data_line)) {
		if (data_line == "") continue;
		v_c.push_back(data_line);
	}
	color_file.close();
}


//set MODE --mode or -m and load arguments
void arg_operations(int arg_numb, char* arg_from_main[], Color& c) {

	if (arg_numb >= 3) {
		c.a1 = arg_from_main[1];
		c.a2 = arg_from_main[2];
	}
	int tmp_arg = 0;

	if (c.a2 != "mix" && c.a2 != "lowest" && c.a2 != "highest" && c.a2 != "mix-saturate") {
		c.a2 = "mix";
		tmp_arg = 1;
	}

	if (!(c.a1 == "--mode" || c.a1 == "-m")) {
		c.a1 = "-m";
		tmp_arg = 2;
	}

	for (int i = 3 - tmp_arg; i < arg_numb; i++) {
		c.a3 = arg_from_main[i];
		if (check_color_type(c.a3) == 0)
			std::cout << "Invalid " << i << ". argument!\n";
		else
			c.vec_colors.push_back(c.a3);
	}
}


//write info about color
std::string write_info(Color c, int arg_numb) {
	if (c.a2 == "mix") c.mixMODE(c.vec_colors);
	else if (c.a2 == "lowest") c.lowestMODE(c.vec_colors);
	else if (c.a2 == "highest")	c.highestMODE(c.vec_colors);
	else if (c.a2 == "mix-saturate") c.mix_saturateMODE(c.vec_colors);
	else c.mixMODE(c.vec_colors);
	return ("R = " + std::to_string((int)c.R) + ", G = " + std::to_string((int)c.G) + ", B = " + std::to_string((int)c.B) + ", Alpha = " + std::to_string((int)c.alpha) +
		"\nHex = " + c.hex +
		"\nHue = " + std::to_string((int)c.hue) + ", Saturation = " + std::to_string(c.saturation) + ", Lightness = " + std::to_string(c.lightness));
}


int main(int argc, char* argv[]) {
	Color col;
	read_file(col.vec_colors);
	arg_operations(argc, argv, col);
	std::cout << "\n" << write_info(col, argc) << "\n";
	return 0;
}