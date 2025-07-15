#include "VertexLoader.h"

#include <iostream>
#include <fstream>

VertexLoader::VertexLoader()
	: m_loadedFile("")
{
}

VertexLoader::~VertexLoader()
{
}

void VertexLoader::LoadeFile(const std::string& path)
{
	m_loadedFile = path;
}