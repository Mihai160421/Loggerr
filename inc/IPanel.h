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
            bool m_Docked = false;
            bool m_Active = false;
        
        private:
            std::string m_PanelName = "Panel";      /* Panel name                               */
            uint64_t m_ID = 0;                      /* Panel unique id                          */
            ImGuiID m_DockspaceID = 0;              /* Panel Dockspace ID if any                */
            std::weak_ptr<IPanel> owner;            /* The Window owner the panel belongs to    */
            
            protected:
            bool m_FirstRender = true;
            bool m_Open = true;                  /* Window has been closed                      */
            
        public:
            virtual ~IPanel();
            
            virtual void OnRender() = 0;
            
            void SetPanelName(const char* name);
            void SetPanelName(std::string name);
            
            uint64_t GetID() const;
            const char* GetPanelName() const;
            ImGuiID GetDockspaceID() const;
            void SetDockspaceID(ImGuiID dockspace);
            
            bool IsClosed() const;
            void Close(bool close = true);
            
            static void RenderPannelList(std::list<std::shared_ptr<IPanel>>& panels);
            static uint64_t GetPanelIDCounter();
            static uint64_t GetPanelDestroyedCount();
            
            // This function should be called after a ImGui::Begin Window was created to update internal class member variables
            void UpdateInternalState();
            
        protected:
            IPanel();
            IPanel(std::weak_ptr<IPanel> owner) : owner(owner) {};
    };
}