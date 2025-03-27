#pragma once

#include "Panel.h"
#include "Headers.h"

namespace Logger
{
    class ConnectionPanel : public IPanel
    {
        public:
            static ConnectionPanel& getInstance()
            {
                static ConnectionPanel instance; 
                return instance;
            }
            ConnectionPanel();
            void Render() override;
    };
}