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

#pragma once

#include "overlay/elements/element.hpp"

#include <string>
#include <functional>

namespace edz::ovl::element {

    class ListItem : public Element {
    public:
        ListItem(std::string text);
        ~ListItem();

        Element* requestFocus(Element *oldFocus, FocusDirection direction) override;

        void draw(ovl::Screen *screen, u16 x, u16 y) override;
        void layout() override;

        bool onClick(s64 key) override;

        void setClickListener(std::function<bool(s64 keysDown)> clickListener) { this->m_clickListener = clickListener; }

    private:
        std::string m_text;
        std::function<bool(s64 keysDown)> m_clickListener = nullptr;
    };

}