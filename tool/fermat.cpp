// vim: set expandtab shiftwidth=4 tabstop=4:

/*
 *  tool/fermat.cpp
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
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <boost/algorithm/string.hpp>
using namespace std;
using namespace boost;

static vector<string> parseSymbols(const string &str) {
    vector<string> symbols;

    split(symbols,str,is_any_of(","));

    return symbols;
}

static void source(Fermat *fermat, const string &filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        throw invalid_argument("unable to open file.");
    }

    string line;

    while (getline(file,line)) {
        size_t pos = line.find(';');
        if (pos != string::npos) {
            line.erase(pos);
        }
        trim(line);
        if (line == "") continue;

        (*fermat)(line);
    }

    file.close();
}

static void usage(string progname) {
    cerr << "Usage: " << progname << " OPTIONS" << endl << endl;
    cerr << left;

    cerr << "OPTIONS:" << endl;
    cerr << setw(60) << "   --symbols <symbols>"                                     << "Add symbols to fermat. <symbols> should be a comma separted list." << endl;
    cerr << setw(60) << "   --source <filename>"                                     << "Execute fermat commands from <filename>." << endl;
    cerr << setw(60) << "   --array"                                                 << "input is an array." << endl;
    cerr << setw(60) << "   --help"                                                  << "this help text." << endl;
    cerr << endl;

    cerr << "ENVIRONMENT:" << endl;
    cerr << setw(60) << "   FERMAT"                                                  << "Path to fermat executable. (default: fer64)" << endl;
    exit(1);
}

static string input() {
    string res="";
    string line;
    while (getline(cin,line)) {
        res += line;
    }
    return res;
}

int main(int argc, char **argv) {
    string fermatpath="fer64";
    
    if (getenv("FERMAT")) {
        fermatpath = getenv("FERMAT");
    }
    
    Fermat fermat(fermatpath);
    fermat("&(_o=0)");

    bool array = false;

    for (int n=1; n<argc; ++n) {
        if (string(argv[n]) == "--symbols") {
            if (++n == argc) usage(argv[0]);
            vector<string> symbols = parseSymbols(argv[n]);

            for (auto &s : symbols) {
                fermat.addSymbol(s);
            }
        } else if (string(argv[n]) == "--source") {
            if (++n == argc) usage(argv[0]);
            source(&fermat,argv[n]);
        } else if (string(argv[n]) == "--array") {
            array = true;
        } else {
            usage(argv[0]);
        }
    }
    
    string str = input();

    if (array) {
        FermatArray array(&fermat,str);
        cout << array.str() << endl;
    } else {
        FermatExpression expr(&fermat,str);
        cout << expr.str() << endl;
    }

    return 0;
}

