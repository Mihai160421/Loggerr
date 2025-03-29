#include "IPanel.h"
#include "headers.h"

#include <list>
#include <memory>

namespace Logger 
{
    class DashboardIPanel : public IPanel
    {
        public:
            // List of log panels to be rendered
            std::list<std::unique_ptr<IPanel>> m_LogPanels;

        public:
            DashboardIPanel();
            void Render() override;

        private:

    };
}

