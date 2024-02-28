//********************************
// άσκηση 1Β της OpenGL

//ΑΜ:4001       Όνομα: ΧΡΙΣΤΙΝΑ ΟΛΥΜΠΙΑ ΣΟΛΔΑΤΟΥ
//ΑΜ:2935       Όνομα: ΙΩΑΝΝΑ ΑΠΟΣΤΟΛΟΠΟΥΛΟΥ

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <ctime>
#include <cstdlib>
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
// GLOBAL μεταβλητές
//****************************************************************
glm::vec4 help_position = glm::vec4(10.0f, 50.0f, 0.0f, 1.0f);
glm::vec3 camera_position = glm::vec3(10.0f, 50.0f, 0.0f);
glm::vec3 helpv = glm::vec3(0.0f,0.0f,0.0f) - camera_position;
glm::mat4 rotate1_x = glm::rotate(glm::mat4(1.0f), 0.01f, glm::vec3(1.0f, 0.0f, 0.0f));
glm::mat4 rotate2_x = glm::rotate(glm::mat4(1.0f), -0.01f, glm::vec3(1.0f, 0.0f, 0.0f));
glm::mat4 rotate1_z = glm::rotate(glm::mat4(1.0f), 0.01f, glm::vec3(0.0f, 0.0f, 1.0f));
glm::mat4 rotate2_z = glm::rotate(glm::mat4(1.0f),-0.01f, glm::vec3(0.0f, 0.0f, 1.0f));
glm::mat4 View;


///****************************************************************
// Συνάρτηση της κάμερας
//****************************************************************
void camera_function()
{
	// ΚΙΝΗΣΗ ΓΥΡΩ ΑΠΟ ΤΟΝ x'x
	if (glfwGetKey(window, GLFW_KEY_W)) {
		help_position = rotate1_x * help_position;
		camera_position = glm::vec3(help_position[0], help_position[1], help_position[2]);
		helpv = glm::vec3(0.0f, 0.0f, 0.0f) - camera_position;
	}
	if (glfwGetKey(window, GLFW_KEY_X)) {
		help_position = rotate2_x * help_position;
		camera_position = glm::vec3(help_position[0], help_position[1], help_position[2]);
		helpv = glm::vec3(0.0f, 0.0f, 0.0f) - camera_position;
	}
	// ΚΙΝΗΣΗ ΓΥΡΩ ΑΠΟ ΤΟΝ z'z
	if (glfwGetKey(window, GLFW_KEY_Q)) {
		help_position = rotate1_z * help_position;
		camera_position = glm::vec3(help_position[0], help_position[1], help_position[2]);
		helpv = glm::vec3(0.0f, 0.0f, 0.0f) - camera_position;
	}
	if (glfwGetKey(window, GLFW_KEY_Z)) {
		help_position = rotate2_z * help_position;
		camera_position = glm::vec3(help_position[0], help_position[1],help_position[2]);
		helpv = glm::vec3(0.0f, 0.0f, 0.0f) - camera_position;
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
		glm::vec3(0.0f, 0.0f, 0.0f),    // η κάμερα κοιτάζει εδώ
		glm::vec3(0.0f, 0.0f, 1.0f)     //up vector
	);
}


//************************************
//  LoadShaders 
//************************************
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


int main(void)
{
	
	//seed
	srand(time(NULL));

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1000, 1000, u8"Εργασία 1Β - Τραπεζοειδές Πρίσμα", NULL, NULL);


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
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	
	glEnable(GL_DEPTH_TEST);
	//Accept fragment if it closer to the camera than the former one
	//glDepthFunc(GL_LESS);
	
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders

	GLuint programID = LoadShaders("ProjBVertexShader.vertexshader", "ProjBFragmentShader.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	//********************
	// Projection matrix : 30° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(30.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	
	//ARXIKOPOIHSH
	// Camera matrix
	View = glm::lookAt(
		camera_position,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	// Model matrix 
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	static const GLfloat g_vertex_buffer_data[] = {
		 -1.0f,  3.0f, -1.0f,
		  1.0f,  3.0f, -1.0f,
		  4.0f, -3.0f, -1.0f,
		  4.0f, -3.0f, -1.0f,
		 -4.0f, -3.0f, -1.0f,
		 -1.0f,  3.0f, -1.0f,

		 -1.0f,  3.0f, -1.0f,
		 -1.0f,  3.0f,  1.0f,
		 -4.0f, -3.0f, -1.0f,
		 -4.0f, -3.0f, -1.0f,
		 -1.0f,  3.0f,  1.0f,
		 -4.0f, -3.0f,  1.0f,

		  1.0f,  3.0f, -1.0f,
		  1.0f,  3.0f,  1.0f,
		  4.0f, -3.0f, -1.0f,
		  4.0f, -3.0f, -1.0f,
		  1.0f,  3.0f,  1.0f,
		  4.0f, -3.0f,  1.0f,

		 -1.0f,  3.0f, -1.0f,
		  1.0f,  3.0f, -1.0f,
		  1.0f,  3.0f,  1.0f,
		  1.0f,  3.0f,  1.0f,
		 -1.0f,  3.0f,  1.0f,
		 -1.0f,  3.0f, -1.0f,

		 -4.0f, -3.0f, -1.0f,
		  4.0f, -3.0f, -1.0f,
		  4.0f, -3.0f,  1.0f,
		  4.0f, -3.0f,  1.0f,
		 -4.0f, -3.0f,  1.0f,
		 -4.0f, -3.0f, -1.0f,

		 -1.0f,  3.0f, 1.0f,
		  1.0f,  3.0f, 1.0f, 
		  4.0f, -3.0f, 1.0f,
		  4.0f, -3.0f, 1.0f,
		 -4.0f, -3.0f, 1.0f,
		 -1.0f,  3.0f, 1.0f

	};
	// Το ίδιο χρώμα σε κάθε πλευρά
	static const GLfloat g_color_buffer_data[] = {
		0.000f,  0.100f,  0.000f,
		0.000f,  0.100f,  0.000f,
		0.000f,  0.100f,  0.000f,
		0.000f,  0.100f,  0.000f,
		0.000f,  0.100f,  0.000f,
		0.000f,  0.100f,  0.000f,
		
		1.000f,  0.000f,  0.000f,
		1.000f,  0.000f,  0.000f,
		1.000f,  0.000f,  0.000f,
		1.000f,  0.000f,  0.000f,
		1.000f,  0.000f,  0.000f,
		1.000f,  0.000f,  0.000f,
		
		0.600f,  0.050f,  1.000f,
		0.600f,  0.050f,  1.000f,
		0.600f,  0.050f,  1.000f,
		0.600f,  0.050f,  1.000f,
		0.600f,  0.050f,  1.000f,
		0.600f,  0.050f,  1.000f,
		
		0.550f,  0.550f,  0.550f,
		0.550f,  0.550f,  0.550f,
		0.550f,  0.550f,  0.550f,
		0.550f,  0.550f,  0.550f,
		0.550f,  0.550f,  0.550f,
		0.550f,  0.550f,  0.550f,
		
		0.000f,  1.000f,  0.050f,
		0.000f,  1.000f,  0.050f,
		0.000f,  1.000f,  0.050f,
		0.000f,  1.000f,  0.050f,
		0.000f,  1.000f,  0.050f,
		0.000f,  1.000f,  0.050f,
		
		1.000f,  1.000f,  1.000f,
		1.000f,  1.000f,  1.000f,
		1.000f,  1.000f,  1.000f,
		1.000f,  1.000f,  1.000f,
		1.000f,  1.000f,  1.000f,
		1.000f,  1.000f,  1.000f
	};


	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	
	

	// τo Sz για το scale matrix, με αυτό πολλαπλασιάζεται η z συνιστώσα που είναι 1 ή -1 ώστε να γίνει z=Sz ή z=-Sz 
	//scale_up_down : random in [1.0, 5.0]
	 float  scale_up_down = 1 + (static_cast <float> (rand()) / static_cast <float>(RAND_MAX))*4;

	cout << scale_up_down;

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		
		//αύξηση ύψους
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			if (scale_up_down < 5.0f) {
				scale_up_down += 0.01f;
			}
		}
		//μείωση ύψους
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
			if (scale_up_down > 1.0f) {
				scale_up_down -= 0.01f;
			}
		}
		//transform matrix for scale up/scale down
		Model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, scale_up_down));

		//for camera movements
		camera_function();
		// new MVP
		MVP = Projection * View * Model;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 3 indices starting at 0 -> 1 triangle


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

