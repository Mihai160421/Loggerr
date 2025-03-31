/*
    This is in Interface Panel class
    This is the base class for all panels in the application.
    It provides a common interface for rendering and managing panel properties.
    Each panel has a unique ID and a name, and can be closed or rendered.

*/

#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <memory>
#include "IPanel.h"

static uint64_t panelIDCounter = 0;     // Static variable to keep track of panel IDs
static uint64_t panelDestroyedCounter = 0; // Static variable to keep track of panel IDs

namespace Loggerr
{
    IPanel::IPanel()
    {
        SetPanelName("Panel");
        m_ID = panelIDCounter++;
    }
    
    IPanel::~IPanel()
    {
        panelDestroyedCounter++;
    };

    void IPanel::UpdateInternalState()
    {
        m_Docked = ImGui::IsWindowDocked();        
        m_Active = ImGui::IsItemActive();        
    }

    void IPanel::SetPanelName(const char* name)
    {
        m_PanelName = name; 
        m_PanelName += "##"; 
        m_PanelName += std::to_string(m_ID); 
    }

    void IPanel::SetPanelName(std::string name)
    {
        m_PanelName = name;
        m_PanelName += "##"; 
        m_PanelName += std::to_string(m_ID);
    }

    uint64_t IPanel::GetID() const { 
        return m_ID; 
    }

    const char* IPanel::GetPanelName() const { 
        return m_PanelName.c_str(); 
    }

    ImGuiID IPanel::GetDockspaceID() const
    {
        return m_DockspaceID;
    }

    void IPanel::SetDockspaceID(ImGuiID dockspace)
    {
        m_DockspaceID = dockspace;
    }

    bool IPanel::IsClosed() const { 
        return m_Closed; 
    }

    void IPanel::Close(bool close)
    {
        m_Closed = close;
    }

    void IPanel::RenderPannelList(std::list<std::unique_ptr<IPanel>>& panels)
    {
        std::list<std::unique_ptr<IPanel>>::iterator it = panels.begin();
        while (it != panels.end()) {
            // If the panel is closed and it's not the connection panel, remove it
            // Note: Connection panel is a singleton and should not be removed
            if ((*it)->m_Closed) {
                it = panels.erase(it);
            } else {
                (*it)->OnRender();
                ++it;
            }
        }
    }

    uint64_t IPanel::GetPanelIDCounter()
    {
        return panelIDCounter;
    }

    uint64_t IPanel::GetPanelDestroyedCount()
    {
        return panelDestroyedCounter;
    }
}