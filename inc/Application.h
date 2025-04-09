#pragma once

#include <iostream>
#include <list>
#include <memory>

#include "pch.h"

#include "IPanel.h"
#include "MainIPanel.h"
#include "DashboardIPanel.h"
#include "Terminal.h"

extern "C" {
    #include <windows.h>
    #include <psapi.h>
}

namespace APPLICATION_NAME
{
    class Application
    {
    private:
        /* GLFW window */
        GLFWwindow* m_Window = nullptr;                      

        // List containing panels rendered by application
        std::list<std::shared_ptr<IPanel>> m_Panels;

        // Flag to render debug window 
        bool m_RenderDebugWindow = true;
        bool m_RenderDemoWindow = false;

        Terminal *terminal = nullptr;
        
        public:
        static Application* getInstance();
        void Run();        
    private:

        Application();
        void Init();
        void Shutdown();
        void RenderMainMenuBar();
        void AddPanel(std::shared_ptr<IPanel> panel);

        void RenderDebugWindow();

        static size_t GetMemoryUsage() {
            PROCESS_MEMORY_COUNTERS pmc;
            GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
            return pmc.WorkingSetSize / 1024; // KB
        }
    };
}