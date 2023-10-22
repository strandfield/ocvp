// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'ocv-playground' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBTYPESET_DEFS_H
#define LIBTYPESET_DEFS_H

#if (defined(WIN32) || defined(_WIN32)) && !defined(PLAYGROUND_STATIC_LINKING)

#if defined(PLAYGROUND_LIBRARY_BUILD)
#define PLAYGROUND_API __declspec(dllexport)
#else
#define PLAYGROUND_API __declspec(dllimport)
#endif

#else

#define PLAYGROUND_API

#endif

#endif // LIBTYPESET_DEFS_H
