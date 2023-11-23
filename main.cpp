#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "WindowClasses.h"

int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 800;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Matrix Calculator", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// for not clicking both windows at the same time
		if (!io.WantCaptureMouse) {}

		// ----------------------------------------------------------------------------
		// de adaugat: inversa unei matrice, rangul unei matrice

		A.Run();
		_A.Run();
		B.Run();
		_B.Run();

		if (rezultate.empty()) {
			MatriceRezultat::contor = 0;
		}
		for (auto rez = rezultate.begin(); rez != rezultate.end(); ++rez) {
			if (!rez->running) {
				if (rez == rezultate.end()) {
					MatriceRezultat::contor--;
				}
				rezultate.erase(rez);
				break;
			}
			else {
				rez->CreateWindow();
			}
		}

		eroare.CreateWindow();

		// ----------------------------------------------------------------------------

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}