#include "figures/Floor.h"

Floor::Floor()
{
    m_nVertices = 4;
    Vec3 positions[4];
    positions[0] = { -10.0f, 0.0f, -10.0f }; // 1
    positions[1] = { 10.0f, 0.0f, -10.0f }; // 2
    positions[2] = { 10.0f,  0.0f, 10.0f }; // 3
    positions[3] = { -10.0f,  0.0f, 10.0f }; // 4

    BatchVertex* buffer = m_Vertices.data();

    // Face 
    buffer->Color = { 1.0f, 1.0f, 0.0f, 1.0f };
    buffer->Position = positions[0];
    buffer->Texture = { 0.0f,0.0f };
    buffer->Normal = { 0.0f, 1.0f, 0.0f };
    buffer++;
    buffer->Color = { 1.0f, 1.0f, 0.0f, 1.0f };
    buffer->Position = positions[1];
    buffer->Texture = { 10.0f,0.0f };
    buffer->Normal = { 0.0f, 1.0f, 0.0f };
    buffer++;
    buffer->Color = { 1.0f, 1.0f, 0.0f, 1.0f };
    buffer->Position = positions[2];
    buffer->Texture = { 10.0f,10.0f };
    buffer->Normal = { 0.0f, 1.0f, 0.0f };
    buffer++;
    buffer->Color = { 1.0f, 1.0f, 0.0f, 1.0f };
    buffer->Position = positions[3];
    buffer->Texture = { 0.0f,10.0f };
    buffer->Normal = { 0.0f, 1.0f, 0.0f };
    buffer++;
}

Floor::~Floor()
{

}