#ifndef _VEC_
#define _VEC_

#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>

class Vec{
    public:
        float v1;   // x
        float v2;   // y 
        float v3;   // z
        float v4;   // a

        Vec(){
            v1 = 0;
            v2 = 0;
            v3 = 0;
            v4 = 0;
        }

        Vec(float v1, float v2, float v3){
            this->v1 = v1;
            this->v2 = v2;
            this->v3 = v3;
            this->v4 = 0;
        }

        Vec(float v1, float v2, float v3, float v4){
            this->v1 = v1;
            this->v2 = v2;
            this->v3 = v3;
            this->v4 = v4;
        }

        Vec(std::vector<std::string> vetor){
            this->v1 = std::stof(vetor[0]);
            this->v2 = std::stof(vetor[1]);
            this->v3 = std::stof(vetor[2]);
            this->v4 = std::stof(vetor[3]);
        }

        Vec(std::string vetor){
            std::istringstream iss(vetor);
            std::vector<std::string> splited((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());
            int v1_;
            std::istringstream(splited[0]) >> v1_;
            int v2_;
            std::istringstream(splited[1]) >> v2_;
            int v3_;
            std::istringstream(splited[2]) >> v3_;
            // int v4_;
            // std::istringstream(splited[3]) >> v4_;

            this->v1 = v1_/255.0;
            this->v2 = v2_/255.0;
            this->v3 = v3_/255.0;
            // this->v4 = v4_/255.0;
        }

        Vec(std::vector<float> vetor){
            this->v1 = vetor[0];
            this->v2 = vetor[1];
            this->v3 = vetor[2];
            this->v4 = vetor[3];
        }

        void print(){
            std::cout << "(" << v1 << ", " << v2 << ", " << v3 << ", " << v4 << ")\n";
        }

        bool operator!=(const Vec &v_) const{
            if((v1 != v_.v1) && (v2 != v_.v2) && (v3 != v_.v3) && (v4 != v_.v4)){
                return true;
            }

            return false;
        }

        bool isBlack(){
            if(v1 == 0 && v2 == 0 && v3 == 0 && v4 == 0){
                return true;
            }

            return false;
        }

        Vec operator+(const Vec &v_){
            float v_v1 = v1 + v_.v1;
            float v_v2 = v2 + v_.v2;
            float v_v3 = v3 + v_.v3;
            float v_v4 = v4 + v_.v4;

            return Vec(v_v1, v_v2, v_v3, v_v4);
        }

        Vec operator-(const Vec &v_){
            float v_v1 = v1 - v_.v1;
            float v_v2 = v2 - v_.v2;
            float v_v3 = v3 - v_.v3;
            float v_v4 = v4 - v_.v4;

            return Vec(v_v1, v_v2, v_v3, v_v4);
        }

        Vec operator*(const float &v_) const{
            float v_v1 = v1 * v_;
            float v_v2 = v2 * v_;
            float v_v3 = v3 * v_;
            float v_v4 = v4 * v_;

            return Vec(v_v1, v_v2, v_v3, v_v4);
        }

        Vec operator*(const Vec & v_) const{
            float v_v1 = v2 * (v_.v3 - v3) * v_.v2;
            float v_v2 = (-1) * (v1 * v_.v3 - v3 * v_.v1);
            float v_v3 = v1 * v_.v2 - v2 * v_.v1;

            return Vec(v_v1, v_v2, v_v3);
        }

        // Normalização do vetor
        Vec normalize(){
            float sq_v1 = v1 * v1;
            float sq_v2 = v2 * v2;
            float sq_v3 = v3 * v3;

            float root = sqrt(sq_v1 + sq_v2 + sq_v3);

            float r_v1 = v1/root;
            float r_v2 = v2/root;
            float r_v3 = v3/root;

            return Vec(r_v1, r_v2, r_v3);
        }

        std::string toRGB(){
            return std::to_string(v1*255.0) + " " + std::to_string(v2*255.0) + " " + std::to_string(v3*255.0);
        }
};

#endif
