#include "MainIPanel.h"
#include "Headers.h"
#include "LoggerMacros.h"

namespace Logger
{
    MainIPanel* MainIPanel::m_Instance = nullptr; // Initialize the static instance variable

    MainIPanel::MainIPanel()
    {
        SetPanelName("MainPanel");
    }

    MainIPanel* MainIPanel::GetInstance()
    {
        if (MainIPanel::m_Instance == nullptr)
        {
            MainIPanel::m_Instance = new MainIPanel();
        }

        return m_Instance;
    }

    void MainIPanel::Render()
    {
        // Try Render a main overall panel with docking enabled, this will be the main panel filled the window
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);


        __PUSH_STYLE(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
        __PUSH_STYLE(ImGuiStyleVar_WindowBorderSize, 0.0f);
        // Make the main panel dockable
        if(ImGui::Begin("MainPanel", nullptr, 
            ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoScrollbar
            | ImGuiWindowFlags_NoScrollWithMouse
            | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoSavedSettings
            | ImGuiWindowFlags_NoMouseInputs
            | ImGuiWindowFlags_NoFocusOnAppearing
            | ImGuiWindowFlags_NoNavInputs
            | ImGuiWindowFlags_NoNavFocus
            | ImGuiWindowFlags_NoBringToFrontOnFocus))
        {
        m_DockspaceID = ImGui::DockSpace(ImGui::GetID("MainPanelDock"), ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
        }
        ImGui::End();


        __POP_STYLE(2);
    }    
}