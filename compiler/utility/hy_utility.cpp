/**
* Author:    Devotes
* Date:      2022/5/21
*
* FixLog:
*
*/

#include "hy_utility.h"
#include <regex>

using namespace std;

bool endsWith(const string& src, const string& decl){

    string pattern = ".*" + decl;
    pattern.append("$");

    regex reg(pattern);

    if(regex_match(src, reg))
        return true;

    return false;
}

int strToInt(const string &num){
    int v;
    if(num.size()>2){
        if(num[0] == '0' && (num[1] == 'x' || num[1] == 'X'))
            sscanf(num.c_str(), "%x", &v);
        else if(num[0] == '0')
            sscanf(num.c_str(), "%o", &v);
        else
            sscanf(num.c_str(), "%d", &v);
    }else{
        sscanf(num.c_str(), "%d", &v);
    }
    return v;
}

double strToFloat(const string &num){
    double v;
    sscanf(num.c_str(), "%lf", &v);
    return v;
}



