#pragma once
#include "Math3D.h"

class ThirdPersonCamera {
public:
    void update(Vec3 target, float lookX, float lookY, float dt) {
        yaw_ += lookX * 2.4f * dt;
        pitch_ += lookY * 1.8f * dt;
        if (pitch_ > 0.75f) pitch_ = 0.75f;
        if (pitch_ < -0.25f) pitch_ = -0.25f;
        const float cp = std::cos(pitch_);
        Vec3 desired{
            target.x - std::sin(yaw_) * cp * distance_,
            target.y + height_ + std::sin(pitch_) * distance_,
            target.z - std::cos(yaw_) * cp * distance_
        };
        const float k = 1.0f - std::exp(-8.0f * dt);
        position_ = position_ + (desired - position_) * k;
        target_ = target + Vec3{0, 1.0f, 0};
    }
    Mat4 view() const { return lookAt(position_, target_, {0,1,0}); }
    Vec3 forwardOnGround() const { return normalize(Vec3{std::sin(yaw_),0,std::cos(yaw_)}); }
    Vec3 rightOnGround() const { Vec3 f=forwardOnGround(); return {f.z,0,-f.x}; }
    Vec3 position() const { return position_; }
private:
    Vec3 position_{0,4,-7};
    Vec3 target_{};
    float yaw_{0};
    float pitch_{0.25f};
    float distance_{7.0f};
    float height_{2.2f};
};
