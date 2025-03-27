#pragma once

#include <iostream>
#include <list>
#include <memory>

#include "Headers.h"

#include "Panel.h"
#include "ConnectionPanel.h"


namespace Logger
{
    class Application
    {
        private:
            /* GLFW window */
            GLFWwindow* window = nullptr;                      
		    
            /* List of pannels to be rendered */
            std::list<IPanel*> panels;

            bool m_RenderDebugWindow = false;
        public:
            Application();
            void Run();
            void AddPanel(IPanel* panel);

            private:
            void Init();
            void Shutdown();
            void RenderMenuBar();
            void HandlePanels();
            void RenderDebugWindow();
    };
}