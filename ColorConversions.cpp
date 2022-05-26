#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <regex>
#include <vector>
#include "ColorConversions.h"


//check color valid
int check_color_type(std::string _color) {
	if (std::regex_match(_color, std::regex("([0-9a-f]{3}|[0-9a-f]{6}|[0-9a-f]{8})")))
		return 1; // HEX type
	else if (std::regex_match(_color, std::regex("([0-9]{1,3},){3}[0-9]{1,3}")))
		return 2; // RGB type
	else
		return 0; // Invalid type
}


//extract data from string to int
std::vector<int> extract_RGBA(std::vector<std::string> vec_RvGvBvA, int it) {
	std::vector <int> vRGBA = {};
	for (auto col : vec_RvGvBvA) {
		if (check_color_type(col) == 1) {
			col = HEX2RGB(col);
		}
		else if (check_color_type(col) == 0) {
			std::cout << "Invalid color! => " << col << "\n" << std::endl;
			break;
		}
		std::string tok = "";  size_t pos = 0; int iter = 0;
		while ((pos = col.find(",")) != std::string::npos) {
			tok = col.substr(0, pos);


			if (iter == it) {
				if (std::stoi(tok) > 255 || std::stoi(tok) < 0) {
					std::cout << "Invalid color value! => " << col << "\n" << std::endl;
					break;

				}
				else
					vRGBA.push_back(std::stoi(tok));
			}
			col.erase(0, pos + 1);
			iter++;

		}
		if (iter == it) {

			if (std::stoi(col) > 255 || std::stoi(col) < 0) {
				std::cout << "Invalid color value! => " << col << "\n" << std::endl;
				break;
			}
			else
				vRGBA.push_back(std::stoi(col));
		}

	}

	return vRGBA;
}


// convert DEC to HEX
std::string decToHexa(int dec_num)
{
	int r;
	std::string hexdec_num = "";
	char hex[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	while (dec_num > 0)
	{
		r = dec_num % 16;
		hexdec_num = hex[r] + hexdec_num;
		dec_num = dec_num / 16;
	}
	return hexdec_num;
}


// convert RGB to HEX
std::string convertRGBtoHex(int R, int G, int B, int Alpha){

	std::string hex;
	if ((R >= 0 && R <= 255)
		&& (G >= 0 && G <= 255)
		&& (B >= 0 && B <= 255)
		&& (Alpha >= 0 && Alpha <= 255)) {

		hex += decToHexa(R);
		hex += decToHexa(G);
		hex += decToHexa(B);
		hex += decToHexa(Alpha);

		return hex;
	}
	else
		return "-1";
}


// convert RGB to HSL
void rgb2hsl(double tR, double tG, double tB, double &hue, double &saturation, double &lightness) {
	//inputs
	double min, max, C;
	double ttR = tR / 255;
	double ttG = tG / 255;
	double ttB = tB / 255;
	min = ttR < ttG ? ttR : ttG;	min = min < ttB ? min : ttB;
	max = ttR > ttG ? ttR : ttG;	max = max > ttB ? max : ttB;

	//lightness
	C = max - min;
	lightness = (max + min) / 2;

	//hue
	if (C == 0)
		hue = 0;
	else if (max == ttR)
		hue = (ttG - ttB) / C;
	else if (max == ttG)
		hue = 2.0 + (ttB - ttR) / C;
	else if (max == ttB)
		hue = 4.0 + (ttR - ttG) / C;

	hue *= 60.0;

	if (hue < 0.0)
		hue += 360.0;

	hue = round(hue);

	//saturation
	if (C == 0)
		saturation = 0;
	else
		saturation = C / (1 - abs(2 * max - C - 1));
}


// convert HEX to RGB
std::string HEX2RGB(std::string inS) {

	int i, r, len = 2, hex = 0;

	std::vector<int> outS;
	std::vector<std::string> seg;
	if (inS.size() == 3)
		seg = { inS.substr(0,1), inS.substr(1,1), inS.substr(2,1), "ff" };
	if (inS.size() == 6)
		seg = { inS.substr(0,2), inS.substr(2,2), inS.substr(4,2), "ff" };
	if (inS.size() == 8)
		seg = { inS.substr(0,2), inS.substr(2,2), inS.substr(4,2), inS.substr(6,2) };

	for (auto num : seg) {
		hex = 0; len = 2;
		for (i = 0; num[i] != '\0'; i++) {
			len--;
			if (num[i] >= '0' && num[i] <= '9')
				r = num[i] - 48;
			else if (num[i] >= 'a' && num[i] <= 'f')
				r = num[i] - 87;
			else if (num[i] >= 'A' && num[i] <= 'F')
				r = num[i] - 55;
			hex += r * pow(16, len);
		}
		outS.push_back(hex);
	}
	return (std::to_string(outS[0]) + ',' + std::to_string(outS[1]) + ',' + std::to_string(outS[2]) + ',' + std::to_string(outS[3]));
}