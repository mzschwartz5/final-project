#ifndef MATHUTILS_H
#define MATHUTILS_H
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <array>
using std::vector;
using glm::vec3;
using glm::vec4;

namespace MathUtils {

    /**
     * @param start The starting point of the line segment
     * @param end The ending point of the line segment
     * @param N The number of points to resample
     * 
     * @return An array of N points linearly resampled between start and end
     */
    template <typename T, std::size_t N>
    std::array<T, N> linearResample(const T& start, const T& end) {
        std::array<T, N> resampled;
        for (int i = 0; i < N; i++) {
            resampled[i] = start + (end - start) * static_cast<float>(i) / static_cast<float>(N);
        }
        return resampled;
    }
}

#endif