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
    
    public func flow() async throws -> Graphic {
        precondition(bits == ._8)
        
        let pixelByteCount = texture.width * texture.height * 4
        guard let buffer = Renderer.metalDevice.makeBuffer(length: pixelByteCount, options: .storageModeShared) else {
            fatalError("Unable to create shared MTLBuffer")
        }
        
        guard let commandQueue = Renderer.metalDevice.makeCommandQueue(),
              let commandBuffer = commandQueue.makeCommandBuffer(),
              let blitEncoder = commandBuffer.makeBlitCommandEncoder() else {
            fatalError("Failed to create command buffer or blit encoder")
        }

        let bytesPerRow = texture.width * 4
        blitEncoder.copy(
            from: texture,
            sourceSlice: 0,
            sourceLevel: 0,
            sourceOrigin: MTLOrigin(x: 0, y: 0, z: 0),
            sourceSize: MTLSize(width: texture.width, height: texture.height, depth: 1),
            to: buffer,
            destinationOffset: 0,
            destinationBytesPerRow: bytesPerRow,
            destinationBytesPerImage: bytesPerRow * texture.height
        )
        blitEncoder.endEncoding()
        commandBuffer.commit()
        
        commandBuffer.waitUntilCompleted()
        
        let dataPointer = buffer.contents()
        let dataLength = buffer.length

        var cfgs = CoreFlowGraphics()
        cfgs.flow(dataPointer, dataLength, Int(width), Int(height))
        
        let textureDescriptor = MTLTextureDescriptor.texture2DDescriptor(
            pixelFormat: bits.metalPixelFormat(),
            width: texture.width,
            height: texture.height,
            mipmapped: false
        )
        textureDescriptor.storageMode = .shared
        textureDescriptor.usage = [.renderTarget, .shaderRead]

        guard let newTexture = buffer.makeTexture(
            descriptor: textureDescriptor,
            offset: 0,
            bytesPerRow: bytesPerRow
        ) else {
            fatalError("Failed to create texture from buffer")
        }

        return try .texture(newTexture)
    }
}
