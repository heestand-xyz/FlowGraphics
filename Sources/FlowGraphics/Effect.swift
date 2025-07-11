//
//  Effect.swift
//  FlowGraphics
//
//  Created by Anton Heestand on 2025-07-09.
//

import Metal
import PixelColor
import AsyncGraphics
import CoreFlowGraphics

extension Graphic {
    
    enum EffectError: LocalizedError {
        case requires8BitGraphic
        case failedToCreateSharedBuffer
        case failedToCreateCommandQueue
        case failedToCreateCommandBuffer
        case failedToCreateBlitEncoder
        case failedToCreateTexture
        var errorDescription: String? {
            switch self {
            case .requires8BitGraphic:
                "Effect requires an 8 bit graphic."
            case .failedToCreateSharedBuffer:
                "Effect failed to create shared buffer."
            case .failedToCreateCommandQueue:
                "Effect failed to create command queue."
            case .failedToCreateCommandBuffer:
                "Effect failed to create command buffer."
            case .failedToCreateBlitEncoder:
                "Effect failed to create blit encoder."
            case .failedToCreateTexture:
                "Effect failed to create texture."
            }
        }
    }
    
    public func rowAlign(
        every pixelsPerRow: Int = 4,
        _ effect: (_ graphic: Graphic) async throws -> Graphic,
    ) async throws -> Graphic {
        if texture.width % pixelsPerRow == 0 {
            return try await effect(self)
        } else {
            let resizedResolution = CGSize(width: ceil(width / CGFloat(pixelsPerRow)) * CGFloat(pixelsPerRow), height: height)
            let resizedGraphic = try await resized(to: resizedResolution, placement: .stretch)
            let effectedGraphic = try await effect(resizedGraphic)
            return try await effectedGraphic.resized(to: resolution, placement: .stretch)
        }
    }
    
    public func effect(
        _ edit: (_ data: UnsafeMutableRawPointer, _ length: Int) -> Void,
    ) async throws -> Graphic {
        
        let buffer: MTLBuffer = try await buffer()
        
        let dataPointer = buffer.contents()
        let dataLength = buffer.length

        edit(dataPointer, dataLength)
        
        let textureDescriptor = MTLTextureDescriptor.texture2DDescriptor(
            pixelFormat: .rgba8Unorm,
            width: texture.width,
            height: texture.height,
            mipmapped: false
        )
        textureDescriptor.storageMode = .shared
        textureDescriptor.usage = [.renderTarget, .shaderRead]

        guard let newTexture = buffer.makeTexture(
            descriptor: textureDescriptor,
            offset: 0,
            bytesPerRow: texture.width * 4
        ) else {
            throw EffectError.failedToCreateTexture
        }

        return try .texture(newTexture)
    }
    
    public func effectWithOriginal(
        backgroundColor: PixelColor,
        _ edit: (_ originalData: UnsafeMutableRawPointer, _ targetData: UnsafeMutableRawPointer, _ length: Int) -> Void,
    ) async throws -> Graphic {
        
        let originalBuffer: MTLBuffer = try await buffer()
        let targetBuffer: MTLBuffer = try await Graphic.color(backgroundColor, resolution: resolution).buffer()
        
        let originalDataPointer = originalBuffer.contents()
        let targetDataPointer = targetBuffer.contents()
        let dataLength = originalBuffer.length

        edit(originalDataPointer, targetDataPointer, dataLength)
        
        let textureDescriptor = MTLTextureDescriptor.texture2DDescriptor(
            pixelFormat: .rgba8Unorm,
            width: texture.width,
            height: texture.height,
            mipmapped: false
        )
        textureDescriptor.storageMode = .shared
        textureDescriptor.usage = [.renderTarget, .shaderRead]

        guard let newTexture = targetBuffer.makeTexture(
            descriptor: textureDescriptor,
            offset: 0,
            bytesPerRow: texture.width * 4
        ) else {
            throw EffectError.failedToCreateTexture
        }

        return try .texture(newTexture)
    }
    
    func buffer() async throws -> MTLBuffer {
        guard bits == ._8 else {
            throw EffectError.requires8BitGraphic
        }
        
        let pixelByteCount = texture.width * texture.height * 4
        guard let buffer: MTLBuffer = Renderer.metalDevice.makeBuffer(length: pixelByteCount, options: .storageModeShared) else {
            throw EffectError.failedToCreateSharedBuffer
        }
        
        guard let commandQueue = Renderer.metalDevice.makeCommandQueue() else {
            throw EffectError.failedToCreateCommandQueue
        }

        guard let commandBuffer = commandQueue.makeCommandBuffer() else {
            throw EffectError.failedToCreateCommandBuffer
        }

        guard let blitEncoder = commandBuffer.makeBlitCommandEncoder() else {
            throw EffectError.failedToCreateBlitEncoder
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
        
        await withCheckedContinuation { continuation in
            commandBuffer.addCompletedHandler { _ in
                continuation.resume()
            }
            commandBuffer.commit()
        }
        
        return buffer
    }
}
