#ifndef __GEOMETRICTOOLS_H
#define __GEOMETRICTOOLS_H

#include <array>
#include <vector>
#include <numeric>

namespace GeometricTools {

    constexpr std::array<float, 3 * 24 * 2> UnitCube3D24WNormals = {
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,// Left - Bottom - Front
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,// Left - Top - Front
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,// Right - Bottom - Front
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,// Right - Top - Front
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,// Right - Bottom - Back
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,

            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,// Right - Top - Back
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,

            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,// Left - Bottom - Back
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,

            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,// Left - Top - Back
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  -1.0f
    };


    std::vector<GLuint> cubeTopologyWNormals = {
            2,5,8,		5,8,11,		//Front
            6,9,12,		9,12,15,	//Right
            14,17,20,	17,20,23,	//Back
            0,3,18,		3,18,21,	//Left
            4,10,16,	4,16,22,	//Top
            1,7,13,		1,13,19		//Bottom

    };

    template<typename T>
    std::vector<float> Cube3DWNormals(T X) {
        std::vector<float>cube;
        cube.resize(3 * 24 * 2);
        for (int i = 0; i < cube.size(); i++)
        {
            cube[i] = UnitCube3D24WNormals[i] / X;
        }
        return cube;
    };



	template<typename T,typename U>
	std::vector<float>	UnitGridGeometry2D(T X, U Y) {
		std::vector<float> vbo;
		vbo.resize((X + 1) * (Y + 1) * 2);

		for (unsigned int j = 0; j < Y + 1; ++j) {
			float y = j / static_cast<float>(Y) - 0.5f;
			for (unsigned int i = 0; i < X + 1; ++i) {
				float x = i / static_cast<float>(X) - 0.5f;
				vbo[(j * (X + 1) + i) * 2 + 0] = x;
				vbo[(j * (X + 1) + i) * 2 + 1] = y;
			}
		}
		return vbo;
	}

    template<typename T, typename U>
	std::vector<unsigned int>	GridIndices(T X, U Y) {
		std::vector<unsigned int> indices;
		indices.resize(X * Y * 6);
		int counter = 0;
		unsigned int offset = 0;

		for (int i = 0; i < Y; i++) {
			for (int j = 0; j < X; j++) {
				
					indices[counter++] = j + offset;
					indices[counter++] = j + X + 1 + offset;
					indices[counter++] = j + X + 2 + offset;

					indices[counter++] = j + offset;
					indices[counter++] = j + 1 + offset;
					indices[counter++] = j + X + 2 + offset;
			}
			offset += X + 1;
		}
	return indices;
	}

}

#endif // !__GEOMETRICTOOLS_H
