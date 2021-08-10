#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(-100.0f, 2.0f, -45.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;


// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni(-95.0f, 1.0f, -45.0f);
bool active;

//Variables para animaciones
float movCanX = 0.0;
float rotCan = 0.0;
float rotBrazo = 0.0f;
float movBrazoX = 0.0;
float movBrazoZ = 0.0;
float movBurguerX = 0.0;
float movBurguerY = 0.0;
float movBurguerZ = 0.0;
float rotBurguer = 0.0;
float movBobX = 0.0;
float movBobZ = 0.0;
float rotBob = 0.0f;
float rotPuerta = 0.0f;
float rotLet = 0.0f;

bool BobFrente = true;
bool BobDer = false;

bool circuito1 = false;
bool circuito2 = false;
bool circuito3 = false;
bool circuito4 = false;
bool circuito5 = false;

bool recorrido1 = true;
bool recorrido2 = false;

bool recorrido3 = true;
bool recorrido4 = false;
bool recorrido5 = false;

bool recorrido6 = true;
bool recorrido7 = false;
bool recorrido8 = false;
bool recorrido9 = false;
bool recorrido10 = false;
bool recorrido11 = false;

bool recorrido12 = true;
bool recorrido13 = false;

bool recorrido14 = true;
bool recorrido15 = false;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame


float posX = PosIni.x, posY = PosIni.y, posZ = PosIni.z;




// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(posX,posY,posZ),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0)
};

glm::vec3 LightP1;



/*Función: main
* Parámetros: Ninguno
* Tipo de retorno: int (entero)
* Descripción: Dentro de esta función se va construyendo todo el espacio, 
* con modelos, skybox y demás.
* */

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	Shader shader("Shaders/modelLoading.vs", "Shaders/modelLoading.frag");


	//Declaramos modelos

	Model BobEsponja((char*)"Models/spongebob-squarepants/bobesponja_sinbrazo.obj");
	Model Cangrejo((char*)"Models/spongebob-squarepants/doncangrejo.obj");
	Model Poste((char*)"Models/Fachada/Poste.obj");
	Model Brazo((char*)"Models/spongebob-squarepants/brazo_izq.obj");
	Model Cangreburguer((char*)"Models/Objetos int/cangreburguer.obj");
	Model Letreros((char*)"Models/Objetos int/letreros.obj");

	Model Edificio((char*)"Models/Fachada/fachada.obj");
	Model Puerta((char*)"Models/Fachada/puerta.obj");

	
	// Build and compile our shader program




	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};


	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
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


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1,&skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");
	
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();


		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		shader.Use();
		GLint viewPosLoc = glGetUniformLocation(shader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), 0.0f, 20.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), 0.0f, -10.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(shader.Program, "model");
		GLint viewLoc = glGetUniformLocation(shader.Program, "view");
		GLint projLoc = glGetUniformLocation(shader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		//glBindTexture(GL_TEXTURE_2D, texture1);*/

		// Bind specular map
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/


		glBindVertexArray(VAO);

		/*Carga de modelos*/
		glm::mat4 tmp = glm::mat4(1.0f); //Temp
		
		//Objetos animados

		view = camera.GetViewMatrix();
		glm::mat4 model(1);
		tmp = model = glm::translate(model, glm::vec3(PosIni.x, PosIni.y,PosIni.z));
		model = glm::translate(model, glm::vec3(-2.40f + movBobX, 1.06f, -0.55f + movBobZ));
		model = glm::rotate(model, glm::radians(rotBob), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BobEsponja.Draw(shader);

		view = camera.GetViewMatrix();
		model = tmp;
		model = glm::translate(model, glm::vec3(-2.43f + movBobX + movBrazoX, 1.08f, -1.15f + movBobZ + movBrazoZ));
		if(BobFrente){
			model = glm::rotate(model, glm::radians(8.5f + rotBrazo), glm::vec3(0.0f, 0.0, 1.0f));
		}
		else {
			if (BobDer) {
				model = glm::rotate(model, glm::radians(-8.5f - rotBrazo), glm::vec3(1.0f, 0.0, 0.0f));
			}
			else {
				model = glm::rotate(model, glm::radians(8.5f + rotBrazo), glm::vec3(1.0f, 0.0, 0.0f));
			}
		}
		model = glm::rotate(model, glm::radians(rotBob), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Brazo.Draw(shader);

		view = camera.GetViewMatrix();
		model = tmp;
		model = glm::translate(model, glm::vec3(movBurguerX + movBobX - 1.6f, movBurguerY + 1.32f, movBurguerZ + movBobZ - 1.17f));
		if (BobFrente) {
			model = glm::rotate(model, glm::radians(rotBurguer), glm::vec3(0.0f, 0.0, 1.0f));
		}
		else {
			if (BobDer) {
				model = glm::rotate(model, glm::radians(rotBurguer), glm::vec3(1.0f, 0.0, 0.0f));
			}
			else {
				model = glm::rotate(model, glm::radians(-rotBurguer), glm::vec3(1.0f, 0.0, 0.0f));
			}
		}
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cangreburguer.Draw(shader);

		view = camera.GetViewMatrix();
		model = tmp;
		model = glm::scale(model, glm::vec3(0.5f, 0.49f, 0.5f));
		model = glm::translate(model, glm::vec3(10.4f, 2.55f, 2.55f));
		model = glm::rotate(model, glm::radians(rotPuerta), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puerta.Draw(shader);

		view = camera.GetViewMatrix();
		model = tmp;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(2.0f, 13.0f, 3.0f));
		model = glm::rotate(model, glm::radians(rotLet), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Letreros.Draw(shader);

		view = camera.GetViewMatrix();
		model = tmp;
		model = glm::translate(model, glm::vec3(35.0f + movCanX, 1.38f, 1.5f));
		model = glm::rotate(model, glm::radians(rotCan), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cangrejo.Draw(shader);

		//    Objetos estáticos
		view = camera.GetViewMatrix();
		model = tmp;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Edificio.Draw(shader);

		view = camera.GetViewMatrix();
		model = tmp;
		model = glm::translate(model, glm::vec3(7.5f, -0.8f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Poste.Draw(shader);

	

		glBindVertexArray(0);


		 //Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		 //Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default




		// Swap the screen buffers
		glfwSwapBuffers(window);
	}




	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();




	return 0;
}


/*Función: KeyCallback
* Parámetros: GLFWwindow *window, int key, int scancode, int action, int mode
* Tipo de retorno: void (vacío)
* Descripción: Función que es llamada cuando se presiona o suelta una tecla,
* manda cerrar una ventana si se presiona ESC, activa y desactiva una bandera
* depndiendo de si se presiona o se suelta una tecla.
* */

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{


	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	
}

/*Función: MouseCallback
* Parámetros: GLFWwindow *window, double xPos, double yPos
* Tipo de retorno: void (vacío)
* Descripción: Guarda las ultimas posiciones del mouse
* */
void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

/*Función: 
* Parámetros: Ninguno
* Tipo de retorno: void (vacío)
* Descripción: Función que asigna a cada tecla la acción de activar
* o desactivar cada animación, además de los controles de la cámara
* */

void DoMovement()
{

	//Activar animaciones
	
	if (keys[GLFW_KEY_1]) //ACTIVA ANIMACION DE DON CANGREJO
	{
		circuito1 = true;
	}

	if (keys[GLFW_KEY_2]) //DESACTIVA ANIMACION DE DON CANGREJO
	{
		circuito1 = false;
	}

	if (keys[GLFW_KEY_3]) //ACTIVA ANIMACION DE BOB ESPONJA 1
	{
		circuito2 = true;
	}

	if (keys[GLFW_KEY_4]) //DESACTIVA ANIMACION DE BOB ESPONJA 1
	{
		circuito2 = false;
	}

	if (keys[GLFW_KEY_5]) //ACTIVA ANIMACION DE BOB ESPONJA 2
	{
		circuito3 = true;
	}

	if (keys[GLFW_KEY_6]) //DESACTIVA ANIMACION DE BOB ESPONJA 2
	{
		circuito3 = false;
	}

	if (keys[GLFW_KEY_7]) //ACTIVA ANIMACION DE PUERTAS
	{
		circuito4 = true;
	}

	if (keys[GLFW_KEY_8]) //DESACTIVA ANIMACION DE PUERTAS
	{
		circuito4 = false;
	}

	if (keys[GLFW_KEY_9]) //ACTIVA ANIMACION DE LETREROS
	{
		circuito5 = true;
	}

	if (keys[GLFW_KEY_0]) //ACTIVA ANIMACION DE LETREROS
	{
		circuito5 = false;
	}



	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}


}


/*Función: animacion
* Parámetros: Ninguno
* Tipo de retorno: void (vacío)
* Descripción: Función que realiza los movimientos que corresponden a cada momento de 
* las animaciones, según se hayan activado y en qué recorrido se encuentren
* */

void animacion()
{

	//Movimiento de Don Cangrejo
	if (circuito1)
	{
		
		if (recorrido1)
		{
			rotCan = 0;
			movCanX -= 0.05f;
			if (movCanX < -20)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}

		if (recorrido2)
		{
			rotCan =180;
			movCanX += 0.05f;
			if (movCanX > 0)
			{
				recorrido2 = false;
				recorrido1 = true;
			}
		}
		
	}

	//Movimiento de Bob (brazo)
	if (circuito2)
	{

		if (recorrido3)
		{
			rotBrazo += 0.3;
			movBurguerY += 0.01;
			rotBurguer += 0.25;

			if (rotBrazo > 25)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}

		if (recorrido4)
		{
			rotBurguer += 5;
			if (rotBurguer > 360)
			{
				recorrido4 = false;
				recorrido5 = true;
			}
		}

		if (recorrido5)
		{
			rotBrazo -= 0.3;
			movBurguerY -= 0.01;
			rotBurguer -= 0.25;
			if (rotBrazo < 0)
			{
				rotBurguer = 0;
				recorrido5 = false;
				recorrido3 = true;
			}
		}
	}

	//Movimiento de Bob (camina)
	if (circuito3)
	{

		if (recorrido6)
		{
			BobDer = true;
			rotBob = -90;
			movBobZ += 0.02f;
			movBrazoX = 0.65;
			movBrazoZ = 0.6;
			movBurguerX = -0.15;
			movBurguerZ = 1.39;
			BobFrente = false;
			if (movBobZ > 4)
			{
				recorrido6 = false;
				recorrido7 = true;
			}
		}

		if (recorrido7)
		{
			BobDer = false;
			rotBob = 90;
			movBobZ -= 0.02f;
			movBrazoX = -0.57;
			movBrazoZ = 0.6;
			movBurguerX = -1.4;
			movBurguerZ = -0.2;
			if (movBobZ < 0)
			{
				recorrido7 = false;
				recorrido8 = true;
			}
		}

		if (recorrido8)
		{
			rotBob = 0;
			movBobX += 0.01f;
			movBrazoZ = 0.0;
			movBrazoX = 0.0;
			BobFrente = true;
			movBurguerX = -0.05;
			movBurguerZ = 0.0;
			if (movBobX > 0.35)
			{
				recorrido8 = false;
				recorrido9 = true;
			}
		}

		if (recorrido9)
		{
			movBobX -= 0.01f;
			if (movBobX < 0)
			{
				recorrido9 = false;
				recorrido10 = true;
			}
		}

		if (recorrido10)
		{
			BobFrente = false;
			rotBob = 90;
			movBobZ -= 0.02f;
			movBrazoZ = 0.69;
			movBrazoX = -0.57;
			movBurguerX = -1.41;
			movBurguerZ = -0.1;
			if (movBobZ < -4)
			{
				recorrido10 = false;
				recorrido11 = true;
			}
		}

		if (recorrido11)
		{
			BobDer = true;
			rotBob = -90;
			movBobZ += 0.02f;
			movBrazoZ = 0.55;
			movBrazoX = 0.62;
			movBurguerX = -0.18;
			movBurguerZ = 1.39;
			if (movBobZ > 0)
			{
				recorrido11 = false;
				recorrido6 = true;
			}
		}

	}

	//Movimiento de Puerta
	if (circuito4)
	{

		if (recorrido12)
		{
			rotPuerta += 0.25;
			if (rotPuerta > 65)
			{
				recorrido12 = false;
				recorrido13 = true;
			}
		}

		if (recorrido13)
		{
			rotPuerta -= 0.1;
			if (rotPuerta < 25)
			{
				recorrido13 = false;
				recorrido12 = true;
			}
		}

	}

	//Movimiento de Letrero
	if (circuito5)
	{

		if (recorrido14)
		{
			rotLet-= 0.08;
			if (rotLet < -8)
			{
				recorrido14 = false;
				recorrido15 = true;
			}
		}

		if (recorrido15)
		{
			rotLet += 0.05;
			if (rotLet > 0.5)
			{
				recorrido15 = false;
				recorrido14 = true;
			}
		}

	}

}