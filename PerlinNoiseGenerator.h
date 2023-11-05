#include <math.h>
#include <cstdlib>
#define PI 3.14159265358979323846 
class PerlinNoiseGenerator {

public:
    float AMPLITUDE = 80.0f;
    int OCTAVES = 7;
    float ROUGHNESS = 0.3f;

    PerlinNoiseGenerator() {
        this->seed = 0;
    }

    //only works with POSITIVE gridX and gridZ values!
    PerlinNoiseGenerator(int gridX, int gridZ, int vertexCount, int seed) {
        this->seed = seed;
        xOffset = gridX * (vertexCount - 1);
        zOffset = gridZ * (vertexCount - 1);
    }

    float generateHeight(int x, int z) {

        x = x < 0 ? -x : x;
        z = z < 0 ? -z : z;

        float total = 0;
        float d = (float)pow(2, OCTAVES - 1);
        for (int i = 0; i < OCTAVES; i++) {
            float freq = (float)(pow(2, i) / d);
            float amp = (float)pow(ROUGHNESS, i) * AMPLITUDE;
            total += getInterpolatedNoise((x + xOffset) * freq, (z + zOffset) * freq) * amp;
        }

        return (float)(int)total;

    }
private:
    int seed;
    int xOffset = 0;
    int zOffset = 0;



    float getInterpolatedNoise(float x, float z) {
        int intX = (int)x;
        int intZ = (int)z;
        float fracX = x - intX;
        float fracZ = z - intZ;

        float v1 = getSmoothNoise(intX, intZ);
        float v2 = getSmoothNoise(intX + 1, intZ);
        float v3 = getSmoothNoise(intX, intZ + 1);
        float v4 = getSmoothNoise(intX + 1, intZ + 1);
        float i1 = interpolate(v1, v2, fracX);
        float i2 = interpolate(v3, v4, fracX);
        return interpolate(i1, i2, fracZ);
    }

    float interpolate(float a, float b, float blend) {
        double theta = blend * PI;
        float f = (float)(1.0f - cos(theta)) * 0.5f;
        return a * (1.0f - f) + b * f;
    }

    float getSmoothNoise(int x, int z) {
        float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + getNoise(x - 1, z + 1)
            + getNoise(x + 1, z + 1)) / 16.0f;
        float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + getNoise(x, z - 1)
            + getNoise(x, z + 1)) / 8.0f;
        float center = getNoise(x, z) / 4.0f;
        return corners + sides + center;
    }

    float getNoise(int x, int z) {
        srand(x * 49632 + z * 325176 + seed);
        return static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
    }


};