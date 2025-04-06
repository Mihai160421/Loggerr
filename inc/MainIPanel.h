#pragma once
/*  
    This is the main pannel rendered first on wich all the other (Dashboard) pannelese are docked first.
    There can only be one in existence at a time.
*/

#include "pch.h"
#include "IPanel.h"

namespace APPLICATION_NAME
{
    class MainIPanel : public IPanel
    {
    private:
        MainIPanel();
        static MainIPanel* m_Instance; // Singleton instance of the main panel
    public:
        
        MainIPanel& operator=(const MainIPanel&) = delete;
        static MainIPanel* GetInstance();

        void OnRender() override;

    };
}
