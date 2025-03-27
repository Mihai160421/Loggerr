#pragma once

#include <stdint.h>
#include <stdio.h>

namespace Logger
{
    class IPanel {
        public:
            bool m_Closed = false;
            char m_PanelName[50];
        
        public:
            virtual ~IPanel() = default;
            virtual void Render() = 0;

            void SetPanelName(const char* name)
            {
                sprintf_s(m_PanelName, "%s#%d", name, m_ID);
            }
        protected:
            uint64_t m_ID = 0;
            IPanel() {
                static uint64_t counter = 0;
                m_ID = counter++;

                SetPanelName("Panel");
            }
    };
}