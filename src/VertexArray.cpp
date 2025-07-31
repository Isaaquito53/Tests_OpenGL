#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
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
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::AddInstancedAtt4fmat(int start)
{
	Bind();
	GLCall(glEnableVertexAttribArray(start));
	GLCall(glVertexAttribPointer(start, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0));
	GLCall(glEnableVertexAttribArray(start+1));
	GLCall(glVertexAttribPointer(start + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4))));
	GLCall(glEnableVertexAttribArray(start+2));
	GLCall(glVertexAttribPointer(start+2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)( 2 * sizeof(glm::vec4))));
	GLCall(glEnableVertexAttribArray(start+3));
	GLCall(glVertexAttribPointer(start+3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)( 3 * sizeof(glm::vec4))));

	GLCall(glVertexAttribDivisor(start, 1));
	GLCall(glVertexAttribDivisor(start + 1, 1));
	GLCall(glVertexAttribDivisor(start + 2, 1));
	GLCall(glVertexAttribDivisor(start + 3, 1));


}

void VertexArray::AddInstancedAtt3fvec(int start)
{
	Bind();
	GLCall(glEnableVertexAttribArray(start));
	GLCall(glVertexAttribPointer(start, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0));
	GLCall(glVertexAttribDivisor(start, 1));
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
