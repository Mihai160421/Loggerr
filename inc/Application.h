#pragma once

#include <iostream>
#include <list>
#include <memory>

#include "Headers.h"

#include "IPanel.h"

extern "C" {
    #include <windows.h>
    #include <psapi.h>
}

namespace Loggerr
{
    class Application
    {
    private:
        /* GLFW window */
        GLFWwindow* m_Window = nullptr;                      
        /* List of pannels to be rendered */
        std::list<std::unique_ptr<IPanel>> m_Panels;

        bool m_RenderDebugWindow = false;
        
    public:
        static Application* getInstance();
        void Run();
        bool IsAnyDashboardActive();
        
    private:

        Application();
        void Init();
        void Shutdown();
        void RenderMainMenuBar();
        void AddPanel(std::unique_ptr<IPanel> panel);

        void RenderDebugWindow();

        static size_t GetMemoryUsage() {
            PROCESS_MEMORY_COUNTERS pmc;
            GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
            return pmc.WorkingSetSize / 1024; // KB
        }
    };
}