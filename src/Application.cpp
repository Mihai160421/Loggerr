#include "Application.h"

#define GLSL_VERSION ("#version 130")

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

namespace Logger
{
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
        window = glfwCreateWindow(1280, 720, "Loggerr", nullptr, nullptr);
        if (window == nullptr)
            return ;
        
        glfwMakeContextCurrent(window);
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
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(GLSL_VERSION);
    }

    void Application::Run()
    {
        while (!glfwWindowShouldClose(window))
        {
            ImGuiIO& io = ImGui::GetIO();
            glfwPollEvents();
            if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
            {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }
    
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            {
                // Make the GLFW window viewport dockable
                auto viewport =  ImGui::GetWindowViewport();
                ImGui::DockSpaceOverViewport(viewport->ID);

                RenderMenuBar();

                HandlePanels();

                if(m_RenderDebugWindow)
                {
                    RenderDebugWindow();
                }
            }

            ImGui::Render();
    
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            
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
    
            glfwSwapBuffers(window);
        }

        Shutdown();
    }

    void Application::RenderMenuBar()
    {
        if (ImGui::BeginMainMenuBar()) {

            /* New menu item */
            if (ImGui::BeginMenu("New")) {
                if (ImGui::MenuItem("New Connection")) {
                    ConnectionPanel& panel = ConnectionPanel::getInstance();

                    if(panel.m_Closed)
                    {
                        panel.m_Closed = false;
                        AddPanel(&panel);
                    }
                }
                ImGui::Separator(); 
                if (ImGui::MenuItem("Exit")) {
                    glfwSetWindowShouldClose(window, true);
                }
                ImGui::EndMenu();
            }

              /* New menu item */
              if (ImGui::BeginMenu("Debug")) {
                m_RenderDebugWindow = true;
                ImGui::EndMenu();
            }
        
        
            ImGui::EndMainMenuBar();
        }
    }

    void Application::AddPanel(IPanel* panel) {
        panels.push_back(panel);
    }

    void Application::HandlePanels() {

        IPanel* panelToRemove = nullptr;
        for (auto& panel : panels) {

            if(panel->m_Closed)
            {
                // If it was closed, remove it from the list and don't render it
                panelToRemove = panel;
            }
            else
            {
                panel->Render();
            }
        }

        // Stupid way to do it but if we do it inside for loop it will seg fault
        // TODO Find a better way
        if(panelToRemove)
        {
            panels.remove(panelToRemove);
        }
    }

    void Application::RenderDebugWindow()
    {
        ImVec2 newSize(800, 600); 
        ImGui::SetNextWindowSize(newSize, 0);
        if(ImGui::Begin("Debug Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGuiIO& io = ImGui::GetIO();
            ImGui::TextWrapped("Pannels: %d", panels.size());
            ImGui::TextWrapped("Framerate: %f", io.Framerate);
            if(ImGui::Button("Add new dashboard"))
            {
                // TODO
            }
        }
        ImGui::End();
    }

    void Application::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}