// swift-tools-version: 6.0

import PackageDescription

let package = Package(
    name: "FlowGraphics",
    platforms: [
        .iOS(.v17),
        .tvOS(.v17),
        .macOS(.v14),
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
            publicHeadersPath: "include",
            cxxSettings: [
                .headerSearchPath("include"),
                .unsafeFlags(["-std=c++2b"])
            ]),
        .testTarget(
            name: "FlowGraphicsTests",
            dependencies: ["FlowGraphics"],
            swiftSettings: [.interoperabilityMode(.Cxx)]),
    ],
    cxxLanguageStandard: .cxx2b
)
