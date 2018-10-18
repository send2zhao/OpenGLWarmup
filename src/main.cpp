
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <SDL.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"

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

	Mesh mesh(vertices, sizeof(vertices)/sizeof(vertices[0]), indices, sizeof(indices)/sizeof(indices[0]));
	Mesh monkey("./res/monkey3.obj");
	Shader shader("./res/basicShader");
	//Texture texture("./res/bricks.jpg");
	Texture texture("./res/img2.jpg");
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
	float counter = 0.0f;
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
			static int counter = 0;
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

			transform.GetRot()->y = ry;
			transform.GetRot()->x = rx;
			transform.GetRot()->z = rz;
			camera.setPosition(pos);
			if (glm::length(forward) != 0) camera.setForward(forward);

		}

		ImGui::Render();
		display.Clear(0.0f, 0.0f, 0.0f, 1.0f);

		//float sinCounter = sinf(counter);
		//float absSinCounter = abs(sinCounter);
		//transform.GetPos()->x = sinCounter;
		//transform.GetRot()->y = f * 100;
		//transform.GetRot()->z = counter * 100;
		//transform.GetScale()->x = absSinCounter;
		//transform.GetScale()->y = absSinCounter;

		shader.Bind();
		shader.Update(transform, camera, texture);
		monkey.Draw();
		mesh.Draw();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		display.SwapBuffers();
		
		SDL_Delay(1);
		counter += 0.01f;
	}

	return 0;
}
