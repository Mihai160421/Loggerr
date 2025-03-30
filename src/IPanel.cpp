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
        m_ID = panelIDCounter++;
        SetPanelName("Panel");

        std::cout << "Panel " << m_PanelName << " created " << std::endl;
    }

    IPanel::~IPanel()
    {
        panelDestroyedCounter++;
        std::cout << "Panel " << m_PanelName << " destroyed" << std::endl;
    };

    void IPanel::SetPanelName(const char* name)
    {
        if (name == nullptr) {
            std::cerr << "Error: Panel name cannot be null" << std::endl;
            return;
        }

        m_PanelName = name; // Set the panel name to the provided name
        m_PanelName += "##"; // Append a space to the panel name
        m_PanelName += std::to_string(m_ID); // Append the panel ID to the name
    }

    void IPanel::SetPanelName(std::string name)
    {
        m_PanelName = name; // Set the panel name to the provided name
        m_PanelName += "##"; // Append a space to the panel name
        m_PanelName += std::to_string(m_ID); // Append the panel ID to the name
    }

    uint64_t IPanel::GetID() const { 
        return m_ID; 
    }

    const char* IPanel::GetPanelName() const { 
        return m_PanelName.c_str(); 
    }

    bool IPanel::IsClosed() const { 
        return m_Closed; 
    }

    // Static
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