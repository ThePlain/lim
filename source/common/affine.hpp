#ifndef LIM_COMMON_AFFINE
#define LIM_COMMON_AFFINE

#include "common/common.hpp"
#include "common/module.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace lim {
    struct f32vec2: public glm::f32vec2 {
        using glm::f32vec2::f32vec2;
    };

    struct f32vec3: public glm::f32vec3 {
        using glm::f32vec3::f32vec3;
    };

    struct f32vec4: public glm::f32vec4 {
        using glm::f32vec4::f32vec4;
    };

    struct i32vec2: public glm::i32vec2 {
        using glm::i32vec2::i32vec2;
    };

    struct i32vec3: public glm::i32vec3 {
        using glm::i32vec3::i32vec3;
    };

    struct i32vec4: public glm::i32vec4 {
        using glm::i32vec4::i32vec4;
    };

    struct f32quat: public glm::f32quat {
        using glm::f32quat::f32quat;
    };

    struct f32mat2: public glm::f32mat2 {
        using glm::f32mat2::f32mat2;
    };

    struct f32mat3: public glm::f32mat3 {
        using glm::f32mat3::f32mat3;
    };

    struct f32mat4: public glm::f32mat4 {
        using glm::f32mat4::f32mat4;
    };
};

#endif