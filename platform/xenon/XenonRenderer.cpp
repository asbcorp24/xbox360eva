#include "XenonRenderer.h"
#include <xenos/xenos.h>
#include <cstring>

struct Vertex { float x,y,z; unsigned int color; };

static const Vertex kCubeVertices[] = {
    {-0.5f,-0.5f,-0.5f,0xFFFF80C0},{0.5f,-0.5f,-0.5f,0xFFFF80C0},
    {0.5f,0.5f,-0.5f,0xFFFF80C0},{-0.5f,0.5f,-0.5f,0xFFFF80C0},
    {-0.5f,-0.5f,0.5f,0xFFFF80C0},{0.5f,-0.5f,0.5f,0xFFFF80C0},
    {0.5f,0.5f,0.5f,0xFFFF80C0},{-0.5f,0.5f,0.5f,0xFFFF80C0}
};

static const unsigned short kCubeIndices[] = {
    0,1,2, 0,2,3, 4,6,5, 4,7,6,
    0,4,5, 0,5,1, 3,2,6, 3,6,7,
    1,5,6, 1,6,2, 0,3,7, 0,7,4
};

bool XenonRenderer::init() {
    xenos_init(VIDEO_MODE_AUTO);
    Xe_Init(&xe_);
    Xe_SetRenderTarget(&xe_, Xe_GetFramebufferSurface(&xe_));
    Xe_SetClearColor(&xe_, 0xFFFFB6D9);
    Xe_SetCullMode(&xe_, XE_CULL_CCW);
    Xe_SetZEnable(&xe_, 1);
    Xe_SetZWrite(&xe_, 1);
    Xe_SetZFunc(&xe_, XE_CMP_LESSEQUAL);

    vertexBuffer_ = Xe_CreateVertexBuffer(&xe_, sizeof(kCubeVertices));
    indexBuffer_ = Xe_CreateIndexBuffer(&xe_, sizeof(kCubeIndices), XE_FMT_INDEX16);
    if (!vertexBuffer_ || !indexBuffer_) return false;
    std::memcpy(Xe_VB_Lock(&xe_, vertexBuffer_, 0, sizeof(kCubeVertices), XE_LOCK_WRITE), kCubeVertices, sizeof(kCubeVertices));
    Xe_VB_Unlock(&xe_, vertexBuffer_);
    std::memcpy(Xe_IB_Lock(&xe_, indexBuffer_, 0, sizeof(kCubeIndices), XE_LOCK_WRITE), kCubeIndices, sizeof(kCubeIndices));
    Xe_IB_Unlock(&xe_, indexBuffer_);
    return true;
}

void XenonRenderer::beginFrame() {
    Xe_SetClearColor(&xe_, 0xFFFFB6D9);
    Xe_Clear(&xe_, XE_CLEAR_COLOR | XE_CLEAR_DS);
}

void XenonRenderer::drawCube(const CubeInstance&, const Mat4&) {
    // Геометрия и GPU-буферы уже подключены. Для фактического вывода нужен
    // скомпилированный Xenos vertex/pixel shader. Он добавляется следующим
    // файлом assets/shaders/cube.* и перед draw вызываются Xe_SetShader,
    // Xe_SetVertexShaderConstantF, Xe_SetStreamSource и Xe_DrawIndexedPrimitive.
}

void XenonRenderer::endFrame() {
    Xe_Resolve(&xe_);
    Xe_Execute(&xe_);
    Xe_Sync(&xe_);
}
