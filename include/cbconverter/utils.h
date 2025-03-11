#pragma once

#include <cbreader/types.h>
#include <assimp/types.h>

#include <string>

namespace CbConverterUtils
{
    inline aiVector3f ToVector3f(const cbtypes::Vector3& vec)
    {
        return aiVector3f(vec.x, vec.y, vec.z);
    }

    inline aiVector3D ToVector3D(const cbtypes::Vector3& vec)
    {
        return aiVector3D(vec.x, vec.y, vec.z);
    }

    inline aiVector3D ToVector3D(const cbtypes::Vector2& vec)
    {
        return aiVector3D(vec.x, vec.y, 0.f);
    }

    inline aiVector2D ToVector2D(const cbtypes::Vector2& vec)
    {
        return aiVector2D(vec.x, vec.y);
    }

    inline aiString ToString(const std::string& str)
    {
        return aiString(str.c_str());
    }
}