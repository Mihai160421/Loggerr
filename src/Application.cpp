#include "Application.h"


#define GLSL_VERSION ("#version 130")

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

namespace APPLICATION_NAME
{
    Application* Application::getInstance()
    {
        static Application* instance = nullptr;

        if(instance == nullptr)
        {
            instance = new Application();
        }

        return instance;
    }

    Application::Application(){
       Init(); 
    }

    void Application::Init()
    {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return ;
    
        // GL 3.0 + GLSL 130
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    
        // Create window with graphics context
        m_Window = glfwCreateWindow(1280, 720, TO_STRING(APPLICATION_NAME), nullptr, nullptr);
        if (m_Window == nullptr)
            return ;
        
        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(1); // Enable vsync
    
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
    
        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init(GLSL_VERSION);

    }

    void Application::Run()
    {
        terminal = new Terminal("Test Terminal");

        // Add the main panel first in the panels list
        m_Panels.push_back(std::shared_ptr<MainIPanel>(MainIPanel::GetInstance()));
        while (!glfwWindowShouldClose(m_Window))
        {
            ImGuiIO& io = ImGui::GetIO();
            glfwPollEvents();
            if (glfwGetWindowAttrib(m_Window, GLFW_ICONIFIED) != 0)
            {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }
    
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Setup font

            //ImGui::PushFont(robotoFont);
            {
                // Render the main menu bar
                RenderMainMenuBar();
                
                ImGui::PushStyleColor(ImGuiCol_TitleBg,             {0.094, 0.094, 0.094, 1});
                ImGui::PushStyleColor(ImGuiCol_TitleBgActive,       {0.094, 0.094, 0.094, 1});
                ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed,    {0.094, 0.094, 0.094, 1});
                ImGui::PushStyleColor(ImGuiCol_Tab,                 {0.094, 0.094, 0.094, 1});
                ImGui::PushStyleColor(ImGuiCol_TabDimmed,           {0.094, 0.094, 0.094, 1});
                ImGui::PushStyleColor(ImGuiCol_TabSelected,         {0.169, 0.169, 0.169, 1});
                ImGui::PushStyleColor(ImGuiCol_TabDimmedSelected,   {0.169, 0.169, 0.169, 1});
                ImGui::PushStyleColor(ImGuiCol_TabSelectedOverline, {0.169, 0.169, 0.169, 1});
                ImGui::PushStyleColor(ImGuiCol_TabHovered,          {0.169, 0.169, 0.169, 1});
                ImGui::PushStyleColor(ImGuiCol_FrameBg,             {0.169, 0.169, 0.169, 1});

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});

                
                IPanel::RenderPannelList(m_Panels); // Render all panels
                terminal->RenderTerminal();
                
                ImGui::PopStyleVar();

                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();



                if(m_RenderDebugWindow)
                {
                    RenderDebugWindow();
                }

                if(m_RenderDemoWindow)
                {
                    ImGui::ShowDemoWindow();
                }
            }

            //ImGui::PopFont();

            ImGui::Render();
    
            int display_w, display_h;
            glfwGetFramebufferSize(m_Window, &display_w, &display_h);
            
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.0f, 0.0f, 0.0f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);
    
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
            // Update and Render additional Platform Windows
            // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
            //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
    
            glfwSwapBuffers(m_Window);
        }

        Shutdown();
    }

    void Application::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Application::RenderMainMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("New"))
            {
                if(ImGui::MenuItem("New Connection", nullptr, false, true))
                {
                    AddPanel(std::make_shared<DashboardIPanel>());
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Debug"))
            {
                if(ImGui::MenuItem("Open Debug Window", nullptr, false, true))
                {
                    m_RenderDebugWindow = true;
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

    void Application::AddPanel(std::shared_ptr<IPanel> panel)
    {
        m_Panels.push_back(panel);
    }

    void Application::RenderDebugWindow()
    {
        if(ImGui::Begin("Debug Window", &m_RenderDebugWindow, 
                                            ImGuiWindowFlags_NoCollapse
                                            | ImGuiWindowFlags_NoDocking
                                            | ImGuiWindowFlags_NoSavedSettings))
        {
            ImGui::Text("Memory Usage: %zu KB", GetMemoryUsage());
            ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
            ImGui::Text("Display Size: %d x %d", (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
            ImGui::Text("Time Spent Rendering: %.2f ms", 1000.0f / ImGui::GetIO().Framerate);
            ImGui::Text("Panels Active: %zu", m_Panels.size());
            ImGui::Text("Panel ID Counter: %zu", IPanel::GetPanelIDCounter());
            ImGui::Text("Panel Destroyed Count: %zu", IPanel::GetPanelDestroyedCount());
            ImGui::Text("App Runtime: %.2f seconds", glfwGetTime());
            ImGui::Separator();
            if(ImGui::Button("Add Empty Dashboard Panel", ImVec2(200, 0)))
            {
                // Todo
            }

            if(ImGui::Button("Show ImGui Demo pannel", ImVec2(200, 0)))
            {
                m_RenderDemoWindow = true;
            }
        }
        ImGui::End();
    }
}