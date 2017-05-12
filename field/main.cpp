//
//  main.cpp
//
//  Copyright © 2017 Max Lunin. All rights reserved.
//

#include <iostream>

#include "Field.hpp"
#include "Display.hpp"

#include <cmath>

typedef double Value;
typedef double Coord;
typedef field::Field<Value, Coord> Field;

int main(int argc, const char * argv[]) {
    
    size_t radius = 5;
    double step = 0.345;
    
    Field field(radius, step);
    
    for (auto i: field) {
        auto p = i.coord;
        auto distance = sqrt(p.x*p.x + p.y*p.y);
        *i = distance;
    }
    
    auto cellSize = 20;
    Display("distance", cellSize).diplay(field, Display::Wait);
    
    return 0;
}
