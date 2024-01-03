#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "IndexBuffer.h"


	// Constructor. It initializes with a data buffer and the size of it.
	// Note that the buffer will be bound on construction.
IndexBuffer::IndexBuffer(unsigned int *indices, unsigned int count) {
	Count = count;
	glGenBuffers(1, &IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &IndexBufferID);
}

	// Bind the vertex buffer
void IndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
}

	// Unbind the vertex buffer
void IndexBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
