#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"
#include "Overlay.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	void playAudio(unsigned int Source, glm::vec3 pos);

	void Skybox();

	GLuint skyboxVAO, skyboxVBO, cubemapTexture;
	vector<std::string> faces;

	Display _gameDisplay;
	GameState _gameState;
	Mesh meshMonkey;
	Mesh meshTeapot;
	Mesh meshSphere;

	Texture textureBricks;
	Texture textureWater;
	Texture skybox;

	Shader shaderExplode;
    Shader shaderEnvMap;
    Shader shaderSpikeBall;
    Shader shaderSkybox;

	glm::mat4 modelView;

	Camera myCamera;
	Audio audioDevice;

    float counter;
    float explodeTimer;
	unsigned int whistle;
	unsigned int backGroundMusic;
};

