#include "DashboardPanel.h"
#include <iostream>
#include <cstdio>  // Pentru sprintf


namespace Logger 
{
    DashboardPanel::DashboardPanel(){
        sprintf_s(m_PanelName, "Panel #%d", m_ID);
    };

    void DashboardPanel::Render()
    {
        if(m_FirstRender)
        {
            auto viewport = ImGui::GetWindowViewport();
            ImGui::SetNextWindowDockID(viewport->ID);
        }

        bool isOpen = true;

        if(ImGui::Begin(m_PanelName, &isOpen))
        {
            if(!isOpen)
            {
                m_Closed = true;
            }

            ImGui::Text("Teeeest");
        }

        ImGui::End();

        m_FirstRender = false;
    }
}