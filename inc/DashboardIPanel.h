#pragma once
#include "pch.h"
#include "IPanel.h"
#include "MainIPanel.h"

namespace APPLICATION_NAME
{
    class DashboardIPanel : public IPanel
    {
    public:
        DashboardIPanel();
        void OnRender() override;
    private:
        void RenderPopUpContext();
    };
}