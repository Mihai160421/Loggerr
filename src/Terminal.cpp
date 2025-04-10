#include "Terminal.h"
#include "Application.h"

#define TAB_HEIGTH (22.5f) // TODO find a way to get the actual height

namespace APPLICATION_NAME
{
    Terminal::Terminal(std::string name)
    {   
        m_Name = name;
        m_ScreenBuffer.m_CellsMatrix = std::make_shared<std::deque<std::vector<Cell>>>();
        m_ScreenBuffer.m_CellsMatrix->push_back(std::vector<Cell>()); // Add a first empty line

        vtparse_init(&m_vtparser, &Terminal::VT100ParserCallback);
        m_vtparser.user_data = this;
    }

    Terminal::~Terminal()
    {
        // TODO Check what resources we should free
    }   

    void Terminal::RenderTerminal()
    {
        if(m_DebugWindow)
        {
            TerminalRenderDebugWindow(&m_DebugWindow);
        }

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::SetNextWindowDockID(MainIPanel::GetInstance()->GetDockspaceID());

        if(ImGui::Begin(m_Name.c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings))
        {
            const ImVec2 AvailableDrawing   = ImGui::GetContentRegionAvail();
            const ImVec2 WindowPos          = ImGui::GetWindowPos();
            const float CharHeight          = ImGui::GetTextLineHeight();
            const float MaxRows             = std::floor(AvailableDrawing.y / CharHeight);
            const float TitleBarHeight      = ImGui::GetWindowSize().y - AvailableDrawing.y;
            ImDrawList* DrawList            = ImGui::GetWindowDrawList();

            // Hack to set the scrollable area for the window based on the number of lines available
            ImGui::Dummy(ImVec2(0, CharHeight * m_ScreenBuffer.m_CellsMatrix->size()));

            float ScrollbarYPos = ImGui::GetScrollY();

            if(ScrollbarYPos >= ImGui::GetScrollMaxY())
            {
                ImGui::SetScrollHereY(1.0f);
                ScrollbarYPos = ImGui::GetScrollY();
            }

            // Fetch the starting from the buffer based on the scrollbar y position

            auto itStart = m_ScreenBuffer.m_CellsMatrix->begin() + (ScrollbarYPos / CharHeight); 

            ImVec2 RenderCursor(WindowPos.x, WindowPos.y + TitleBarHeight);
            unsigned RowsRendered = 0;

            for(auto it = itStart; it <= m_ScreenBuffer.m_CellsMatrix->end(); it++)
            {
                if(RowsRendered++ == MaxRows)
                {
                    // Stop drawing if outside the limit
                    break;
                }
                for(Cell& cell : *it)
                {
                    DrawList->AddText(RenderCursor, cell.foreground, cell.character);
                    RenderCursor.x += ImGui::CalcTextSize(cell.character).x;
                }
                RenderCursor.y += CharHeight;
                RenderCursor.x = WindowPos.x;
            }

        }
        ImGui::End();
        ImGui::PopStyleVar(2);
    }

    void Terminal::Write(std::vector<uint8_t> data)
    {
        for(uint8_t byte : data)
        {
            VT100ParseByte(byte);
        }
    }

    void Terminal::VT100ParseByte(uint8_t byte)
    {
        vtparse(&m_vtparser, (unsigned char*)&byte, 1);
    }

    ImVec4 Terminal::VT100ColorToImVec2(VT100Color color)
    {
        switch (color)
        {
            case VT100_BLACK:
                return ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
            case VT100_RED:
                return ImVec4(170.0f / 255.0f, 0.0f, 0.0f, 1.0f);
            case VT100_GREEN:
                return ImVec4(0.0f, 170.0f / 255.0f, 0.0f, 1.0f);
            case VT100_YELLOW:
                return ImVec4(170.0f / 255.0f, 85.0f / 255.0f, 0.0f, 1.0f);
            case VT100_BLUE:
                return ImVec4(0.0f, 0.0f, 170.0f / 255.0f, 1.0f);
            case VT100_MAGENTA:
                return ImVec4(170.0f / 255.0f, 0.0f, 170.0f / 255.0f, 1.0f);
            case VT100_CYAN:
                return ImVec4(0.0f, 170.0f / 255.0f, 170.0f / 255.0f, 1.0f);
            case VT100_WHITE:
                return ImVec4(170.0f / 255.0f, 170.0f / 255.0f, 170.0f / 255.0f, 1.0f);
            case VT100_BRIGHT_BLACK:
                return ImVec4(85.0f / 255.0f, 85.0f / 255.0f, 85.0f / 255.0f, 1.0f);
            case VT100_BRIGHT_RED:
                return ImVec4(255.0f / 255.0f, 85.0f / 255.0f, 85.0f / 255.0f, 1.0f);
            case VT100_BRIGHT_GREEN:
                return ImVec4(85.0f / 255.0f, 255.0f / 255.0f, 85.0f / 255.0f, 1.0f);
            case VT100_BRIGHT_YELLOW:
                return ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 85.0f / 255.0f, 1.0f);
            case VT100_BRIGHT_BLUE:
                return ImVec4(85.0f / 255.0f, 85.0f / 255.0f, 255.0f / 255.0f, 1.0f);
            case VT100_BRIGHT_MAGENTA:
                return ImVec4(255.0f / 255.0f, 85.0f / 255.0f, 255.0f / 255.0f, 1.0f);
            case VT100_BRIGHT_CYAN:
                return ImVec4(85.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f);
            case VT100_BRIGHT_WHITE:
                return ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f);
            default:
                return ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        }
    }

    void Terminal::VT100ParserCallback(struct vtparse* parser, vtparse_action_t action, unsigned char ch) {
        // Obtaine the class instance
        Terminal *self= static_cast<Terminal*>(parser->user_data);
        size_t& CursorPosX = self->m_ScreenBuffer.WriteCursor.x;
        size_t& CursorPosY = self->m_ScreenBuffer.WriteCursor.y;

        if(self->m_DebugWindow)
        {
            std::ostringstream logStream;

            logStream << "Parser Action [" << ACTION_NAMES[action] << "] ";
            if (ch != 0) {
                logStream << "Char: 0x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(ch)
                        << " ('" << static_cast<char>(ch) << "')\n";
            }

            if (parser->num_intermediate_chars > 0) {
                logStream << parser->num_intermediate_chars << " Intermediate chars:\n";
                for (int i = 0; i < parser->num_intermediate_chars; i++) {
                    logStream << "  0x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
                            << static_cast<int>(parser->intermediate_chars[i])
                            << " ('" << parser->intermediate_chars[i] << "')\n";
                }
            }

            if (parser->num_params > 0) {
                logStream << parser->num_params << " Parameters:\n";
                for (int i = 0; i < parser->num_params; i++) {
                    logStream << "\t" << parser->params[i] << "\n";
                }
            }
            
            logStream << "\n";

            self->AddDebugLog(DebugLogs::INFO, logStream.str());
        }

        switch (action) {
            case VTPARSE_ACTION_PRINT:
                {
                    // New cell
                    VT100Attributes atributes;
                    Cell cell(ch, VT100_BRIGHT_WHITE, VT100_BLACK, atributes);

                    self->m_ScreenBuffer.AddCell(cell);
                }              
            break;

            case VTPARSE_ACTION_EXECUTE:
                switch (ch)
                {
                case '\n':
                self->m_ScreenBuffer.NewLine();
                    break;
                
                case '\r':
                        CursorPosX=0; 
                    break;

                case '\t':
                        // Add 4 empty cell for tabs starting from current position
                        self->m_ScreenBuffer.AddCells(std::vector<Cell>(4, Cell::EMPTY_CELL()));
                    break;
                default:
                        self->AddDebugLog(DebugLogs::WARING, "Unknown Execute");
                    break;
                }



            break;
            case VTPARSE_ACTION_HOOK:               break;
            case VTPARSE_ACTION_PUT:                break;
            case VTPARSE_ACTION_OSC_START:          break;
            case VTPARSE_ACTION_OSC_PUT:            break;
            case VTPARSE_ACTION_OSC_END:            break;  
            case VTPARSE_ACTION_UNHOOK:             break;
            case VTPARSE_ACTION_CSI_DISPATCH:       break;
            case VTPARSE_ACTION_ESC_DISPATCH:       break;
            case VTPARSE_ACTION_ERROR:              break;

            default:
                break;
        }
    }

    void Terminal::TerminalRenderDebugWindow(bool* open)
    {
        std::string name = m_Name + " Debug Window"; 
        if(ImGui::Begin(name.c_str(), open, ImGuiWindowFlags_NoSavedSettings))
        {
            if(ImGui::BeginChild("##LogWindow",  {1000, 400}))
            {
                int it = 0;
                for (auto log : m_DebugLogs)
                {
                    it++;
                    if(log.type == log._ERROR)
                    {
                        ImGui::TextColored({1.f, 0.f, 0.f, 1.f}, "%d. [ERR] %s",    it,  log.log.c_str());
                    }else if (log.type == log.WARING)
                    {
                        ImGui::TextColored({1.f, 1.f, 0.f, 1.f}, "%d. [WARNING] %s",it,  log.log.c_str());
                    }else if (log.type == log.INFO)
                    {
                        ImGui::TextColored({1.f, 1.f, 1.f, 1.f}, "%d. [INFO] %s",   it,  log.log.c_str());
                    }
                }
            }
            ImGui::EndChild();

            static char buffer[2000] = "Lorem Ipsum is simply dummy text of the printing and typesetting industry.";
            ImGui::InputText("##InputTextDebug", buffer, sizeof(buffer));
            ImGui::SameLine();
            if(ImGui::Button("Write"))
            {
                std::vector<uint8_t> vec(reinterpret_cast<uint8_t*>(buffer), reinterpret_cast<uint8_t*>(buffer + strlen(buffer)));
                Write(vec);
            }
            ImGui::SameLine();
            if(ImGui::Button("Write NL"))
            {
                char buff[2] = {'\r', '\n'};
                int iter = 1;
                while(iter--)
                {
                    std::vector<uint8_t> vec(reinterpret_cast<uint8_t*>(buffer), reinterpret_cast<uint8_t*>(buffer + strlen(buffer)));
                    std::vector<uint8_t> vec2(reinterpret_cast<uint8_t*>(buff), reinterpret_cast<uint8_t*>(buff + 2));
                    Write(vec);
                    Write(vec2);
                }
            }

            ImGui::SameLine();


            if(ImGui::Button("Insert: \\n"))
            {
                static char insert[2] = "\n";
                std::vector<uint8_t> vec(reinterpret_cast<uint8_t*>(insert), reinterpret_cast<uint8_t*>(insert+1));
                Write(vec);
            }
            ImGui::SameLine();
            if(ImGui::Button("Insert: \\r"))
            {
                static char insert[2] = "\r";
                std::vector<uint8_t> vec(reinterpret_cast<uint8_t*>(insert), reinterpret_cast<uint8_t*>(insert+1));
                Write(vec);
            }
            ImGui::SameLine();
            if(ImGui::Button("Insert: \\t"))
            {
                static char insert[2] = "\t";
                std::vector<uint8_t> vec(reinterpret_cast<uint8_t*>(insert), reinterpret_cast<uint8_t*>(insert+1));
                Write(vec);
            }
            ImGui::SameLine();
            if(ImGui::Button("Insert: \\a"))
            {
                static char insert[2] = "\a";
                std::vector<uint8_t> vec(reinterpret_cast<uint8_t*>(insert), reinterpret_cast<uint8_t*>(insert+1));
                Write(vec);
            }

        }
        ImGui::End();
    }
    
    void Terminal::AddDebugLog(DebugLogs::ltype type, std::string log)
    {
        if(m_DebugWindow == true)                     
        {                                   
            DebugLogs Log;                  
            Log.type = type; 
            Log.log = log;           
            this->m_DebugLogs.push_back(Log);     

            if(this->m_DebugLogs.size() > 100)
            {
                this->m_DebugLogs.erase(m_DebugLogs.begin());
            }
        }
    }
}
