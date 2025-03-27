#pragma once

#include "Panel.h"
#include "Headers.h"

namespace Logger
{
    class DashboardPanel : public IPanel
    {
        public:
            DashboardPanel();
            void Render() override;
    };
}