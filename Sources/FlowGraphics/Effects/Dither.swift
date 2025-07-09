//
//  Dither.swift
//  FlowGraphics
//
//  Created by Anton Heestand on 2025-07-09.
//

import Metal
import PixelColor
import AsyncGraphics
import CoreFlowGraphics

extension Graphic {
    
    public func ditherFloydSteinberg(
        levels: Int = 2
    ) async throws -> Graphic {
        
        let fgResolution = FGSize(
            width: texture.width,
            height: texture.height
        )
        
        return try await effect { data, length in
            var dither = Dither()
            dither.floydSteinberg(
                data,
                length,
                fgResolution,
                Int64(min(max(levels, 2), 256))
            )
        }
    }
    
    public func ditherAtkinson(
        levels: Int = 2
    ) async throws -> Graphic {
        
        let fgResolution = FGSize(
            width: texture.width,
            height: texture.height
        )
        
        return try await effect { data, length in
            var dither = Dither()
            dither.atkinson(
                data,
                length,
                fgResolution,
                Int64(min(max(levels, 2), 256))
            )
        }
    }
}
