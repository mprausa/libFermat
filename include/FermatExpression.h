// vim: set expandtab shiftwidth=4 tabstop=4:

/*
 *  include/FermatExpression.h
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

#ifndef __FERMAT_EXPRESSION_H
#define __FERMAT_EXPRESSION_H

#include <Fermat.h>
#include <string>

class FermatExpression {
    protected:
        Fermat *fermat;
        std::string _name;
    public:
        FermatExpression();
        FermatExpression(const FermatExpression &expr);
        FermatExpression(Fermat *fermat);
        FermatExpression(Fermat *fermat, std::string expr);
        FermatExpression(Fermat *fermat, int num);
        ~FermatExpression();
       
        std::string str() const;
        std::string name() const;
        Fermat *fer() const;

        FermatExpression &operator=(const std::string &expr);
        FermatExpression &operator=(int num);
        FermatExpression &operator=(const FermatExpression &expr);

        FermatExpression operator+(const FermatExpression &other) const;
        FermatExpression operator-(const FermatExpression &other) const;
        FermatExpression operator*(const FermatExpression &other) const;
        FermatExpression operator*(int i) const;
        FermatExpression operator/(const FermatExpression &other) const;
        FermatExpression operator/(int i) const;
        FermatExpression operator%(uint64_t i) const;
        FermatExpression operator-() const;

        bool operator==(const FermatExpression &other) const;
        bool operator<(const FermatExpression &other) const;

        FermatExpression numer() const;
        FermatExpression denom() const;
        int deg(std::string symbol) const;

        FermatExpression subst(std::string symbol, const FermatExpression &repl) const;
        FermatExpression subst(std::string symbol, int i) const;
        FermatExpression deriv(std::string symbol, int n) const;
};

#endif //__FERMAT_EXPRESSION_H

