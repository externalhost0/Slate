//
// Created by Hayden Rivas on 10/30/24.
//

#pragma once

namespace Slate::Primitives {

// all primitives follow:
// position, normal, texCoords
// except for flat surfaces like plane and quad cause they only have one possible direction/normal
// except for the quad cause its unneeded, just use a plane
// should be flowing counter clockwise (CCW)
    constexpr float quadVertices2D[] = {
            // positions               // texCoords
            -1.f, 1.f,  0.f, 1.f, // top left
            -1.f, -1.f, 0.f, 0.f, // bottom left
            1.f, -1.f, 1.f, 0.f, // bottom right
            1.f, 1.f, 1.f, 1.f // top right
    };
	constexpr float quadVertices3DNoNormals[] = {
			// positions                 		  // texCoords
			-1.f, 1.f,   1.f,  0.f, 1.f, // top left
			-1.f, -1.f, 1.f,   0.f, 0.f, // bottom left
			1.f, -1.f, 1.f, 1.f, 0.f, // bottom right
			1.f, 1.f,  1.f, 1.f, 1.f // top right
	};
    constexpr float quadVertices3D[] = {
            // positions                          // normals            		  // texCoords
            -1.f, 1.f,   0.f,  0.f,  0.f,  1.f, 0.f, 1.f, // top left
            -1.f, -1.f, 0.f,  0.f, 0.f, 1.f, 0.f, 0.f, // bottom left
            1.f, -1.f, 0.f,  0.f, 0.f, 1.f,1.f, 0.f, // bottom right
            1.f, 1.f,  0.f, 0.f, 0.f, 1.f, 1.f, 1.f // top right
    };


    constexpr unsigned int quadIndices[] = {
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };
    constexpr float planeVertices[] = {
            // Position                          // Normal                         // TexCoord
            -1.f, 0.f,   -1.f,   0.f,  1.f,  0.f,   0.f, 1.f,  // Top left
            -1.f, 0.f,   1.f,    0.f, 1.f, 0.f,  0.f, 0.f, // Bottom left
            1.f, 0.f,   1.f,    0.f, 1.f, 0.f, 1.f, 0.f, // Bottom right
            1.f, 0.f, -1.f,  0.f, 1.f, 0.f, 1.f, 1.f // Top right
    };
    constexpr unsigned int planeIndices[] = {
            0, 1, 3,
            1, 2, 3
    };

    const float cubeVertices[] = {
            // Position                                 // Normal                                   // TexCoord
            -1.f,  -1.f,   -1.f,      0.f,  0.f,  -1.f,        0.f, 0.f,  // A 0
            1.f,   -1.f,  -1.f,     0.f,  0.f,  -1.f,       1.f, 0.f,  // B 1
            1.f,   1.f,  -1.f,    0.f,  0.f,  -1.f,       1.f, 1.f,  // C 2
            -1.f,  1.f,  -1.f,   0.f,  0.f,  -1.f,       0.f, 1.f,  // D 3
            -1.f, -1.f,  1.f,     0.f,  0.f,  1.f,       0.f, 0.f,  // E 4
            1.f,  -1.f,  1.f,    0.f,  0.f,  1.f,        1.f, 0.f,   // F 5
            1.f,   1.f,  1.f,    0.f,  0.f,  1.f,        1.f, 1.f,   // G 6
            -1.f,  1.f,  1.f,    0.f,  0.f,  1.f,        0.f, 1.f,   // H 7

            -1.f,  1.f,  -1.f,    -1.f,  0.f,  0.f,      0.f, 0.f,  // D 8
            -1.f, -1.f,  -1.f,    -1.f,  0.f,  0.f,      1.f, 0.f,  // A 9
            -1.f, -1.f,  1.f,    -1.f,  0.f,  0.f,       1.f, 1.f,  // E 10
            -1.f,  1.f,  1.f,    -1.f,  0.f,  0.f,       0.f, 1.f,  // H 11
            1.f,  -1.f,  -1.f,   1.f,  0.f,  0.f,      0.f, 0.f,   // B 12
            1.f,  1.f, -1.f,  1.f,  0.f,  0.f,      1.f, 0.f,   // C 13
            1.f,  1.f,  1.f,  1.f,  0.f,  0.f,       1.f, 1.f,   // G 14
            1.f, -1.f,  1.f,  1.f,  0.f,  0.f,      0.f, 1.f,   // F 15

            -1.f, -1.f, -1.f,  0.f,  -1.f,  0.f,    0.f, 0.f,  // A 16
            1.f,  -1.f, -1.f,  0.f,  -1.f,  0.f,    1.f, 0.f,   // B 17
            1.f,  -1.f, 1.f,  0.f,  -1.f,  0.f,     1.f, 1.f,   // F 18
            -1.f, -1.f, 1.f,   0.f,  -1.f,  0.f,     0.f, 1.f,  // E 19
            1.f,  1.f,  -1.f,    0.f,  1.f,  0.f,    0.f, 0.f,  // C 20
            -1.f, 1.f,  -1.f,    0.f,  1.f,  0.f,    1.f, 0.f,  // D 21
            -1.f, 1.f,  1.f,     0.f,  1.f,  0.f,     1.f, 1.f,  // H 22
            1.f,  1.f,  1.f,     0.f,  1.f,  0.f,    0.f, 1.f,  // G 23
    };
    const unsigned int cubeIndices[] = {
            // front and back
            0, 3, 2,
            2, 1, 0,
            4, 5, 6,
            6, 7 ,4,
            // left and right
            11, 8, 9,
            9, 10, 11,
            12, 13, 14,
            14, 15, 12,
            // bottom and top
            16, 17, 18,
            18, 19, 16,
            20, 21, 22,
            22, 23, 20
    };


}

