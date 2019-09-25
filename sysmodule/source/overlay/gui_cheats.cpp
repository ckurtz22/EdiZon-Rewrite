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

#include "overlay/gui_cheats.hpp"
#include "overlay/styles.h"
#include "cheat/cheat.hpp"
#include <thread>

namespace edz::ovl {
    
    GuiCheats::GuiCheats(Screen *screen) : Gui(screen) {
    }

    GuiCheats::~GuiCheats() {

    }

    void GuiCheats::createUI() {
        lv_obj_t *title = lv_label_create(lv_scr_act(), nullptr);
        //set_background_style(lv_scr_act());
        lv_label_set_text(title, "Cheats");
        lv_label_set_align(title, LV_ALIGN_CENTER);
        lv_obj_set_size(title, 256, 20);

        lv_obj_t *list = lv_list_create(lv_scr_act(), nullptr);
        lv_obj_set_pos(list, 0, 20);
        lv_obj_set_size(list, lv_obj_get_width(lv_scr_act()), lv_obj_get_height(lv_scr_act()));
        lv_obj_set_size(list, 256, 512);
        set_list_styles(list);

        edz::cheat::CheatManager::forceAttach();
        edz::cheat::CheatManager::reload();

        for (auto cheat : edz::cheat::CheatManager::getCheats())
            lv_list_add_btn(list, nullptr, cheat->getName().c_str());
    }

    void GuiCheats::update() {

    }

}