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
    template <typename T>
    vector<T> linearResample(const T& start, const T& end, int sampleDensity) {
        vector<T> resampled;
        for (int i = 0; i < sampleDensity; i++) {
            T resampled_i = start + (end - start) * static_cast<float>(i + 1) / static_cast<float>(sampleDensity);
            resampled.push_back(resampled_i);
        }
        return resampled;
    }


    /**
     * @param start The starting point of the line segment
     * @param end The ending point of the line segment
     * @param bias The bias factor for resampling
     * @param N The number of points to resample
     * 
     * @return An array of N points resampled between start and end with a bias factor
     */
    template <typename T>
    vector<T> biasResample(const T& start, const T& end, int sampleDensity, float bias) {
        assert(bias >= 0.0f && bias <= 1.0f);
        vector<T> resampled;
        for (int i = 0; i < sampleDensity; i++) {
            T resample_i = start + (end - start) * static_cast<float>(pow(static_cast<float>(i + 1) / static_cast<float>(sampleDensity), log(bias)/log(0.5f)));
            resampled.push_back(resample_i);
        }
        return resampled;

    }

}

#endif