#include "DashboardIPanel.h"
#include "MainIPanel.h"
#include "LoggerMacros.h"
#include "Application.h"

#include <list>
#include <iostream>

#define PANNEL_PADDING_X 5.0f // Padding for the panels
#define PANNEL_PADDING_Y 3.0f // Padding for the panels

namespace Loggerr 
{
    DashboardIPanel::DashboardIPanel() {
        SetPanelName("Dashboard");
        AddLogPanel(); // Add the first log panel here
    };

    void DashboardIPanel::OnRender() {
        // Set the next window dock ID to the main panel's dock ID
        ImGui::SetNextWindowDockID(MainIPanel::GetInstance()->GetDockspaceID(), ImGuiCond_FirstUseEver);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {PANNEL_PADDING_X, PANNEL_PADDING_Y}); // Set the window padding

        // Create the dashboard panel window
        if(ImGui::Begin(GetPanelName(), &m_Open, ImGuiWindowFlags_NoCollapse 
                                                        | ImGuiWindowFlags_NoSavedSettings
                                                        | ImGuiWindowFlags_NoScrollbar 
                                                        | ImGuiWindowFlags_NoScrollWithMouse))
        {
            UpdateInternalState();

            m_WindowSize = ImGui::GetContentRegionAvail(); // Get the available size of the window

            /* Render the log panels in the dashboard panel */
            {
                RenderLogPanels(); // Render the list of log panels
            }

        } ImGui::End();

        ImGui::PopStyleVar(); // Pop the window padding style variable
    }

    void DashboardIPanel::RenderLogPanels() {
        float percent = m_PreferencePannel ? 0.75f : 1.0f;

        float logPanelWidth = m_WindowSize.x * percent; // Width of the log panels
        float logPanelHeight = m_WindowSize.y - 5.f; // Height of the log panels
        
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

    void DashboardIPanel::AddLogPanel() {

//        std::shared_ptr<DashboardIPanel> self  = shared_from_this();
        //m_LogPanels.push_back(std::make_unique<LogIPanel>(self));
        m_LogPanels.push_back(std::make_unique<LogIPanel>(this));
    }
}

