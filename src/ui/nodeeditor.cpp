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

int NodeEditor::getNewId() {
    return uniqueId++;
}

void NodeEditor::addNode(NodeType nodeType, const string& name) {
    int nodeId = getNewId();
    ImNodes::SetNodeScreenSpacePos(nodeId, ImGui::GetMousePos());
    auto it = nodeList.insert(nodeList.end(), {nodeType, name, nodeId, getNewId(), getNewId()});
    nodeIdMap[nodeId] = it;
    dirty = true;
}

void NodeEditor::maybeChangeNodeMenuState() {
    const bool open_popup = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
                            ImNodes::IsEditorHovered() && ImGui::IsKeyReleased(ImGuiKey_Tab);

    if (!ImGui::IsAnyItemHovered() && open_popup)
    {
        ImGui::OpenPopup("Add Node");
    }

    if (ImGui::BeginPopup("Add Node"))
    {
        if (ImGui::MenuItem("Move")) {
            addNode(NodeType::MOVE, "Move");
        }
        else if (ImGui::MenuItem("Restore")) {
            addNode(NodeType::RESTORE, "Restore");
        }
        else if (ImGui::MenuItem("Store")) {
            addNode(NodeType::STORE, "Store");
        }
        
        ImGui::EndPopup();
    }
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
    ImGui::Begin("Node editor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse );
    ImNodes::BeginNodeEditor();

    maybeChangeNodeMenuState();

    for (auto& node : nodeList) {
        ImNodes::BeginNode(node.id);
        ImNodes::BeginNodeTitleBar();
        ImGui::Text("%s", node.name.c_str());
        ImNodes::EndNodeTitleBar();
        ImNodes::BeginInputAttribute(node.inpinId + 1);
        ImGui::Text("input pin");
        ImNodes::EndInputAttribute();
        ImNodes::BeginOutputAttribute(node.outpinId + 2);
        ImGui::Text("output pin");
        ImNodes::EndOutputAttribute();
        ImNodes::EndNode();
    }

    ImNodes::EndNodeEditor();
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    dirty = false;
}

list<uPtr<Node>> NodeTranslator::translate(list<UINode> uiNodeList) {
    return {};
}