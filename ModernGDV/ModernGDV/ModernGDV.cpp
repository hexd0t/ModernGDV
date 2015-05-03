#include "ModernGDV.h"

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>

ModernGDV::Driver::Driver()
	: ShaderLib(), glfwInitialized( false ), window( nullptr ),
	aspectRatio(4.f/3.f), fov(45.f), farDist(100.f),
	textureCache(), app( nullptr )
{
	if (!glfwInit()) //GLFW Initialisieren
		throw std::runtime_error( "Cannot initialize GLFW" );

	glfwInitialized = true;
	std::cout << "GLFW initialized" << std::endl;

	try
	{
		createWindow();
		std::cout << "Window created" << std::endl;

		ShaderLib.UseShader( ShaderLib.GetShaderID( "default" ) );
		Callbacks::mgdvDriverInstance = this;

		updateProj();

		glEnable( GL_SAMPLE_ALPHA_TO_COVERAGE );
	}
	catch (...)
	{
		deinit(); //Destructor would not be called on exception in constructor (since there never really was an instance to destruct) -> manually clean up
		throw;
	}

	std::cout << "ModernGDV Initialization finished" << std::endl;
}

ModernGDV::Driver::~Driver()
{
	deinit();
}

void ModernGDV::Driver::Run()
{
	if (app == nullptr)
		throw std::logic_error( "Must call SetApp before running" );

	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose( window )) { //Dauerschleife, solange das Fenster offen ist
		ShaderLib.ReapplyShader();

		double now = glfwGetTime();
		app->Update( static_cast<float>( now - lastTime ) );
		lastTime = now;

		app->Render(  );

		glfwSwapBuffers( window ); //Gezeichnetes auf den Monitor bringen
		glfwPollEvents(); //Tastendr�cke, Mausbewegungen, Meldungen vom Betriebssystem etc. verarbeiten
	}
}

void ModernGDV::Driver::SetApp( App* application )
{
	if (application == nullptr)
		throw std::logic_error( "Passed invalid app pointer" );
	app = application;
}

void ModernGDV::Driver::FramebufferSizeChanged(GLFWwindow* window, int width, int height)
{
	glViewport( 0, 0, width, height );
	aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	updateProj();
}

GLFWwindow* ModernGDV::Driver::GetWindow()
{
	return window;
}

void ModernGDV::Driver::SetProjectionOptions(const float& fov, const float& farPlane)
{
	this->fov = fov;
	farDist = farPlane;
	updateProj();
}

ModernGDV::Textures::Texture* ModernGDV::Driver::GetTexture( const std::string& filename )
{
	//Suche nach Datei in Texturcache
	auto cacheEntry = textureCache.find( filename );
	if (cacheEntry != textureCache.end())
		return &cacheEntry->second; //Gib gefundene ID zur�ck

	//Speichere die Textur im cache und gib ihre ID zur�ck
	return &textureCache.insert( std::pair<std::string, Textures::Texture>(
		filename, Textures::Texture( "../Data/" + filename + ".dds" )  ) ).first->second; 
}

GLuint ModernGDV::Driver::CreateVertexBuffer( const std::vector<ModernGDV::Vertex>& vertices )
{//Vertices aus dem CPU-Hauptspeicher in den Grafik-RAM kopieren
	GLuint vertexBuffer;
	glGenBuffers( 1, &vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( Vertex ), &vertices[0], GL_STATIC_DRAW );
	return vertexBuffer;
}

void ModernGDV::Driver::createWindow()
{
	glfwWindowHint( GLFW_SAMPLES, 4 ); // 4x Antialiasing
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 ); //OpenGL-Version 4.0 verwenden
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	window = glfwCreateWindow( 1024, 768, "GDV: Bastian Kreuzer (734877), Adrian Mueller (734922)", nullptr, nullptr ); //Fenster erstellen
	glfwSetFramebufferSizeCallback( window, Callbacks::glfwFramebufferSizeCallback );

	if (!window)
		throw std::runtime_error( "Cannot create window" );

	glfwMakeContextCurrent( window ); //Fenster f�r alle zuk�nftigen OpenGL-Aufrufe als Ziel setzen
}

void ModernGDV::Driver::updateProj()
{
	glm::mat4 proj = glm::perspective( fov, aspectRatio, 0.1f, farDist );
	ShaderLib.SetProj( proj );
}

void ModernGDV::Driver::deinit()
{
	for (auto tex = textureCache.begin(); tex != textureCache.end(); ++tex)
		tex->second.Unload();
	if (window) {
		glfwDestroyWindow( window ); //Fenster entladen, falls vorhanden
		glfwPollEvents();
	}
	if (glfwInitialized)
		glfwTerminate(); //GLFW beenden, falls initialisiert
	Callbacks::mgdvDriverInstance = nullptr;
}