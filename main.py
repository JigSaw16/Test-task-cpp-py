#!/usr/bin/python
import re
import colorsys
import sys

reHEX = r"^([0-9a-f]{3}|[0-9a-f]{6}|[0-9a-f]{8})$"
reRGBA = r"^([0-9]{1,3},){3}[0-9]{1,3}$"
rePATH = "main.py"


class Color:
    def __init__(self, R_, G_, B_, alpha_, hex_, hue_, saturation_, lightness_):
        self.R_ = R_
        self.G_ = G_
        self.B_ = B_
        self.alpha_ = alpha_
        self.hex_ = hex_
        self.hue_ = hue_
        self.saturation_ = saturation_
        self.lightness_ = lightness_
        self.vR_ = []
        self.vG_ = []
        self.vB_ = []
        self.vAL_ = []

    def colorVector(self, vec_colors):
        for x in range(len(vec_colors)):
            self.vR_.append(vec_colors[x][0])
            self.vG_.append(vec_colors[x][1])
            self.vB_.append(vec_colors[x][2])
            self.vAL_.append(vec_colors[x][3])

    def mixMODE_(self):
        self.R_ = int(sum(self.vR_) / len(self.vR_))
        self.G_ = int(sum(self.vG_) / len(self.vG_))
        self.B_ = int(sum(self.vB_) / len(self.vB_))
        self.alpha_ = int(sum(self.vAL_) / len(self.vAL_))

    def lowestMODE_(self):
        self.R_ = min(self.vR_)
        self.G_ = min(self.vG_)
        self.B_ = min(self.vB_)
        self.alpha_ = min(self.vAL_)

    def highestMODE_(self):
        self.R_ = max(self.vR_)
        self.G_ = max(self.vG_)
        self.B_ = max(self.vB_)
        self.alpha_ = max(self.vAL_)

    def mix_saturateMODE_(self):
        self.R_ = self.vR_[len(self.vR_) - 1]
        self.G_ = self.vG_[len(self.vG_) - 1]
        self.B_ = self.vB_[len(self.vB_) - 1]
        self.alpha_ = self.vAL_[len(self.vAL_) - 1]
        r, g, b = [x / 255.0 for x in (self.R_, self.G_, self.B_)]
        uless1, uless2, self.saturation_ = colorsys.rgb_to_hls(r, g, b)

    def write_info_(self):
        print('R =', round(self.R_), 'G =', round(self.G_), 'B =', round(self.B_), 'Alpha =', round(self.alpha_))
        print('Hex =', self.hex_)
        print('Hue =', int(self.hue_ * 360), 'Saturation =', round(self.saturation_, 2), 'Lightness =', round(self.lightness_, 2))


def ColorsLoad(arg_col):
    vec_col = []
    with open("colors.txt") as f:
        file_colors = [line.strip() for line in f if line.strip()]
    f.close()

    for x in file_colors:
        if re.search(reRGBA, x):
            tmp_rgba_ = x.split(",")
            rgba_ = [int(tmp_array_) for tmp_array_ in tmp_rgba_]
            vec_col.append(rgba_)
            for xx in rgba_:
                if not 255 >= xx >= 0:
                    vec_col.remove(rgba_)

        elif re.search(reHEX, x):
            hex_ = HEX2RGB(x)
            vec_col.append(hex_)

    vec_col = vec_col + arg_col
    return vec_col


def RGB2HEX(c):
    vrgb = [c.R_, c.G_, c.B_, c.alpha_]
    for v in vrgb:
        if 255 >= int(v) >= 0:
            c.hex_ = '#%02x%02x%02x%02x' % (int(vrgb[0]), int(vrgb[1]), int(vrgb[2]), int(vrgb[3]))


def HEX2RGB(vhex_):
    out = []
    if len(vhex_) == 3:
        out.append(int(vhex_[0:1], 16)), out.append(int(vhex_[1:2], 16)), out.append(int(vhex_[2:3], 16)), out.append(255)
    if len(vhex_) == 6:
        out.append(int(vhex_[0:2], 16)), out.append(int(vhex_[2:4], 16)), out.append(int(vhex_[4:6], 16)), out.append(255)
    if len(vhex_) == 8:
        out.append(int(vhex_[0:2], 16)), out.append(int(vhex_[2:4], 16)), out.append(int(vhex_[4:6], 16)), out.append(int(vhex_[6:8], 16))

    return out


def RGB2HLS(c):
    r, g, b = [x / 255.0 for x in (c.R_, c.G_, c.B_)]
    c.hue_, c.lightness_, c.saturation_ = colorsys.rgb_to_hls(r, g, b)


def argOperations(col_):
    arg_colors = []
    for x in sys.argv:
        if not re.search(rePATH, x, re.IGNORECASE) and x != "-m" and x != "--mode" and \
                x != "mix" and x != "lowest" and x != "highest" and x != "mix-saturate":
            print("Invalid argument! =>", x)
        if re.search(reRGBA, x):
            tmp_rgba_ = x.split(",")
            rgba_ = [int(tmp_array_) for tmp_array_ in tmp_rgba_]
            arg_colors.append(rgba_)
            for xx in rgba_:
                if not 255 >= xx >= 0:
                    arg_colors.remove(rgba_)

        elif re.search(reHEX, x):
            hex_ = HEX2RGB(x)
            arg_colors.append(hex_)

    col_.colorVector(ColorsLoad(arg_colors))
    for x in sys.argv:

        if x == "-m" or x == "--mode":
            for y in sys.argv:
                if y == "mix":
                    col_.mixMODE_()
                elif y == "lowest":
                    col_.lowestMODE_()
                elif y == "highest":
                    col_.highestMODE_()
                elif y == "mix-saturate":
                    col_.mix_saturateMODE_()
            break
        else:
            col_.mixMODE_()


if __name__ == '__main__':
    col = Color([0], [0], [0], [0], "", 0, 0, 0)
    argOperations(col)

    RGB2HLS(col)
    RGB2HEX(col)

    col.write_info_()
