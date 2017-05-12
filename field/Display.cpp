//
//  Display.cpp
//
//  Copyright © 2017 cpp_cgdk. All rights reserved.
//

#include "Display.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#pragma clang diagnostic pop

#include <string>
#include <set>

static std::set<std::string> windows;
const int grid = 250;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshorten-64-to-32"

Display::Display(const char* name, size_t factor) : _name(name), _factor(factor){
    
    std::string windowName(name);
    
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
    
    if (windows.find(windowName) == windows.end()) {
        auto x = windows.size() % 2;
        auto y = windows.size() / 2;
        cv::moveWindow(windowName, x * grid, y * grid);
    }
    
    windows.insert(windowName);
}

void Display::diplay(const field::FieldDouble& field, Waiting wait, double from, double to){
    
    auto mat = cv::Mat(field.size() * _factor, field.size() * _factor, CV_8UC3, cv::Scalar(0,0,0));
    
    uint8_t* pixelPtr = (uint8_t*)mat.data;
    int cn = mat.channels();
    cv::Scalar_<uint8_t> bgrPixel;
    
    field::FieldDouble diplay = field;
    diplay.normalize(from, to);
    diplay.normalize(0., 255.);
    
    for(int i = 0; i < mat.rows; i++){
        for(int j = 0; j < mat.cols; j++){
            
            pixelPtr[i*mat.cols*cn + j*cn + 0] = diplay(i/_factor, j/_factor); // B
            
            bool centerOfField = mat.rows/2 == i && mat.cols/2 == j;
            if(centerOfField){
                pixelPtr[i*mat.cols*cn + j*cn + 1] = 255.;
            }
//            pixelPtr[i*mat.cols*cn + j*cn + 2]; // R
        }
    }
    
    cv::imshow(_name, mat);
    cv::waitKey(wait);
}

#pragma clang diagnostic pop

Display::~Display(){

}

