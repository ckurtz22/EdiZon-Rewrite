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

#include <cstring>
#include <vector>

namespace edz::cheat::types {

#define STRATEGY(operation) &edz::cheat::types::Value::operator operation
#define INVALID_REGION      edz::cheat::types::Region(0x00, 0);

    class DataType {
    public:
        enum Type : u8 {
        //  Name    Size  Type
            INVALID = 0 | 0x00,
            U8      = 1 | 0x00,
            S8      = 1 | 0x80,
            U16     = 2 | 0x00,
            S16     = 2 | 0x80,
            U32     = 4 | 0x00,
            S32     = 4 | 0x80,
            U64     = 8 | 0x00,
            S64     = 8 | 0x80,
            FLOAT   = 4 | 0x40,
            DOUBLE  = 8 | 0x40,
            STRING  = 0 | 0x20,
            ARRAY   = 0 | 0x10
        };

        DataType(DataType& type) : m_type(type.getType()) { }
        DataType(DataType::Type type) : m_type(type) { }

        u8 getSize()           { return (this->m_type & 0x0F); }

        bool isUnsigned()      { return (this->m_type & 0xF0) == 0x00; }
        bool isSigned()        { return (this->m_type & 0xF0) == 0x80; }
        bool isFloatingPoint() { return (this->m_type & 0xF0) == 0x40; }
        bool isArray()         { return (this->m_type & 0xF0) == 0x10 
                                     || (this->m_type & 0xF0) == 0x20; }

        DataType::Type getType() { return this->m_type; }

    private:
        DataType::Type m_type;
    };

    class SearchOperation {
    public:
        enum Operation : u8 {
        //  Name             ID   Type
            EQUALS          = 0 | 0x00,
            GREATER_THAN    = 1 | 0x00,
            LESS_THAN       = 2 | 0x00,
            BETWEEN         = 3 | 0x00,
            SAME            = 0 | 0x80,
            DIFFERENT       = 1 | 0x80,
            INCREASED       = 2 | 0x80,
            DECREASED       = 3 | 0x80
        };

        SearchOperation() = default;
        constexpr SearchOperation(SearchOperation &operation) : m_operation(operation.getOperation()) { }
        constexpr SearchOperation(SearchOperation::Operation operation) : m_operation(operation) { }

        constexpr SearchOperation::Operation getOperation() { return this->m_operation; }

        constexpr bool isKnown()   { return (this->m_operation & 0x80) == 0x00; }
        constexpr bool isUnknown() { return !isKnown(); }

    private:
        SearchOperation::Operation m_operation;
    };

    class Value {
    private:
        u8 *m_value;
        size_t m_size;
        DataType m_type;
        bool m_copied;
    
    public:
        Value() : m_value(nullptr), m_size(0), m_type(DataType::INVALID), m_copied(false) { }

        Value(void *value, DataType type, bool copy = false) : m_size(type.getSize()), m_type(type), m_copied(copy) {
            if (copy) {
                this->m_value = new u8[type.getSize()];
                
                if (value != nullptr)
                    std::memcpy(this->m_value, value, type.getSize());
            }
            else
                this->m_value = reinterpret_cast<u8*>(value);
        }

        ~Value() {
            if (this->m_value != nullptr && this->m_copied)
                delete[] this->m_value;
        }

        void setValue(void *value) {
            if (this->m_copied) {
                this->m_copied = false;
                
                if (this->m_value != nullptr)
                    delete[] this->m_value;
            }
            
            this->m_value = reinterpret_cast<u8*>(value);
        }

        template<typename T>
        T get() {
            return *reinterpret_cast<T*>(this->m_value);
        }

        size_t getSize() {
            return this->m_size;
        }

        DataType& getType() {
            return this->m_type;
        }

        Value& operator=(Value &&other) {
            if (this->m_copied && this->m_value != nullptr)
                delete[] this->m_value;
            
            this->m_size = other.m_size;
            this->m_type = other.m_type;

            this->m_value = new u8[this->m_size];
            this->m_copied = true;

            if (other.m_value != nullptr)
                std::memcpy(this->m_value, other.m_value, this->m_size);
            else
                std::memset(this->m_value, 0x00, this->m_size);

            return *this;
        }

        bool operator==(Value& other) {
            if (this->m_type.getType() != other.m_type.getType() || this->m_size != other.m_size)
                return false;
            return std::memcmp(this->m_value, other.m_value, this->m_size) == 0;
        }

        bool operator!=(Value& other) {
            return !operator==(other);
        }

        bool operator>(Value& other) {
            if (this->m_type.getType() != other.m_type.getType() || this->m_size != other.m_size)
                return false;

            if (this->m_type.isSigned() || this->m_type.isFloatingPoint()) {

            }
            else if (this->m_type.isUnsigned()) {
                for (u8 i = this->m_size - 1; i > 0; i--)
                    if (reinterpret_cast<u8*>(this->m_value)[i] > reinterpret_cast<u8*>(other.m_value)[i])
                        return true;
            }

            return false;
        }

        bool operator>=(Value& other) {
            if (this->m_type.getType() != other.m_type.getType() || this->m_size != other.m_size)
                return false;

            return operator>(other) || operator==(other);
        }

        bool operator<(Value& other) {
            if (this->m_type.getType() != other.m_type.getType() || this->m_size != other.m_size)
                return false;

            return operator<=(other) && !operator==(other);
        }

        bool operator<=(Value& other) {
            if (this->m_type.getType() != other.m_type.getType() || this->m_size != other.m_size)
                return false;

            return !operator>(other);
        }

    };

    /*template<typename T>
    class Value {
    private:
        T m_value;

    public:
        constexpr Value<T>() : m_value(T()) { }
        constexpr Value<T>(u8 *ptr) : m_value(*reinterpret_cast<T*>(ptr)) { }
        constexpr Value<T>(T value) : m_value(value) { }
        constexpr Value<T>(Value<T>& value) : m_value(value.m_value) { }

        constexpr void operator=(Value& other) {
            this->m_value = other.m_value;
        }

        constexpr void operator=(T& other) {
            this->m_value = other;
        }

        constexpr bool operator==(Value<T>& other) {
            return this->m_value == other.m_value;
        }

        constexpr bool operator!=(Value<T>& other) {
            return this->m_value != other.m_value;
        }

        constexpr bool operator>(Value<T>& other) {
            return this->m_value > other.m_value;
        }

        constexpr bool operator<(Value<T>& other) {
            return this->m_value < other.m_value;
        }
    };*/

    class ByteArray {
    private:
        u8* m_data = nullptr;
        size_t m_size = 0;

    public:
        constexpr ByteArray() {}
        constexpr ByteArray(u8* data, size_t size)  { 
            if (this->m_data != nullptr)
                delete[] this->m_data;
            this->m_data = new u8[size];
            std::memcpy(&this->m_data, data, size);
            this->m_size = size;
        }

        constexpr ByteArray(ByteArray &other) = default;

        constexpr ByteArray(ByteArray &&other) = default;

        constexpr void operator=(ByteArray& other) {
            ByteArray(other.m_data, other.m_size);
        }

        constexpr bool operator==(ByteArray& other) {
            return false;
        }

        constexpr bool operator!=(ByteArray& other) {
            return false;
        }

        constexpr bool operator>(ByteArray& other) {
            return false;
        }

        constexpr bool operator<(ByteArray& other) {
            return false;
        }
    };

    template<typename T>
    static T NOREF = 0;

    class Region {
    public:
        constexpr Region() : m_base(0), m_size(0) { }
        constexpr Region(addr_t base, size_t size) : m_base(base), m_size(size) { }

        constexpr addr_t getBase() const { return this->m_base; }
        constexpr size_t getSize() const { return this->m_size; }

        constexpr bool contains(addr_t address) { return address >= getBase() && address < getBase() + getSize(); }

    private:
        addr_t m_base;
        size_t m_size;
    };

    using Operation = bool(Value::*)(Value&);

    using RegionList = std::vector<Region>;

}