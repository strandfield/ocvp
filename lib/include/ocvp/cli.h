// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef CLI_H
#define CLI_H

#include <string>

namespace ocvp
{

namespace cli
{

inline bool is_help(const std::string& str)
{
    return str == "--help" || str == "-h";
}

} // namespace cli

} // namespace ocvp

#endif // CLI_H
