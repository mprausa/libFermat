// vim: set expandtab shiftwidth=4 tabstop=4:

/*
 *  src/Fermat.cpp
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

#include <Fermat.h>
#include <FermatException.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace redi;

Fermat::Fermat(string path, bool verbose) {
    string str;
    serial = 1;

    _path = path;
    this->verbose = verbose;

    strm.open(path,pstream::pstdin|pstream::pstdout);
   
    strm << "&(t=0); &(_t=0); &(_s=0); &(U=1)" << endl << endl; 

    while(getline(strm,str)) {
        if (str == ">") break;
    }
}

Fermat::~Fermat() {
    (*this)("&q");
}

string Fermat::path() {
    return _path;
}

void Fermat::addSymbol(string sym) {
    (*this)("&(J="+sym+")");
}

void Fermat::dropSymbol(string sym) {
    (*this)("&(J=-"+sym+")");
}

string Fermat::getUnique() {
    string str;
    int num = serial++;
    char cstr[2];
    cstr[1] = 0;

    for (int n=0;n<9; ++n) {
        int digit = num%62;
        num /= 62;
        cstr[0] = (char)(digit<10?digit+'0':(digit<36?digit-10+'a':digit-36+'A'));

        str = cstr + str;
    }

    str = "t" + str;

    return str;
}

string Fermat::operator() (string in) {
    bool first=true;

    string out="";
    string str;
    strm << in << endl << endl; 

    if (verbose) cout << "<< " << in << endl;

    while(getline(strm,str)) {
        if (str == "") continue;
        if (first) {
            first = false;
            if (str[0] == '>') {
                str.erase(0,1);
            }
        }
        if (str == ">") break;
        if (verbose) cout << ">> " << str << endl;
        out += str;
    }

    if (out.find("***") != string::npos) {
        if (out.find("entry > 30 or < 5 means turn off mono multiply.") != string::npos) {
            return out;
        }
        
        if (out.find("Converting variables...") != string::npos) {
            return out;
        }

        while(getline(strm,str)) {
            if (str == ">") break;
        }
        
        if (out.find("can't divide by 0.") != string::npos) {
            throw FermatDivByZero("Fermat error:\n"+out+"\ninput was: "+in);
        } else {
            throw FermatException("Fermat error:\n"+out+"\ninput was: "+in);
        }
    }
    return out;
}


