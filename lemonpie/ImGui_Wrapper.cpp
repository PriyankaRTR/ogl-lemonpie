#include "ImGui_Wrapper.h"
	

// to understand initialization of reference
// https://stackoverflow.com/questions/12890438/reference-variable-with-error-must-be-initialized-in-constructor-base-member-in
// https://stackoverflow.com/questions/16888740/initializing-a-reference-variable-in-a-class-constructor

ImGui_Wrapper::ImGui_Wrapper(HWND hwnd, ImGuiIO& newRef) : io(newRef) 
{

	(void)io;
	// Our state ImGui
	show_demo_window = true;
	show_another_window = false;
	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	matrixToDisplay = vmath::mat4::identity();
	inputCameraAngle = 90.0;
}

ImGui_Wrapper::~ImGui_Wrapper()
{

}

bool ImGui_Wrapper::setupImGui()
{
	io.MouseDrawCursor = true; // added to show mouse cursor in full screen mode
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

	////setup Dear ImGui style
	//ImGui::StyleColorsDark();
	////ImGui::StyleColorsClassic();

	//// Setup Platform/Renderer backends
	//ImGui_ImplWin32_InitForOpenGL(hwnd);
	//ImGui_ImplOpenGL3_Init();

	return true;
}
	

void ImGui_Wrapper::gameLoopUIUpdatesImGui(ImGuiContext* newCtx)
{
	ImGuiContext* tempCtx = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(newCtx);
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("CameraAngle", &inputCameraAngle, 360.0f, 0.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

		if (ImGui::BeginTable("table1", 4))
		{
			for (int row = 0; row < 4; row++)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < 4; column++)
				{
				/*	ImGui::TableSetColumnIndex(column);
					ImGui::Text("Row %d Column %d", row, column);*/
					ImGui::TableSetColumnIndex(column);
					ImGui::Text("Col %d : %f", column, matrixToDisplay[row][column]);
				}
			}
			ImGui::EndTable();
		}

		//if (ImGui::BeginTable("table1", 3))
		//{
		//	for (int row = 0; row < 4; row++)
		//	{
		//		ImGui::TableNextRow();
		//		for (int column = 0; column < 3; column++)
		//		{
		//			ImGui::TableSetColumnIndex(column);
		//			ImGui::Text("Row %d Column %d", row, column);
		//		}
		//	}
		//	ImGui::EndTable();
		//}

		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
		else
			ImGui::Text("Mouse pos: <INVALID>");
		ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
		ImGui::Text("Mouse down:");
		for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDown(i)) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
		ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);

		ImGui::End();


	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}


	// Rendering
	ImGui::Render();
	//glViewport(0, 0, 600, 400);
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool ImGui_Wrapper::cleanUpImGui(void)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	return true;
}


void ImGui_Wrapper::setMatrix(vmath::mat4& newMatrix)
{

	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			matrixToDisplay[row][column] = newMatrix[row][column];
		}
	}
}

float ImGui_Wrapper::getInputCameraAngle(void)
{
	return inputCameraAngle;
}