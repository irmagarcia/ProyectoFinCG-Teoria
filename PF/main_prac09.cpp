/*---------------------------------------------------------*/
/* ----------------   Proyecto Final --------------------------*/
/*-----------------    2021-1   ---------------------------*/
/*------------- Alumno: García Ramírez Irma  ---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include<time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void my_input(GLFWwindow *window);
void animate(void);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(-62.0f, 10.0f, 13.0f));
float MovementSpeed = 0.01f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

// Light
//glm::vec3 lightDirection = glm::vec3(-0.2f, -1.0f, -0.3f);
//Lighting
glm::vec3 lightPosition(21.0f, 11.0f, -8.0f);
glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);

glm::vec3 lightPosition2(-5.0f, 7.0f, 6.0f); 
glm::vec3 lightDirection2(0.0f, -1.0f, 0.0f);

glm::vec3 lightPosition3(-6.45f, 11.5f, -7.0f); 
glm::vec3 lightDirection3(0.0f, -1.0f, 0.0f);

glm::vec3 lightPosition4(18.3f, 9.5f, 0.0f);
glm::vec3 lightDirection4(0.0f, -1.0f, 0.0f);

glm::vec3 lightPosition5(21.3f, 10.5f, 2.2f); 
glm::vec3 lightDirection5(0.0f, -1.0f, 0.0f);

glm::vec3 lightPosition6(-6.5f, 10.2f, -7.0f); 
glm::vec3 lightDirection6(0.0f, -1.0f, 0.0f);

glm::vec3 lightPosition7(22.0f, 10.0f, 10.0f); 
glm::vec3 lightDirection7(0.0f, -1.0f, 0.0f);

glm::vec3 lightPosition8(16.0f, 10.0f, -2.0f); 
glm::vec3 lightDirection8(0.0f, -1.0f, 0.0f);

glm::vec3 lightPosition9(16.19f, 10.47f, 6.6f); 
glm::vec3 lightDirection9(0.0f, -1.0f, 0.0f);

glm::vec3 lightPosition10(7.0f, 11.0f, -6.0f); 
glm::vec3 lightDirection10(0.0f, -1.0f, 0.0f);

glm::vec3 lightPosition11(22.0f, 8.0f, 0.0f);
glm::vec3 lightDirection11(0.0f, -1.0f, 0.0f);

glm::vec3 lightPosition12(13.0f, 8.0f, -6.0f);
glm::vec3 lightDirection12(0.0f, -1.0f, 0.0f);

glm::vec3 lightPosition13(22.0f, 5.0f, 0.0f);
glm::vec3 lightDirection13(0.0f, -1.0f, 0.0f);
// posiciones
float x = 0.0f;
float y = 0.0f;
float movAuto_z = 0.0f;
float movAuto_y = -1.75f;
bool animacion = false;
bool avanza = false;
bool eleva = false;
bool reversa = true;
bool levitacion = false;
bool aniD = false;


void animate(void)
{
	if (animacion)
	{
		std::cout << "posicion camara= " << camera.Position.x << " en X" << std::endl;
		std::cout << "posicion camara= " << camera.Position.y << " en Y" << std::endl;
		std::cout << "posicion camara= " << camera.Position.z << " en Z" << std::endl;
	}
	
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 9", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	//Shader staticShader("Shaders/lightVertex.vs", "Shaders/lightFragment.fs");
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");

	vector<std::string> faces
	{
		"resources/skybox/right1.jpg",
		"resources/skybox/left1.jpg",
		"resources/skybox/top1.jpg",
		"resources/skybox/bottom1.jpg",
		"resources/skybox/front1.jpg",
		"resources/skybox/back1.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models MODELOS 3D
	// -----------
	Model piso("resources/objects/pisoarena/pisoarena.obj");
	Model estufa("resources/objects/estufa/estufa.obj");
	Model casa("resources/objects/casa/casa.obj");
	Model refrigerador("resources/objects/refrigerador/refrigerador.obj");
	Model lampara("resources/objects/lampara/lampara.obj");
	Model mecedora("resources/objects/mecedora/mecedora.obj");
	Model alfombra("resources/objects/alfombra/alfombra.obj");
	Model tv("resources/objects/tv/tv.obj");
	Model sillon("resources/objects/sillon/sillon.obj");
	Model mesa("resources/objects/mesa/mesa.obj");

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje 1
		// -------------------------------------------------------------------------------------------------------------------------
		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		//staticShader.setVec3("viewPos", camera.Position);
		//staticShader.setVec3("dirLight.direction", lightDirection); //fuente de luz direccional, comportamiento del sol-tierra- fuente inf grande y lejos de nosotros
		//staticShader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f)); //caras menos iluminadas tienden a color...
		//staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f)); //afecta a la cara poligonos
		//staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f)); //brillo en los objetos

		//staticShader.setVec3("pointLight[0].position", lightPosition); //como un foco colocamos una posicion y los rayos salen todas las posiciones
		//staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f)); 
		//staticShader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f)); //diferentes efectos de color
		//staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f)); 
		//staticShader.setFloat("pointLight[0].constant", 0.8f); // que tan brillosa va a ser --la potencia con la que se ve, es inversa, más cerca del cero es más potente
		//staticShader.setFloat("pointLight[0].linear", 0.009f); //entre mas pequeños (linear,cuadr) mas van a viajar los rayos luminosos
		//staticShader.setFloat("pointLight[0].quadratic", 0.032f); //la luz vieja más en el escenario

		//staticShader.setVec3("pointLight[1].position", glm::vec3(15.0, 0.0f, 0.0f));
		//staticShader.setVec3("pointLight[1].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		//staticShader.setVec3("pointLight[1].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		//staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		//staticShader.setFloat("pointLight[1].constant", 1.0f);
		//staticShader.setFloat("pointLight[1].linear", 0.009f);
		//staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		//staticShader.setFloat("material_shininess", 32.0f);
		//-------------------------------------------------------------------------------
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.8f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(0.0, 9.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 0.1f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setVec3("pointLight[2].position", lightPosition2); 
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[2].constant", 0.1f);//intensidad de la luz
		staticShader.setFloat("pointLight[2].linear", 0.0009f);//cuanto viaja la luz
		staticShader.setFloat("pointLight[2].quadratic", 0.032f);//cuanto viaja la luz

		staticShader.setVec3("pointLight[3].position", lightPosition3);
		staticShader.setVec3("pointLight[3].ambient", glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setVec3("pointLight[3].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[3].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[3].constant", 0.1f);//intensidad de la luz
		staticShader.setFloat("pointLight[3].linear", 0.0009f);//cuanto viaja la luz
		staticShader.setFloat("pointLight[3].quadratic", 0.032f);//cuanto viaja la luz

		staticShader.setVec3("pointLight[4].position", lightPosition4);
		staticShader.setVec3("pointLight[4].ambient", glm::vec3(0.4f, 0.4f, 0.4f));
		staticShader.setVec3("pointLight[4].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[4].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[4].constant", 0.1f);//intensidad de la luz
		staticShader.setFloat("pointLight[4].linear", 0.0009f);//cuanto viaja la luz
		staticShader.setFloat("pointLight[4].quadratic", 0.032f);//cuanto viaja la luz

		staticShader.setVec3("pointLight[5].position", lightPosition5);
		staticShader.setVec3("pointLight[5].ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setVec3("pointLight[5].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[5].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[5].constant", 0.1f);//intensidad de la luz
		staticShader.setFloat("pointLight[5].linear", 0.0009f);//cuanto viaja la luz
		staticShader.setFloat("pointLight[5].quadratic", 0.032f);//cuanto viaja la luz

		staticShader.setVec3("pointLight[6].position", lightPosition6);
		staticShader.setVec3("pointLight[6].ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setVec3("pointLight[6].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[6].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[6].constant", 0.1f);//intensidad de la luz
		staticShader.setFloat("pointLight[6].linear", 0.0009f);//cuanto viaja la luz
		staticShader.setFloat("pointLight[6].quadratic", 0.032f);//cuanto viaja la luz

		staticShader.setVec3("pointLight[7].position", lightPosition7);
		staticShader.setVec3("pointLight[7].ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setVec3("pointLight[7].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[7].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[7].constant", 0.1f);//intensidad de la luz
		staticShader.setFloat("pointLight[7].linear", 0.0009f);//cuanto viaja la luz
		staticShader.setFloat("pointLight[7].quadratic", 0.032f);//cuanto viaja la luz

		staticShader.setVec3("pointLight[8].position", lightPosition8);
		staticShader.setVec3("pointLight[8].ambient", glm::vec3(0.35f, 0.35f, 0.35f));
		staticShader.setVec3("pointLight[8].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[8].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[8].constant", 0.1f);//intensidad de la luz
		staticShader.setFloat("pointLight[8].linear", 0.0009f);//cuanto viaja la luz
		staticShader.setFloat("pointLight[8].quadratic", 0.032f);//cuanto viaja la luz

		staticShader.setVec3("pointLight[9].position", lightPosition9);
		staticShader.setVec3("pointLight[9].ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setVec3("pointLight[9].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[9].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[9].constant", 0.1f);//intensidad de la luz
		staticShader.setFloat("pointLight[9].linear", 0.0009f);//cuanto viaja la luz
		staticShader.setFloat("pointLight[9].quadratic", 0.032f);//cuanto viaja la luz

		staticShader.setVec3("pointLight[10].position", lightPosition10);
		staticShader.setVec3("pointLight[10].ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setVec3("pointLight[10].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[10].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[10].constant", 0.1f);//intensidad de la luz
		staticShader.setFloat("pointLight[10].linear", 0.0009f);//cuanto viaja la luz
		staticShader.setFloat("pointLight[10].quadratic", 0.032f);//cuanto viaja la luz

		staticShader.setVec3("pointLight[11].position", lightPosition10);
		staticShader.setVec3("pointLight[11].ambient", glm::vec3(0.35f, 0.35f, 0.35f));
		staticShader.setVec3("pointLight[11].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[11].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[11].constant", 0.1f);//intensidad de la luz
		staticShader.setFloat("pointLight[11].linear", 0.0009f);//cuanto viaja la luz
		staticShader.setFloat("pointLight[11].quadratic", 0.032f);//cuanto viaja la luz

		staticShader.setVec3("pointLight[12].position", lightPosition10);
		staticShader.setVec3("pointLight[12].ambient", glm::vec3(0.35f, 0.35f, 0.35f));
		staticShader.setVec3("pointLight[12].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[12].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[12].constant", 0.1f);//intensidad de la luz
		staticShader.setFloat("pointLight[1].linear", 0.0009f);//cuanto viaja la luz
		staticShader.setFloat("pointLight[12].quadratic", 0.032f);//cuanto viaja la luz

		staticShader.setVec3("pointLight[13].position", lightPosition10);
		staticShader.setVec3("pointLight[13].ambient", glm::vec3(0.35f, 0.35f, 0.35f));
		staticShader.setVec3("pointLight[13].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[13].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[13].constant", 0.1f);//intensidad de la luz
		staticShader.setFloat("pointLight[13].linear", 0.0009f);//cuanto viaja la luz
		staticShader.setFloat("pointLight[13].quadratic", 0.032f);//cuanto viaja la luz


		staticShader.setFloat("material_shininess", 32.0f);

		//para usar más luces posicionales, se modifica en shader ligh fs, en define y se deben definir 
		//para que no cause error en los vectores [..]


		glm::mat4 model = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(1.0f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);



		staticShader.setVec3("material.specular", glm::vec3(0.5f));
		staticShader.setFloat("material.shininess", 32.0f);
		staticShader.setVec3("light.ambient", ambientColor);
		staticShader.setVec3("light.diffuse", diffuseColor);
		staticShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		staticShader.setVec3("light.direction", lightDirection);
		staticShader.setVec3("viewPos", camera.Position);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		casa.Draw(staticShader);  

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8, -1.5f, -3.5f));
		model = glm::scale(model, glm::vec3(0.1f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		refrigerador.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(22.5, -1.5f, 10.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		estufa.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5, -1.5f, 2.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		mecedora.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-9.5, -1.5f, 2.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		sillon.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.0, -1.4f,-1.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		alfombra.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0, -1.4f, 6.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		lampara.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.5, -1.5f, -7.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		tv.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(22.0, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);
		//glEnable(GL_BLEND);

		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Creo que limitan el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (animacion)
			animacion = false;
		else
			animacion = true;
	}
	
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.MovementSpeed = MovementSpeed * 2.5f;
	else
		camera.MovementSpeed = MovementSpeed;

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		lightPosition.x -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		lightPosition.x += 1.0f;

	

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}