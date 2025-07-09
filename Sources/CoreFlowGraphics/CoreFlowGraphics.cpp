//
//  CoreFlowGraphics.cpp
//  CoreFlowGraphics
//
//  Created by a-heestand on 2024/10/29.
//

#include "CoreFlowGraphics.h"
#include <vector>
#include <mdspan>
#include <iostream>
#include <cmath>

CoreFlowGraphics::CoreFlowGraphics() {}

double checkContrast(const FGColor& leading, const FGColor& trailing) {
    double red = double(leading.red) / 255 - double(trailing.red) / 255;
    double green = double(leading.green) / 255 - double(trailing.green) / 255;
    double blue = double(leading.blue) / 255 - double(trailing.blue) / 255;
    double alpha = double(leading.alpha) / 255 - double(trailing.alpha) / 255;
    return std::sqrt(red * red + green * green + blue * blue + alpha * alpha);
}

void CoreFlowGraphics::floodFill(void* graphic,
                                 size_t length,
                                 FGSize resolution,
                                 FGColor color,
                                 FGPoint location,
                                 double threshold) {
    if (location.x < 0 || location.x >= resolution.width || location.y < 0 || location.y >= resolution.height) {
        return;
    }

    using byte = unsigned char;
    constexpr size_t channels = 4;
    
    size_t expectedLength = resolution.count() * channels;
    if (length < expectedLength) {
        return;
    }
    
    byte* pixels = static_cast<byte*>(graphic);
    
    constexpr size_t channel_extent = channels;
    using extents_t = std::extents<size_t, std::dynamic_extent, std::dynamic_extent, channel_extent>;
    using mdspan_type = std::mdspan<byte, extents_t>;
    
    mdspan_type image(pixels, resolution.height, resolution.width);
    
    size_t seedRed = image[location.y, location.x, 0];
    size_t seedGreen = image[location.y, location.x, 1];
    size_t seedBlue = image[location.y, location.x, 2];
    size_t seedAlpha = image[location.y, location.x, 3];
    FGColor seedColor = FGColor(seedRed, seedGreen, seedBlue, seedAlpha);

    using visited_extents_t = std::extents<size_t, std::dynamic_extent, std::dynamic_extent>;
    using visited_mdspan_type = std::mdspan<bool, visited_extents_t>;

    auto flatVisited = std::make_unique<bool[]>(resolution.count());

    std::fill_n(flatVisited.get(), resolution.count(), false);

    visited_mdspan_type visited(flatVisited.get(), resolution.height, resolution.width);

    const int dx[4] = {0, 0, -1, 1};
    const int dy[4] = {-1, 1, 0, 0};

    visited[location.y, location.x] = true;

    std::vector<std::pair<int, int>> pixelQueue;
    pixelQueue.push_back({location.x, location.y});

    while (!pixelQueue.empty()) {
        auto [x, y] = pixelQueue.front();
        pixelQueue.erase(pixelQueue.begin());

        image[y, x, 0] = color.red;
        image[y, x, 1] = color.green;
        image[y, x, 2] = color.blue;
        image[y, x, 3] = color.alpha;

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < resolution.width && ny >= 0 && ny < resolution.height && !visited[ny, nx]) {

                size_t sampleRed = image[ny, nx, 0];
                size_t sampleGreen = image[ny, nx, 1];
                size_t sampleBlue = image[ny, nx, 2];
                size_t sampleAlpha = image[ny, nx, 3];
                FGColor sampleColor = FGColor(sampleRed, sampleGreen, sampleBlue, sampleAlpha);

                double contrast = checkContrast(seedColor, sampleColor);

                if (contrast < threshold) {
                    pixelQueue.push_back({nx, ny});
                    visited[ny, nx] = true;
                }
            }
        }
    }
}
