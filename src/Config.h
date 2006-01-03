/*****************************************************************************
 * Author:   R�mi Flament <remipouak@yahoo.fr>
 *
 *****************************************************************************
 * Copyright (c) 2005, R�mi Flament
 *
 * This library is free software; you can distribute it and/or modify it under
 * the terms of the GNU General Public License (GPL), as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GPL in the file COPYING for more
 * details.
 *
 */

#ifndef LOGGEDFS_CONFIG_H
#define LOGGEDFS_CONFIG_H

using namespace std;
#include <vector>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "Filter.h"

class Config
{
public:
    Config();
    ~Config();

    bool load(const char *fileName);
    bool loadFromXml(const char *fileName);
    bool isEnabled() {return enabled;};
    bool shouldLog(const char* filename, int uid, const char* action);
    char* toString();


private:
    void parse(xmlNode*);
    std::vector<Filter> includes;
    std::vector<Filter> excludes;
    bool enabled;
};

#endif