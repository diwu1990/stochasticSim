#pragma once
#ifndef CONV2D_H
#define CONV2D_H

#include <cstdio>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include "cfadd.hpp"
#include "cfmul.hpp"
#include "perfsim.hpp"
using namespace std;

struct DataSize
{
    public:
        DataSize() = default;
        DataSize(const size_t _number, const size_t _channels, const size_t _width, const size_t _height)
            :number(_number),channels(_channels), width(_width), height(_height){}
        inline size_t totalSize() const { return _4DSize(); }
        inline size_t _4DSize() const { return number*channels*width*height; }
        inline size_t _3DSize() const { return channels*width*height; }
        inline size_t _2DSize() const { return width*height; }
        inline bool operator==(const DataSize& other) const{ 
            return other.number == number && other.channels == channels && other.width == width && other.height == height;
        }
        inline bool operator!=(const DataSize& other) const{
            return !(*this==(other));
        }
        inline size_t getIndex(const size_t in, const size_t ic, const size_t ih, const size_t iw) const{
            return in*channels*height*width + ic*height*width + ih*width + iw;
        }
        inline size_t getIndex(const size_t ic, const size_t ih, const size_t iw) const{
            return ic*height*width + ih*width + iw;
        }
        size_t number = 0;
        size_t channels = 0;
        size_t width = 0;
        size_t height = 0;
};

class CONV2D
{
    // 2D convolution
    // follow the algorithm from easyCNN convolution2d_same/validate
    // https://github.com/xylcbd/EasyCNN/blob/master/src/MathFunctions.cpp

    // batch size is always 1

    // input feature map size is  (ifm channel,              ifm height, ifm width)
    // weight size            is  (ifm channel, ofm channel, wgt height, wgt width)
    // bias size              is  (             ofm channel                       )
    // output feature map size is (             ofm channel, ofm height, ofm width)

    // Init input
    vector<vector<vector<float>>> iFmProb;
    vector<vector<vector<vector<float>>>> wgtProb;
    vector<float> biasProb;
    unsigned int issame; // same mode
    unsigned int stride; // always same stride for height and width

    // Calc input
    vector<vector<vector<char>>> iFm;

    // output 
    vector<vector<vector<char>>> oFm;

    unsigned int ifm_c;
    unsigned int ifm_h;
    unsigned int ifm_w;

    unsigned int wgt_h;
    unsigned int wgt_w;

    unsigned int ofm_c;
    unsigned int ofm_h;
    unsigned int ofm_w;

    vector<vector<vector<vector<CFMUL>>>> mulInst;
    vector<vector<vector<CFADD>>> addInst;

    // perfsim output
    #ifdef PERFSIM
        vector<vector<vector<vector<char>>>> oBS;
        vector<vector<vector<float>>> wProb;
        vector<vector<vector<float>>> theoProb;
        vector<vector<vector<float>>> wBias;
        vector<vector<vector<unsigned int>>> cTime;
    #endif

    public:
        CONV2D();
        ~CONV2D();
        void Help();
        void Init(vector<vector<vector<float>>>, vector<vector<vector<vector<float>>>>, vector<float>, unsigned int, unsigned int);
        void Calc();

        #ifdef PERFSIM
            vector<vector<vector<vector<char>>>> OutBS();
            vector<vector<vector<float>>> WProb();
            vector<vector<vector<float>>> TheoProb();
            vector<vector<vector<float>>> WBias();
            vector<vector<vector<unsigned int>>> CTime();
        #endif
};

#endif