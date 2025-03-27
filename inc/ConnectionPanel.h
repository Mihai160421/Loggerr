#pragma once

#include "Panel.h"
#include "Headers.h"

namespace Logger
{
    class ConnectionPanel : public IPanel
    {
        public:
            ConnectionPanel();
            void Render() override;
    };
}