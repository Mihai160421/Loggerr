#pragma once
/*  
    This is the main pannel rendered first on wich all the other (Dashboard) pannelese are docked first.
    There can only be one in existence at a time.
*/

#include "IPanel.h"
#include "Headers.h"

namespace Loggerr
{
    class MainIPanel : public IPanel
    {
    private:
        MainIPanel();
        static MainIPanel* m_Instance; // Singleton instance of the main panel
        ImGuiID m_DockspaceID = 0; // ID for the dockspace
    public:
        MainIPanel& operator=(const MainIPanel&) = delete;
        static MainIPanel* GetInstance();

        void OnRender() override;

        ImGuiID GetDockspaceID() const { return m_DockspaceID; } // Getter for the dockspace ID
    };
}
