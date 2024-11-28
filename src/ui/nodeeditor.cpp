#include "nodeeditor.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imnodes.h"

void NodeEditor::init(GLFWwindow* window) {
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImNodes::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void NodeEditor::reset() {
    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void NodeEditor::teardown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImNodes::DestroyContext();
    ImGui::DestroyContext();
}

void NodeEditor::show(
    int editorPosX,
    int editorPosY,
    int editorWidth,
    int editorHeight
) {

    // Set ImGui window pos and size to only overlay the right viewport
    ImGui::SetNextWindowPos(ImVec2(editorPosX, editorPosY));
    ImGui::SetNextWindowSize(ImVec2(editorWidth, editorHeight));

    ImGui::Begin("Node editor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    ImNodes::BeginNodeEditor();
    ImNodes::BeginNode(0);
    ImGui::Dummy(ImVec2(80.0f, 45.0f));
    ImNodes::BeginOutputAttribute(1);
    ImGui::Text("output pin");
    ImNodes::EndOutputAttribute();
    ImNodes::EndNode();
    ImNodes::EndNodeEditor();
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}