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

static uint64_t panelIDCounter = 0;         // Static variable to keep track of panel IDs
static uint64_t panelDestroyedCounter = 0; // Static variable to keep track of panel IDs

static inline uint64_t GENERATE_ID()
{
    uint64_t _id = panelIDCounter;
    panelIDCounter++;

    return _id;
}

namespace APPLICATION_NAME
{
    IPanel::IPanel(const char* PanelName) 
    {
        m_ID = GENERATE_ID();
        SetPanelName(PanelName);
    }

    IPanel::~IPanel()
    {
        panelDestroyedCounter++;
    }

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
        return !m_Open; 
    }

    void IPanel::Close(bool close)
    {
        m_Open = !close;
    }

    void IPanel::RenderPannelList(std::list<std::shared_ptr<IPanel>>& panels)
    {
        std::list<std::shared_ptr<IPanel>>::iterator it = panels.begin();
        while (it != panels.end()) {
            // If the panel is closed and it's not the connection panel, remove it
            // Note: Connection panel is a singleton and should not be removed
            if ((*it)->IsClosed()) {
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