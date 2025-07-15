#pragma once

#include <vector>

class BatchIndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	BatchIndexBuffer(const unsigned int* data, unsigned int count);
	~BatchIndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};