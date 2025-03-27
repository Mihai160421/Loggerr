#include "ConnectionPanel.h"

namespace Logger
{
    ConnectionPanel::ConnectionPanel() {
        SetPanelName("Connection Panel");
    }

    void ConnectionPanel::Render()
    {
        bool isOpen = true;
        
        if(ImGui::Begin(m_PanelName, &isOpen, ImGuiWindowFlags_NoDocking 
            | ImGuiWindowFlags_NoCollapse 
            | ImGuiWindowFlags_NoSavedSettings))
        {
            ImGui::Text("This is a connection pannel test");

            if (!isOpen)
            {
                m_Closed = true;
            }
        }
        ImGui::End();
    }
}