#pragma once

#include "../ModernGDV/OGLInc.h"
#include "../ModernGDV/MGDVVertex.h"
#include "../ModernGDV/ModernGDV.h"

class Skybox
{
	GLuint vertexBuffer;
	ModernGDV::Driver* mgdv;
	GLuint texture;
public:
	Skybox( ModernGDV::Driver* mgdv );
	~Skybox();

	void Render();
};
