#include "imfilebrowser.h"
#include "imgui/imgui.h"

using namespace ImGui;

FileBrowser::FileBrowser(ImGuiFileBrowserFlags flags)
    : width_(700), height_(450), flags_(flags),
    openFlag_(false), closeFlag_(false), isOpened_(false), ok_(false),
    inputNameBuf_(std::make_unique<std::array<char, INPUT_NAME_BUF_SIZE>>())
{
    if (flags_ & ImGuiFileBrowserFlags_CreateNewDir)
        newDirNameBuf_ = std::make_unique<
        std::array<char, INPUT_NAME_BUF_SIZE>>();

    inputNameBuf_->front() = '\0';
    inputNameBuf_->back() = '\0';
    SetTitle("file browser");
    SetPwd(std::filesystem::current_path());

    typeFilters_.clear();
    typeFilterIndex_ = 0;

#ifdef _WIN32
    drives_ = GetDrivesBitMask();
#endif
}

FileBrowser::FileBrowser(const FileBrowser& copyFrom)
    : FileBrowser()
{
    *this = copyFrom;
}

FileBrowser& FileBrowser::operator=(
    const FileBrowser& copyFrom)
{
    flags_ = copyFrom.flags_;
    SetTitle(copyFrom.title_);

    openFlag_ = copyFrom.openFlag_;
    closeFlag_ = copyFrom.closeFlag_;
    isOpened_ = copyFrom.isOpened_;
    ok_ = copyFrom.ok_;

    statusStr_ = "";
    pwd_ = copyFrom.pwd_;
    selectedFilenames_ = copyFrom.selectedFilenames_;

    fileRecords_ = copyFrom.fileRecords_;

    *inputNameBuf_ = *copyFrom.inputNameBuf_;

    if (flags_ & ImGuiFileBrowserFlags_CreateNewDir)
    {
        newDirNameBuf_ = std::make_unique<
            std::array<char, INPUT_NAME_BUF_SIZE>>();
        *newDirNameBuf_ = *copyFrom.newDirNameBuf_;
    }

    return *this;
}

void FileBrowser::SetWindowSize(int width, int height) noexcept
{
    assert(width > 0 && height > 0);
    width_ = width;
    height_ = height;
}

void FileBrowser::SetTitle(std::string title)
{
    title_ = std::move(title);
    openLabel_ = title_ + "##filebrowser_" +
        std::to_string(reinterpret_cast<size_t>(this));
    openNewDirLabel_ = "new dir##new_dir_" +
        std::to_string(reinterpret_cast<size_t>(this));
}

void FileBrowser::Open()
{
    ClearSelected();
    statusStr_ = std::string();
    openFlag_ = true;
    closeFlag_ = false;
}

void FileBrowser::Close()
{
    ClearSelected();
    statusStr_ = std::string();
    closeFlag_ = true;
    openFlag_ = false;
}

bool FileBrowser::IsOpened() const noexcept
{
    return isOpened_;
}

void FileBrowser::Display()
{
    PushID(this);
    ScopeGuard exitThis([this]
        {
            openFlag_ = false;
            closeFlag_ = false;
            PopID();
        });

    if (openFlag_)
        OpenPopup(openLabel_.c_str());
    isOpened_ = false;

    // open the popup window

    if (openFlag_ && (flags_ & ImGuiFileBrowserFlags_NoModal))
    {
        SetNextWindowSize(
            ImVec2(static_cast<float>(width_), static_cast<float>(height_)));
    }
    else
    {
        SetNextWindowSize(
            ImVec2(static_cast<float>(width_), static_cast<float>(height_)),
            ImGuiCond_FirstUseEver);
    }
    if (flags_ & ImGuiFileBrowserFlags_NoModal)
    {
        if (!BeginPopup(openLabel_.c_str()))
            return;
    }
    else if (!BeginPopupModal(openLabel_.c_str(), nullptr,
        flags_ & ImGuiFileBrowserFlags_NoTitleBar ?
        ImGuiWindowFlags_NoTitleBar : 0))
    {
        return;
    }
    isOpened_ = true;
    ScopeGuard endPopup([] { EndPopup(); });

    // display elements in pwd

#ifdef _WIN32
    char currentDrive = static_cast<char>(pwd_.c_str()[0]);
    char driveStr[] = { currentDrive, ':', '\0' };

    PushItemWidth(4 * GetFontSize());
    if (BeginCombo("##select_drive", driveStr))
    {
        ScopeGuard guard([&] { EndCombo(); });
        for (int i = 0; i < 26; ++i)
        {
            if (!(drives_ & (1 << i)))
                continue;
            char driveCh = static_cast<char>('A' + i);
            char selectableStr[] = { driveCh, ':', '\0' };
            bool selected = currentDrive == driveCh;
            if (Selectable(selectableStr, selected) && !selected)
            {
                char newPwd[] = { driveCh, ':', '\\', '\0' };
                SetPwd(newPwd);
            }
        }
    }
    PopItemWidth();

    SameLine();
#endif

    int secIdx = 0, newPwdLastSecIdx = -1;
    for (auto& sec : pwd_)
    {
#ifdef _WIN32
        if (secIdx == 1)
        {
            ++secIdx;
            continue;
        }
#endif
        PushID(secIdx);
        if (secIdx > 0)
            SameLine();
        if (SmallButton(u8StrToStr(sec.u8string()).c_str()))
            newPwdLastSecIdx = secIdx;
        PopID();
        ++secIdx;
    }

    if (newPwdLastSecIdx >= 0)
    {
        int i = 0;
        std::filesystem::path newPwd;
        for (auto& sec : pwd_)
        {
            if (i++ > newPwdLastSecIdx)
                break;
            newPwd /= sec;
        }
#ifdef _WIN32
        if (newPwdLastSecIdx == 0)
            newPwd /= "\\";
#endif
        SetPwd(newPwd);
    }

    SameLine();

    if (SmallButton("*"))
        SetPwd(pwd_);

    if (newDirNameBuf_)
    {
        SameLine();
        if (SmallButton("+"))
        {
            OpenPopup(openNewDirLabel_.c_str());
            (*newDirNameBuf_)[0] = '\0';
        }

        if (BeginPopup(openNewDirLabel_.c_str()))
        {
            ScopeGuard endNewDirPopup([] { EndPopup(); });

            InputText("name", newDirNameBuf_->data(), newDirNameBuf_->size());
            SameLine();

            if (Button("ok") && (*newDirNameBuf_)[0] != '\0')
            {
                ScopeGuard closeNewDirPopup([] { CloseCurrentPopup(); });
                if (create_directory(pwd_ / newDirNameBuf_->data()))
                    SetPwd(pwd_);
                else
                {
                    statusStr_ = "failed to create " +
                        std::string(newDirNameBuf_->data());
                }
            }
        }
    }

    // browse files in a child window

    float reserveHeight = GetFrameHeightWithSpacing();
    std::filesystem::path newPwd; bool setNewPwd = false;
    if (!(flags_ & ImGuiFileBrowserFlags_SelectDirectory) &&
        (flags_ & ImGuiFileBrowserFlags_EnterNewFilename))
        reserveHeight += GetFrameHeightWithSpacing();
    {
        BeginChild("ch", ImVec2(0, -reserveHeight), true,
            (flags_ & ImGuiFileBrowserFlags_NoModal) ?
            ImGuiWindowFlags_AlwaysHorizontalScrollbar : 0);
        ScopeGuard endChild([] { EndChild(); });

        for (auto& rsc : fileRecords_)
        {
            if (!rsc.isDir && typeFilters_.size() > 0 &&
                static_cast<size_t>(typeFilterIndex_) < typeFilters_.size() &&
                !(rsc.extension == typeFilters_[typeFilterIndex_]))
                continue;

            if (!rsc.name.empty() && rsc.name.c_str()[0] == '$')
                continue;

            bool selected = selectedFilenames_.find(rsc.name)
                != selectedFilenames_.end();

            if (Selectable(rsc.showName.c_str(), selected,
                ImGuiSelectableFlags_DontClosePopups))
            {
                const bool multiSelect =
                    (flags_ & ImGuiFileBrowserFlags_MultipleSelection) &&
                    IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
                    (GetIO().KeyCtrl || GetIO().KeyShift);

                if (selected)
                {
                    if (!multiSelect)
                        selectedFilenames_.clear();
                    else
                        selectedFilenames_.erase(rsc.name);

                    (*inputNameBuf_)[0] = '\0';
                }
                else if (rsc.name != "..")
                {
                    if ((rsc.isDir && (flags_ & ImGuiFileBrowserFlags_SelectDirectory)) ||
                        (!rsc.isDir && !(flags_ & ImGuiFileBrowserFlags_SelectDirectory)))
                    {
                        if (!multiSelect)
                            selectedFilenames_.clear();
                        selectedFilenames_.insert(rsc.name);
                        if (!(flags_ & ImGuiFileBrowserFlags_SelectDirectory))
                        {
#ifdef _MSC_VER
                            strcpy_s(
                                inputNameBuf_->data(), inputNameBuf_->size(),
                                u8StrToStr(rsc.name.u8string()).c_str());
#else
                            std::strncpy(inputNameBuf_->data(),
                                u8StrToStr(rsc.name.u8string()).c_str(),
                                inputNameBuf_->size() - 1);
#endif
                        }
                    }
                }
                else
                {
                    if (!multiSelect)
                        selectedFilenames_.clear();
                }
            }

            if (IsItemClicked(0) && IsMouseDoubleClicked(0))
            {
                if (rsc.isDir)
                {
                    setNewPwd = true;
                    newPwd = (rsc.name != "..") ? (pwd_ / rsc.name) :
                        pwd_.parent_path();
                }
                else if (!(flags_ & ImGuiFileBrowserFlags_SelectDirectory))
                {
                    selectedFilenames_ = { rsc.name };
                    ok_ = true;
                    CloseCurrentPopup();
                }
            }
        }
    }

    if (setNewPwd)
        SetPwd(newPwd);

    if (!(flags_ & ImGuiFileBrowserFlags_SelectDirectory) &&
        (flags_ & ImGuiFileBrowserFlags_EnterNewFilename))
    {
        PushID(this);
        ScopeGuard popTextID([] { PopID(); });

        PushItemWidth(-1);
        if (InputText("", inputNameBuf_->data(), inputNameBuf_->size()) &&
            inputNameBuf_->at(0) != '\0')
        {
            selectedFilenames_ = { inputNameBuf_->data() };
        }
        PopItemWidth();
    }

    if (!(flags_ & ImGuiFileBrowserFlags_SelectDirectory))
    {
        if (Button(" ok ") && !selectedFilenames_.empty())
        {
            ok_ = true;
            CloseCurrentPopup();
        }
    }
    else
    {
        if (Button(" ok "))
        {
            ok_ = true;
            CloseCurrentPopup();
        }
    }

    SameLine();

    int escIdx = GetIO().KeyMap[ImGuiKey_Escape];
    if (Button("cancel") || closeFlag_ ||
        ((flags_ & ImGuiFileBrowserFlags_CloseOnEsc) &&
            IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
            escIdx >= 0 && IsKeyPressed(escIdx)))
        CloseCurrentPopup();

    if (!statusStr_.empty() && !(flags_ & ImGuiFileBrowserFlags_NoStatusBar))
    {
        SameLine();
        Text("%s", statusStr_.c_str());
    }

    if (!typeFilters_.empty())
    {
        SameLine();
        PushItemWidth(8 * GetFontSize());
        Combo("##type_filters", &typeFilterIndex_,
            typeFilters_.data(), int(typeFilters_.size()));
        PopItemWidth();
    }
}

bool FileBrowser::HasSelected() const noexcept
{
    return ok_;
}

bool FileBrowser::SetPwd(const std::filesystem::path& pwd)
{
    try
    {
        SetPwdUncatched(pwd);
        return true;
    }
    catch (const std::exception& err)
    {
        statusStr_ = std::string("last error: ") + err.what();
    }
    catch (...)
    {
        statusStr_ = "last error: unknown";
    }

    SetPwdUncatched(std::filesystem::current_path());
    return false;
}

const class std::filesystem::path& FileBrowser::GetPwd() const noexcept
{
    return pwd_;
}

std::filesystem::path FileBrowser::GetSelected() const
{
    // when ok_ is true, selectedFilenames_ may be empty if SelectDirectory
    // is enabled. return pwd in that case.
    if (selectedFilenames_.empty())
        return pwd_;
    return pwd_ / *selectedFilenames_.begin();
}

std::vector<std::filesystem::path>
FileBrowser::GetMultiSelected() const
{
    if (selectedFilenames_.empty())
        return { pwd_ };
    std::vector<std::filesystem::path> ret;
    ret.reserve(selectedFilenames_.size());
    for (auto& s : selectedFilenames_)
        ret.push_back(pwd_ / s);
    return ret;
}

void FileBrowser::ClearSelected()
{
    selectedFilenames_.clear();
    (*inputNameBuf_)[0] = '\0';
    ok_ = false;
}

void FileBrowser::SetTypeFilters(
    const std::vector<const char*>& typeFilters)
{
    typeFilters_ = typeFilters;
    typeFilterIndex_ = 0;
}

void FileBrowser::SetPwdUncatched(const std::filesystem::path& pwd)
{
    fileRecords_ = { FileRecord{ true, "..", "[D] ..", "" } };

    for (auto& p : std::filesystem::directory_iterator(pwd))
    {
        FileRecord rcd;

        if (p.is_regular_file())
            rcd.isDir = false;
        else if (p.is_directory())
            rcd.isDir = true;
        else
            continue;

        rcd.name = p.path().filename();
        if (rcd.name.empty())
            continue;

        rcd.extension = p.path().filename().extension();

        rcd.showName = (rcd.isDir ? "[D] " : "[F] ") +
            u8StrToStr(p.path().filename().u8string());
        fileRecords_.push_back(rcd);
    }

    std::sort(fileRecords_.begin(), fileRecords_.end(),
        [](const FileRecord& L, const FileRecord& R)
        {
            return (L.isDir ^ R.isDir) ? L.isDir : (L.name < R.name);
        });

    pwd_ = absolute(pwd);
    selectedFilenames_.clear();
    (*inputNameBuf_)[0] = '\0';
}

#if defined(__cpp_lib_char8_t)
std::string FileBrowser::u8StrToStr(std::u8string s)
{
    return std::string(s.begin(), s.end());
}
#endif

std::string FileBrowser::u8StrToStr(std::string s)
{
    return s;
}

#ifdef _WIN32

#ifndef _INC_WINDOWS

#ifndef WIN32_LEAN_AND_MEAN

#define IMGUI_FILEBROWSER_UNDEF_WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#endif // #ifndef WIN32_LEAN_AND_MEAN

#include <windows.h>

#ifdef IMGUI_FILEBROWSER_UNDEF_WIN32_LEAN_AND_MEAN
#undef IMGUI_FILEBROWSER_UNDEF_WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif // #ifdef IMGUI_FILEBROWSER_UNDEF_WIN32_LEAN_AND_MEAN

#endif // #ifdef _INC_WINDOWS

std::uint32_t FileBrowser::GetDrivesBitMask()
{
    DWORD mask = GetLogicalDrives();
    uint32_t ret = 0;
    for (int i = 0; i < 26; ++i)
    {
        if (!(mask & (1 << i)))
            continue;
        char rootName[4] = { static_cast<char>('A' + i), ':', '\\', '\0' };
        UINT type = GetDriveTypeA(rootName);
        if (type == DRIVE_REMOVABLE || type == DRIVE_FIXED)
            ret |= (1 << i);
    }
    return ret;
}

#endif
