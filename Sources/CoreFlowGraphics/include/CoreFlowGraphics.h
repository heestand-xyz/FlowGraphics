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

struct FGColor {
    uint8_t red, green, blue, alpha;
};

struct FGPoint {
    size_t x, y;
};

struct FGSize {
    size_t width, height;
    constexpr size_t count() const {
        return width * height;
    };
};

class CoreFlowGraphics {
public:
    CoreFlowGraphics();
    void floodFill(void* data,
                   size_t size,
                   FGSize resolution,
                   FGColor color,
                   FGPoint location,
                   double threshold);
};

#pragma GCC visibility pop
#endif
