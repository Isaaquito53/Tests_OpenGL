#pragma once

#include <string>

#include "batch/BatchVertex.h"

class VertexLoader
{
private:
	std::string m_loadedFile;
public:
	VertexLoader();
	~VertexLoader();

	void LoadeFile(const std::string& path);
};