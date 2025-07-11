//
//  Histogram.h
//  CoreFlowGraphics
//
//  Created by a-heestand on 2024/10/29.
//

#ifndef Histogram_
#define Histogram_

#pragma GCC visibility push(default)

#include "GraphicTypes.h"
#include <vector>

using namespace std;

class Histogram {
public:
    Histogram();
    const size_t* histogramColor(void* data,
                                 size_t size,
                                 FGSize resolution);
    const size_t* histogramChannel(void* data,
                                   size_t size,
                                   FGSize resolution,
                                   size_t channel);
};

#pragma GCC visibility pop
#endif
