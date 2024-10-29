//
//  CoreFlowGraphics.h
//  CoreFlowGraphics
//
//  Created by a-heestand on 2024/10/29.
//

#ifndef CoreFlowGraphics_
#define CoreFlowGraphics_

#pragma GCC visibility push(default)

#include <string>

//struct Color {
//    uint8_t red, green, blue, alpha;
//};

class CoreFlowGraphics {
public:
    CoreFlowGraphics();
    void flow(void* data, size_t size, size_t width, size_t height);
};

#pragma GCC visibility pop
#endif
