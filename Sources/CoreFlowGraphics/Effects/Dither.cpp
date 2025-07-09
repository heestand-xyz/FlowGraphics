//
//  Dither.cpp
//  FlowGraphics
//
//  Created by Anton Heestand on 2025-07-09.
//

#include "Dither.h"
#include <vector>
#include <mdspan>
#include <iostream>
#include <cmath>

Dither::Dither() {}

void Dither::floydSteinberg(void* graphic,
                            size_t length,
                            FGSize resolution,
                            int64_t levels) {
    auto pixels = std::mdspan<uint8_t, std::dextents<size_t, 2>>(static_cast<uint8_t*>(graphic), resolution.height, resolution.width);
    
    auto clamp = [](int value, int max) {
        return std::max(0, std::min(value, max));
    };
    
    for (int y = 0; y < resolution.height; ++y) {
        for (int x = 0; x < resolution.width; ++x) {
            uint8_t old_pixel = pixels[y, x];
            uint8_t new_pixel = std::round(float(old_pixel) * (levels - 1) / 255.0f) * (255 / (levels - 1));
            int error = old_pixel - new_pixel;
            pixels[y, x] = new_pixel;
            
            if (x + 1 < resolution.width) pixels[y, x + 1] = clamp(pixels[y, x + 1] + error * 7 / 16, 255);
            if (x > 0 && y + 1 < resolution.height) pixels[y + 1, x - 1] = clamp(pixels[y + 1, x - 1] + error * 3 / 16, 255);
            if (y + 1 < resolution.height) pixels[y + 1, x] = clamp(pixels[y + 1, x] + error * 5 / 16, 255);
            if (x + 1 < resolution.width && y + 1 < resolution.height) pixels[y + 1, x + 1] = clamp(pixels[y + 1, x + 1] + error * 1 / 16, 255);
        }
    }
}

void Dither::atkinson(void* graphic,
                      size_t length,
                      FGSize resolution,
                      int64_t levels) {
    
    auto pixels = std::mdspan<uint8_t, std::dextents<size_t, 2>>(static_cast<uint8_t*>(graphic), resolution.height, resolution.width);
    
    auto clamp = [](int value, int max) {
        return std::max(0, std::min(value, max));
    };
    
    for (int y = 0; y < resolution.height; ++y) {
        for (int x = 0; x < resolution.width; ++x) {
            uint8_t old_pixel = pixels[y, x];
            uint8_t new_pixel = std::round(float(old_pixel) * (levels - 1) / 255.0f) * (255 / (levels - 1));
            int error = old_pixel - new_pixel;
            pixels[y, x] = new_pixel;
            
            const int q = error / 8;
            if (x + 1 < resolution.width) pixels[y, x + 1] = clamp(pixels[y, x + 1] + q, 255);
            if (x + 2 < resolution.width) pixels[y, x + 2] = clamp(pixels[y, x + 2] + q, 255);
            if (y + 1 < resolution.height) {
                if (x > 0) pixels[y + 1, x - 1] = clamp(pixels[y + 1, x - 1] + q, 255);
                pixels[y + 1, x] = clamp(pixels[y + 1, x] + q, 255);
                if (x + 1 < resolution.width) pixels[y + 1, x + 1] = clamp(pixels[y + 1, x + 1] + q, 255);
            }
            if (y + 2 < resolution.height) pixels[y + 2, x] = clamp(pixels[y + 2, x] + q, 255);
        }
    }
}
