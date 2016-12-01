// vim: set expandtab shiftwidth=4 tabstop=4:


/*
 *  include/FermatArray.h
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

#ifndef __FERMAT_ARRAY_H
#define __FERMAT_ARRAY_H

#include <Fermat.h>
#include <FermatExpression.h>
#include <string>

class FermatArray {
	protected:
        Fermat *fermat;
        std::string _name;
        int r;
        int c;
    public:
        FermatArray();
        FermatArray(const FermatArray &array);
        FermatArray(Fermat *fermat);
        FermatArray(Fermat *fermat, int n);
        FermatArray(Fermat *fermat, int r, int c, bool sparse=false);
        FermatArray(Fermat *fermat, std::string str);
        FermatArray(const FermatArray &array, int rfrom, int rto, int cfrom, int cto);
        FermatArray(const FermatArray &mat1, const FermatArray &mat2);
        FermatArray(const FermatArray &mat1, const FermatArray &mat2, const FermatArray &mat3);

        virtual ~FermatArray();

        void set(int r, int c, const FermatExpression &expr);
        void set(int n, const FermatExpression &expr);
        void set(int r, int c, std::string expr);
        void set(int n, std::string expr);
        void set(int r, int c, int i);

        void setColumn(int c, const FermatArray &v);
        void setRow(int r, const FermatArray &v);
        
        FermatArray concatenate(const FermatArray &array) const;
        FermatArray transpose() const;
        FermatArray inverse() const;

        int rank();
        int rowEchelon();
        int colReduce(FermatArray &A, FermatArray &B);

        std::string name() const;
        Fermat *fer() const;

        int rows() const;
        int cols() const;
        
        void assign(std::string str);
        void drop();

        FermatExpression operator()(int r, int c) const;
        FermatExpression operator()(int n) const;

        FermatArray operator+(const FermatArray &other) const;
        FermatArray operator-(const FermatArray &other) const;
        FermatArray operator*(const FermatArray &other) const;
        FermatArray operator*(const FermatExpression &expr) const;
        FermatArray operator/(const FermatExpression &expr) const;

        FermatArray operator*(int i) const;
        FermatArray operator-() const;

        FermatArray &operator=(const FermatArray &array);
        FermatArray &operator*=(int i);
        FermatArray &operator+=(const FermatArray &array);
        FermatArray &operator-=(const FermatArray &array);

        FermatExpression det() const;
        FermatExpression chPoly() const;
        bool isZero() const;
        FermatArray subst(std::string symbol, int i) const;
        FermatArray subst(std::string symbol, const FermatExpression &ex) const;

        virtual std::string str() const;
        virtual std::string sstr() const;
};

#endif //__FERMAT_ARRAY_H

