#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int mRendererID;
	std::string mFilePath;
	unsigned char* mLocalBuffer;
	int mWidth, mHeight, mBPP;
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() { return mWidth; };
	inline int GetHeight() { return mHeight; };
};