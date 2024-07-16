#pragma once

#include <cstdint>

class Texture
{
public:
	uint32_t id = 0;
	uint32_t width = 0;
	uint32_t height = 0;

	bool IsValid();

	static Texture LoadTextureFromFile(const char* filename);
};
