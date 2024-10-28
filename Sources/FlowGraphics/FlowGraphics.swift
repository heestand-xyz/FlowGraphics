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
    
    public func flow() {
        let texturePointer = UnsafeMutableRawPointer(Unmanaged.passUnretained(texture).toOpaque())
        var cfgs = CoreFlowGraphics()
        let color = cfgs.flow(texturePointer)
        let pixelColor = PixelColor(
            red255: Int(color.red),
            green255: Int(color.green),
            blue255: Int(color.blue),
            alpha255: Int(color.alpha)
        )
        print("Pixel Color:", pixelColor)
    }
}
