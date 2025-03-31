#pragma once

#include "IPanel.h"


namespace Loggerr
{
    class DashboardIPanel; // Forward declaration of DashboardIPanel
    
    class LogIPanel : public IPanel
    {
        public:
        
        private:
            DashboardIPanel* m_DashboardPanelOwner; // Weak pointer to the DashboardIPanel
            bool m_Dockit = true;
            bool m_Docked = false;
            char m_Filter[128] = {0};
        public:
            LogIPanel(DashboardIPanel* dashboardPanel);
            LogIPanel(std::weak_ptr<IPanel> owner) : IPanel(owner) {};


            void OnRender() override;
            void ForceDock() { m_Dockit = true; } // Force the panel to dock

        private:
            void HandlePopupContext();
            void UpdatePanelName();
            void HandleFilterInputText();
    };
}