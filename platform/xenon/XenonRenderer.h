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
    XenosDevice xe_{};
    XenosVertexBuffer* vertexBuffer_{};
    XenosIndexBuffer* indexBuffer_{};
};
