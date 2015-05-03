#include "Foot.h"

#include "../Primitives/Quad.h"

using ModernGDV::Vertex;
using Geometries::Primitives::Quad;
using glm::vec3;
using glm::vec2;


Geometries::Robot::Foot::Foot(ModernGDV::Driver* mgdv)
: vertexBuffer(0U), mgdv(mgdv), texture(nullptr)
{
	std::vector<Vertex> vertices;

	vec3 cuboidBottomFrontLeft(-0.075f, -0.05f, +0.24f);
	vec3 cuboidBottomFrontRight(+0.075f, -0.05f, +0.24f);
	vec3 cuboidBottomBackLeft(-0.075f, -0.05f, -0.24f);
	vec3 cuboidBottomBackRight(+0.075f, -0.05f, -0.24f);

	vec3 cuboidTopFrontLeft(-0.075f, +0.05f, +0.24f);
	vec3 cuboidTopFrontRight(+0.075f, +0.05f, +0.24f);
	vec3 cuboidTopBackLeft(-0.075f, +0.05f, -0.24f);
	vec3 cuboidTopBackRight(+0.075f, +0.05f, -0.24f);

	Quad::Create( vertices, cuboidBottomFrontLeft, vec2( 0.f, 0.f ), cuboidBottomFrontRight, vec2( 1.f, 0.f ),
		cuboidBottomBackRight, vec2(1.f, 1.f), cuboidBottomBackLeft, vec2(0.f, 1.f));				//Bodenfläche Quader

	Quad::Create(vertices, cuboidTopFrontLeft, vec2(0.f, 0.f), cuboidTopFrontRight, vec2(1.f, 0.f),
		cuboidBottomFrontRight, vec2(1.f, 1.f), cuboidBottomFrontLeft, vec2(0.f, 1.f));				//Seitenflächen Quader
	Quad::Create(vertices, cuboidTopFrontRight, vec2(0.f, 0.f), cuboidTopBackRight, vec2(1.f, 0.f),
		cuboidBottomBackRight, vec2(1.f, 1.f), cuboidBottomFrontRight, vec2(0.f, 1.f));
	Quad::Create(vertices, cuboidTopBackRight, vec2(0.f, 0.f), cuboidTopBackLeft, vec2(1.f, 0.f),
		cuboidBottomBackLeft, vec2(1.f, 1.f), cuboidBottomBackRight, vec2(0.f, 1.f));
	Quad::Create(vertices, cuboidTopBackLeft, vec2(0.f, 0.f), cuboidTopFrontLeft, vec2(1.f, 0.f),
		cuboidBottomFrontLeft, vec2(1.f, 1.f), cuboidBottomBackLeft, vec2(0.f, 1.f));

	Quad::Create(vertices, cuboidTopFrontLeft, vec2(0.f, 1.f), cuboidTopBackLeft, vec2(0.f, 0.f),
		cuboidTopBackRight, vec2(1.f, 0.f), cuboidTopFrontRight, vec2(1.f, 1.f));					//Oberseite Quader

	vertexBuffer = mgdv->CreateVertexBuffer(vertices);

	texture = mgdv->GetTexture("test");
}

Geometries::Robot::Foot::~Foot()
{

}

void Geometries::Robot::Foot::Render()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	Vertex::SetLayout();
	mgdv->ShaderLib.SetDiffuseTex( texture );
	unsigned char index = 0U;

	index = Quad::Draw(index); //Bodenfläche Quader

	index = Quad::Draw(index); //Seitenflächen Quader
	index = Quad::Draw(index);
	index = Quad::Draw(index);
	index = Quad::Draw(index);

	index = Quad::Draw(index); //Oberseite Quader

	Vertex::ResetLayout();
}

