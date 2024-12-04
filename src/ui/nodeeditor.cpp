#include "nodeeditor.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imnodes.h"
#include "uimovenode.h"
#include "uiyawnode.h"
#include "uipitchnode.h"
#include "uistoretransformnode.h"
#include "uirestoretransformnode.h"
#include "uisampledensitynode.h"
#include "uisamplebiasnode.h"
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <regex>

NodeEditor::NodeEditor(GLFWwindow* window, const std::string& fileNamePrefix) {
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImNodes::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImNodes::PushStyleVar(ImNodesStyleVar_PinHoverRadius, 10);
    ImNodes::PushStyleVar(ImNodesStyleVar_PinCircleRadius, 5);
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    loadNodeEditor(fileNamePrefix);

    // Strip any timestamp numbers off the end of the given filenamePrefix.
    // That way, we can save the file with a new timestamp.
    std::regex re("\\d+$");
    this->fileNamePrefix = std::regex_replace(fileNamePrefix, re, "");

    // Create a beginNode if it doesn't exist (wasn't loaded in from file)
    if (nodeIdMap.empty()) {
        beginNodeId = getNewId();
        nodeIdMap[beginNodeId] = std::move(mkU<BeginUINode>(beginNodeId, getNewId(), getNewId()));
    }
}

void NodeEditor::reset() {
    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void NodeEditor::teardown() {
    saveNodeEditor();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImNodes::DestroyContext();
    ImGui::DestroyContext();
}

int NodeEditor::getNewId() {
    return uniqueId++;
}

void NodeEditor::handleMenuChanges(bool linkDropped) {
    const bool open_popup = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
                            ImNodes::IsEditorHovered() && (ImGui::IsKeyReleased(ImGuiKey_A) || linkDropped);

    if (!ImGui::IsAnyItemHovered() && open_popup)
    {
        ImGui::OpenPopup("Add Node");
        if (linkDropped) createLinkWithNode = true;
    }

    if (ImGui::BeginPopup("Add Node"))
    {
        // TODO: turn this into a for loop with a factory pattern to create the nodes
        if (ImGui::MenuItem("Move")) {
            addNode(std::move(mkU<UIMoveNode>(getNewId(), getNewId(), getNewId())));
        }
        else if (ImGui::MenuItem("Yaw")) {
            addNode(std::move(mkU<UIYawNode>(getNewId(), getNewId(), getNewId())));
        }
        else if (ImGui::MenuItem("Pitch")) {
            addNode(std::move(mkU<UIPitchNode>(getNewId(), getNewId(), getNewId())));
        }
        else if (ImGui::MenuItem("Restore")) {
            addNode(std::move(mkU<UIRestoreTransformNode>(getNewId(), getNewId(), getNewId())));
        }
        else if (ImGui::MenuItem("Store")) {
            addNode(std::move(mkU<UIStoreTransformNode>(getNewId(), getNewId(), getNewId())));
        }
        else if (ImGui::MenuItem("SampleDensityNode")) {
            addNode(std::move(mkU<UISampleDensityNode>(getNewId(), getNewId(), getNewId())));
        }
        else if (ImGui::MenuItem("SampleBiasNode")) {
            addNode(std::move(mkU<UISampleBiasNode>(getNewId(), getNewId(), getNewId())));
        }
        
        ImGui::EndPopup();
    }
}

void NodeEditor::addNode(uPtr<UINode> node) {
    int nodeId = node->getId();
    ImNodes::SetNodeScreenSpacePos(nodeId, ImGui::GetMousePos());
    nodeIdMap[nodeId] = std::move(node);
    dirty = true;
}

bool NodeEditor::maybeAddLink() {
    Link link;
    if (ImNodes::IsLinkCreated(&link.startNodeId, &link.startPinId, &link.endNodeId, &link.endPinId)) {
        link.id = getNewId();
        linkIdMap[link.id] = link;

        nodeIdMap[link.startNodeId]->setOutLinkId(link.id);
        nodeIdMap[link.endNodeId]->setInLinkId(link.id);

        dirty = true;
        return true;
    }
    return false;
}

bool NodeEditor::shouldDeleteLink(int linkId) {
    if (ImNodes::IsLinkSelected(linkId) && ImGui::IsKeyReleased(ImGuiKey_Delete)) {
        return true;
    }
    return false;
}

void NodeEditor::deleteLink(int linkId) {
    auto it = linkIdMap.find(linkId);
    if (it == linkIdMap.end()) return;

    nodeIdMap[it->second.startNodeId]->setOutLinkId(-1);
    nodeIdMap[it->second.endNodeId]->setInLinkId(-1);
    
    linkIdMap.erase(linkId);

    dirty = true;
}

bool NodeEditor::shouldDeleteNode(int nodeId) {
    if (nodeId == beginNodeId) return false;
    if (ImNodes::IsNodeSelected(nodeId) && ImGui::IsKeyReleased(ImGuiKey_Delete)) {
        return true;
    }
    return false;
}

void NodeEditor::deleteNode(int nodeId) {
    auto it = nodeIdMap[nodeId].get();

    // Remove all links connected to this node
    std::vector<int> linksToDelete;
    for (const auto& linkEntry : linkIdMap) {
        if (linkEntry.second.startPinId == it->getEndPinId() || linkEntry.second.endPinId == it->getStartPinId()) {
            linksToDelete.push_back(linkEntry.second.id);
        }
    }

    for (int linkId : linksToDelete) {
        deleteLink(linkId);
    }

    nodeIdMap.erase(nodeId);

    dirty = true;
}

void NodeEditor::show(
    int editorPosX,
    int editorPosY,
    int editorWidth,
    int editorHeight
) {
    if (showSaveIndicator) {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(220, 220, 220, 255)); 
    }

    // Set ImGui window pos and size to only overlay the right viewport
    ImGui::SetNextWindowPos(ImVec2(editorPosX, editorPosY));
    ImGui::SetNextWindowSize(ImVec2(editorWidth, editorHeight));
    ImGui::Begin("Node editor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    if (showSaveIndicator) {
        ImGui::PopStyleColor();
    }

    maybeAddLink();
    
    bool linkDropped = ImNodes::IsLinkDropped();

    ImNodes::BeginNodeEditor();
    handleMenuChanges(linkDropped);

    std::vector<int> nodesToDelete;
    std::vector<int> linksToDelete;

    for (const auto& nodeEntry : nodeIdMap) {
        auto node = nodeEntry.second.get();
        if (shouldDeleteNode(node->getId())) {
            nodesToDelete.push_back(node->getId());
            continue;
        }

        if (node->show()) dirty = true;
    }

    for (const auto& linkEntry : linkIdMap) {
        const auto& link = linkEntry.second;
        if (shouldDeleteLink(link.id)) {
            linksToDelete.push_back(link.id);
            continue;
        }
        ImNodes::Link(link.id, link.startPinId, link.endPinId);
    }

    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_TopRight);
    ImNodes::EndNodeEditor();

    if( ImNodes::IsEditorHovered() && ImGui::GetIO().MouseWheel != 0 )
    {
        float zoom = ImNodes::EditorContextGetZoom() + ImGui::GetIO().MouseWheel * 0.1f;
        ImNodes::EditorContextSetZoom( zoom, ImGui::GetMousePos() );
    }

    for (int nodeId : nodesToDelete) {
        deleteNode(nodeId);
    }

    for (int linkId : linksToDelete) {
        deleteLink(linkId);
    }

    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_S, false)) {
        saveNodeEditor();
        lastSaveTime = std::chrono::steady_clock::now();
        showSaveIndicator = true;
    }

    // Reset the save indicator after 0.5 seconds
    if (showSaveIndicator && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastSaveTime).count() > 500) {
        showSaveIndicator = false;
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

list<uPtr<Node>> NodeEditor::getNodeList() const {
    list<uPtr<Node>> nodes;

    // This is pretty janky... a result of a bad choice of data model, but it works as a proof of concept.
    // A better data model would be a graph.
    int inLink = nodeIdMap.at(beginNodeId)->getOutLinkId();
    while (inLink != -1) {
        auto it = linkIdMap.find(inLink);
        if (it == linkIdMap.end()) break;

        int nodeId = it->second.endNodeId;
        auto nodeIt = nodeIdMap.find(nodeId);
        if (nodeIt == nodeIdMap.end()) break;

        list<uPtr<Node>> interpreterNodes = nodeIt->second->toInterpreterNodes();
        nodes.splice(nodes.end(), interpreterNodes);

        inLink = nodeIt->second->getOutLinkId();
    }

    return nodes;
}

void NodeEditor::saveNodeEditor() {
    // Get current time
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    // Create a string stream to format the time
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d%H%M%S");

    // Create the filename with the timestamp
    string prefix = fileNamePrefix.empty() ? "nodeeditor" : fileNamePrefix;
    string filename = prefix + oss.str();

    ImNodes::SaveCurrentEditorStateToIniFile((filename + ".ini").c_str());


    std::ofstream os(filename + ".cereal", std::ios::binary);
    cereal::BinaryOutputArchive archive( os );
    archive(*this);
}

void NodeEditor::loadNodeEditor(const string& fileNamePrefix) {
    if (fileNamePrefix.empty()) return;

    ImNodes::LoadCurrentEditorStateFromIniFile((fileNamePrefix + ".ini").c_str());
    std::ifstream is((fileNamePrefix + ".cereal").c_str(), std::ios::binary);
    if (!is.good()) return;

    cereal::BinaryInputArchive archive( is );
    archive(*this);
}