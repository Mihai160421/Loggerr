#include "DashboardIPanel.h"
#include "MainIPanel.h"

namespace Logger 
{
    DashboardIPanel::DashboardIPanel() {
        SetPanelName("Dashboard");
        // Todo Maybe add a first panel for logs here
    };

    void DashboardIPanel::Render() {
        bool _WindowOpen = true;

        ImGui::SetNextWindowDockID(MainIPanel::GetInstance()->GetDockspaceID(), ImGuiCond_FirstUseEver);
        
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        // Create the dashboard panel window
        if(ImGui::Begin(m_PanelName, &_WindowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
        {
            ImVec2 windowSize = ImGui::GetWindowSize();
            windowSize.x -= ImGui::GetStyle().WindowPadding.x * 2;
            windowSize.y -= ImGui::GetStyle().WindowPadding.y * 2;
            windowSize.x -= ImGui::GetStyle().FramePadding.x * 2;
            windowSize.y -= ImGui::GetStyle().FramePadding.y * 2;
            windowSize.x -= ImGui::GetStyle().ItemSpacing.x * 2;
            windowSize.y -= ImGui::GetStyle().ItemSpacing.y * 2;
            windowSize.x -= ImGui::GetStyle().ScrollbarSize * 2;
            windowSize.y -= ImGui::GetStyle().ScrollbarSize * 2;
            windowSize.x -= ImGui::GetStyle().WindowBorderSize * 2;
            windowSize.y -= ImGui::GetStyle().WindowBorderSize * 2;

            /*
                Render the log panels in the dashboard panel

                All the logs panels stored in m_LogPanel list should ocupy 75% of the dashboard panel width
                and 80% of the dashboard panel height.
            */

            // Calculate the size of the log panels
            if(ImGui::BeginChild("##LogPanels", ImVec2(windowSize.x * 0.75f, windowSize.y * 0.94f), true))
            {
                ImGuiID dockspaceID = ImGui::GetID("MainPanel");
                ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);                 
            }
            ImGui::EndChild();

            /*
                Render the insert command panel in the dashboard panel
            */
            // Calculate the size of the insert command panel
            if(ImGui::BeginChild("##InsertCommandPanel", ImVec2(windowSize.x * 0.75f, windowSize.y * 0.06f), false))
            {
                char m_CommandBuffer[256] = {0}; // Buffer to store the command input

                ImGui::PushItemWidth(0.75f * windowSize.x); // Set the width of the input text field

                if(ImGui::InputText("##Command", m_CommandBuffer, sizeof(m_CommandBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    // Handle the command input here
                    // For example, you can send the command to the log panel or execute it
                    std::cout << "Command: " << m_CommandBuffer << std::endl;
                    memset(m_CommandBuffer, 0, sizeof(m_CommandBuffer)); // Clear the buffer after processing

                    // Set input text back to active
                    ImGui::SetKeyboardFocusHere(-1); // Focus on the input text field
                }        

                ImGui::PopItemWidth(); // Pop the item width
            }
            ImGui::EndChild();
        }
        ImGui::End();

        ImGui::PopStyleVar(); // Pop the window padding style variable
        ImGui::PopStyleVar(); // Pop the window border size style variable

        // If the window is closed, set the closed flag to true
        if (!_WindowOpen) {
            m_Closed = true;
        }
    }
}

