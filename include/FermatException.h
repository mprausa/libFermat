// vim: set expandtab shiftwidth=4 tabstop=4:

/*
 *  include/FermatException.h
 *
 *  Copyright (C) 2016 Mario Prausa
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as 
 *  published by the Free Software Foundation, either version 3 of the 
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __FERMAT_EXCEPTION_H
#define __FERMAT_EXCEPTION_H

#include <exception>

class FermatException : public std::exception {
    protected:
        std::string msg;
    public:
        FermatException(const std::string &msg) {
            this->msg = msg;
        }

        virtual const char *what() const noexcept {
            return msg.c_str();
        }
};

class FermatDivByZero : public FermatException {
    public:
        FermatDivByZero(const std::string &msg) : FermatException(msg) {}
};

#endif //__FERMAT_EXCEPTION_H

