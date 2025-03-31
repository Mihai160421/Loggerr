#pragma once
#include "IPanel.h"
#include "LogIPanel.h"
#include "headers.h"
#include <list>
#include <memory>

namespace Loggerr
{
    class DashboardIPanel : public IPanel
    {
        public:
            // List of log panels to be rendered
            std::list<std::unique_ptr<LogIPanel>> m_LogPanels;
        private:
            // Window size
            ImVec2 m_WindowSize = {0, 0}; /* Drawable available space on dashboard */
        public:
            DashboardIPanel();
            void OnRender() override;
            void AddLogPanel();
            size_t GetLogPanelCount() const { return m_LogPanels.size(); } // Get the number of log panels

        private:
            void RenderLogPanels();
            void RenderInsertCommandPanel();
    };
}