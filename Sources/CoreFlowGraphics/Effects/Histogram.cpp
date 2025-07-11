//
//  Histogram.cpp
//  FlowGraphics
//
//  Created by Anton Heestand on 2025-07-09.
//

#include "Histogram.h"
#include <vector>
#include <mdspan>
#include <iostream>
#include <cmath>
#include <algorithm>

Histogram::Histogram() {}

const size_t* Histogram::histogramColor(void* data,
                                        size_t length,
                                        FGSize resolution) {
    static size_t histogram[256 * 3] = {0};

    std::fill(std::begin(histogram), std::end(histogram), 0);

    uint8_t* pixels = static_cast<uint8_t*>(data);
    size_t pixelCount = resolution.width * resolution.height;
    size_t stride = length / pixelCount;

    for (size_t i = 0; i < pixelCount; ++i) {
        size_t index = i * stride;
        if (index + 2 < length) {
            uint8_t r = pixels[index + 0];
            uint8_t g = pixels[index + 1];
            uint8_t b = pixels[index + 2];
            histogram[r + 0]++;
            histogram[g + 256]++;
            histogram[b + 512]++;
        }
    }

    return histogram;
}

const size_t* Histogram::histogramChannel(void* data,
                                          size_t length,
                                          FGSize resolution,
                                          size_t channel) {
    static size_t histogram[256] = {0};

    std::fill(std::begin(histogram), std::end(histogram), 0);

    uint8_t* pixels = static_cast<uint8_t*>(data);
    size_t pixelCount = resolution.width * resolution.height;
    size_t stride = length / pixelCount;

    for (size_t i = 0; i < pixelCount; ++i) {
        size_t index = i * stride + channel;
        if (index < length) {
            uint8_t value = pixels[index];
            histogram[value]++;
        }
    }

    return histogram;
}
