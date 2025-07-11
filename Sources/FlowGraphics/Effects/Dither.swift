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
        levels: Int = 2,
        monochrome: Bool = true
    ) async throws -> Graphic {
        
        try await rowAlign { graphic in
            
            let fgResolution = FGSize(
                width: graphic.texture.width,
                height: graphic.texture.height
            )
            
            return try await graphic.effect { data, length in
                var dither = Dither()
                dither.floydSteinberg(
                    data,
                    length,
                    fgResolution,
                    Int64(min(max(levels, 2), 256)),
                    monochrome
                )
            }
        }
    }
    
    public func ditherAtkinson(
        levels: Int = 2,
        monochrome: Bool = true
    ) async throws -> Graphic {
        
        try await rowAlign { graphic in
            
            let fgResolution = FGSize(
                width: graphic.texture.width,
                height: graphic.texture.height
            )
            
            return try await graphic.effect { data, length in
                var dither = Dither()
                dither.atkinson(
                    data,
                    length,
                    fgResolution,
                    Int64(min(max(levels, 2), 256)),
                    monochrome
                )
            }
        }
    }
}
