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
#include <stdexcept>
using namespace std;

static vector<string> parseSymbols(const string &str) {
    vector<string> symbols;
    string sym="";

    for (auto &c : str) {
        if (c == ',') {
            symbols.push_back(sym);
            sym = "";
        } else {
            sym += c;
        }
    }
    symbols.push_back(sym);

    return symbols;
}

// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
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

