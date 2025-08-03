#include "scene/figures/Cube.h"

Cube::Cube()
{
    m_nVertices = 8;
    Vec3 positions[8];
    positions[0] = { -0.5f, -0.5f, -0.5f }; // 1
    positions[1] = { 0.5f, -0.5f, -0.5f }; // 2
    positions[2] = { 0.5f,  0.5f, -0.5f }; // 3
    positions[3] = { -0.5f,  0.5f, -0.5f }; // 4
    positions[4] = { -0.5f, -0.5f,  0.5f }; // 5
    positions[5] = { 0.5f, -0.5f,  0.5f }; // 6
    positions[6] = { 0.5f,  0.5f,  0.5f }; // 7
    positions[7] = { -0.5f,  0.5f,  0.5f }; // 8

    BatchVertex* buffer = m_Vertices.data();

    // Face 1
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[0];
    buffer->Texture = { 0.0f,0.0f };
    buffer->Normal = { 0.0f, 0.0f, -1.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[1];
    buffer->Texture = { 1.0f,0.0f };
    buffer->Normal = { 0.0f, 0.0f, -1.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[2];
    buffer->Texture = { 1.0f,1.0f };
    buffer->Normal = { 0.0f, 0.0f, -1.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[3];
    buffer->Texture = { 0.0f,1.0f };
    buffer->Normal = { 0.0f, 0.0f, -1.0f };
    buffer++;
    // Face 2
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[4];
    buffer->Texture = { 0.0f,0.0f };
    buffer->Normal = { 0.0f, 0.0f, 1.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[5];
    buffer->Texture = { 1.0f,0.0f };
    buffer->Normal = { 0.0f, 0.0f, 1.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[6];
    buffer->Texture = { 1.0f,1.0f };
    buffer->Normal = { 0.0f, 0.0f, 1.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[7];
    buffer->Texture = { 0.0f,1.0f };
    buffer->Normal = { 0.0f, 0.0f, 1.0f };
    buffer++;
    // Face 3
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[0];
    buffer->Texture = { 0.0f,0.0f };
    buffer->Normal = { -1.0f, 0.0f, 0.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[4];
    buffer->Texture = { 1.0f,0.0f };
    buffer->Normal = { -1.0f, 0.0f, 0.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[7];
    buffer->Texture = { 1.0f,1.0f };
    buffer->Normal = { -1.0f, 0.0f, 0.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[3];
    buffer->Texture = { 0.0f,1.0f };
    buffer->Normal = { -1.0f, 0.0f, 0.0f };
    buffer++;
    // Face 4
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[1];
    buffer->Texture = { 0.0f,0.0f };
    buffer->Normal = { 1.0f, 0.0f, 0.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[5];
    buffer->Texture = { 1.0f,0.0f };
    buffer->Normal = { 1.0f, 0.0f, 0.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[6];
    buffer->Texture = { 1.0f,1.0f };
    buffer->Normal = { 1.0f, 0.0f, 0.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[2];
    buffer->Texture = { 0.0f,1.0f };
    buffer->Normal = { 1.0f, 0.0f, 0.0f };
    buffer++;
    // Face 5
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[7];
    buffer->Texture = { 0.0f,0.0f };
    buffer->Normal = { 0.0f, 1.0f, 0.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[6];
    buffer->Texture = { 1.0f,0.0f };
    buffer->Normal = { 0.0f, 1.0f, 0.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[2];
    buffer->Texture = { 1.0f,1.0f };
    buffer->Normal = { 0.0f, 1.0f, 0.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[3];
    buffer->Texture = { 0.0f,1.0f };
    buffer->Normal = { 0.0f, 1.0f, 0.0f };
    buffer++;
    // Face 6
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[4];
    buffer->Texture = { 0.0f,0.0f };
    buffer->Normal = { 0.0f, -1.0f, 0.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[5];
    buffer->Texture = { 1.0f,0.0f };
    buffer->Normal = { 0.0f, -1.0f, 0.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[1];
    buffer->Texture = { 1.0f,1.0f };
    buffer->Normal = { 0.0f, -1.0f, 0.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = positions[0];
    buffer->Texture = { 0.0f,1.0f };
    buffer->Normal = { 0.0f, -1.0f, 0.0f };
    buffer++;
}

Cube::~Cube()
{

}