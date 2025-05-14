#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

namespace Steins
{
    namespace 
    {
        inline glm::vec2 ToGLM(const Vector2& _v) { return glm::vec2(_v.x, _v.y); }
        inline Vector2 FromGLM(const glm::vec2& _gv) { return Vector2(_gv.x, _gv.y); }

        inline glm::vec3 ToGLM(const Vector3& _v) { return glm::vec3(_v.x, _v.y, _v.z); }
        inline Vector3 FromGLM(const glm::vec3& _gv) { return Vector3(_gv.x, _gv.y, _gv.z); }

        inline glm::vec4 ToGLM(const Vector4& _v) { return glm::vec4(_v.x, _v.y, _v.z, _v.w); }
        inline Vector4 FromGLM(const glm::vec4& _gv) { return Vector4(_gv.x, _gv.y, _gv.z, _gv.w); }

        //inline glm::mat3 toGlm(const Matrix3x3& m) {
        //    // Matrix3x3의 val이 column-major 순서라고 가정
        //    return glm::make_mat3(m.val);
        //}
        //inline void fromGlm(const glm::mat3& gm, Matrix3x3& out_m) {
        //    std::memcpy(out_m.val, glm::value_ptr(gm), 9 * sizeof(float));
        //}

        inline glm::mat4 ToGLM(const Matrix4x4& _mat)
        {
            glm::mat4 mat = glm::make_mat4(_mat.values);
            return glm::transpose(mat);
        }
        inline void FromGLM(const glm::mat4& _gm, Matrix4x4& _outMat)
        {
            glm::mat4 transposed = glm::transpose(_gm);
            std::memcpy(_outMat.values, glm::value_ptr(_gm), 16 * sizeof(Float32));
        }
    }
}