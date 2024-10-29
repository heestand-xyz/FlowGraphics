//
//  FlowGraphics.swift
//  FlowGraphics
//
//  Created by a-heestand on 2024/10/29.
//

import Metal
import PixelColor
import AsyncGraphics
import CoreFlowGraphics

extension Graphic {
    
    /// Top left color of the graphic.
    /// The graphics must be 8 bit (default).
    public func topLeftPixelColor() -> PixelColor {
        precondition(bits == ._8)
        let texturePointer = UnsafeMutableRawPointer(Unmanaged.passUnretained(texture).toOpaque())
        var cfgs = CoreFlowGraphics()
        let color = cfgs.flow(texturePointer)
        return PixelColor(
            red255: Int(color.red),
            green255: Int(color.green),
            blue255: Int(color.blue),
            alpha255: Int(color.alpha)
        )
    }
}
