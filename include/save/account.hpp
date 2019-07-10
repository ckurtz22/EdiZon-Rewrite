#pragma once

#include "edizon.hpp"

namespace edz::save {

    class Account {
    public:
        Account(u128 userID);
        u128 getID();

    private:
        u128 m_userID;
    };

}