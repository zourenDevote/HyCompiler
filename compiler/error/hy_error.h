/**
 * Author:  Devotes
 * Date:    2022/5/18
 */

#ifndef COMPILER_ERROR_H
#define COMPILER_ERROR_H

#include <string>
#include "../common.h"

std::string message_error(const position &pos, const std::string& msg, ErrorCode error_code);
std::string message_warning(const position &pos, const std::string& msg, WarningCode warn_code);

#endif //COMPILER_ERROR_H
