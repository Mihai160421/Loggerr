#pragma once

#include "Panel.h"
#include "Headers.h"

namespace Logger
{
    class DashboardPanel : public IPanel
    {
        public:
            bool m_FirstRender = true; /* Set to false after the first render */

        public:
            DashboardPanel();
            void Render() override;

        private:
            void RenderMenu();
            void RenderLogPanels();
            void RenderCommandText();
            void RenderModulePanel();
    };
}