// vim: set expandtab shiftwidth=4 tabstop=4:

/*
 *  src/FermatArray.cpp
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

#include <FermatArray.h>
#include <FermatException.h>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <boost/range/algorithm/remove_if.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/utility.hpp>
using namespace std;
using namespace boost;

FermatArray::FermatArray() {
    fermat = NULL;
    r=c=0;
}

FermatArray::FermatArray(const FermatArray &array) {
    fermat = NULL;
    *this = array;
}

FermatArray::FermatArray(Fermat *fermat) {
    this->fermat = fermat;
    _name = fermat->getUnique();
    r=c=0;
}

FermatArray::FermatArray(Fermat *fermat, int n) {
	stringstream strm;

	this->fermat = fermat;
	_name = fermat->getUnique();

	r = n;
	c = -1;
	
	strm << "Array " << _name << "[" << n << "]";
    
    (*fermat)("&U");    //workaround
	(*fermat)(strm.str());
    (*fermat)("&U");    //workaround
	(*fermat)(string("[")+_name+"]");
}

FermatArray::FermatArray(Fermat *fermat, int r, int c, bool sparse) {
    if (r<=0 || c<=0) {
        this->fermat = NULL;
        this->r = this->c = 0;
        return;
    }

	stringstream strm;

	this->fermat = fermat;
	_name = fermat->getUnique();

	this->r = r;
	this->c = c;
	
	strm << "Array " << _name << "[" << r << "," << c << "]";
    if (sparse) {
        strm << " Sparse";
    }

    (*fermat)("&U");    //workaround
	(*fermat)(strm.str());
    (*fermat)("&U");    //workaround
	if (!sparse) {
        (*fermat)(string("[")+_name+"]");
    }
}

FermatArray::FermatArray(Fermat *fermat, std::string str) {
	stringstream strm;
    string tmp;

	this->fermat = fermat;
	_name = fermat->getUnique();

    r = count(str.begin(),str.end(),'{')-1;
    c = (count(str.begin(),str.end(),',')+1)/r;
    
    str.erase(remove_if(str, is_any_of("{} \t")), str.end());

    tmp = fermat->getUnique();
    
    strm << "Array " << tmp << "[" << c << "," << r << "]";

    (*fermat)("&U");    //workaround
    (*fermat)(strm.str());
    (*fermat)("&U");    //workaround
    (*fermat)(string("[")+tmp+"] := [["+str+"]]");
    (*fermat)(string("[")+_name+"] := Trans["+tmp+"]");
    (*fermat)(string("@[")+tmp+"]");
}

FermatArray::FermatArray(const FermatArray &array, int rfrom, int rto, int cfrom, int cto) {
    if (rto < rfrom || cto < cfrom) {
        fermat = NULL;
        r=c=0;
        return;
    }

    fermat = array.fermat;
    _name = fermat->getUnique();

    stringstream strm;

    r = rto-rfrom+1;
    c = cto-cfrom+1;

    strm << "[" << _name << "] := [" << array._name << "[" << rfrom << "~" << rto << "," << cfrom << "~" << cto << "]]";

    (*fermat)(strm.str());
}

FermatArray::FermatArray(const FermatArray &mat1, const FermatArray &mat2) {
    fermat = mat1.fermat;
    _name = fermat->getUnique();
    
    r = mat1.r;
    c = mat2.c;

    (*fermat)(string("[")+_name+"] := ["+mat1._name+"]*["+mat2._name+"]");
}

FermatArray::FermatArray(const FermatArray &mat1, const FermatArray &mat2, const FermatArray &mat3) {
    fermat = mat1.fermat;
    _name = fermat->getUnique();
    
    r = mat1.r;
    c = mat3.c;

    (*fermat)(string("[")+_name+"] := ["+mat1._name+"]*["+mat2._name+"]*["+mat3._name+"]");
}

FermatArray::~FermatArray() {
    if (!fermat) return;

	(*fermat)(string("@[")+_name+"]");
}

string FermatArray::name() const {
    return _name;
}

Fermat *FermatArray::fer() const {
    return fermat;
}

void FermatArray::assign(string str) {
    if (!fermat) throw invalid_argument("not initialized");

    stringstream strm;

    (*fermat)("["+_name+"]:="+str);

    strm.str((*fermat)("Cols["+_name+"]"));
    strm >> c;

    strm.clear();
    strm.str((*fermat)("Deg["+_name+"]"));
    strm >> r;
    
    r /= c;
}
    
void FermatArray::drop() {
    if (!fermat) throw invalid_argument("not initialized");
   if (r) (*fermat)("@["+_name+"]");
   r = c = 0;
}

void FermatArray::set(int r, int c, const FermatExpression &expr) {
    if (!fermat) throw invalid_argument("not initialized");

    stringstream strm;

    strm << _name << "[" << r << "," << c << "]:=" << expr.name();

    (*fermat)(strm.str());
}

void FermatArray::set(int r, int c, int i) {
    if (!fermat) throw invalid_argument("not initialized");

    stringstream strm;

    strm << _name << "[" << r << "," << c << "]:=" << i;

    (*fermat)(strm.str());
}

void FermatArray::set(int n, const FermatExpression &expr) {
    if (!fermat) throw invalid_argument("not initialized");

    stringstream strm;

    strm << _name << "[" << n << "]:=" << expr.name();

    (*fermat)(strm.str());
}

void FermatArray::set(int r, int c, string expr) {
    if (!fermat) throw invalid_argument("not initialized");

    stringstream strm;

    strm << _name << "[" << r << "," << c << "]:=" << expr;

    (*fermat)(strm.str());
}

void FermatArray::set(int n, string expr) {
    if (!fermat) throw invalid_argument("not initialized");

    stringstream strm;

    strm << _name << "[" << n << "]:=" << expr;

    (*fermat)(strm.str());
}
        
void FermatArray::setColumn(int c, const FermatArray &v) {
    if (!fermat) throw invalid_argument("not initialized");

    stringstream strm;
    
    strm << "[" << _name << "[1~" << r << "," << c << "]] := [" << v._name << "]";

    (*fermat)(strm.str());
}

void FermatArray::setRow(int r, const FermatArray &v) {
    if (!fermat) throw invalid_argument("not initialized");

    stringstream strm;
    
    strm << "[" << _name << "[" << r << ",1~" << c << "]] := ";
    if (v.r != 1) {
        strm << "Trans[" << v._name << "]";
    } else {
        strm << "[" << v._name << "]";
    } 

    (*fermat)(strm.str());
}

FermatArray FermatArray::concatenate(const FermatArray &array) const {
    if (!fermat) {
        FermatArray n(array);
        return n;
    }

    FermatArray n(fermat);
    
    n.assign("["+_name+"]_["+array._name+"]");

    if (r == 1 && c == 1 && array.r == 1 && array.c == 1 && n.r == 1 && n.c == 2) { //workaround
        n = n.transpose();
    }

    return n;
}

FermatArray FermatArray::transpose() const {
    if (!fermat) throw invalid_argument("not initialized");

    FermatArray n(fermat);

    n.assign("Trans["+_name+"]");
    return n;
}

FermatArray FermatArray::inverse() const {
    if (!fermat) throw invalid_argument("not initialized");

    FermatArray n(fermat);

    n.assign("1/["+_name+"]");
    return n;
}

int FermatArray::rank() {
    int rk;
    stringstream strm;
    string tmp = fermat->getUnique();

    (*fermat)("["+tmp+"] := ["+_name+"]");
    strm.str((*fermat)("Colreduce(["+tmp+"])"));
    strm >> rk;
    (*fermat)("@["+tmp+"]");

    return rk;
}

int FermatArray::rowEchelon() {
    int rk;
    stringstream strm;

    strm.str((*fermat)("Redrowech(["+_name+"])"));
    strm >> rk;

    return rk;
}

int FermatArray::colReduce(FermatArray &A, FermatArray &B) {
    stringstream strm;
    int rk;

    A = FermatArray(fermat,r,r);
    B = FermatArray(fermat,c,c);

    strm.str((*fermat)("Colreduce(["+_name+"],["+A._name+"],["+B._name+"])"));
    strm >> rk;

    return rk;
}

int FermatArray::rows() const {
    return r;
}

int FermatArray::cols() const {
    return c;
}


FermatExpression FermatArray::operator()(int r, int c) const {
    if (!fermat) throw invalid_argument("not initialized");

    FermatExpression nn(fermat);
    stringstream strm;

    strm << nn.name() << ":=" << _name << "[" << r << "," << c << "]";

    (*fermat)(strm.str());

    return nn;
}

FermatExpression FermatArray::operator()(int n) const {
    if (!fermat) throw invalid_argument("not initialized");

    FermatExpression nn(fermat);
    stringstream strm;

    strm << nn.name() << ":=" << _name << "[" << n << "]";

    (*fermat)(strm.str());

    return nn;
}
        
FermatArray FermatArray::operator+(const FermatArray &other) const {
    if (!fermat) return other;
    if (!other.fermat) return *this;

    FermatArray n(fermat);

    n.assign(string("[")+_name+"]+["+other._name+"]");

    return n;
}

FermatArray FermatArray::operator-(const FermatArray &other) const {
    if (!fermat) return -other;
    if (!other.fermat) return *this;
    FermatArray n(fermat);

    n.assign(string("[")+_name+"]-["+other._name+"]");

    return n;
}

FermatArray FermatArray::operator*(const FermatArray &other) const {
    if (!fermat || !other.fermat) return FermatArray();

    FermatArray n(fermat);

    n.assign(string("[")+_name+"]*["+other._name+"]");

    return n;
}

FermatArray FermatArray::operator*(const FermatExpression &expr) const {
    if (!fermat) return FermatArray();

    FermatArray n(fermat);

    n.assign(string("[")+_name+"]*"+expr.name());

    return n;
}

FermatArray FermatArray::operator/(const FermatExpression &expr) const {
    if (!fermat) return FermatArray();

    FermatArray n(fermat);

    n.assign(string("[")+_name+"]/"+expr.name());

    return n;
}

FermatArray FermatArray::operator*(int i) const {
    if (!fermat) return FermatArray();

    FermatArray n(fermat);
    stringstream strm;

    strm << "[" << _name << "]*(" << i << ")";

    n.assign(strm.str());

    return n;
}


FermatArray FermatArray::operator-() const {
    if (!fermat) return FermatArray();

    FermatArray n(fermat);

    n.assign("-["+_name+"]");

    return n;
}

FermatArray &FermatArray::operator=(const FermatArray &array) {
    if (!array.fermat) {
        if (fermat) (*fermat)(string("@[")+_name+"]");
        fermat = NULL;
        r = c = 0;
        return *this;
    }
    if (!fermat) {
        stringstream strm;

        fermat = array.fermat;
        _name = fermat->getUnique();
    } else if (r && c && (r != array.r || c != array.c)) {
        (*fermat)(string("@[")+_name+"]");
    }
        
    r = array.r;
    c = array.c;

    (*fermat)(string("[")+_name+"]:=["+array._name+"]");
    return *this;
}

FermatArray &FermatArray::operator*=(int i) {
    if (!fermat) return *this;
   
    stringstream strm;

    strm << "[" << _name << "] := [" << _name << "]*(" << i << ")";

    (*fermat)(strm.str());

    return *this;
}

FermatArray &FermatArray::operator+=(const FermatArray &array) {
    if (!array.fermat) return *this;
    if (!fermat) {
        *this = array;
        return *this;
    }

    if (r != array.r || c != array.c) throw invalid_argument("matrices not compatible.");

    (*fermat)(string("[")+_name+"]:=["+_name+"]+["+array._name+"]");

    return *this;
}

FermatArray &FermatArray::operator-=(const FermatArray &array) {
    if (!array.fermat) return *this;
    if (!fermat) {
        *this = array;
        return *this;
    }

    if (r != array.r || c != array.c) throw invalid_argument("matrices not compatible.");

    (*fermat)(string("[")+_name+"]:=["+_name+"]-["+array._name+"]");

    return *this;
}

FermatExpression FermatArray::det() const {
    if (!fermat) throw invalid_argument("not initialized");

    FermatExpression expr(fermat);

    (*fermat)(expr.name()+":=Det["+_name+"]");

    return expr;
}

FermatExpression FermatArray::chPoly() const {
    if (!fermat) throw invalid_argument("not initialized");

    FermatExpression expr(fermat);

    (*fermat)(expr.name()+":=Chpoly(["+_name+"])");

    return expr;
}

bool FermatArray::isZero() const {
    if (!fermat) return true;

    string res;

    res = (*fermat)(string("Iszero[")+_name+"]");

    res.erase(remove_if(res, ::isspace), res.end());
   
    return res == "1";
}

FermatArray FermatArray::subst(string symbol, int i) const {
    if (!fermat) throw invalid_argument("not initialized");
    
    FermatArray n(fermat);
    stringstream strm;

    strm << "[" << _name << "]#(" << symbol << "=" << i << ")";

    try {
        n.assign(strm.str());
    } catch(const FermatException &e) {
        n.fermat = NULL;
        throw;
    }

    return n;
}

FermatArray FermatArray::subst(string symbol, const FermatExpression &ex) const {
    if (!fermat) throw invalid_argument("not initialized");
    
    FermatArray n(fermat);

    try {
        n.assign("["+_name+"]#("+symbol+"="+ex.name()+")");
    } catch(const FermatException &e) {
        n.fermat = NULL;
        throw;
    }

    return n;
}

string FermatArray::str() const {
    if (!fermat) return "<uninitialized>";

    string str = (*fermat)(string("[")+_name+"]");

    str.erase(remove_if(str, ::isspace), str.end());
    str.erase(prior(str.end()));

    replace_all(str,"[","{");
    replace_all(str,"]","}");

    return str;
}

string FermatArray::sstr() const {
    if (!fermat) return "<uninitialized>";

    string str = (*fermat)(string("![")+_name+"]");
    
    str.erase(remove_if(str, ::isspace), str.end());
    str.erase(prior(str.end()));

    str.erase(0,str.find(":=")+2);
    
    replace_all(str,"][","],[");
    return str;  
}
   
    
