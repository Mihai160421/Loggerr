
#pragma once
#include "pch.h"
#include "MainIPanel.h"

namespace APPLICATION_NAME
{
    enum VT100Color
    {
        VT100_BLACK,
        VT100_RED,
        VT100_GREEN,
        VT100_YELLOW,
        VT100_BLUE,
        VT100_MAGENTA,
        VT100_CYAN,
        VT100_WHITE,
        VT100_BRIGHT_BLACK,
        VT100_BRIGHT_RED,
        VT100_BRIGHT_GREEN,
        VT100_BRIGHT_YELLOW,
        VT100_BRIGHT_BLUE,
        VT100_BRIGHT_MAGENTA,
        VT100_BRIGHT_CYAN,
        VT100_BRIGHT_WHITE,
        VT100_COLOR_COUNT
    } ;

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
        VT100Color background = VT100_BLACK; /* Not yet implemented */
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
        std::shared_ptr<std::deque<std::vector<Cell>>> m_Buffer;
        Cursor WriteCursor = {0, 0};

        void AddCell(Cell &cell)
        {
            // Get reference to bufer line
            std::vector<Cell> *line = &(*m_Buffer)[WriteCursor.y];

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
                m_Buffer->push_back(std::vector<Cell>(WriteCursor.x+1, Cell::EMPTY_CELL()));
            }   
        }

        size_t Rows()
        {
            return m_Buffer->size();
        }
    };
    
    class  Terminal
    {
    private:
        uint64_t m_RowSize   = 0;               // Number of characters that fit on a row
        uint64_t m_ColSize   = 0;               // Number of characters that fit on a column
        uint64_t m_LinesRendered    = 0;        // Number of lines wraps
        uint64_t m_CellsRendered    = 0;        // Number of lines wraps
        ImVec2 m_MouseRelPositon;                // Mouse relative position to window terminal
        Cell m_HoveredCell;
        std::string m_Name;

        bool m_DebugWindow = true;
        std::list<DebugLogs> m_DebugLogs;
        
        /* Used for selecting and coying text from the terminal */
        bool m_IsSelectableActive = false;
        bool m_IsHeadSelected = false;
        Cursor m_SelectableTail = {0, 0};
        Cursor m_SelectableHead = {0, 0};

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