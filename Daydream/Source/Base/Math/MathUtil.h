#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

namespace Daydream
{
    namespace 
    {
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