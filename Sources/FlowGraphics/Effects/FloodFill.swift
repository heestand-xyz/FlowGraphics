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
        
        let fgResolution = FGSize(
            width: texture.width,
            height: texture.height
        )
        
        let fgColor = FGColor(
            red: UInt8(color.red * 255),
            green: UInt8(color.green * 255),
            blue: UInt8(color.blue * 255),
            alpha: UInt8(color.opacity * 255)
        )
        
        guard Int(location.x) >= 0, Int(location.x) < texture.width,
              Int(location.y) >= 0, Int(location.y) < texture.height else {
            throw FloodFillError.pixelLocationOutOfBounds
        }
        
        let fgLocation = FGPoint(
            x: Int(location.x),
            y: Int(location.y)
        )
        
        return try await effectWithOriginal(backgroundColor: backgroundColor) { original, target, length in
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
