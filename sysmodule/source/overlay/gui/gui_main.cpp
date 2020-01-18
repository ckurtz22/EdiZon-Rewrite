/**
 * Copyright (C) 2020 WerWolv
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

#include "overlay/gui/gui_main.hpp"

#include "overlay/gui/gui_cheats.hpp"
#include "overlay/gui/gui_stats.hpp"

#include "overlay/elements/frame.hpp"
#include "overlay/elements/custom_drawer.hpp"
#include "overlay/elements/list.hpp"
#include "overlay/elements/list_item.hpp"

#include "logo_bin.h"

namespace edz::ovl::gui {

    GuiMain::GuiMain() {

    }

    GuiMain::~GuiMain() {

    }


    Element* GuiMain::createUI() {
        auto rootFrame = new element::Frame();

        auto header = new element::CustomDrawer(0, 0, 100, FB_WIDTH, [](u16 x, u16 y, Screen *screen){
            screen->drawRGBA8Image(20, 20, 84, 31, logo_bin);
            screen->drawString("v" VERSION_STRING, false, 108, 51, 15, a(0xFFFF));
        });

        auto list = new element::List();

        auto cheatsItem = new element::ListItem("\uE225   Cheats");
        auto statsItem  = new element::ListItem("\uE202   Stats");
        cheatsItem->setClickListener([](s64 keys) {
            if (keys & KEY_A) {
                Gui::changeTo<GuiCheats>();
                return true;
            }

            return false;
        });

        statsItem->setClickListener([](s64 keys) {
            if (keys & KEY_A) {
                Gui::changeTo<GuiStats>();
                return true;
            }

            return false;
        });

        list->addItem(cheatsItem);
        list->addItem(new element::ListItem("\uE22B   Notes"));
        list->addItem(statsItem);
        list->addItem(new element::ListItem("\uE227   Settings"));

        rootFrame->addElement(header);
        rootFrame->addElement(list);

        return rootFrame;
    }

    void GuiMain::update() {
        
    }

}