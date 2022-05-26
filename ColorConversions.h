#pragma once
#include <string>
#include <vector>


//check color valid
int check_color_type(std::string _color);


//extract data from string to int
std::vector<int> extract_RGBA(std::vector<std::string> vec_RvGvBvA, int it);


//convert DEC to HEX
std::string decToHexa(int dec_num);


// convert RGB to HEX
std::string convertRGBtoHex(int R, int G, int B, int Alpha);


// convert RGB to HSL
void rgb2hsl(double tR, double tG, double tB, double &hue, double &saturation, double &lightness);


// convert HEX to RGB
std::string HEX2RGB(std::string inS);