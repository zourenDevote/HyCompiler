/**
 * Author:  Devotes
 * Date:    2022/5/21
 *
 * OverView:
 *      Utility module give some utility function to help coding
 *      convenient.
 *
 * FixLog:
 *
 */

#ifndef COMPILER_HY_UTILITY_H
#define COMPILER_HY_UTILITY_H

#include<string>

/// A function like python's str endswith
/// Case:
///      string a = 1.v;
///      if(endsWith(a, ".v")){
///         printf("a is an verilog file\n");
///     }

bool endsWith(const std::string& src, const std::string& decl);

/// str转换为int值，包括16进制，8进制，10进制
int strToInt(const std::string &num);

/// str转换为double值，包括16进制，8进制，10进制, 科学计数法
double strToFloat(const std::string &num);

#endif //COMPILER_HY_UTILITY_H
