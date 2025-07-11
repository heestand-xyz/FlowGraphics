//
//  FloodFill.swift
//  FlowGraphics
//
//  Created by Anton Heestand on 2024-10-29.
//

import Metal
import PixelColor
import AsyncGraphics
import CoreFlowGraphics

extension Graphic {
    
    enum FloodFillError: LocalizedError {
        case pixelLocationOutOfBounds
        var errorDescription: String? {
            switch self {
            case .pixelLocationOutOfBounds:
                "Pixel location is out of bounds."
            }
        }
    }
    
    public func floodFill(
        color: PixelColor,
        backgroundColor: PixelColor = .clear,
        at location: CGPoint,
        threshold: CGFloat = 0.5
    ) async throws -> Graphic {
        
        let fgColor = FGColor(
            red: UInt8(min(max(color.red, 0.0), 1.0) * 255),
            green: UInt8(min(max(color.green, 0.0), 1.0) * 255),
            blue: UInt8(min(max(color.blue, 0.0), 1.0) * 255),
            alpha: UInt8(min(max(color.opacity, 0.0), 1.0) * 255)
        )
        
        return try await rowAlign { graphic in
            
            let fgResolution = FGSize(
                width: graphic.texture.width,
                height: graphic.texture.height
            )
            
            guard Int(location.x) >= 0, Int(location.x) < graphic.texture.width,
                  Int(location.y) >= 0, Int(location.y) < graphic.texture.height else {
                throw FloodFillError.pixelLocationOutOfBounds
            }
            
            let fgLocation = FGPoint(
                x: Int(location.x),
                y: Int(location.y)
            )
            
            return try await graphic.effectWithOriginal(backgroundColor: backgroundColor) { original, target, length in
                var floodFill = FloodFill()
                floodFill.floodFill(
                    original,
                    target,
                    length,
                    fgResolution,
                    fgColor,
                    fgLocation,
                    threshold
                )
            }
        }
    }
}
