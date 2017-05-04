// vim: set expandtab shiftwidth=4 tabstop=4:

/*
 *  include/Fermat.h
 *
 *  Copyright (C) 2016, 2017 Mario Prausa
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

#ifndef __FERMAT_H
#define __FERMAT_H

#include <string>
#include <lfpstream.h>

class Fermat {
    protected:
        std::string _path;
        redi::pstream strm;
        int serial;
        bool verbose;
    public:
        Fermat(std::string path, bool verbose=false);
        ~Fermat();

        std::string path();

        void addSymbol(std::string sym);
        void dropSymbol(std::string sym);
        std::string getUnique();
        std::string operator() (std::string in);
};

#endif //__FERMAT_H

