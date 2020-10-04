/*
MIT License

Copyright (c) 2019-2020 Zhuang Guan

https://github.com/AirGuanZ

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <algorithm>
#include <array>
#include <cstring>
#include <filesystem>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include "imgui/imgui.h"

using ImGuiFileBrowserFlags = int;

enum ImGuiFileBrowserFlags_
{
    ImGuiFileBrowserFlags_SelectDirectory   = 1 << 0, // select directory instead of regular file
    ImGuiFileBrowserFlags_EnterNewFilename  = 1 << 1, // allow user to enter new filename when selecting regular file
    ImGuiFileBrowserFlags_NoModal           = 1 << 2, // file browsing window is modal by default. specify this to use a popup window
    ImGuiFileBrowserFlags_NoTitleBar        = 1 << 3, // hide window title bar
    ImGuiFileBrowserFlags_NoStatusBar       = 1 << 4, // hide status bar at the bottom of browsing window
    ImGuiFileBrowserFlags_CloseOnEsc        = 1 << 5, // close file browser when pressing 'ESC'
    ImGuiFileBrowserFlags_CreateNewDir      = 1 << 6, // allow user to create new directory
    ImGuiFileBrowserFlags_MultipleSelection = 1 << 7, // allow user to select multiple files. this will hide ImGuiFileBrowserFlags_EnterNewFilename
};

namespace ImGui
{
    class FileBrowser
    {
    public:

        // pwd is set to current working directory by default
        explicit FileBrowser(ImGuiFileBrowserFlags flags = 0);

        FileBrowser(const FileBrowser &copyFrom);

        FileBrowser &operator=(const FileBrowser &copyFrom);

        // set the window size (in pixels)
        // default is (700, 450)
        void SetWindowSize(int width, int height) noexcept;

        // set the window title text
        void SetTitle(std::string title);

        // open the browsing window
        void Open();

        // close the browsing window
        void Close();

        // the browsing window is opened or not
        bool IsOpened() const noexcept;

        // display the browsing window if opened
        void Display();

        // returns true when there is a selected filename and the "ok" button was clicked
        bool HasSelected() const noexcept;

        // set current browsing directory
        bool SetPwd(const std::filesystem::path &pwd =
                                    std::filesystem::current_path());

        // get current browsing directory
        const std::filesystem::path &GetPwd() const noexcept;

        // returns selected filename. make sense only when HasSelected returns true
        // when ImGuiFileBrowserFlags_MultipleSelection is enabled, only one of
        // selected filename will be returned
        std::filesystem::path GetSelected() const;

        // returns all selected filenames.
        // when ImGuiFileBrowserFlags_MultipleSelection is enabled, use this
        // instead of GetSelected
        std::vector<std::filesystem::path> GetMultiSelected() const;

        // set selected filename to empty
        void ClearSelected();

        // set file type filters. eg. { ".h", ".cpp", ".hpp", ".cc", ".inl" }
        void SetTypeFilters(const std::vector<const char*> &typeFilters);

    private:
    
        template <class Functor>
        struct ScopeGuard
        {
            ScopeGuard(Functor&& t) : func(std::move(t)) { }

            ~ScopeGuard()
            {
                func();
            }

        private:

            Functor func;
        };

        void SetPwdUncatched(const std::filesystem::path &pwd);

#ifdef _WIN32
        static std::uint32_t GetDrivesBitMask();
#endif

        // for c++17 compatibility

#if defined(__cpp_lib_char8_t)
        static std::string u8StrToStr(std::u8string s);
#endif
        static std::string u8StrToStr(std::string s);

        int width_;
        int height_;
        ImGuiFileBrowserFlags flags_;

        std::string title_;
        std::string openLabel_;

        bool openFlag_;
        bool closeFlag_;
        bool isOpened_;
        bool ok_;

        std::string statusStr_;

        std::vector<const char*> typeFilters_;
        int typeFilterIndex_;

        std::filesystem::path pwd_;
        std::set<std::filesystem::path> selectedFilenames_;

        struct FileRecord
        {
            bool isDir = false;
            std::filesystem::path name;
            std::string showName;
            std::filesystem::path extension;
        };
        std::vector<FileRecord> fileRecords_;

        // IMPROVE: truncate when selectedFilename_.length() > inputNameBuf_.size() - 1
        static constexpr size_t INPUT_NAME_BUF_SIZE = 512;
        std::unique_ptr<std::array<char, INPUT_NAME_BUF_SIZE>> inputNameBuf_;

        std::string openNewDirLabel_;
        std::unique_ptr<std::array<char, INPUT_NAME_BUF_SIZE>> newDirNameBuf_;

#ifdef _WIN32
        uint32_t drives_;
#endif
    };
} // namespace ImGui
