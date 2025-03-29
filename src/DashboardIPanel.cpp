#include "DashboardIPanel.h"
#include "MainIPanel.h"
#include "LoggerMacros.h"

#define PANNEL_PADDING_X 10.0f // Padding for the panels
#define PANNEL_PADDING_Y 3.0f // Padding for the panels
#define PANAEL_TEXTBOX_HEIGHT 20.0f // Height of the text box for command input

namespace Logger 
{
    DashboardIPanel::DashboardIPanel() {
        SetPanelName("Dashboard");
        // Todo Maybe add a first panel for logs here
    };

    void DashboardIPanel::Render() {
        bool _WindowOpen = true;

        ImGui::SetNextWindowDockID(MainIPanel::GetInstance()->GetDockspaceID(), ImGuiCond_FirstUseEver);
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {PANNEL_PADDING_X, PANNEL_PADDING_Y}); // Set the window padding

        // Create the dashboard panel window
        if(ImGui::Begin(m_PanelName, &_WindowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
        {
            ImVec2 windowSize = ImGui::GetContentRegionAvail(); // Get the available size of the window

            /* Render the log panels in the dashboard panel */
            {
               
               float logPanelWidth = windowSize.x * 0.75f; // Width of the log panels
               float logPanelHeight = windowSize.y - PANAEL_TEXTBOX_HEIGHT - 5.f; // Height of the log panels
               
               
               ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0}); // Set the window padding
               if(ImGui::BeginChild("##LogPanels", {logPanelWidth ,logPanelHeight}, true))
               {
                   // TODO handle log pannels
                   ImGui::DockSpace(ImGui::GetID("MainPanel"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);                 
                }
                ImGui::EndChild();
                ImGui::PopStyleVar(); // Pop the window padding style variable
            }
            
            /* Render the insert command panel in the dashboard panel */
            {
                char m_CommandBuffer[256] = {0}; // Buffer to store the command input
                
                ImGui::PushItemWidth(0.75f * windowSize.x); // Set the width of the input text field

                // Set input text at the bottom of the dashboard panel
                ImGui::SetCursorPosY(windowSize.y); // Set the cursor position to the bottom of the window

                if(ImGui::InputText("##Command", m_CommandBuffer, sizeof(m_CommandBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    // Handle the command input here
                    std::cout << "Command: " << m_CommandBuffer << std::endl;
                    memset(m_CommandBuffer, 0, sizeof(m_CommandBuffer)); // Clear the buffer after processing
                    ImGui::SetKeyboardFocusHere(-1); // Focus on the input text field
                } ImGui::PopItemWidth(); // Pop the item width
            }
        } ImGui::End();

        ImGui::PopStyleVar(); // Pop the window padding style variable

        // If the window is closed, set the closed flag to true
        if (!_WindowOpen) {
            m_Closed = true;
        }
    }
}

