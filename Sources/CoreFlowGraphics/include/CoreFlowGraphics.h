//
//  CoreFlowGraphics.h
//  CoreFlowGraphics
//
//  Created by a-heestand on 2024/10/29.
//

#ifndef CoreFlowGraphics_
#define CoreFlowGraphics_

#pragma GCC visibility push(default)

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include <string>

struct Color {
    uint8_t red, green, blue, alpha;
};

class CoreFlowGraphics {
public:
    CoreFlowGraphics();
    Color flow(void* texturePointer);
};

#pragma GCC visibility pop
#endif
