#include <iostream>
#include <glad/glad.h>
#include <SDL.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float lastX = 400, lastY = 300;

float pitch = 0.f;
float yaw = -90.f;

float fov = 45.f;

static void processKeyboard(float deltaTime)
{
	float cameraSpeed = 4.f * deltaTime; // adjust accordingly
	const Uint8* keyState;
	keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keyState[SDL_SCANCODE_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keyState[SDL_SCANCODE_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keyState[SDL_SCANCODE_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

}
static void processMouse(SDL_Event ev, float deltaTime)
{

	if (ev.type == SDL_MOUSEMOTION)
	{
		float xpos = ev.button.x;
		float ypos = ev.button.y;

		static bool firstMouse = true;
		if (firstMouse)
		{
			firstMouse = false;
			lastX = xpos;
			lastY = ypos;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
		lastX = xpos;
		lastY = ypos;
		float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(front);

		if (ev.type == SDL_MOUSEWHEEL)
		{
			if (fov >= 1.0f && fov <= 45.0f)
				fov -= ev.wheel.y;
			if (fov <= 1.0f)
				fov = 1.0f;
			if (fov >= 45.0f)
				fov = 45.0f;
		}
	}
}

int main(int argc, char **argv)
{
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	SDL_Init(SDL_INIT_VIDEO);

	float screenWidth = 800;

	float screenHeight = 600;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Window* window = SDL_CreateWindow("Welcome to Computer Graphics Class!!!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
	if (window == nullptr)
	{
		std::cout << "Failed to create SDL Window" << std::endl;
		SDL_Quit();
		return -1;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		SDL_Quit();
		return -2;
	}
	SDL_ShowCursor(SDL_DISABLE);
	SDL_CaptureMouse(SDL_TRUE);


	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};	

	float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	GLuint vbo; // vertex buffer object
	glGenBuffers(1, &vbo); // Generate 1 buffer

	GLuint ebo;
	glGenBuffers(1, &ebo);	

	GLuint vao;
	glGenVertexArrays(1, &vao);

	// 1. bind Vertex Array Object
	glBindVertexArray(vao);

	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnable(GL_DEPTH_TEST); 

	//vertex shader source code
	const char* vertexShaderSource = R"glsl(
		#version 330 core

		in vec3 position;
		in vec3 color;
		in vec2 textcoord;

		out vec3 Color;
		out vec2 Textcoord;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
			gl_Position = projection * view * model * vec4(position, 1.0);
			Color = color;
			Textcoord = textcoord;
		}
		)glsl";

	// Fragment Shader source code

	const char* fragmentShaderSource = R"glsl(
		#version 330 core

		in vec3 Color;
		in vec2 Textcoord;

		out vec4 outColor;

		uniform sampler2D ourTexture;
		uniform sampler2D ourTexture2;

		void main()
		{
			vec4 colTex1 = texture(ourTexture, Textcoord);
			vec4 colTex2 = texture(ourTexture2, Textcoord);
			outColor = mix(colTex1, colTex2, 0.5);
		})glsl";

	//Create the shader program using the shader class
	Shader shaderProgram(vertexShaderSource, fragmentShaderSource);

	// 3. then set our vertex attributes pointers
	GLint posAttrib = glGetAttribLocation(shaderProgram.ID, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	GLint textCoordAttrib = glGetAttribLocation(shaderProgram.ID, "textcoord");
	glEnableVertexAttribArray(textCoordAttrib);
	glVertexAttribPointer(textCoordAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{ 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); 
		glGenerateMipmap(GL_TEXTURE_2D); 
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	shaderProgram.use();
	GLuint textureLocation;
	GLuint textureLocation2;
	textureLocation = glGetUniformLocation(shaderProgram.ID, "ourTexture");
	textureLocation2 = glGetUniformLocation(shaderProgram.ID, "ourTexture2");

	glUniform1i(textureLocation, 0);
	glUniform1i(textureLocation2, 1);

	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(fov), screenWidth / screenHeight, 0.1f, 100.0f);

	glm::vec3 cubePositions[] = { 
	glm::vec3(0.0f,  0.0f,  0.0f), 
	glm::vec3(2.0f,  5.0f, -15.0f), 
	glm::vec3(-1.5f, -2.2f, -2.5f), 
	glm::vec3(-3.8f, -2.0f, -12.3f), 
	glm::vec3(2.4f, -0.4f, -3.5f), 
	glm::vec3(-1.7f,  3.0f, -7.5f), 
	glm::vec3(1.3f, -2.0f, -2.5f), 
	glm::vec3(1.5f,  2.0f, -2.5f), 
	glm::vec3(1.5f,  0.2f, -1.5f), 
	glm::vec3(-1.3f,  1.0f, -1.5f) 
	}; 

	GLuint modelLocation = glGetUniformLocation(shaderProgram.ID, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	GLuint viewLocation = glGetUniformLocation(shaderProgram.ID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	GLuint projectionLocation = glGetUniformLocation(shaderProgram.ID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
	
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	int start = SDL_GetTicks();

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrameTime = SDL_GetTicks(); // Time of last frame

	bool gameIsRunning = true;

	SDL_Event windowEvent;
	while (gameIsRunning)
	{
		int now = SDL_GetTicks();
		float deltaTime = (now - lastFrameTime) / 1000.0f;
		lastFrameTime = now;
		float time = (now - start) / 1000.0f;

		//event loop
		while (SDL_PollEvent(&windowEvent) != 0)
		{
			if (windowEvent.type == SDL_QUIT) gameIsRunning = false;

			processMouse(windowEvent, deltaTime);
		}

		processKeyboard(deltaTime);

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

		projection = glm::perspective(glm::radians(fov), screenWidth / screenHeight, 0.1f, 100.0f);
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		shaderProgram.use();
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0); 
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1); 
		glBindTexture(GL_TEXTURE_2D, texture2); 
		
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}