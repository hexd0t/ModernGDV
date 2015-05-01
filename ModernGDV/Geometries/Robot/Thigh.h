#pragma once

#include "../../ModernGDV/OGLInc.h"
#include <vector>

#include "../../ModernGDV/MGDVVertex.h"
#include "../../ModernGDV/ModernGDV.h"

class Thigh {
private:
	GLuint vertexBuffer;
	ModernGDV::Driver* mgdv;
	GLuint texture;
public:
	Thigh( ModernGDV::Driver* mgdv );
	~Thigh();

	void Render();

};
