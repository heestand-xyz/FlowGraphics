// swift-tools-version: 6.0

import PackageDescription

let package = Package(
    name: "FlowGraphics",
    platforms: [
        .iOS(.v16),
        .tvOS(.v16),
        .macOS(.v13),
        .visionOS(.v1)
    ],
    products: [
        .library(
            name: "FlowGraphics",
            targets: ["FlowGraphics"]),
    ],
    dependencies: [
        .package(path: "../AsyncGraphics")
    ],
    targets: [
        .target(
            name: "FlowGraphics",
            dependencies: ["AsyncGraphics", "CoreFlowGraphics"],
            swiftSettings: [.interoperabilityMode(.Cxx)]),
        .target(
            name: "CoreFlowGraphics",
            cxxSettings: [
                .unsafeFlags(["-std=c++2b"])
            ]),
        .testTarget(
            name: "FlowGraphicsTests",
            dependencies: ["FlowGraphics"],
            swiftSettings: [.interoperabilityMode(.Cxx)]),
    ],
    cxxLanguageStandard: .cxx2b
)
