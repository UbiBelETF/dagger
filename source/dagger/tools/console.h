#pragma once

#include <imgui/imgui.h>

#include "core/system.h"
#include "core/core.h"
#include "core/graphics/window.h"

using namespace dagger;

#pragma region IMGUI DEBUG CONSOLE IMPLEMENTATION

struct DebugConsole
{
    char                  m_InputBuf[256];
    ImVector<char*>       m_Items;
    ImVector<const char*> m_Commands;
    ImVector<char*>       m_History;
    int                   m_HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter       m_Filter;
    bool                  m_AutoScroll;
    bool                  m_ScrollToBottom;

    DebugConsole();
    ~DebugConsole();

    // Portable helpers
    static int   Stricmp(const char* s1, const char* s2); 
    static int   Strnicmp(const char* s1, const char* s2, int n); 
    static char* Strdup(const char* s);
    static void  Strtrim(char* s);

    void ClearLog();
    void AddLog(const char* fmt_, ...);
    void Draw(const char* title_, bool* open_);
    void ExecCommand(const char* command_);

    static int TextEditCallbackStub(ImGuiInputTextCallbackData* data_)
    {
        DebugConsole* console = (DebugConsole*)data_->UserData;
        return console->TextEditCallback(data_);
    }
        
    int TextEditCallback(ImGuiInputTextCallbackData* data_);
};
#pragma endregion

class ConsoleSystem 
    : public System
    , public Subscriber<GUIRender, Log>
{
    DebugConsole m_Console;
	bool m_IsOpen;
	void RenderGUI();
	void ReceiveLog(Log log);

public:
	ConsoleSystem()
		: m_Console{}
		, m_IsOpen{ false }
	{}

    inline String SystemName() { return "Console System"; }

	void SpinUp() override;
	void WindDown() override;
};