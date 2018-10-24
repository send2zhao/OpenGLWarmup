
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <SDL.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"

/*
	arcball: http://courses.cms.caltech.edu/cs171/assignments/hw3/hw3-notes/notes-hw3.html


*/

const double pi = 3.1415926535897;

static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 600;

int main(int argc, char** argv)
{
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");

	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1)),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0)),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0)),
	};

	unsigned int indices[] = {0, 1, 2,
							  0, 2, 3,

							  6, 5, 4,
							  7, 6, 4,

							  10, 9, 8,
							  11, 10, 8,

							  12, 13, 14,
							  12, 14, 15,

							  16, 17, 18,
							  16, 18, 19,

							  22, 21, 20,
							  23, 22, 20
	                          };

	Vertex vertices2[] =
	{
	
		Vertex(glm::vec3(-1, -1,  1),  glm::vec2(0, 1), glm::vec3(0, 0, -1)),  // 0 - 3: front face
		Vertex(glm::vec3( 1, -1,  1),  glm::vec2(1, 1), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(-1,  1,  1),  glm::vec2(0, 0), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3( 1,  1,  1),  glm::vec2(1, 0), glm::vec3(0, 0, -1)),

		Vertex(glm::vec3( 1, -1,  1),  glm::vec2(0, 1), glm::vec3(1, 0, 0)),   // 4 - 7: right face
		Vertex(glm::vec3( 1, -1, -1),  glm::vec2(1, 1), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3( 1,  1,  1),  glm::vec2(0, 0), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3( 1,  1, -1),  glm::vec2(1, 0), glm::vec3(1, 0, 0)),

		Vertex(glm::vec3( 1, -1, -1),  glm::vec2(0, 1), glm::vec3(0, -1, 0)),  // 8 - 11: back face
		Vertex(glm::vec3(-1, -1, -1),  glm::vec2(1, 1), glm::vec3(0, -1, 0)),
		Vertex(glm::vec3( 1,  1, -1),  glm::vec2(0, 0), glm::vec3(0, -1, 0)),
		Vertex(glm::vec3(-1,  1, -1),  glm::vec2(1, 0), glm::vec3(0, -1, 0)),

		Vertex(glm::vec3(-1, -1, -1),  glm::vec2(0, 1), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, -1,  1),  glm::vec2(1, 1), glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1,  1, -1),  glm::vec2(0, 0), glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1,  1,  1),  glm::vec2(1, 0), glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1),  glm::vec2(0, 1), glm::vec3(-1, 0, 0)),
		Vertex(glm::vec3( 1, -1, -1),  glm::vec2(1, 1), glm::vec3(-1, 0, 0)),
		Vertex(glm::vec3(-1, -1,  1),  glm::vec2(0, 0), glm::vec3(-1, 0, 0)),
		Vertex(glm::vec3( 1, -1,  1),  glm::vec2(1, 0), glm::vec3(-1, 0, 0)),

		Vertex(glm::vec3(-1,  1,  1),  glm::vec2(0, 1), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3( 1,  1,  1),  glm::vec2(1, 1), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1,  1, -1),  glm::vec2(0, 0), glm::vec3(1, 0, 0)),
		Vertex(glm::vec3( 1,  1, -1),  glm::vec2(1, 0), glm::vec3(1, 0, 0)),
	};

	unsigned int indices2[] = {
		//Faces definition
		0,1,3, 0,3,2,           //Face front
		4,5,7, 4,7,6,           //Face right
		8,9,11, 8,11,10,        //...
		12,13,15, 12,15,14,
		16,17,19, 16,19,18,
		20,21,23, 20,23,22
	};


	unsigned int indices3[] = {
		//Faces definition
		0,1,3, 0,3,2,           //Face front
		4,5,7, 4,7,6,           //Face right
		8,9,11, 8,11,10,        //...
		12,13,15, 12,15,14,
		16,17,19, 16,19,18,
		20,21,23, 20,23,22
		
	};


	// Mesh mesh(vertices, sizeof(vertices)/sizeof(vertices[0]), indices, sizeof(indices)/sizeof(indices[0]));
	// Mesh mesh(vertices2, sizeof(vertices2) / sizeof(vertices2[0]), indices2, sizeof(indices2) / sizeof(indices2[0]));
	Mesh mesh(vertices2, sizeof(vertices2) / sizeof(vertices2[0]), indices3, sizeof(indices3) / sizeof(indices3[0]));
	// Mesh monkey("./res/monkey3.obj");
	Shader shader("./res/basicShader");
	//Texture texture("./res/bricks.jpg");
	//Texture texture  = Texture("./res/img2.jpg"); texture.print();
	//Texture texture2 = Texture("./res/bricks.jpg"); texture2.print();

	std::vector<std::shared_ptr<Texture>> t_list;
	{
		std::shared_ptr<Texture> t1 = std::shared_ptr<Texture>(new Texture("./res/LETTER-A.png"));
		std::shared_ptr<Texture> t2 = std::shared_ptr<Texture>(new Texture("./res/LETTER-B.png"));
		std::shared_ptr<Texture> t3 = std::shared_ptr<Texture>(new Texture("./res/LETTER-C.png"));
		std::shared_ptr<Texture> t4 = std::shared_ptr<Texture>(new Texture("./res/LETTER-D.png"));
		std::shared_ptr<Texture> t5 = std::shared_ptr<Texture>(new Texture("./res/LETTER-E.png"));
		std::shared_ptr<Texture> t6 = std::shared_ptr<Texture>(new Texture("./res/LETTER-F.png"));
		t_list.push_back(t1); t_list.push_back(t2); t_list.push_back(t3);
		t_list.push_back(t4); t_list.push_back(t5); t_list.push_back(t6);
	}

	Transform transform;
	Camera camera(glm::vec3(0.0f, 0.0f, -5.0f), 70.0f, (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, 0.1f, 100.0f);
	camera.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
	camera.setForward(glm::vec3(0.0f, 0.0f, -1.0f));
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	ImGui_ImplSDL2_InitForOpenGL(display.getWindow(), display.getContext() );
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	bool show_demo_window = true;

	SDL_Event event;
	bool isRunning = true;
	// float counter = 0.0f;
	static int counter = 0;
	while(isRunning)
	{
		while(SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if(event.type == SDL_QUIT)
				isRunning = false;
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(display.getWindow()))
				isRunning = false;
		}


		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(display.getWindow());
		ImGui::NewFrame();


		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float ry = 0.0f;
			static float rx = 0.0f;
			static float rz = 0.0f;
			// static int counter = 0;
			static glm::vec3 pos = glm::vec3(0.0f, 0.0f, 5.0f);
			static glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			// ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("ry", &ry, 0.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("rx", &rx, 0.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::SliderFloat("rz", &rz, 0.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat3("position", &pos.x, -5.0f, 5.0f);
			ImGui::SliderFloat3("forward", &forward.x, -5.0f, 5.0f);

			// ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

			transform.GetRot()->y = ry / 180 * pi;
			transform.GetRot()->x = rx / 180 * pi;
			transform.GetRot()->z = rz / 180 * pi;
			camera.setPosition(pos);
			if (glm::length(forward) != 0) camera.setForward(forward);

		}

		ImGui::Render();
		display.Clear(0.0f, 0.0f, 0.0f, 1.0f);

		shader.Bind();
		// shader.Update(transform, camera, texture);
		mesh.Bind();
		//shader.Update(transform, camera, texture);
		//shader.Update2(transform, camera, texture);
		// shader.Update(transform, camera, textures);
		shader.Update(transform, camera, t_list);
		// monkey.Draw();
		// mesh.Draw();
		size_t size = (size_t)(DISPLAY_HEIGHT * DISPLAY_WIDTH);
		std::unique_ptr<char[]> pt = std::make_unique<char[]>(size*4);
		// GLubyte *data = new GLubyte[size * 4];
		

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		display.SwapBuffers();
		if (counter == 3)
		{
			counter = 0;
			glReadPixels(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pt.get());
			std::ofstream myFile("c:/temp/openGL.raw", std::ios::out | std::ios::binary);
			myFile.write((char*)pt.get(), size * 3);
			myFile.close();
		}

		
		SDL_Delay(1);
		// counter += 0.01f;
	}

	return 0;
}
