// Άσκηση 1Γ της OpenGL

//ΑΜ: 2935      Όνομα: ΙΩΑΝΝΑ ΑΠΟΣΤΟΛΟΠΟΥΛΟΥ   
//ΑΜ: 4001      Όνομα: ΧΡΙΣΤΙΝΑ ΟΛΥΜΠΙΑ ΣΟΛΔΑΤΟΥ  


#define _CRT_SECURE_NO_WARNINGS

#define STB_IMAGE_IMPLEMENTATION

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "stb_image.h"
#include <cstdlib>
#include <ctime>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;

///****************************************************************
// GLOBAL 
//****************************************************************
glm::vec4 help_position = glm::vec4(21.5f, -109.4f, 134.8f, 1.0f);
glm::vec3 camera_position = glm::vec3(21.5f, -109.4f, 134.8f);
glm::vec3 helpv = glm::vec3(50.0f, 50.0f, 0.0f) - camera_position;
glm::mat4 rotate1_x = glm::rotate(glm::mat4(1.0f), 0.01f, glm::vec3(1.0f, 0.0f, 0.0f));
glm::mat4 rotate2_x = glm::rotate(glm::mat4(1.0f), -0.01f, glm::vec3(1.0f, 0.0f, 0.0f));
glm::mat4 rotate1_z = glm::rotate(glm::mat4(1.0f), 0.01f, glm::vec3(0.0f, 0.0f, 1.0f));
glm::mat4 rotate2_z = glm::rotate(glm::mat4(1.0f), -0.01f, glm::vec3(0.0f, 0.0f, 1.0f));
glm::mat4 View;

glm::mat4 ModelMatrix_ball;
float  xb, yb;
float  zb = 20.0f;
float  zb_step = 0.0f;
bool  ball_falling = false;

std::vector<glm::vec3> crater_trans;
int craters_num = 0;
glm::mat4 ModelMatrix_crater=glm::mat4(1.0f);






// Η παρακάτω συνάρτηση είναι από http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
// H συνάρτηση loadOBJ φορτώνει ένα αντικείμενο από το obj αρχείο του και φορτώνει και normals kai uv συντεταγμένες 

bool loadOBJ(
	const char* path,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals
) {
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				fclose(file);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}
	fclose(file);
	return true;
}


//  LOADSHADERS

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


///****************************************************************
//  Εδω θα υλοποιήσετε την συνάρτηση της κάμερας
//****************************************************************
void camera_function()
{
	// ΚΙΝΗΣΗ ΓΥΡΩ ΑΠΟ ΤΟΝ x'x
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		help_position = rotate1_x * help_position;
		camera_position = glm::vec3(help_position[0], help_position[1], help_position[2]);
		helpv = glm::vec3(50.0f, 50.0f, 0.0f) - camera_position;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		help_position = rotate2_x * help_position;
		camera_position = glm::vec3(help_position[0], help_position[1], help_position[2]);
		helpv = glm::vec3(50.0f, 50.0f, 0.0f) - camera_position;
	}
	// ΚΙΝΗΣΗ ΓΥΡΩ ΑΠΟ ΤΟΝ z'z
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		help_position = rotate1_z * help_position;
		camera_position = glm::vec3(help_position[0], help_position[1], help_position[2]);
		helpv = glm::vec3(50.0f, 50.0f, 0.0f) - camera_position;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		help_position = rotate2_z * help_position;
		camera_position = glm::vec3(help_position[0], help_position[1], help_position[2]);
		helpv = glm::vec3(50.0f, 50.0f, 0.0f) - camera_position;
	}
	//ZOOM
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		camera_position += 0.01f * helpv;
		help_position = glm::vec4(camera_position[0], camera_position[1], camera_position[2], 1.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		camera_position -= 0.01f * helpv;
		help_position = glm::vec4(camera_position[0], camera_position[1], camera_position[2], 1.0f);
	}

	View = glm::lookAt(
		camera_position,   // θέση της κάμερας
		glm::vec3(50, 50, 0),    // η κάμερα κοιτάζει εδώ
		glm::vec3(0, 0, 1)     //up vector
	);

	//cout << camera_position[0] << " " << camera_position[1] << " " << camera_position[2] << "\n";

}


//gia tous kratires
//o pinakas crater_trans[] periexei tis theseis stis opoies prepei na metatopisoume ton kratira diladi ekei pou peftei i mpala
//craters_num einai to plithos twn kratirwn
void crater_function(float x_b, float y_b) {
	float xmod = fmod(x_b, 5.0f);
	float ymod = fmod(y_b, 5.0f);
	float x_new, y_new;
	//vriskoume to pio kontino pollaplasio tou 5 twn x,y tis mpalas 
	//epeidi ta x,y twn korifwn tou plegmatos einai pollaplasia tou 5
	if (xmod < 2.5f) {
		x_new = x_b - xmod;
	}
	else {
		x_new = x_b + (5.0f - xmod);
	}
	if (ymod < 2.5f) {
		y_new = y_b - ymod;
	}
	else {
		y_new = y_b + (5.0f - ymod);
	}
	//epeidi o kratiras tha metatopistei kata x_new sto x kai kata y_new sto y, gia na min vgei ektos plegmatos 
	if (x_new == 100.0f) {
		x_new = 95.0f;
	}
	if (y_new == 100.0f) {
		y_new = 95.0f;
	}
	//an exei ksanapesei sto ido simeio i mpala na min apothikeftei pali i thesi
	for (int i = 0; i < craters_num; i++) {
		if (x_new==crater_trans[i].x && y_new == crater_trans[i].y) {
			break;
			return;
		}
	}
	crater_trans.push_back(glm::vec3(x_new, y_new, 0.0f));
	craters_num += 1;
}


//sinartisi gia tis kiniseis ts mpalas
//gia na peftei i mpala ginetai translation - meiwnetai to z 
void ball_function() {
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		if (!ball_falling) {
			zb_step = -0.05f;
			ball_falling = true;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		if(ball_falling){
			//afksisi taxititas
			zb_step = -0.10f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		if (ball_falling) {
			//meiwsi taxititas
			zb_step = -0.01f;
		}
	}
	zb += zb_step;
	ModelMatrix_ball = glm::translate(glm::mat4(1.0), glm::vec3(xb, yb, zb)) * glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 5.0f));
	
	if (zb <= 1.25) {
		//an i mpala akoumpisei to edafos
		// kaleitai i sinartisi gia tous kratires, gia na prostethei mia akomi thesi metatopisis(emfanisis) tou kratira
		crater_function(xb,yb);
		//nees tyxaies sintetagmenes tis mpalas
		xb = (static_cast <float> (rand()) / static_cast <float>(RAND_MAX)) * 100;
		yb = (static_cast <float> (rand()) / static_cast <float>(RAND_MAX)) * 100;
		//to z ginetai pali 20
		zb = 20.0f;
		//neos pinakas metasximatismou
		ModelMatrix_ball = glm::translate(glm::mat4(1.0), glm::vec3(xb, yb, 20.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 5.0f));
		//to vima 0 gia na menei statheri i mpala
		zb_step = 0.0f;
		//ball_falling = false gia na min sxediazetai mexri na ksanapatithei to B
		ball_falling = false;
		
	}
}


int main(void)
{

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	//seed
	srand(time(NULL));

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1000, 1000, u8"Εργασία 1Γ - Καταστροφή", NULL, NULL);


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	
	GLuint programID = LoadShaders("ProjCVertexShader.vertexshader", "ProjCFragmentShader.fragmentshader");
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	

	float x = 0.0f;
	float y = 100.0f;
	float z = 0.0f;
	int v = 0;
	//-----GROUND XYZ coords 
	GLfloat ground_vertices[21 * 21 * 3] = { 0 };
	for (int i = 0; i < 21 * 21 * 3; i += 3) {
		ground_vertices[i] = x;
		ground_vertices[i + 1] = y;
		ground_vertices[i + 2] = z;
		y = y - 5.0f;
		v = v + 1;
		if ((v) % 21 == 0) {
			x = x + 5.0f;
			y = 100.0f;
		}
	}
	GLuint ground_vertexbuffer;
	glGenBuffers(1, &ground_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ground_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertices), ground_vertices, GL_STATIC_DRAW);

	//-----GROUND  UV coords 
	y = 100.0f;
	x = 0.0f;
	v = 0;
	GLfloat ground_uvs[21 * 21 * 2] = { 0 };
	for (int i = 0; i < 21 * 21 * 2; i += 2) {
		ground_uvs[i] = float(x) / 100.0f;
		ground_uvs[i + 1] = float(y) / 100.0f;
		y = y - 5.0f;
		v = v + 1;
		if ((v) % 21 == 0) {
			x = x + 5.0f;
			y = 100.0f;
		}
	}
	GLuint ground_uvbuffer;
	glGenBuffers(1, &ground_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ground_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_uvs), ground_uvs, GL_STATIC_DRAW);


	//kathe trigwno perigrafetai apo treis akeraious, enan akeraio gia kathe korifi
	//o pinakas indices exei tis perigrafes twn 800 trigwnwn tou plegmatos
	GLuint indices[400 * 2 * 3] = { 0 };
	int vertice = 0;
	for (int i = 0; i < 400 * 2 * 3; i += 6) {
		if (vertice % 21 == 20) {
			vertice += 1;
		}
		indices[i] = vertice;
		indices[i + 1] = vertice + 21;
		indices[i + 2] = vertice + 22;
		indices[i + 3] = vertice + 22;
		indices[i + 4] = vertice + 1;
		indices[i + 5] = vertice;
		vertice += 1;
	}
	GLuint element_buffer;
	glGenBuffers(1, &element_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// -----GROUND texture load
	int width1, height1, nrChannels1;
	unsigned char* ground_data = stbi_load("ground1.jpg", &width1, &height1, &nrChannels1, 0);

	if (!ground_data)
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	GLuint ground_textureID;
	glGenTextures(1, &ground_textureID);
	glBindTexture(GL_TEXTURE_2D, ground_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, ground_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	GLuint Ground_TextureID = glGetUniformLocation(programID, "ground_texture");

	//-----CRATER  XYZ coords 
	GLfloat crater_vertices[] = {
		0.0f , 5.0f, 0.02f,
		5.0f , 5.0f, 0.02f,
		5.0f , 0.0f, 0.02f,
		5.0f , 0.0f, 0.02f,
		0.0f , 0.0f, 0.02f,
		0.0f , 5.0f, 0.02f
	};
	GLuint crater_vertexbuffer;
	glGenBuffers(1, &crater_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, crater_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(crater_vertices), crater_vertices, GL_STATIC_DRAW);

	//----- CRATER UV coords 
	GLfloat crater_uvs[] = {
		0.0f , 1.0f,
		1.0f , 1.0f,
		1.0f , 0.0f,
		1.0f , 0.0f,
		0.0f , 0.0f,
		0.0f , 1.0f
	};

	GLuint crater_uvbuffer;
	glGenBuffers(1, &crater_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, crater_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(crater_uvs), &crater_uvs, GL_STATIC_DRAW);


	//------ CRATER  texture load
	int width2, height2, nrChannels2;
	unsigned char* crater_data = stbi_load("crater.jpg", &width2, &height2, &nrChannels2, 0);

	if (!crater_data)
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	GLuint crater_textureID;
	glGenTextures(1, &crater_textureID);
	glBindTexture(GL_TEXTURE_2D, crater_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, crater_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	GLuint Crater_TextureID = glGetUniformLocation(programID, "crater_texture");
	
	// ------BALL texture load
	int width3, height3, nrChannels3;
	unsigned char* ball_data = stbi_load("ball.jpg", &width3, &height3, &nrChannels3, 0);

	if (!ball_data)
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	GLuint ball_textureID;
	glGenTextures(1, &ball_textureID);
	glBindTexture(GL_TEXTURE_2D, ball_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width3, height3, 0, GL_RGB, GL_UNSIGNED_BYTE, ball_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	GLuint Ball_TextureID = glGetUniformLocation(programID, "ball_texture");

	// Read ball.obj file
	std::vector<glm::vec3> ball_vertices;
	std::vector<glm::vec3> ball_normals;
	std::vector<glm::vec2> ball_uvs;
	bool res = loadOBJ("ball.obj", ball_vertices, ball_uvs, ball_normals);

	GLuint ball_vertexbuffer;
	glGenBuffers(1, &ball_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ball_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, ball_vertices.size() * sizeof(glm::vec3), &ball_vertices[0], GL_STATIC_DRAW);

	GLuint ball_uvbuffer;
	glGenBuffers(1, &ball_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ball_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, ball_uvs.size() * sizeof(glm::vec2), &ball_uvs[0], GL_STATIC_DRAW);

	//arxiki thesi tis mpalas, to x kai to y einai tyxaia, to z einai 20.
	xb = (static_cast <float> (rand()) / static_cast <float>(RAND_MAX)) * 100;
	yb = (static_cast <float> (rand()) / static_cast <float>(RAND_MAX)) * 100;
	//gia na megalosei kai na metatopistei h mpala 
	ModelMatrix_ball = glm::translate(glm::mat4(1.0), glm::vec3(xb, yb, 20.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 5.0f));

	// Projection matrix : 30° Field of View, 4:3 ratio, display range : 0.1 unit <-> 500 units
	glm::mat4 Projection = glm::perspective(glm::radians(30.0f), 4.0f / 3.0f, 0.1f, 500.0f);
	//pinakas model gia to edafos 
	glm::mat4 Model = glm::mat4(1.0f);

	
	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		//ga tis kiniseis tis kameras
		camera_function();
		
		// ----GROUND------
		glm::mat4 MVP = Projection * View * Model;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		
		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ground_textureID);
		// Set "Ground_TextureID" sampler to use Texture Unit 0
		glUniform1i(Ground_TextureID, 0);
		
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, ground_vertexbuffer);
		glVertexAttribPointer(
			0,                  
			3,                  
			GL_FLOAT,           
			GL_FALSE,           
			0,                  
			(void*)0            
		);
		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, ground_uvbuffer);
		glVertexAttribPointer(
			1,                                
			2,                                
			GL_FLOAT,                         
			GL_FALSE,                         
			0,                                
			(void*)0                          
		);
		//gia ton sxediasmo tou edafous
		glDrawElements(GL_TRIANGLES, 2400,GL_UNSIGNED_INT,(void*)0);
		
		// ----BALL------
		ball_function();
		if (ball_falling) {
			MVP = Projection * View * ModelMatrix_ball;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ball_textureID);
			glUniform1i(Ball_TextureID, 0);

			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, ball_vertexbuffer);
			glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
			);
			// 2nd attribute buffer : UVs
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, ball_uvbuffer);
			glVertexAttribPointer(
				1,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
			);
			//gia ton sxediasmo tis mpalas
			glDrawArrays(GL_TRIANGLES, 0, ball_vertices.size());

		}

		// ----CRATERS------
		for (int i = 0; i < craters_num; i++) {
			ModelMatrix_crater = glm::translate(glm::mat4(1.0), crater_trans[i]);
			MVP = Projection * View * ModelMatrix_crater;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, crater_textureID);
			glUniform1i(Crater_TextureID, 0);
			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, crater_vertexbuffer);
			glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
			);
			// 2nd attribute buffer : UVs
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, crater_uvbuffer);
			glVertexAttribPointer(
				1,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)0
			);
			//gia ton sxediasmo twn kratirwn
			glDrawArrays(GL_TRIANGLES, 0, 2*3);
		}
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		
	} // Check if the SPACE key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
	
	// Cleanup 
	glDeleteBuffers(1, &ground_vertexbuffer);
	glDeleteBuffers(1, &crater_vertexbuffer);
	glDeleteBuffers(1, &ball_vertexbuffer);
	glDeleteBuffers(1, &ground_uvbuffer);
	glDeleteBuffers(1, &crater_uvbuffer);
	glDeleteBuffers(1, &ball_uvbuffer);
	glDeleteTextures(1, &ball_textureID);
	glDeleteTextures(1, &ground_textureID);
	glDeleteTextures(1, &crater_textureID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);
	
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;

}

