/*
    This is in Interface Panel class
    This is the base class for all panels in the application.
    It provides a common interface for rendering and managing panel properties.
    Each panel has a unique ID and a name, and can be closed or rendered.

*/
#pragma once

#include "Headers.h"
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <memory>

namespace Loggerr
{
    class IPanel {
        public:
            bool m_Closed = false;
            std::string m_PanelName = "Panel";
            
        protected:
            uint64_t m_ID = 0;
            bool m_FirstRender = true;
        public:
            virtual void OnRender() = 0;
            void SetPanelName(const char* name);
            void SetPanelName(std::string name);
            uint64_t GetID() const;
            const char* GetPanelName() const;
            bool IsClosed() const;
            static void RenderPannelList(std::list<std::unique_ptr<IPanel>>& panels);
            static uint64_t GetPanelIDCounter();
            static uint64_t GetPanelDestroyedCount();
            virtual ~IPanel();
        

        protected:
            // Constructor is protected to prevent direct instantiation of IPanel
            IPanel();
    };
}