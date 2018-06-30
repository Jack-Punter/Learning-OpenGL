#pragma once

#include <vector>
#include <GL\glew.h>

#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOftype(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> mElements;
	unsigned int mStride;
public:
	VertexBufferLayout()
		: mStride(0)
	{}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		mElements.push_back({ GL_FLOAT, count, GL_FALSE });
		mStride += count * VertexBufferElement::GetSizeOftype(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		mElements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		mStride += count * VertexBufferElement::GetSizeOftype(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		mElements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		mStride += count * VertexBufferElement::GetSizeOftype(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return mElements; };
	inline unsigned int GetStride() const { return mStride; };
};
