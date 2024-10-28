//
//  CoreFlowGraphics.cpp
//  CoreFlowGraphics
//
//  Created by a-heestand on 2024/10/29.
//

#include "CoreFlowGraphics.h"
#include <iostream>
#include <array>

CoreFlowGraphics::CoreFlowGraphics() {}

Color CoreFlowGraphics::flow(void* texturePointer) {
    auto* texture = static_cast<MTL::Texture*>(texturePointer);

    if (texture->pixelFormat() != MTL::PixelFormatRGBA8Unorm) {
        return {0, 0, 0, 0};
    }
    
    std::array<uint8_t, 4> pixelData = {0, 0, 0, 0};
    MTL::Region region = MTL::Region::Make2D(0, 0, 1, 1);
    
    texture->getBytes(&pixelData, 4, region, 0);
    
    return {pixelData[0], pixelData[1], pixelData[2], pixelData[3]};
}
