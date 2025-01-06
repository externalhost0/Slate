//
// Created by Hayden Rivas on 1/2/25.
//

#pragma once

#include <cmath>
#include <vector>

inline std::vector<float> GenerateGridVertices(float size, unsigned int numLines) {
    std::vector<float> vertices; // tempy for return and loop
    // auto calc spacing
    float spacing = size / static_cast<float>(numLines);
    // lines along x and z
    for (unsigned int i = 0; i <= numLines; ++i) {
        float pos = -size / 2.0f + i * spacing;  // line position

        // x-axis
        vertices.push_back(-size / 2.0f);
        vertices.push_back(0.0f);
        vertices.push_back(pos);

        vertices.push_back(size / 2.0f);
        vertices.push_back(0.0f);
        vertices.push_back(pos);

        // z-axis
        vertices.push_back(pos);
        vertices.push_back(0.0f);
        vertices.push_back(-size / 2.0f);

        vertices.push_back(pos);
        vertices.push_back(0.0f);
        vertices.push_back(size / 2.0f);
    }

    return vertices;
}
inline std::vector<float> GenerateCircleVertices(int numSegments) {
    float radius = 1.0f;
    std::vector<float> vertices;
    float angleIncrement = 2.0f * (float) M_PI / (float) numSegments;

    for (int i = 0; i < numSegments; ++i) {
        float angle = (float) i * angleIncrement;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);  // sphere is 2D
    }

    return vertices;
}

inline std::vector<float> GenerateSphereVertices(unsigned int numVertical, unsigned int numHorizontal, unsigned int numSegments) {
    // dont change the base radius value
    float radius = 1.0f;
    if (numVertical < 1) {
        fprintf(stderr, "Invalid Vertical Count");
    }
    std::vector<float> vertices;

    // Angle increments
    float ringAngleIncrement = static_cast<float>(M_PI) / static_cast<float>(numHorizontal - 1); // Latitude spacing
    float verticalAngleIncrement = 2.0f * static_cast<float>(M_PI) / static_cast<float>(numVertical); // Longitude spacing

    // Generate latitude rings (horizontal slices)
    for (int i = 0; i < numHorizontal; ++i) {
        float ringAngle = static_cast<float>(i) * ringAngleIncrement; // Latitude angle
        float y = radius * cos(ringAngle); // Y position for this latitude
        float ringRadius = radius * sin(ringAngle); // Radius of the sphere at this latitude

        for (int j = 0; j <= numVertical; ++j) { // Add one more vertical slice to close the sphere
            float verticalAngle = static_cast<float>(j) * verticalAngleIncrement; // Longitude angle
            float x = ringRadius * cos(verticalAngle); // X position
            float z = ringRadius * sin(verticalAngle); // Z position

            vertices.push_back(x); // X-coordinate
            vertices.push_back(y); // Y-coordinate (up in the 3D engine)
            vertices.push_back(z); // Z-coordinate
        }
    }

    // Generate vertical slices (longitude lines)
    for (int j = 0; j <= numVertical; ++j) { // Add one more vertical slice to close the sphere
        float verticalAngle = static_cast<float>(j) * verticalAngleIncrement; // Fixed longitude angle
        float cosVertical = cos(verticalAngle);
        float sinVertical = sin(verticalAngle);

        for (int i = 0; i < numHorizontal; ++i) {
            float ringAngle = static_cast<float>(i) * ringAngleIncrement; // Latitude angle
            float y = radius * cos(ringAngle); // Y position
            float ringRadius = radius * sin(ringAngle); // Radius of the sphere at this latitude

            float x = ringRadius * cosVertical; // X position
            float z = ringRadius * sinVertical; // Z position

            vertices.push_back(x); // X-coordinate
            vertices.push_back(y); // Y-coordinate
            vertices.push_back(z); // Z-coordinate
        }
    }
    return vertices;
}

inline std::vector<float> GenerateSimpleSphereVertices(unsigned int numSegments) {
    float radius = 1.0f;
    std::vector<float> vertices;

    if (numSegments%2!=0) {
        numSegments+=1;
    }


    float resolutionIncrement = 2.0f * (float) M_PI / (float) numSegments;

    // first vertical
    for (int i = 0; i < numSegments; i++) {
        float increment = (float)i * resolutionIncrement;
        float x = radius * cos(increment);
        float y = radius * sin(increment);

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
    }
    // first horizontal
    for (int i = 0; i < numSegments+(numSegments/4); i++) {
        float increment = (float)i * resolutionIncrement;
        float x = radius * cos(increment);
        float z = radius * sin(increment);

        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(z);
    }
    // second vertical
    for (int i = 0; i < numSegments+(numSegments/4); i++) {
        float increment = (float)i * resolutionIncrement;
        float x = radius * cos(increment);
        float y = radius * sin(increment);

        vertices.push_back(0.0f);
        vertices.push_back(y);
        vertices.push_back(x);
    }

    return vertices;
}

// not a very good function, refine later
inline std::vector<float> GenerateSpotVertices(unsigned int numSegments) {
    float radius = 0.18f;
    float height = 10.0f;
    std::vector<float> vertices;

    float angleIncrement = 2.0f * static_cast<float>(M_PI) / static_cast<float>(numSegments);

    // Generate base circle vertices
    for (int i = 0; i < numSegments; ++i) {
        float angle = static_cast<float>(i) * angleIncrement;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // Add base circle vertices
        vertices.push_back(x);
        vertices.push_back(-height); // y = -height for base
        vertices.push_back(z);
    }

    // Add apex of the cone (tip)
    vertices.push_back(0.0f); // x = 0
    vertices.push_back(0.0f); // y = 0 (apex)
    vertices.push_back(0.0f); // z = 0

    // Generate side vertices (triangular faces)
    for (int i = 0; i < numSegments; ++i) {
        int nextIndex = (i + 1) % numSegments;

        // Base vertices
        vertices.push_back(vertices[i * 3]);
        vertices.push_back(vertices[i * 3 + 1]);
        vertices.push_back(vertices[i * 3 + 2]);

        // Next base vertex
        vertices.push_back(vertices[nextIndex * 3]);
        vertices.push_back(vertices[nextIndex * 3 + 1]);
        vertices.push_back(vertices[nextIndex * 3 + 2]);

        // Apex vertex
        vertices.push_back(0.0f); // x = 0
        vertices.push_back(0.0f); // y = 0 (apex)
        vertices.push_back(0.0f); // z = 0
    }

    return vertices;
}

inline std::vector<float> GenerateArrow2DMesh(float shaftLength, float shaftWidth, float tipWidth, float tipHeight) {
    std::vector<float> vertices;



    // bottom vertex
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    // top vertex
    vertices.push_back(0.0f);
    vertices.push_back(-shaftLength);
    vertices.push_back(0.0f);

    // triangle tip
    vertices.push_back(-tipWidth);
    vertices.push_back(-shaftLength);
    vertices.push_back(0.0f);

    vertices.push_back(0.0f);
    vertices.push_back(-shaftLength - tipHeight);
    vertices.push_back(0.0f);

    vertices.push_back(tipWidth);
    vertices.push_back(-shaftLength);
    vertices.push_back(0.0f);

    // and back to top vertex
    vertices.push_back(0.0f);
    vertices.push_back(-shaftLength);
    vertices.push_back(0.0f);

    // now again but on the z axis so we have two arrow tips!
    vertices.push_back(0.0f);
    vertices.push_back(-shaftLength);
    vertices.push_back(-tipWidth);

    vertices.push_back(0.0f);
    vertices.push_back(-shaftLength - tipHeight);
    vertices.push_back(0.0f);

    vertices.push_back(0.0f);
    vertices.push_back(-shaftLength);
    vertices.push_back(tipWidth);

    // and back again to top vertex
    vertices.push_back(0.0f);
    vertices.push_back(-shaftLength);
    vertices.push_back(0.0f);

    return vertices;
}
