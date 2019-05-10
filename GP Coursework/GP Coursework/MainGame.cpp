#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
    Mesh* mesh1();
	Mesh* mesh2();
	Mesh* mesh3();
	Audio* audioDevice();
	Texture* texture(); //load texture
	Texture* texture1(); //load texture
	Overlay* overlay(); //load texture
	Shader* shaderPass();
	Shader* shaderBlur();
	Shader* shaderToon();
	Shader* shaderRim();
}

MainGame::~MainGame() 
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	backGroundMusic = audioDevice.loadSound("..\\res\\Background.wav");
	textureBricks.init("..\\res\\Bricks.jpg"); //load texture
	textureWater.init("..\\res\\Water.jpg"); //load texture

	shaderSkybox.init("..\\res\\shaderSkybox.vert", "..\\res\\shaderSkybox.frag", "");

	shaderExplode.init("..\\res\\shaderExplode.vert", "..\\res\\shaderExplode.frag", "..\\res\\shaderExplode.geom");
    shaderEnvMap.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag", "");
    shaderSpikeBall.init("..\\res\\shaderSpikeBall.vert", "..\\res\\shaderSpikeBall.frag", "..\\res\\shaderSpikeBall.geom");

	meshMonkey.loadModel("..\\res\\Monkey.obj");
	meshTeapot.loadModel("..\\res\\Teapot.obj");
	meshSphere.loadModel("..\\res\\Sphere.obj");
	
	myCamera.initCamera(glm::vec3(0, 0, -10.0), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;

	vector<std::string> faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};
	cubemapTexture = skybox.loadCubemap(faces); //Load the cubemap using "faces" into cubemapTextures

	float skyboxVertices[] = {
		// positions          
		-6.0f,  6.0f, -6.0f,
		-6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		-6.0f,  6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f,  6.0f
	};

	//use openGL functionality to generate & bind data into buffers
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
		playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
	
}

void MainGame::Skybox()
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	shaderSkybox.Bind();
	shaderSkybox.setInt("skybox", 0);
	//view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
	shaderSkybox.setMat4("view", myCamera.GetView());
	shaderSkybox.setMat4("projection", myCamera.GetProjection());
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
{
	
	ALint state; 
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	/*
	Possible values of state
	AL_INITIAL
	AL_STOPPED
	AL_PLAYING
	AL_PAUSED
	*/
	if (AL_PLAYING != state)
	{
		//audioDevice.playSound(Source, pos);
	}
}


void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);


    //Skybox
	Skybox();


    //Monkey - Explode
	transform.SetPos(glm::vec3(-2, 2, -2.0));
    transform.SetRot(glm::vec3(0.0, counter * 2, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

    explodeTimer += 0.02f;
	shaderExplode.Bind();
    shaderExplode.setMat4("mvp", myCamera.GetProjection() * myCamera.GetView() * transform.GetModel());
    shaderExplode.setFloat("time", explodeTimer);
    textureWater.Bind(0);
	meshMonkey.draw();
	
	

    //Teapot - Reflection
	transform.SetPos(glm::vec3(0, -2.0, 4 + sinf(counter)*3));
	transform.SetRot(glm::vec3(counter, counter * 2, 0.0));
	transform.SetScale(glm::vec3(0.05, 0.05, 0.05));

    shaderEnvMap.Bind();
    shaderEnvMap.setMat4("model", transform.GetModel());
    shaderEnvMap.setMat4("view", myCamera.GetView());
    shaderEnvMap.setMat4("projection", myCamera.GetProjection());
    shaderEnvMap.setVec3("cameraPos", myCamera.GetPos());
	meshTeapot.draw();



    //Ball - Extra
    transform.SetPos(glm::vec3(1, 1, -6));
	transform.SetRot(glm::vec3(0, counter / 3, 0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	shaderSpikeBall.Bind();
    shaderSpikeBall.setMat4("model", transform.GetModel());
    shaderSpikeBall.setMat4("view", myCamera.GetView());
    shaderSpikeBall.setMat4("projection", myCamera.GetProjection());
    textureBricks.Bind(0);
	meshSphere.draw();


	counter = counter + 0.01f;

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 