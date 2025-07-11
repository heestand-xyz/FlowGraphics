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
    
    public enum HistogramChannel: Int {
        case red
        case green
        case blue
        case alpha
    }
    
    /// Histogram
    /// - Parameter channel: Red, green, blue or alpha.
    /// - Returns: An array of 256 values, each with a count of pixels with the brightness (index).
    public func histogram(
        channel: HistogramChannel
    ) async throws -> [Int] {
        
        let fgResolution = FGSize(
            width: texture.width,
            height: texture.height
        )
        
        let buffer: MTLBuffer = try await buffer()
    
        var histogram = Histogram()
        let histogramPointer = histogram.histogramChannel(
            buffer.contents(),
            buffer.length,
            fgResolution,
            channel.rawValue
        )
        
        return Array(UnsafeBufferPointer(start: histogramPointer, count: 256))
    }
    
    /// Histogram
    /// - Parameter channel: Red, green, blue or alpha.
    /// - Returns: An array of 256 values, each with a count of pixels with the brightness (index).
    public func histogram(
        normalize: Bool
    ) async throws -> [PixelColor] {
        
        let fgResolution = FGSize(
            width: texture.width,
            height: texture.height
        )
        
        let buffer: MTLBuffer = try await buffer()
    
        var histogram = Histogram()
        let histogramPointer = histogram.histogramColor(
            buffer.contents(),
            buffer.length,
            fgResolution
        )
        
        let rgbValues: [Int] = Array(UnsafeBufferPointer(start: histogramPointer, count: 256 * 3))
        
        let rValues = Array(rgbValues[0..<256])
        let gValues = Array(rgbValues[256..<512])
        let bValues = Array(rgbValues[512..<768])

        let maxValue: Int = if normalize {
            rgbValues.max() ?? 1
        } else { 1 }

        let colors: [PixelColor] = (0..<256).map { i in
            var color = PixelColor(
                red: CGFloat(rValues[i]),
                green: CGFloat(gValues[i]),
                blue: CGFloat(bValues[i])
            )
            if normalize {
                color /= CGFloat(maxValue)
            }
            return color
        }

        return colors
    }
}
