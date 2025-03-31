#include "LogIPanel.h"

#include "DashboardIPanel.h"


namespace Loggerr
{
    // Ensure the SetPanelName method is called from the base class IPanel
    LogIPanel::LogIPanel( DashboardIPanel* dashboardPanelOwner)
        : m_DashboardPanelOwner(dashboardPanelOwner) {
    };

    void LogIPanel::OnRender() {
        bool _WindowOpen = true; // Flag to control the window open state
        ImGuiWindowFlags AdditionalFlags = 0;

        if (m_Dockit) { 
            m_Dockit = false;
            ImGui::SetNextWindowDockID(m_DashboardPanelOwner->GetDockspaceID()); // Set the next window dock ID to the dashboard panel's dock ID
        }
        
        {
            UpdatePanelName();
        }

        // Set the window name to the owner's name and the panel name
        if(ImGui::Begin(GetPanelName(), &_WindowOpen, ImGuiWindowFlags_NoCollapse 
                                                    | ImGuiWindowFlags_NoSavedSettings 
                                                    | AdditionalFlags)) // Begin the window with the panel name
        {
            UpdateInternalState();
            
            HandlePopupContext();

            // Render filter search bar

            HandleFilterInputText();
        }
        ImGui::End(); // End the window
            
        if(! _WindowOpen) { // If the window is closed
            Close();
        }
    };

    void LogIPanel::HandlePopupContext()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 10.f, 10.f });
        if (ImGui::BeginPopupContextWindow("##LogPanel")) { // Begin a popup context menu for the log panel
            if (ImGui::MenuItem("Add Log Panel")) {
                m_DashboardPanelOwner->AddLogPanel(); // Add a new log panel when the menu item is clicked
            }

            if (m_Docked == false && ImGui::MenuItem("Dockit")) {
                m_Dockit = true; // Set the docked flag to true when the dock menu item is clicked
            }

            if(m_DashboardPanelOwner->GetLogPanelCount() > 1) { // If there are multiple log panels
                if (ImGui::MenuItem("Remove Log Panel")) {
                    Close();
                }
            }
            ImGui::EndPopup(); // End the popup menu
        }
        ImGui::PopStyleVar();
    }

    void LogIPanel::UpdatePanelName()
    {
        // Set the IPanel name to:
        // Owner name up until ## + LogPanel + ## + Panel ID
        // This is to ensure that the name is unique and can be used for docking
        // and other operations.
        std::string ownerWindowName = m_DashboardPanelOwner->GetPanelName(); // Get the panel name from the dashboard panel owner
        std::string ownerName = ownerWindowName.substr(0, ownerWindowName.find("##")); // Extract the owner name up until "##"

        SetPanelName(ownerName + "/Log"); // Set the panel name to the owner name and "Log Panel"
    }

    void LogIPanel::HandleFilterInputText()
    {
        char Filter[128] = {0};
        ImGui::SetCursorPos({10.f, 5.0f}); // Put text filter a little bit to right and down
        if(ImGui::InputTextWithHint("##FilterText", "Filter", m_Filter, sizeof(m_Filter), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            strcpy_s(m_Filter, Filter);
        }
    }
};