//
//  CoreFlowGraphics.cpp
//  CoreFlowGraphics
//
//  Created by a-heestand on 2024/10/29.
//

#include "CoreFlowGraphics.h"
#include <mdspan>

CoreFlowGraphics::CoreFlowGraphics() {}

void CoreFlowGraphics::flow(void* data, size_t size, size_t width, size_t height) {
    using byte = unsigned char;
    constexpr size_t channels = 4; // Assuming RGBA format
    
    // Verify that the data size matches the expected size
    size_t expectedSize = width * height * channels;
    if (size < expectedSize) {
        // Handle error: data size is smaller than expected
        return;
    }
    
    byte* pixels = static_cast<byte*>(data);
    
    constexpr size_t channel_extent = channels; // Static extent for channels
    using extents_t = std::extents<size_t, std::dynamic_extent, std::dynamic_extent, channel_extent>;
    using mdspan_type = std::mdspan<byte, extents_t>;
    
    mdspan_type imageSpan(pixels, height, width);
    
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            byte& r = imageSpan[y, x, 0];
            byte& g = imageSpan[y, x, 1];
            byte& b = imageSpan[y, x, 2];
            byte& a = imageSpan[y, x, 3];
            r = r;
            g = g / 2;
            b = 0;
        }
    }
}
