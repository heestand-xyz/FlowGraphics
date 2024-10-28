import Testing
@testable import FlowGraphics
import CoreGraphics
import AsyncGraphics

@Test func example() async throws {
    let graphic: Graphic = try await .color(.orange, resolution: CGSize(width: 1, height: 1))
    graphic.flow()
}
