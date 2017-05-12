//
//  Display.hpp
//
//  Copyright © 2017 Max Lunin. All rights reserved.
//

#ifndef Display_hpp
#define Display_hpp

/*
 HEADER_SEARCH_PATHS = /usr/local/Cellar/opencv3/3.2.0/include
 LIBRARY_SEARCH_PATHS = /usr/local/Cellar/opencv3/3.2.0/lib
 
 LINK:
 /usr/local/Cellar/opencv3/3.2.0/lib/libopencv_core.dylib
 /usr/local/Cellar/opencv3/3.2.0/lib/libopencv_highgui.dylib
 */

#include "Field.hpp"

class Display{
    const size_t _factor;
    const char* _name;
public:
    
    enum Waiting{
        Wait = 0,
        NotWait = 1
    };
    
    Display(const char* name, size_t factor);
    
    void diplay(const field::FieldDouble& f, Waiting wait = NotWait, double from=0, double to=1);
    
    ~Display();
};

#endif /* Display_hpp */
