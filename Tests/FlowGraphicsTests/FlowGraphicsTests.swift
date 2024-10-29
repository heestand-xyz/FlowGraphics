import Testing
@testable import FlowGraphics
import CoreGraphics
import AsyncGraphics
import PixelColor

@Test func example() async throws {
    let graphic: Graphic = try await .color(.orange, resolution: CGSize(width: 1, height: 1))
    try await #expect(graphic.topLeftPixelColor() == PixelColor.orange)
}
