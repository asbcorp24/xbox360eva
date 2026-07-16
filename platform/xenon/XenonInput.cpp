#include "XenonInput.h"
#include <input/input.h>
#include <usb/usbmain.h>
#include <cstring>
#include <cmath>

static float axis(short value) {
    const float v = static_cast<float>(value) / 32767.0f;
    const float dead = 0.18f;
    if (std::fabs(v) < dead) return 0.0f;
    return (v > 0 ? 1.0f : -1.0f) * ((std::fabs(v) - dead) / (1.0f - dead));
}

void XenonInput::init() {
    usb_init();
    usb_do_poll();
}

PadState XenonInput::poll() {
    usb_do_poll();
    controller_data_s c{};
    PadState out{};
    if (!get_controller_data(&c, 0)) return out;

    out.moveX = axis(c.s1_x);
    out.moveY = -axis(c.s1_y);
    out.lookX = axis(c.s2_x);
    out.lookY = -axis(c.s2_y);
    out.jump = c.a;
    out.magic = c.x;
    out.cupcake = c.b;
    out.start = c.start;
    return out;
}
