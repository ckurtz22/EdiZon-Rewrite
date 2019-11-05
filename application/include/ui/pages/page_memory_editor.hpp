/**
 * Copyright (C) 2019 WerWolv
 * 
 * This file is part of EdiZon.
 * 
 * EdiZon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * EdiZon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with EdiZon.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <edizon.hpp>
#include <borealis.hpp>

#include "cheat/cheat_engine.hpp"

namespace edz::ui {
    class GuiCheatEngine;
}

namespace edz::ui::page {

    class PageMemoryEditor : public brls::View {
    public:
        enum class SettingType {
            DATA_TYPE,
            SEARCH_TYPE,
            MEMORY_REGION,
            VALUE_INPUT,
            DUMP_MEMORY,
            LOADING
        };

        PageMemoryEditor(brls::StagedAppletFrame *stageFrame, GuiCheatEngine *cheatEngineGui, SettingType settingType);
        virtual ~PageMemoryEditor();

        void draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height, brls::Style* style, brls::FrameContext* ctx) override;
        void layout(NVGcontext* vg, brls::Style* style, brls::FontStash* stash) override;
        brls::View* requestFocus(brls::FocusDirection direction, brls::View* oldFocus, bool fromUp = false) override;

        bool onCancel() override;

    private:
        SettingType m_settingType = SettingType::DATA_TYPE;
        GuiCheatEngine *m_cheatEngineGui = nullptr;

        brls::Button *m_confirmButton = nullptr;
        brls::Label *m_description = nullptr;
        brls::List *m_settingList = nullptr;
        brls::ProgressDisplay *m_progressBar = nullptr;

        union {
            u128 u;
            s128 s;
            long double f;
        } m_searchValue;
        
        size_t m_valueSize;
        cheat::DataType m_dataType;
        cheat::CompareFunction m_compareFunc;
        cheat::ReadFunction m_regionFunc;

        u8 m_progress = 0;
        
        void createSelectionList(brls::List *list, std::vector<std::string> options, std::string description, std::function<void(u8)> selectCallback);
    };

}