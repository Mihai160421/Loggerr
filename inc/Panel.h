#pragma once

#include <stdint.h>
#include <stdio.h>
#include <iostream>

namespace Logger
{
    class IPanel {
        public:
            bool m_Closed = false;
            char m_PanelName[50];
        
        public:
            virtual ~IPanel() 
            {
                std::cout << "Panel " << m_PanelName << " destroyed" << std::endl;
            };
            virtual void Render() = 0;

            void SetPanelName(const char* name)
            {
                sprintf_s(m_PanelName, "%s#%d", name, m_ID);
            }

            uint64_t GetID() const { return m_ID; }

        protected:
            uint64_t m_ID = 0;
            IPanel() {
                static uint64_t counter = 0;
                m_ID = counter++;

                SetPanelName("Panel");
                
                std::cout << "Panel " << m_PanelName << " created" << std::endl;
            }
    };
}