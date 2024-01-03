#ifndef INDEXBUFFER_H_
#define INDEXBUFFER_H_

#include <glad/glad.h>

class IndexBuffer
{
private:
	GLuint IndexBufferID;
	GLuint Count;
public:
	// Constructor. It initializes with a data buffer and the size of it.
	// Note that the buffer will be bound on construction.
	IndexBuffer(unsigned int *indices, unsigned int count);
	~IndexBuffer();

	// Bind the vertex buffer
	void Bind() const;

	// Unbind the vertex buffer
	void Unbind() const;

	// Get the number of elements
	inline GLuint GetCount() const { return Count; }

};

#endif // VERTEXBUFFER_H_
