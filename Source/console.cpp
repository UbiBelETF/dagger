#include "console.h"
#include "engine.h"

using namespace dagger;

#pragma region IMGUI DEBUG CONSOLE IMPLEMENTATION

DebugConsole::DebugConsole()
{
    ClearLog();
    memset(m_InputBuf, 0, sizeof(m_InputBuf));
    m_HistoryPos = -1;

    // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
    m_Commands.push_back("HELP");
    m_Commands.push_back("HISTORY");
    m_Commands.push_back("CLEAR");
    m_Commands.push_back("CLASSIFY");
    m_AutoScroll = true;
    m_ScrollToBottom = false;
}

DebugConsole::~DebugConsole()
{
    ClearLog();
    for (int i = 0; i < m_History.Size; i++)
        free(m_History[i]);
}

// Portable helpers
int   DebugConsole::Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
int   DebugConsole::Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
char* DebugConsole::Strdup(const char* s) { size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
void  DebugConsole::Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

void DebugConsole::ClearLog()
{
    for (int i = 0; i < m_Items.Size; i++)
        free(m_Items[i]);
    m_Items.clear();
}

void DebugConsole::AddLog(const char* fmt_, ...)
{
    // FIXME-OPT
    char buf[1024];
    va_list args;
    va_start(args, fmt_);
    vsnprintf(buf, IM_ARRAYSIZE(buf), fmt_, args);
    buf[IM_ARRAYSIZE(buf) - 1] = 0;
    va_end(args);
    m_Items.push_back(Strdup(buf));
}

void DebugConsole::Draw(const char* title_, bool* open_ = nullptr)
{
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(140, 30));
    if (!ImGui::Begin(title_, open_))
    {
        ImGui::End();
        return;
    }

    // Reserve enough left-over height for 1 separator + 1 input text
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::Selectable("Clear")) ClearLog();
        ImGui::EndPopup();
    }

    // Display every line as a separate entry so we can change their color or add custom widgets.
    // If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
    // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
    // to only process visible items. The clipper will automatically measure the height of your first item and then
    // "seek" to display only items in the visible area.
    // To use the clipper we can replace your standard loop:
    //      for (int i = 0; i < Items.Size; i++)
    //   With:
    //      ImGuiListClipper clipper;
    //      clipper.Begin(Items.Size);
    //      while (clipper.Step())
    //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
    // - That your items are evenly spaced (same height)
    // - That you have cheap random access to your elements (you can access them given their index,
    //   without processing all the ones before)
    // You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
    // We would need random-access on the post-filtered list.
    // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
    // or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
    // and appending newly elements as they are inserted. This is left as a task to the user until we can manage
    // to improve this example code!
    // If your items are of variable height:
    // - Split them into same height items would be simpler and facilitate random-seeking into your list.
    // - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
    for (int i = 0; i < m_Items.Size; i++)
    {
        const char* item = m_Items[i];
        if (!m_Filter.PassFilter(item))
            continue;

        // Normally you would store more information in your item than just a string.
        // (e.g. make Items[] an array of structure, store color/type etc.)
        ImVec4 color;
        bool has_color = false;
        if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
        else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
        if (has_color)
            ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(item);
        if (has_color)
            ImGui::PopStyleColor();
    }

    if (m_ScrollToBottom || (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);
    m_ScrollToBottom = false;

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::Separator();

    // Command-line
    bool reclaim_focus = false;
    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
    if (ImGui::InputText("", m_InputBuf, IM_ARRAYSIZE(m_InputBuf), input_text_flags, &TextEditCallbackStub, (void*)this))
    {
        char* s = m_InputBuf;
        Strtrim(s);
        if (s[0])
            ExecCommand(s);
        strcpy_s(s, sizeof s, "");
        reclaim_focus = true;
    }

    // Auto-focus on window apparition
    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

    ImGui::End();
}

void DebugConsole::ExecCommand(const char* command_)
{
    AddLog("# %s\n", command_);

    m_HistoryPos = -1;
    for (int i = m_History.Size - 1; i >= 0; i--)
        if (Stricmp(m_History[i], command_) == 0)
        {
            free(m_History[i]);
            m_History.erase(m_History.begin() + i);
            break;
        }
    m_History.push_back(Strdup(command_));

    Engine::Dispatch().trigger<Command>(Command(command_));

    m_ScrollToBottom = true;
}

int DebugConsole::TextEditCallback(ImGuiInputTextCallbackData* data_)
{
    //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
    switch (data_->EventFlag)
    {
    case ImGuiInputTextFlags_CallbackCompletion:
    {
        // Example of TEXT COMPLETION

        // Locate beginning of current word
        const char* word_end = data_->Buf + data_->CursorPos;
        const char* word_start = word_end;
        while (word_start > data_->Buf)
        {
            const char c = word_start[-1];
            if (c == ' ' || c == '\t' || c == ',' || c == ';')
                break;
            word_start--;
        }

        // Build a list of candidates
        ImVector<const char*> candidates;
        for (int i = 0; i < m_Commands.Size; i++)
            if (Strnicmp(m_Commands[i], word_start, (int)(word_end - word_start)) == 0)
                candidates.push_back(m_Commands[i]);

        if (candidates.Size == 0)
        {
            // No match
            AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
        }
        else if (candidates.Size == 1)
        {
            // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
            data_->DeleteChars((int)(word_start - data_->Buf), (int)(word_end - word_start));
            data_->InsertChars(data_->CursorPos, candidates[0]);
            data_->InsertChars(data_->CursorPos, " ");
        }
        else
        {
            // Multiple matches. Complete as much as we can..
            // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
            int match_len = (int)(word_end - word_start);
            for (;;)
            {
                int c = 0;
                bool all_candidates_matches = true;
                for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                    if (i == 0)
                        c = toupper(candidates[i][match_len]);
                    else if (c == 0 || c != toupper(candidates[i][match_len]))
                        all_candidates_matches = false;
                if (!all_candidates_matches)
                    break;
                match_len++;
            }

            if (match_len > 0)
            {
                data_->DeleteChars((int)(word_start - data_->Buf), (int)(word_end - word_start));
                data_->InsertChars(data_->CursorPos, candidates[0], candidates[0] + match_len);
            }

            // List matches
            AddLog("Possible matches:\n");
            for (int i = 0; i < candidates.Size; i++)
                AddLog("- %s\n", candidates[i]);
        }

        break;
    }
    case ImGuiInputTextFlags_CallbackHistory:
    {
        // Example of HISTORY
        const int prev_history_pos = m_HistoryPos;
        if (data_->EventKey == ImGuiKey_UpArrow)
        {
            if (m_HistoryPos == -1)
                m_HistoryPos = m_History.Size - 1;
            else if (m_HistoryPos > 0)
                m_HistoryPos--;
        }
        else if (data_->EventKey == ImGuiKey_DownArrow)
        {
            if (m_HistoryPos != -1)
                if (++m_HistoryPos >= m_History.Size)
                    m_HistoryPos = -1;
        }

        // A better implementation would preserve the data on the current input line along with cursor position.
        if (prev_history_pos != m_HistoryPos)
        {
            const char* history_str = (m_HistoryPos >= 0) ? m_History[m_HistoryPos] : "";
            data_->DeleteChars(0, data_->BufTextLen);
            data_->InsertChars(0, history_str);
        }
    }
    }
    return 0;
}

#pragma endregion

void ConsoleSystem::RenderGUI() 
{
    m_Console.Draw("Console");
}

void ConsoleSystem::ReceiveLog(Log log)
{
    m_Console.AddLog(log.Message().c_str());
}

void ConsoleSystem::SpinUp(Engine& engine_) 
{
    Engine::Dispatch().sink<GUIRender>().connect<&ConsoleSystem::RenderGUI>(this);
    Engine::Dispatch().sink<Log>().connect<&ConsoleSystem::ReceiveLog>(this);
}

void ConsoleSystem::WindDown(Engine& engine_) 
{
    Engine::Dispatch().sink<GUIRender>().disconnect<&ConsoleSystem::RenderGUI>(this);
    Engine::Dispatch().sink<Log>().disconnect<&ConsoleSystem::ReceiveLog>(this);
}
