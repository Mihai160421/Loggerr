#pragma once

namespace Logger
{
    class IPanel {
        public:
            bool m_Closed = false;
            virtual ~IPanel() = default;
            virtual void Render() = 0;
    };
}