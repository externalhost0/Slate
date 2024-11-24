//
// Created by Hayden Rivas on 10/30/24.
//

#ifndef SLATE_PRIMITIVES_H
#define SLATE_PRIMITIVES_H

namespace Slate {

// all primitives follow:
// position, normal, texCoords
// except for the quad cause its unneeded, just use a plane
// should be flowing counter clockwise (CCW)

    constexpr float quadVertices[] = {
            // positions             // texCoords
            -1.0f, 1.0f,  0.0f, 1.0f, // top left
            -1.0f,  -1.0f,  0.0f, 0.0f, // bottom left
            1.0f, -1.0f,  1.0f, 0.0f, // bottom right
            1.0f,  1.0f,  1.0f, 1.0f // top right
    };
    constexpr unsigned int quadIndices[] = {
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };

    constexpr float planeVertices[] = {
            // Position          // Normal           // TexCoord
            -1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,  // Top left
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom left
            1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // Bottom right
            1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f // Top right
    };
    constexpr unsigned int planeIndices[] = {
            0, 1, 3,
            1, 2, 3
    };
    constexpr float cubeVertices[] = {
            // Position             // Normal           // TexCoord
            -1.0f,  1.0f,  1.0f,    0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  // Front top left
            -1.0f, -1.0f,  1.0f,    0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  // Front bottom left
            1.0f, -1.0f,  1.0f,    0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  // Front bottom right
            1.0f,  1.0f,  1.0f,    0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  // Front top right

            -1.0f,  1.0f, -1.0f,    0.0f,  0.0f, -1.0f,  0.0f,  1.0f,  // Back top left
            -1.0f, -1.0f, -1.0f,    0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  // Back bottom left
            1.0f, -1.0f, -1.0f,    0.0f,  0.0f, -1.0f,  1.0f,  0.0f,  // Back bottom right
            1.0f,  1.0f, -1.0f,    0.0f,  0.0f, -1.0f,  1.0f,  1.0f,  // Back top right
    };
    constexpr unsigned int cubeIndices[] = {
            0, 1, 3, 3, 1, 2,
            1, 5, 2, 2, 5, 6,
            5, 4, 6, 6, 4, 7,
            4, 0, 7, 7, 0, 3,
            3, 2, 7, 7, 2, 6,
            4, 5, 0, 0, 5, 1
    };

// random vertex colors
    const float cubeVertexColors[24] = {
            0.014f, 0.184f, 0.576f,
            0.771f, 0.328f, 0.970f,
            0.406f, 0.615f, 0.116f,
            0.676f, 0.977f, 0.133f,
            0.971f, 0.572f, 0.833f,
            0.140f, 0.616f, 0.489f,
            0.997f, 0.513f, 0.064f,
            0.945f, 0.719f, 0.592f,
    };

}

#endif //SLATE_PRIMITIVES_H
