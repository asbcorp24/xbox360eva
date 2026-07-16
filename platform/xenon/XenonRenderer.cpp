#include "XenonRenderer.h"
#include "GeneratedCubeShaders.h"

#include <xenos/xenos.h>
#include <cstring>

struct Vertex {
    float x, y, z;
    unsigned int color;
};

static const Vertex kCubeVertices[] = {
    {-0.5f,-0.5f,-0.5f,0xFFFF80C0},{ 0.5f,-0.5f,-0.5f,0xFFFF80C0},
    { 0.5f, 0.5f,-0.5f,0xFFFF80C0},{-0.5f, 0.5f,-0.5f,0xFFFF80C0},
    {-0.5f,-0.5f, 0.5f,0xFFFF80C0},{ 0.5f,-0.5f, 0.5f,0xFFFF80C0},
    { 0.5f, 0.5f, 0.5f,0xFFFF80C0},{-0.5f, 0.5f, 0.5f,0xFFFF80C0}
};

static const unsigned short kCubeIndices[] = {
    0,1,2, 0,2,3, 4,6,5, 4,7,6,
    0,4,5, 0,5,1, 3,2,6, 3,6,7,
    1,5,6, 1,6,2, 0,3,7, 0,7,4
};

Mat4 XenonRenderer::makeWorld(const CubeInstance& cube) const {
    Mat4 world = identity();
    world.m[0] = cube.scale.x;
    world.m[5] = cube.scale.y;
    world.m[10] = cube.scale.z;
    world.m[12] = cube.position.x;
    world.m[13] = cube.position.y;
    world.m[14] = cube.position.z;
    return world;
}

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

    void* vb = Xe_VB_Lock(&xe_, vertexBuffer_, 0, sizeof(kCubeVertices), XE_LOCK_WRITE);
    void* ib = Xe_IB_Lock(&xe_, indexBuffer_, 0, sizeof(kCubeIndices), XE_LOCK_WRITE);
    if (!vb || !ib) return false;
    std::memcpy(vb, kCubeVertices, sizeof(kCubeVertices));
    std::memcpy(ib, kCubeIndices, sizeof(kCubeIndices));
    Xe_VB_Unlock(&xe_, vertexBuffer_);
    Xe_IB_Unlock(&xe_, indexBuffer_);

    Xe_LoadShaderFromMemory(&xe_, &vertexShader_,
                            const_cast<unsigned char*>(kEvaCubeVertexShader));
    Xe_LoadShaderFromMemory(&xe_, &pixelShader_,
                            const_cast<unsigned char*>(kEvaCubePixelShader));
    Xe_InstantiateShader(&xe_, &vertexShader_, 0);
    Xe_InstantiateShader(&xe_, &pixelShader_, 0);
    Xe_ShaderApplyVFetchPatches(&xe_, &vertexShader_, 0, vertexBuffer_);
    shadersReady_ = true;
    return true;
}

void XenonRenderer::beginFrame() {
    Xe_SetClearColor(&xe_, 0xFFFFB6D9);
    Xe_Clear(&xe_, XE_CLEAR_COLOR | XE_CLEAR_DS);
}

void XenonRenderer::drawCube(const CubeInstance& cube, const Mat4& viewProjection) {
    if (!shadersReady_) return;

    const Mat4 world = makeWorld(cube);
    const Mat4 worldViewProjection = multiply(world, viewProjection);

    Xe_SetShader(&xe_, SHADER_TYPE_VERTEX, &vertexShader_, 0);
    Xe_SetShader(&xe_, SHADER_TYPE_PIXEL, &pixelShader_, 0);

    // The pinned LibXenon shader expects modelView in c0-c3 and modelWorld
    // in c4-c7. For this simple coloured cube both receive usable transforms.
    Xe_SetVertexShaderConstantF(&xe_, 0, worldViewProjection.m, 4);
    Xe_SetVertexShaderConstantF(&xe_, 4, world.m, 4);

    float lightDirection[4] = {-0.35f, -0.8f, -0.45f, 0.0f};
    Xe_SetPixelShaderConstantF(&xe_, 0, lightDirection, 1);

    Xe_SetStreamSource(&xe_, 0, vertexBuffer_, 0, sizeof(Vertex));
    Xe_SetIndices(&xe_, indexBuffer_);
    Xe_DrawIndexedPrimitive(&xe_, XE_PRIMTYPE_TRIANGLELIST,
                            0, 0, 8, 0, 12);
}

void XenonRenderer::endFrame() {
    Xe_Resolve(&xe_);
    Xe_Execute(&xe_);
    Xe_Sync(&xe_);
}
