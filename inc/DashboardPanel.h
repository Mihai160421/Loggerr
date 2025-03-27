#pragma once

#include "Panel.h"
#include "Headers.h"
#include <list>

namespace Logger
{
    typedef struct 
    {
        uint64_t m_ID;
        char m_filterText[50] = {0};
        bool applyFilter = true;
    } LogPanel_t;

    class DashboardPanel : public IPanel
    {
        private:
            bool m_FirstRender = true;              /* Set to false after the first render */
            std::list<LogPanel_t> m_LogPanels;    /* Log panels */
        public:
            DashboardPanel();
            void Render() override;

        private:
            void AddLogPanel();
            void RenderMenu();
            void RenderLogPanels();
            void RenderCommandText();
            void RenderModulePanel();
    };
}