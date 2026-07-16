#pragma once
#include "Math3D.h"
#include <xenos/xe.h>

struct CubeInstance {
    Vec3 position;
    Vec3 scale{1,1,1};
    unsigned int color{0xFFFF80C0};
};

class XenonRenderer {
public:
    bool init();
    void beginFrame();
    void drawCube(const CubeInstance& cube, const Mat4& viewProjection);
    void endFrame();

private:
    Mat4 makeWorld(const CubeInstance& cube) const;

    XenosDevice xe_{};
    XenosVertexBuffer* vertexBuffer_{};
    XenosIndexBuffer* indexBuffer_{};
    XenosShader vertexShader_{};
    XenosShader pixelShader_{};
    bool shadersReady_{false};
};
