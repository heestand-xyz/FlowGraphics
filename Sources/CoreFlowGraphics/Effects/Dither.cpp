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
                            int64_t levels,
                            bool monochrome) {
    auto pixels = std::mdspan<uint8_t, std::dextents<size_t, 3>>(static_cast<uint8_t*>(graphic), resolution.height, resolution.width, 4);
    
    auto clamp = [](int value, int max) {
        return std::max(0, std::min(value, max));
    };
    
    for (int y = 0; y < resolution.height; ++y) {
        for (int x = 0; x < resolution.width; ++x) {
            if (monochrome) {
                // Use only the red component for grayscale
                uint8_t old_pixel = pixels[y, x, 0];
                uint8_t new_pixel = std::round(float(old_pixel) * (levels - 1) / 255.0f) * (255 / (levels - 1));
                int error = old_pixel - new_pixel;
                pixels[y, x, 0] = new_pixel;
                pixels[y, x, 1] = new_pixel;
                pixels[y, x, 2] = new_pixel;
                // Alpha unchanged
                if (x + 1 < resolution.width) {
                    for (int c = 0; c < 3; ++c)
                        pixels[y, x + 1, c] = clamp(pixels[y, x + 1, c] + error * 7 / 16, 255);
                }
                if (x > 0 && y + 1 < resolution.height) {
                    for (int c = 0; c < 3; ++c)
                        pixels[y + 1, x - 1, c] = clamp(pixels[y + 1, x - 1, c] + error * 3 / 16, 255);
                }
                if (y + 1 < resolution.height) {
                    for (int c = 0; c < 3; ++c)
                        pixels[y + 1, x, c] = clamp(pixels[y + 1, x, c] + error * 5 / 16, 255);
                }
                if (x + 1 < resolution.width && y + 1 < resolution.height) {
                    for (int c = 0; c < 3; ++c)
                        pixels[y + 1, x + 1, c] = clamp(pixels[y + 1, x + 1, c] + error * 1 / 16, 255);
                }
            } else {
                for (int c = 0; c < 3; ++c) { // R,G,B
                    uint8_t old_pixel = pixels[y, x, c];
                    uint8_t new_pixel = std::round(float(old_pixel) * (levels - 1) / 255.0f) * (255 / (levels - 1));
                    int error = old_pixel - new_pixel;
                    pixels[y, x, c] = new_pixel;
                    if (x + 1 < resolution.width)
                        pixels[y, x + 1, c] = clamp(pixels[y, x + 1, c] + error * 7 / 16, 255);
                    if (x > 0 && y + 1 < resolution.height)
                        pixels[y + 1, x - 1, c] = clamp(pixels[y + 1, x - 1, c] + error * 3 / 16, 255);
                    if (y + 1 < resolution.height)
                        pixels[y + 1, x, c] = clamp(pixels[y + 1, x, c] + error * 5 / 16, 255);
                    if (x + 1 < resolution.width && y + 1 < resolution.height)
                        pixels[y + 1, x + 1, c] = clamp(pixels[y + 1, x + 1, c] + error * 1 / 16, 255);
                }
            }
        }
    }
}

void Dither::atkinson(void* graphic,
                      size_t length,
                      FGSize resolution,
                      int64_t levels,
                      bool monochrome) {
    auto pixels = std::mdspan<uint8_t, std::dextents<size_t, 3>>(static_cast<uint8_t*>(graphic), resolution.height, resolution.width, 4);

    auto clamp = [](int value, int max) {
        return std::max(0, std::min(value, max));
    };

    for (int y = 0; y < resolution.height; ++y) {
        for (int x = 0; x < resolution.width; ++x) {
            if (monochrome) {
                uint8_t old_pixel = pixels[y, x, 0];
                uint8_t new_pixel = std::round(float(old_pixel) * (levels - 1) / 255.0f) * (255 / (levels - 1));
                int error = old_pixel - new_pixel;
                pixels[y, x, 0] = new_pixel;
                pixels[y, x, 1] = new_pixel;
                pixels[y, x, 2] = new_pixel;
                // Alpha unchanged
                const int q = error / 8;
                if (x + 1 < resolution.width)
                    for (int c = 0; c < 3; ++c)
                        pixels[y, x + 1, c] = clamp(pixels[y, x + 1, c] + q, 255);
                if (x + 2 < resolution.width)
                    for (int c = 0; c < 3; ++c)
                        pixels[y, x + 2, c] = clamp(pixels[y, x + 2, c] + q, 255);
                if (y + 1 < resolution.height) {
                    if (x > 0)
                        for (int c = 0; c < 3; ++c)
                            pixels[y + 1, x - 1, c] = clamp(pixels[y + 1, x - 1, c] + q, 255);
                    for (int c = 0; c < 3; ++c)
                        pixels[y + 1, x, c] = clamp(pixels[y + 1, x, c] + q, 255);
                    if (x + 1 < resolution.width)
                        for (int c = 0; c < 3; ++c)
                            pixels[y + 1, x + 1, c] = clamp(pixels[y + 1, x + 1, c] + q, 255);
                }
                if (y + 2 < resolution.height)
                    for (int c = 0; c < 3; ++c)
                        pixels[y + 2, x, c] = clamp(pixels[y + 2, x, c] + q, 255);
            } else {
                for (int c = 0; c < 3; ++c) {
                    uint8_t old_pixel = pixels[y, x, c];
                    uint8_t new_pixel = std::round(float(old_pixel) * (levels - 1) / 255.0f) * (255 / (levels - 1));
                    int error = old_pixel - new_pixel;
                    pixels[y, x, c] = new_pixel;
                    const int q = error / 8;
                    if (x + 1 < resolution.width)
                        pixels[y, x + 1, c] = clamp(pixels[y, x + 1, c] + q, 255);
                    if (x + 2 < resolution.width)
                        pixels[y, x + 2, c] = clamp(pixels[y, x + 2, c] + q, 255);
                    if (y + 1 < resolution.height) {
                        if (x > 0)
                            pixels[y + 1, x - 1, c] = clamp(pixels[y + 1, x - 1, c] + q, 255);
                        pixels[y + 1, x, c] = clamp(pixels[y + 1, x, c] + q, 255);
                        if (x + 1 < resolution.width)
                            pixels[y + 1, x + 1, c] = clamp(pixels[y + 1, x + 1, c] + q, 255);
                    }
                    if (y + 2 < resolution.height)
                        pixels[y + 2, x, c] = clamp(pixels[y + 2, x, c] + q, 255);
                }
            }
        }
    }
}
