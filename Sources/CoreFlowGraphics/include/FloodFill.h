//
//  FloodFill.h
//  CoreFlowGraphics
//
//  Created by a-heestand on 2024/10/29.
//

#ifndef FloodFill_
#define FloodFill_

#pragma GCC visibility push(default)

#include "GraphicTypes.h"

class FloodFill {
public:
    FloodFill();
    void floodFill(void* originalGraphic,
                   void* targetGraphic,
                   size_t size,
                   FGSize resolution,
                   FGColor color,
                   FGPoint location,
                   double threshold);
};

#pragma GCC visibility pop
#endif
