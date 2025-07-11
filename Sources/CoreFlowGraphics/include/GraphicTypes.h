//
//  GraphicTypes.h
//  CoreFlowGraphics
//
//  Created by a-heestand on 2024/10/29.
//

#ifndef GraphicTypes_
#define GraphicTypes_

#pragma GCC visibility push(default)

#include <stdint.h>
#include <stddef.h>

struct FGColor {
    uint8_t red, green, blue, alpha;
};

struct FGFloatColor {
    float red, green, blue, alpha;
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

#pragma GCC visibility pop
#endif
