#include "XenonInput.h"
#include "XenonRenderer.h"
#include "ThirdPersonCamera.h"
#include <xenon_soc/xenon_power.h>

int main() {
    XenonInput input;
    XenonRenderer renderer;
    ThirdPersonCamera camera;

    input.init();
    if (!renderer.init()) return 1;

    Vec3 eva{0, 0.75f, 0};
    Vec3 velocity{};
    const Mat4 projection = perspective(60.0f * 3.14159265f / 180.0f, 16.0f/9.0f, 0.1f, 100.0f);

    for (;;) {
        constexpr float dt = 1.0f / 60.0f;
        const PadState pad = input.poll();
        camera.update(eva, pad.lookX, pad.lookY, dt);

        Vec3 move = camera.rightOnGround() * pad.moveX + camera.forwardOnGround() * pad.moveY;
        eva = eva + move * (4.0f * dt);
        if (pad.jump && eva.y <= 0.76f) velocity.y = 5.5f;
        velocity.y -= 12.0f * dt;
        eva.y += velocity.y * dt;
        if (eva.y < 0.75f) { eva.y = 0.75f; velocity.y = 0; }

        const Mat4 viewProjection = multiply(camera.view(), projection);
        renderer.beginFrame();

        renderer.drawCube({eva, {0.8f,1.5f,0.8f}, 0xFFFF69B4}, viewProjection);
        for (int z=-5; z<=5; ++z)
            for (int x=-5; x<=5; ++x)
                renderer.drawCube({{float(x)*2.0f,-0.25f,float(z)*2.0f},{1.9f,0.5f,1.9f},((x+z)&1)?0xFFFFC0E0:0xFFFFE4F1}, viewProjection);

        renderer.endFrame();
        xenon_yield();
    }
}
