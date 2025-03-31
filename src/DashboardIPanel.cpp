#include "DashboardIPanel.h"
#include "MainIPanel.h"
#include "LoggerMacros.h"
#include "Application.h"

#include <list>
#include <iostream>

#define PANNEL_PADDING_X 5.0f // Padding for the panels
#define PANNEL_PADDING_Y 3.0f // Padding for the panels
#define PANAEL_TEXTBOX_HEIGHT 20.0f // Height of the text box for command input

namespace Loggerr 
{
    DashboardIPanel::DashboardIPanel() {
        SetPanelName("Dashboard");
        AddLogPanel(); // Add the first log panel here
    };

    void DashboardIPanel::OnRender() {
        bool _WindowOpen = true;

        // Set the next window dock ID to the main panel's dock ID
        ImGui::SetNextWindowDockID(MainIPanel::GetInstance()->GetDockspaceID(), ImGuiCond_FirstUseEver);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {PANNEL_PADDING_X, PANNEL_PADDING_Y}); // Set the window padding
        // Create the dashboard panel window
        if(ImGui::Begin(GetPanelName(), &_WindowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
        {
            UpdateInternalState();

            m_WindowSize = ImGui::GetContentRegionAvail(); // Get the available size of the window

            /* Render the log panels in the dashboard panel */
            {
                RenderLogPanels(); // Render the list of log panels
            }
            
            /* Render the insert command panel in the dashboard panel */
            {
                RenderInsertCommandPanel(); // Render the command input panel
            }


        } ImGui::End();

        ImGui::PopStyleVar(); // Pop the window padding style variable

        // If the window is closed, set the closed flag to true
        if (!_WindowOpen) {
           Close();
        }
    }

    void DashboardIPanel::RenderLogPanels() {
        float logPanelWidth = m_WindowSize.x * 0.75f; // Width of the log panels
        float logPanelHeight = m_WindowSize.y - PANAEL_TEXTBOX_HEIGHT - 5.f; // Height of the log panels
        
        // Render the log panels space
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0}); // Set the window padding
        if(ImGui::BeginChild("##LogPanels", {logPanelWidth ,logPanelHeight}, true))
        {
            SetDockspaceID(ImGui::DockSpace(ImGui::GetID("LogIPanelDockingSpace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_AutoHideTabBar));    
        }
        ImGui::EndChild();

        // Render all logs pannel
        std::list<std::unique_ptr<LogIPanel>>::iterator it = m_LogPanels.begin();
        while (it != m_LogPanels.end()) {
            // If the panel is closed and it's not the last panel
            if ((*it)->IsClosed())
            {
                if(m_LogPanels.size() == 1) {
                    // If there is only one log panel, do not close it
                    (*it)->Close(false);
                    ++it;
                    // Show prompt saying "Cannot close the last log panel"
                    ImGui::OpenPopup("Cannot close the last log panel");
                }
                else
                {
                    it = m_LogPanels.erase(it);
                }
            } else {
                (*it)->OnRender();
                ++it;
            }
        }

        ImGui::PopStyleVar(); // Pop the window padding style variable
    }

    void DashboardIPanel::RenderInsertCommandPanel() {
        char m_CommandBuffer[256] = {0}; // Buffer to store the command input
                
        ImGui::PushItemWidth(0.75f * m_WindowSize.x); // Set the width of the input text field

        // Set input text at the bottom of the dashboard panel
        ImGui::SetCursorPosY(m_WindowSize.y); // Set the cursor position to the bottom of the window

        if(ImGui::InputText("##Command", m_CommandBuffer, sizeof(m_CommandBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
            // Handle the command input here
            std::cout << "Command: " << m_CommandBuffer << std::endl;
            memset(m_CommandBuffer, 0, sizeof(m_CommandBuffer)); // Clear the buffer after processing
            ImGui::SetKeyboardFocusHere(-1); // Focus on the input text field
        } ImGui::PopItemWidth(); // Pop the item width
    }

    void DashboardIPanel::AddLogPanel() {
        m_LogPanels.push_back(std::make_unique<LogIPanel>(this));
    }
}

