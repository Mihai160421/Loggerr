#pragma once

#include <iostream>
#include <list>
#include <memory>

#include "Headers.h"

#include "Panel.h"
#include "ConnectionPanel.h"
#include "DashboardPanel.h"

extern "C" {
    #include <windows.h>
    #include <psapi.h>
}

namespace Logger
{
    class Application
    {
    private:
        /* GLFW window */
        GLFWwindow* window = nullptr;                      
        
        /* List of pannels to be rendered */
        std::list<std::unique_ptr<IPanel>> panels;

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

        static size_t GetMemoryUsage() {
            PROCESS_MEMORY_COUNTERS pmc;
            GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
            return pmc.WorkingSetSize / 1024; // KB
        }
    };
}