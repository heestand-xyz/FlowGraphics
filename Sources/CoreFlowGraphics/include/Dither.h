//
//  Dither.h
//  CoreFlowGraphics
//
//  Created by a-heestand on 2024/10/29.
//

#ifndef Dither_
#define Dither_

#pragma GCC visibility push(default)

#include "GraphicTypes.h"

class Dither {
public:
    Dither();
    void floydSteinberg(void* data,
                        size_t size,
                        FGSize resolution,
                        int64_t levels,
                        bool monochrome);
    void atkinson(void* data,
                  size_t size,
                  FGSize resolution,
                  int64_t levels,
                  bool monochrome);
};

#pragma GCC visibility pop
#endif
