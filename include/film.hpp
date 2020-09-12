#ifndef _FILM_
#define _FILM_

#include <vector>
#include "./vec.hpp"
#include "../lib/lodepng.h"
#include <fstream>

class Film
{
private:
    /* data */
    std::string filename;
    std::string img_type;
    std::string type;
    int x_res;
    int y_res;
    std::vector<std::vector<Vec>> output;

public:
    Film(std::string filename, std::string img_type, std::string type, int x_res, int y_res);
    ~Film();
    int width();
    int height();
    void fillEmpty();
    void add(int x, int y, Vec color);
    void write_image();
};

void Film::fillEmpty(){
    for(int i =0; i < x_res; i++){
        std::vector<Vec> vector;
        for(int j=0; j < y_res; j++){
            vector.push_back(Vec(0.0, 0.0, 0.0));
        }
        output.push_back(vector);
    }
}

Film::Film(std::string filename, std::string img_type, std::string type, int x_res, int y_res)
{
    this->filename = filename;
    this->img_type = img_type;
    this->type = type;
    this->x_res = x_res;
    this->y_res = y_res;
    fillEmpty();
}

Film::~Film() = default;

int Film::width(){
    return x_res;
}

int Film::height(){
    return y_res;
}

void Film::add(int x, int y, Vec color){
    output[x][y] = color;
}

void Film::write_image(){
    if (img_type == "ppm") {
        std::ofstream outfile;
        outfile.open(filename);
        outfile << "P3" << std::endl;
        outfile << width() << " " << height() << std::endl;
        outfile << "255" << std::endl;
        for(int i = height() - 1; i >= 0; i--){
            for(int j = 0; j < width(); j++){
                outfile << output[j][i].toRGB() << std::endl;
            }
        }
        outfile.close();
    }
}
#endif
