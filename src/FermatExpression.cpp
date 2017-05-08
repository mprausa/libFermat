// vim: set expandtab shiftwidth=4 tabstop=4:

/*
 *  src/FermatExpression.cpp
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

#include <FermatExpression.h>
#include <stdexcept>
#include <sstream>
using namespace std;

FermatExpression::FermatExpression() {
    fermat = NULL;
}

FermatExpression::FermatExpression(const FermatExpression &expr) {
    fermat = expr.fermat;
    _name = fermat->getUnique();

    *this = expr;
}

FermatExpression::FermatExpression(Fermat *fermat) {
	this->fermat = fermat;
	_name = fermat->getUnique();

    *this = 0;
}

FermatExpression::FermatExpression(Fermat *fermat, string expr) {
	this->fermat = fermat;
	_name = fermat->getUnique();

	*this = expr;
}

FermatExpression::FermatExpression(Fermat *fermat, int num) {
	this->fermat = fermat;
	_name = fermat->getUnique();

	*this = num;
}

FermatExpression::~FermatExpression() {
    if (!fermat) return;

	(*fermat)(string("@") + _name);
}

string FermatExpression::str() const {
    if (!fermat) return "<uninitialized>";

	string res = (*fermat)(_name);
    res.erase(remove_if(res.begin(), res.end(), ::isspace), res.end());

    size_t pos = res.find(";or");
    if (pos != string::npos) {
        res.erase(pos);
    }

    return res;
}

string FermatExpression::name() const {
    return _name;
}

Fermat *FermatExpression::fer() const {
    return fermat;
}

FermatExpression &FermatExpression::operator=(const string &expr) {
    if (!fermat) throw invalid_argument("not initialized");

    (*fermat)("&(U=0)");

    try {
	    (*fermat)(_name+":="+expr);
    } catch(...) {
        (*fermat)("&(U=1)");
        throw;
    }        

    (*fermat)("&(U=1)");

	return *this;
}

FermatExpression &FermatExpression::operator=(int num) {
    if (!fermat) throw invalid_argument("not initialized");

    stringstream strm;

    strm.str("");
    strm << _name << ":=" << num;
    (*fermat)(strm.str());

    return *this;
}

FermatExpression &FermatExpression::operator=(const FermatExpression &expr) {
    if (!fermat) {
        if (!expr.fermat) return *this;
        fermat = expr.fermat;
        _name = fermat->getUnique();
    }

    if (!expr.fermat) {
	    (*fermat)(string("@") + _name);
        fermat = NULL;
        return *this;
    }

    (*fermat)(_name+":="+expr._name);
    return *this;
}

FermatExpression FermatExpression::operator+(const FermatExpression &other) const {
    if (!fermat) throw invalid_argument("not initialized");
	
    FermatExpression n(fermat);

	(*fermat)(n._name+":="+_name+"+"+other._name);

	return n;
}

FermatExpression FermatExpression::operator-(const FermatExpression &other) const {
    if (!fermat) throw invalid_argument("not initialized");

	FermatExpression n(fermat);

	(*fermat)(n._name+":="+_name+"-"+other._name);

	return n;
}

FermatExpression FermatExpression::operator*(const FermatExpression &other) const {
    if (!fermat) throw invalid_argument("not initialized");

	FermatExpression n(fermat);

	(*fermat)(n._name+":="+_name+"*"+other._name);

	return n;
}

FermatExpression FermatExpression::operator*(int i) const {
    if (!fermat) throw invalid_argument("not initialized");

	FermatExpression n(fermat);
    stringstream strm;

    strm << n._name << ":=" << _name << "*(" << i << ")";

	(*fermat)(strm.str());

	return n;
}

FermatExpression FermatExpression::operator/(const FermatExpression &other) const {
    if (!fermat) throw invalid_argument("not initialized");

	FermatExpression n(fermat);

	(*fermat)(n._name+":="+_name+"/"+other._name);

	return n;
}

FermatExpression FermatExpression::operator/(int i) const {
    if (!fermat) throw invalid_argument("not initialized");

	FermatExpression n(fermat);
    stringstream strm;

    strm << n._name << ":=" << _name << "/(" << i << ")";

	(*fermat)(strm.str());

	return n;
}

FermatExpression FermatExpression::operator%(uint64_t i) const {
    if (!fermat) throw invalid_argument("not initialized");

	FermatExpression n(fermat);
    stringstream strm;

    strm << n._name << ":=" << _name << "|(" << i << ")";

	(*fermat)(strm.str());

	return n;
}

FermatExpression FermatExpression::operator-() const {
    if (!fermat) throw invalid_argument("not initialized");

    FermatExpression n(fermat);

    (*fermat)(n._name+":= -"+_name);

    return n;
}

bool FermatExpression::operator==(const FermatExpression &other) const {
    if (!fermat) throw invalid_argument("not initialized");

    return (*this - other).str() == "0";
}

bool FermatExpression::operator<(const FermatExpression &other) const {
    return str() < other.str();
}

FermatExpression FermatExpression::numer() const {
    if (!fermat) throw invalid_argument("not initialized");

    FermatExpression n(fermat);

    (*fermat)(n._name+":= Numer("+_name+")");

    return n;
}

FermatExpression FermatExpression::denom() const {
    if (!fermat) throw invalid_argument("not initialized");

    FermatExpression n(fermat);

    (*fermat)(n._name+":= Denom("+_name+")");

    return n;
}

int FermatExpression::deg(string symbol) const {
    if (!fermat) throw invalid_argument("not initialized");

    int n;
    stringstream strm;

    strm.str((*fermat)("Deg("+_name+","+symbol+")"));
    strm >> n;

    return n;
}

FermatExpression FermatExpression::subst(string symbol, const FermatExpression &repl) const {
    if (!fermat) throw invalid_argument("not initialized");

    return FermatExpression(fermat,_name+"#("+symbol+"="+repl._name+")");
}

FermatExpression FermatExpression::subst(string symbol, int i) const {
    if (!fermat) throw invalid_argument("not initialized");
    stringstream strm;

    strm << _name << "#(" << symbol << "=" << i << ")";

    return FermatExpression(fermat,strm.str());
}

FermatExpression FermatExpression::deriv(string symbol, int n) const {
    if (!fermat) throw invalid_argument("not initialized");
    stringstream strm;
    
    strm << "Deriv(" << _name << "," << symbol << "," << n << ")";

    return FermatExpression(fermat,strm.str());
}

