#include "scene/figures/Axis.h"

Axis::Axis()
{
    BatchVertex* buffer = m_Vertices.data();

    // X
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = { 0.0f, 0.0f, 0.0f };
    buffer->Texture = { 0.0f,0.0f };
    buffer->Normal = { 0.0f, 0.0f, 0.0f };
    buffer++;
    buffer->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    buffer->Position = { 1.0f,0.0f,0.0f };
    buffer->Texture = { 0.0f,0.0f };
    buffer->Normal = { 0.0f, 0.0f, 0.0f };
    buffer++;
    // Y
    buffer->Color = { 0.0f, 1.0f, 0.0f, 1.0f };
    buffer->Position = { 0.0f, 0.0f, 0.0f };
    buffer->Texture = { 0.0f,0.0f };
    buffer->Normal = { 0.0f, 0.0f, 0.0f };
    buffer++;
    buffer->Color = { 0.0f, 1.0f, 0.0f, 1.0f };
    buffer->Position = { 0.0f,1.0f,0.0f };
    buffer->Texture = { 0.0f,0.0f };
    buffer->Normal = { 0.0f, 0.0f, 0.0f };
    buffer++;
    // Z
    buffer->Color = { 0.0f, 0.0f, 1.0f, 1.0f };
    buffer->Position = { 0.0f, 0.0f, 0.0f };
    buffer->Texture = { 0.0f,0.0f };
    buffer->Normal = { 0.0f, 0.0f, 0.0f };
    buffer++;
    buffer->Color = { 0.0f, 0.0f, 1.0f, 1.0f };
    buffer->Position = { 0.0f,0.0f,1.0f };
    buffer->Texture = { 0.0f,0.0f };
    buffer->Normal = { 0.0f, 0.0f, 0.0f };
    buffer++;
}

Axis::~Axis()
{
}