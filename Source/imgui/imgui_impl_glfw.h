// ImGui Platform Binding for: GLFW
// This needs to be used along with a Renderer (e.g. OpenGL3, Vulkan..)
// (Info: GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

// Implemented features:
//  [X] Platform: Clipboard support.
//  [X] Platform: Gamepad navigation mapping. Enable with 'io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad'.
//  [x] Platform: Mouse cursor shape and visibility. Disable with 'io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange'. FIXME: 3 cursors types are missing from GLFW.
//  [X] Platform: Keyboard arrays indexed using GLFW_KEY_* codes, e.g. ImGui::IsKeyPressed(GLFW_KEY_SPACE).

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

// About GLSL version:
// The 'glsl_version' initialization parameter defaults to "#version 150" if NULL.
// Only override if your GL version doesn't handle this GLSL version. Keep NULL if unsure!
#include "imgui.h"
#include "../structures.h"

struct GLFWwindow;

struct ImguiInputWrapper
{
	static IMGUI_IMPL_API bool     ImGui_ImplGlfw_InitForOpenGL(GLFWwindow* window, bool install_callbacks);
	static IMGUI_IMPL_API bool     ImGui_ImplGlfw_InitForVulkan(GLFWwindow* window, bool install_callbacks);
	static IMGUI_IMPL_API void     ImGui_ImplGlfw_Shutdown();
	static IMGUI_IMPL_API void     ImGui_ImplGlfw_NewFrame();

	// GLFW callbacks (installed by default if you enable 'install_callbacks' during initialization)
	// Provided here if you want to chain callbacks.
	// You can also handle inputs yourself and use those as a reference.
	static IMGUI_IMPL_API void     ImGui_ImplGlfw_MouseButtonCallback(MouseEvent mouse_);
	static IMGUI_IMPL_API void     ImGui_ImplGlfw_ScrollCallback(ScrollEvent scroll_);
	static IMGUI_IMPL_API void     ImGui_ImplGlfw_KeyCallback(KeyboardEvent key_);
	static IMGUI_IMPL_API void     ImGui_ImplGlfw_CharCallback(CharEvent char_);
};