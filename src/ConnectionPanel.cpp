#include "ConnectionPanel.h"

namespace Logger
{
    ConnectionPanel::ConnectionPanel()
    {
        m_Closed = true; /* We start as a closed panel */
    }

    void ConnectionPanel::Render()
    {
        bool isOpen = true;
        
        if(ImGui::Begin("ConnectionPanel", &isOpen, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
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