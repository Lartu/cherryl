#ifndef CHTEXT
#define CHTEXT
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string.h>

using namespace std;

class chText {
    private:
    vector<string> buffer;
    // Gets length of utf8-encoded c++ string
    size_t chText_get_str_utf8length(const string cstr){
        size_t len = cstr.size();
        size_t utf8len = 0;
        unsigned int c;
        for (size_t i = 0; i < len; i++)
        {
            size_t l = 0;
            c = (unsigned char) cstr[i];
            if (c >= 0 && c <= 127) l = 0;
            else if ((c & 0xE0) == 0xC0) l = 1;
            else if ((c & 0xF0) == 0xE0) l = 2;
            else if ((c & 0xF8) == 0xF0) l = 3;
            else if (c>=248 && c<=255) return 0; //invalid utf8
            i += l;
            utf8len++;
        }
        return utf8len;
    }
    // Fills buffer with utf8-encoded c++ string
    void createFromString(const string & cstr){
        buffer.clear();
        size_t cstrlen = cstr.size();
        size_t chPos = 0;
        for(size_t i = 0; i < cstrlen; ++i){
            string ch = "";
            char c = cstr[i];
            if (c >= 0 && c <= 127){
                ch += c;
            }
            else if ((c & 0xE0) == 0xC0){
                ch += c;
                ch += cstr[++i];
            }
            else if ((c & 0xF0) == 0xE0){
                ch += c;
                ch += cstr[++i];
                ch += cstr[++i];
            }
            else if ((c & 0xF8) == 0xF0){
                ch += c;
                ch += cstr[++i];
                ch += cstr[++i];
                ch += cstr[++i];
            }
            buffer.push_back(ch);
            chPos++;
        }
    }
    // Fills buffer with utf8-encoded c++ string
    void createFromChar(const char * cstr){
        buffer.clear();
        size_t cstrlen = strlen(cstr);
        size_t chPos = 0;
        for(size_t i = 0; i < cstrlen; ++i){
            string ch = "";
            char c = cstr[i];
            if (c >= 0 && c <= 127){
                ch += c;
            }
            else if ((c & 0xE0) == 0xC0){
                ch += c;
                ch += cstr[++i];
            }
            else if ((c & 0xF0) == 0xE0){
                ch += c;
                ch += cstr[++i];
                ch += cstr[++i];
            }
            else if ((c & 0xF8) == 0xF0){
                ch += c;
                ch += cstr[++i];
                ch += cstr[++i];
                ch += cstr[++i];
            }
            buffer.push_back(ch);
            chPos++;
        }
    }

    public:
    size_t size(){
        return buffer.size();
    }
    // default constructor
    chText(){}
    // conversion from string (constructor):
    chText (const string& x) {
        createFromString(x);
    }

    // conversion from double (constructor):
    chText (const double& f) {
        std::string str = to_string (f);
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        str.erase ( str.find_last_not_of('.') + 1, std::string::npos );
        createFromString(str);
    }

    // conversion from string (assignment):
    chText& operator= (const string& x) {
        createFromString(x);
        return *this;
    }
    // conversion from char * (constructor):
    chText (const char * x) {
        createFromChar(x);
    }
    // conversion from char * (assignment):
    chText& operator= (const char * x) {
        createFromChar(x);
        return *this;
    }
    // [] for reading
    chText operator [](size_t i) const {
        if(i >= buffer.size()){
            cout << "Out-of-bounds index access." << endl;
            exit(1);
        }
        chText c = buffer[i];
        return c;
    }
    // Output operator
    friend ostream & operator << (ostream &out, const chText &c);
    // + operator
    friend chText operator+(const chText &c1, const chText &c2);
    // + operator
    friend chText operator+(const string &c1, const chText &c2);
    // + operator
    friend chText operator+(const chText &c1, const string &c2);
    // + operator
    friend chText operator+(const char * c1, const chText &c2);
    // + operator
    friend chText operator+(const chText &c1, const char * c2);
    // < operator
    friend bool operator<(const chText &c1, const chText &c2);
    // > operator
    friend bool operator>(const chText &c1, const chText &c2);
    // Equality operator
    friend bool operator==(const chText& ch1, const chText& ch2);
    // Load a file into this chText
    bool loadFile(const string &fileName){
        string line;
        string fileContents = "";
        ifstream myfile (fileName);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                fileContents += line + "\n";
            }
            myfile.close();
        }
        else return false; 
        createFromString(fileContents);
        return true;
    }
    // += operator
    chText & operator += (const chText & txt){
        for(const string & s : txt.buffer){
            buffer.push_back(s);
        }
        return *this;
    }
    // += operator
    chText & operator += (const string & txt){
        chText c2 = txt;
        for(const string & s : c2.buffer){
            buffer.push_back(s);
        }
        return *this;
    }
    // += operator
    chText & operator += (const char * txt){
        chText c2 = txt;
        for(const string & s : c2.buffer){
            buffer.push_back(s);
        }
        return *this;
    }

    bool isAlphanumeric(){
        for(const string & s : buffer){
            for(const char & c : s)
                if(!isalnum(c)) return false;
        }
        return true;
    }

    bool isAlphanumeric(size_t from){
        for(size_t i = from; i < size(); ++i){
            for(const char & c : buffer[i])
                if(!isalnum(c)) return false;
        }
        return true;
    }

    bool isNumber(){
        string number = "";
        for(size_t i = 0; i < size(); ++i){
            number += buffer[i];
        }
        unsigned int firstchar = 0;
        if(number[0] == '-') firstchar = 1;
        if(number[firstchar] == '.') return false; //.12 is not a valid decimal in LDPL, 0.12 is.
        if(number[firstchar] == '+') return false; //+5 is not a valid decimal in LDPL, 5 is.
        istringstream iss(number);
        double f;
        iss >> f;
        bool isNumber = iss.eof() && !iss.fail();
        //If it is a number, it might be an octal literal (e.g. 025, 067, 003 etc)
        //so we proceed to fix the original number to make it decimal.
        if(isNumber){
            string f_number = "";
            unsigned int i;
            for(i = 1; i < number.length(); ++i){
                //If prev char not 0
                if(number[i - 1] != '0'){
                    //if prev char is -, continue check
                    if(number[i - 1] == '-') f_number += '-';
                    //if prev char is number, break
                    else break;
                }
                //If prev number is 0
                else if(number[i] == '.') {
                    f_number += '0';
                }
            }
            f_number += number.substr(i - 1);
            number = f_number;
            return true;
        }else{
            return false;
        }
    }

    double getNumber(){
        string number = "";
        for(size_t i = 0; i < size(); ++i){
            number += buffer[i];
        }
        return stod(number);
    }
};

ostream & operator << (ostream &out, const chText &c){
    for(const string & s : c.buffer){
        out << s;
    }
    return out;
}

chText operator+(const chText &c1, const chText &c2){
    chText res = c1;
	for(const string & s : c2.buffer){
        res.buffer.push_back(s);
    }
    return res;
}

chText operator+(const string &c1, const chText &c2){
    chText res = c1;
	for(const string & s : c2.buffer){
        res.buffer.push_back(s);
    }
    return res;
}

chText operator+(const chText &c1, const string &str){
    chText res = c1;
    chText c2 = str;
	for(const string & s : c2.buffer){
        res.buffer.push_back(s);
    }
    return res;
}

chText operator+(const char * c1, const chText &c2){
    chText res = c1;
	for(const string & s : c2.buffer){
        res.buffer.push_back(s);
    }
    return res;
}

chText operator+(const chText &c1, const char * str){
    chText res = c1;
    chText c2 = str;
	for(const string & s : c2.buffer){
        res.buffer.push_back(s);
    }
    return res;
}

bool operator==(const chText& ch1, const chText& ch2){
    return ch1.buffer == ch2.buffer;
}

bool operator==(const string& c1, const chText& ch2){
    const chText ch1 = c1;
    return ch1 == ch2;
}

bool operator==(const chText& ch1, const string& c2){
    const chText ch2 = c2;
    return ch1 == ch2;
}

bool operator==(const char * c1, const chText& ch2){
    const chText ch1 = c1;
    return ch1 == ch2;
}

bool operator==(const chText& ch1, const char * c2){
    const chText ch2 = c2;
    return ch1 == ch2;
}

bool operator<(const chText &c1, const chText &c2){
	size_t max = c1.buffer.size() > c2.buffer.size() ? c2.buffer.size() : c1.buffer.size();
    for(size_t i = 0; i < max; ++i){
        if(c1.buffer[i] < c2.buffer[i]) return true;
        else if (c1.buffer[i] > c2.buffer[i]) return false;
    }
    return false;
}

bool operator>(const chText &c1, const chText &c2){
	size_t max = c1.buffer.size() > c2.buffer.size() ? c2.buffer.size() : c1.buffer.size();
    for(size_t i = 0; i < max; ++i){
        if(c1.buffer[i] > c2.buffer[i]) return true;
        else if (c1.buffer[i] < c2.buffer[i]) return false;
    }
    return false;
}

#endif
