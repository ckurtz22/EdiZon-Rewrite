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

#include "ui/gui_splash.hpp"
#include "ui/pages/page_splash.hpp"
#include "ui/gui_main.hpp"

namespace edz::ui {

    brls::View* GuiSplash::setupUI() {
        bool isAMSVersionSupported = hlp::getAtmosphereVersion() >= MINIMUM_AMS_VERSION_REQUIRED;
        
        if (isAMSVersionSupported)
            Gui::runLater([this] { Gui::changeTo<GuiMain>(); }, 20);

        return new ui::page::PageSplash(!isAMSVersionSupported);
    }

    void GuiSplash::update() {

    }

}