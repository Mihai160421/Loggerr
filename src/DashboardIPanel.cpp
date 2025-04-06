#include "DashboardIPanel.h"
#include "windows.h"

#include <vector>

namespace APPLICATION_NAME
{
    DashboardIPanel::DashboardIPanel() : IPanel("Dashboard")
    {   
        m_Docked = true; // Start docked
    }

    void DashboardIPanel::OnRender()
    {
        if(m_Docked)
        {
            m_Docked = false;
            ImGui::SetNextWindowDockID(MainIPanel::GetInstance()->GetDockspaceID());
        }

        if(ImGui::Begin(GetPanelName(), &m_Open, ImGuiWindowFlags_NoSavedSettings))
        {

        } ImGui::End();
    }

    void DashboardIPanel::RenderPopUpContext()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {5.f, 5.f});


        // TODO Color code black the menu bar ? 
        if (ImGui::BeginPopupContextItem("DashboardPopUpContext"))
        {
            if (ImGui::MenuItem("New Terminal"))
            {
            }
            ImGui::EndPopup();
        }
        ImGui::PopStyleVar();
    }        
}