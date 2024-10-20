#pragma once

namespace gmlib::world {

enum class FillMode : int {
    Replace = 0, //
    Keep    = 1, //
    Outline = 2, //
    Hollow  = 3,
    Destroy = 4
};

}