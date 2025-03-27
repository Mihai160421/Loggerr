#include "DashboardPanel.h"
#include <iostream>
#include <cstdio>  // Pentru sprintf

namespace Logger 
{
    DashboardPanel::DashboardPanel(){
        sprintf_s(m_PanelName, "Panel #%d", m_ID);

        // Add a log panel by default
        AddLogPanel();
    };

    void DashboardPanel::Render()
    {
        if(m_FirstRender)
        {
            m_FirstRender = false;
            auto viewport = ImGui::GetWindowViewport();
            ImGui::SetNextWindowDockID(viewport->ID);
        }

        bool isOpen = true;

        if(ImGui::Begin(m_PanelName, &isOpen))
        {
            // TODO: Work on this one 
            // Add a small delay maybe
            if(!ImGui::IsWindowDocked())
            // {
            //     if(!ImGui::IsItemActive())
            //     {
            //         m_FirstRender = true;
            //     }
            // }

            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("New Log Panel"))
                {
                    AddLogPanel();
                }
                ImGui::EndPopup();
            }

            RenderLogPanels();
        }
        ImGui::End();

        if(!isOpen)
        {
            m_Closed = true;
        }
    }

    void DashboardPanel::RenderLogPanels()
    {
        auto viewport = ImGui::GetWindowSize();
        auto width = viewport.x;
        width = width / m_LogPanels.size() * 0.7; // rest of 0.3 should be space for module panel

        // make height 80% of the window
        auto height = viewport.y;
        height = height * 0.8;

        std::list<LogPanel_t>::iterator it = m_LogPanels.begin();
        while (it != m_LogPanels.end()) {
            // Convert i to a string
            char buffer[10];
            sprintf(buffer, "%d", it->m_ID);

            ImGui::SameLine();
            if(ImGui::BeginChild(buffer, { width, height }, true))
            {
                // Render filter text and close button
                ImGui::InputTextWithHint("##Filter", "Filter", it->m_filterText, IM_ARRAYSIZE(it->m_filterText));
                ImGui::SameLine();
                if(ImGui::Button("X"))
                {
                    if(m_LogPanels.size()-1 != 0)
                    {
                        // Remove the panel except if it's the last one
                        m_LogPanels.erase(it);
                    }
                }
                else
                {
                    std::string log = "Log Panel " + std::to_string(it->m_ID);
                    ImGui::Text(log.c_str());
                    // Only increment when one was erased
                    ++it;
                }
            }
            ImGui::EndChild();
        }

        // Render command text
        char buffer[50];
        ImGui::SetNextWindowSize({ viewport.x, 0 }, 0);
        ImGui::InputTextWithHint("##Command", "Command", buffer, 50);        
    }

    void DashboardPanel::AddLogPanel()
    {
        // Maybe make it a member variable instead of static
        static uint64_t counter = 0;
        LogPanel_t panel;
        panel.m_ID = counter++;
        m_LogPanels.push_back(panel);
    }
}