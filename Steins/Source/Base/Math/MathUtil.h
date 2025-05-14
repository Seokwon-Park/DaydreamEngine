#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

namespace Steins
{
    inline glm::vec2 ToGLM(const Vector2& v) { return glm::vec2(v.x, v.y); }
    inline Vector2 FromGLM(const glm::vec2& gv) { return Vector2(gv.x, gv.y); }

    inline glm::vec3 ToGLM(const Vector3& v) { return glm::vec3(v.x, v.y, v.z); }
    inline Vector3 FromGLM(const glm::vec3& gv) { return Vector3(gv.x, gv.y, gv.z); }

    inline glm::vec4 ToGLM(const Vector4& v) { return glm::vec4(v.x, v.y, v.z, v.w); }
    inline Vector4 FromGLM(const glm::vec4& gv) { return Vector4(gv.x, gv.y, gv.z, gv.w); }

    //inline glm::mat3 toGlm(const Matrix3x3& m) {
    //    // Matrix3x3의 val이 column-major 순서라고 가정
    //    return glm::make_mat3(m.val);
    //}
    //inline void fromGlm(const glm::mat3& gm, Matrix3x3& out_m) {
    //    std::memcpy(out_m.val, glm::value_ptr(gm), 9 * sizeof(float));
    //}

    inline glm::mat4 ToGLM(const Matrix4x4& m) {
        // Matrix4x4의 val이 column-major 순서라고 가정
        return glm::make_mat4(m.values);
    }
    inline void FromGLM(const glm::mat4& gm, Matrix4x4& out_m) {
        std::memcpy(out_m.values, glm::value_ptr(gm), 16 * sizeof(float));
    }
}