
#pragma once
#include "pch.h"
#include "MainIPanel.h"

namespace APPLICATION_NAME
{    
    enum VT100Color : ImU32
    {
        VT100_BLACK           = IM_COL32(0,   0,   0,   255),
        VT100_RED             = IM_COL32(128, 0,   0,   255),
        VT100_GREEN           = IM_COL32(0,   128, 0,   255),
        VT100_YELLOW          = IM_COL32(128, 128, 0,   255),
        VT100_BLUE            = IM_COL32(0,   0,   128, 255),
        VT100_MAGENTA         = IM_COL32(128, 0,   128, 255),
        VT100_CYAN            = IM_COL32(0,   128, 128, 255),
        VT100_WHITE           = IM_COL32(192, 192, 192, 255),  
        
        VT100_BRIGHT_BLACK    = IM_COL32(128, 128, 128, 255),
        VT100_BRIGHT_RED      = IM_COL32(255, 0,   0,   255),
        VT100_BRIGHT_GREEN    = IM_COL32(0,   255, 0,   255),
        VT100_BRIGHT_YELLOW   = IM_COL32(255, 255, 0,   255),
        VT100_BRIGHT_BLUE     = IM_COL32(0,   0,   255, 255),
        VT100_BRIGHT_MAGENTA  = IM_COL32(255, 0,   255, 255),
        VT100_BRIGHT_CYAN     = IM_COL32(0,   255, 255, 255),
        VT100_BRIGHT_WHITE    = IM_COL32(255, 255, 255, 255),
    };

    struct VT100Attributes 
    {
        bool bold       = false; /* Not Implemented */
        bool underline  = false; /* Not Implemented */
        bool inverse    = false; /* Not Implemented */
        bool blink      = false; /* Not implemented */
        bool conceal    = false; /* Not implemented */
    };

    struct Cursor
    {
        size_t x, y;
    };

    struct Cell {
        /*
            When printing the character it is required to use a null terminated string
            For optimization purposes we pre-compute the caracter as a null terminated string
        */
        char character[2] = {' ', '\0'};
        VT100Color foreground = VT100_BRIGHT_WHITE;
        VT100Color background = VT100_BLACK;        /* Not yet implemented */
        VT100Attributes attr;
    
        Cell() = default;

        static Cell EMPTY_CELL() { Cell Empty; return Empty; };
    
        Cell(char c, VT100Color fg, VT100Color bg, const VT100Attributes& a)
            :foreground(fg), background(bg), attr(a) 
            {
                character[0] = c;
            }
    };

    struct DebugLogs
    {
        enum ltype
        {
            _ERROR,
            INFO,
            WARING,
        };
        ltype type;
        std::string log;
    };

    struct ScreenBuffer
    {
        std::shared_ptr<std::deque<std::vector<Cell>>> m_CellsMatrix;
        Cursor WriteCursor = {0, 0};

        void AddCell(Cell &cell)
        {
            // Get reference to bufer line
            std::vector<Cell> *line = &(*m_CellsMatrix)[WriteCursor.y];

            if(line->size() == WriteCursor.x) 
            {
                line->push_back(Cell::EMPTY_CELL()); // Add a line if neccesary
            }
            
            (*line)[WriteCursor.x] = cell; // Overwrite this cell
            WriteCursor.x++;
        };

        void AddCells(std::vector<Cell> cells) 
        {
            for(Cell& cell : cells)
            {
                AddCell(cell);
            }
        }

        void NewLine()
        {
            // Handle a new line [\n]
            WriteCursor.y++; 

            // IF we reached the end, add a new empty line in size as the CursorPosX+1 
            if (Rows() == WriteCursor.y)
            {
                m_CellsMatrix->push_back(std::vector<Cell>(WriteCursor.x+1, Cell::EMPTY_CELL()));
            }   
        }

        size_t Rows()
        {
            return m_CellsMatrix->size();
        }
    };

    class  Terminal
    {
    private:
        std::string m_Name;

        bool m_DebugWindow = true;
        std::list<DebugLogs> m_DebugLogs;

        /*
            This is the buffer of cells (characters with additional data) that
            gets rendered to the terminal
        */
       vtparse_t m_vtparser;

       /*
            The Screen Buffer
       */
       ScreenBuffer m_ScreenBuffer;
       
    public:
        Terminal(std::string name);
        ~Terminal();

        /*
            Render the terminal
        */
        void RenderTerminal();

        /*
            Write data into the terminal

            @param[in] data: Data to be written into terminal
        */
        void Write(std::vector<uint8_t> data);

        /*
            Parse a single byte and set the Parser context for future parsing
            @param[in] byte: Byte to be parsed 
        */
        void VT100ParseByte(uint8_t byte);

        void TerminalRenderDebugWindow(bool* open);

        void AddDebugLog(DebugLogs::ltype, std::string);
    private:
        static ImVec4 VT100ColorToImVec2(VT100Color color);

        static void VT100ParserCallback(struct vtparse* parser, vtparse_action_t action, unsigned char ch);
    };
}