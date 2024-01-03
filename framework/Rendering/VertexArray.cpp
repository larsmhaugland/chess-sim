#include <glad/glad.h>
#include "VertexBuffer.h"
#include "BufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "ShadersDataTypes.h"


	VertexArray::VertexArray() {
		glGenVertexArrays(1, &m_vertexArrayID);
		glBindVertexArray(m_vertexArrayID);
	}

	VertexArray::~VertexArray(){
		glDeleteVertexArrays(1, &m_vertexArrayID);
	}

	// Bind vertex array
	void VertexArray::Bind() const {
		glBindVertexArray(m_vertexArrayID);
	}
	// Unbind vertex array
	void VertexArray::Unbind() const {
		glBindVertexArray(0);
	}

	
	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
		
		vertexBuffer->Bind();
		const auto& layout = vertexBuffer->GetLayout();
		const auto& elements = layout.GetAttributes();
		for (int i = 0; i < elements.size(); i++) {
			const auto& element = elements[i];
			const auto type = ShaderDataTypeToOpenGLBaseType(element.Type);
			const auto size = ShaderDataTypeComponentCount(element.Type);
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, size, type, element.Normalized, layout.GetStride(), reinterpret_cast<const void *>(element.Offset));
		}

		VertexBuffers.push_back(vertexBuffer);

	}
	// Set index buffer
	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
		indexBuffer->Bind();
		IdxBuffer = indexBuffer;

	}