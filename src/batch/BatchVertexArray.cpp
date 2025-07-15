#include "BatchVertexArray.h"

#include "BatchVertexBufferLayout.h"
#include "BatchRenderer.h"

BatchVertexArray::BatchVertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

BatchVertexArray::~BatchVertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void BatchVertexArray::AddBuffer(const BatchVertexBuffer& vb, const BatchVertexBufferLayout& layout)
{
	Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * BatchVertexBufferElement::GetSizeOfType(element.type);
	}
}

void BatchVertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void BatchVertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
