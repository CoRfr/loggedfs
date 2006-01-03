/*****************************************************************************
 * Author:   Rémi Flament <remipouak@yahoo.fr>
 *
 *****************************************************************************
 * Copyright (c) 2005, Rémi Flament
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

#include "Config.h"
#include <fstream>
#include <iostream>




xmlChar* INCLUDE=xmlCharStrdup("include");
xmlChar* EXCLUDE=xmlCharStrdup("exclude");
xmlChar* USER=xmlCharStrdup("uid");
xmlChar* EXTENSION=xmlCharStrdup("extension");
xmlChar* ACTION=xmlCharStrdup("action");
xmlChar* ROOT=xmlCharStrdup("loggedFS");
xmlChar* LOG_ENABLED=xmlCharStrdup("logEnabled");


Config::Config()
{
    enabled=true; // default
}

Config::~Config()
{
    includes.clear();
    excludes.clear();
}

void Config::parse(xmlNode * a_node)
{

xmlNode *cur_node = NULL;

for (cur_node = a_node; cur_node; cur_node = cur_node->next) 
	{
	if (cur_node->type == XML_ELEMENT_NODE) 
		{
		xmlAttr *attr=cur_node->properties;
		if (xmlStrcmp(cur_node->name,ROOT)==0)
			{
			while (attr!=NULL)
				{
				if (xmlStrcmp(attr->name,LOG_ENABLED)==0)
					{
					//enable or disable loggedfs
					if (xmlStrcmp(attr->children->content,xmlCharStrdup("true"))!=0)
						{
						enabled=false;
						printf("Log disabled\n");
						}
					else	{
						printf("Log enabled\n");
						}
					}
				else printf("unknown attribute : %s\n",attr->name);
				attr=attr->next;
				}
			}
		if (xmlStrcmp(cur_node->name,INCLUDE)==0 || xmlStrcmp(cur_node->name,EXCLUDE)==0)
			{
			Filter* filter=new Filter();
			char* buffer=new char[100];
			while (attr!=NULL)
				{
				
				sprintf(buffer,"%s",attr->children->content); // I guess there's another way to do that
				if (xmlStrcmp(attr->name,EXTENSION)==0)
					{
					filter->setExtension(buffer);
					printf("Setting extension %s for a loggedfs filter\n",buffer);
					}
				else if (xmlStrcmp(attr->name,USER)==0)
					{
					if (strcmp(buffer,"*"))
						filter->setUID(atoi(buffer));
					else filter->setUID(-1); // every users
					printf("Setting uid %d for a loggedfs filter\n",filter->getUID());
					}
				else if (xmlStrcmp(attr->name,ACTION)==0)
					{
					filter->setAction(buffer);
					printf("Setting action %s for a loggedfs filter\n",buffer);
					}
				else printf("unknown attribute : %s\n",attr->name);
				attr=attr->next;
				}
			
			if (xmlStrcmp(cur_node->name,INCLUDE)==0)
				{
				includes.push_back(*filter);
				}
			else excludes.push_back(*filter);
			delete buffer;
			}		
    		}
		

   	parse(cur_node->children);
	}

}

bool Config::loadFromXml(const char* filename)
{
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;
	
	LIBXML_TEST_VERSION
	
	doc = xmlReadFile(filename, NULL, 0);
	root_element = xmlDocGetRootElement(doc);
	
	parse(root_element);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return true;
}

bool Config::shouldLog(const char* filename, int uid, const char* action)
{
    bool should=false;
    if (enabled)
    {
    	if (includes.size()>0)
	{
		for (unsigned int i=0;i<includes.size() && !should;i++)
		{
		Filter f=includes[i];
		if (f.matches(filename,uid,action))
			should=true;
		}
		for (unsigned int i=0;i<excludes.size() && should;i++)
		{
		Filter f=excludes[i];
		if (f.matches(filename,uid,action))
			should=false;
		}
	}
	else
	{
		should=true;
	}

    }
    

    return should;
}

char* Config::toString()
{

    string buf;

    buf="logEnabled : ";

    if (enabled)
        buf+="true\n";
    else
        buf="false\n";


   /* for (unsigned int i=0;i<includes.size();i++)
    {
        buf+="include "+includes[i]+"\n";
    }

    for (unsigned int i=0;i<excludes.size();i++)
    {
        buf+="exclude "+excludes[i]+"\n";
    }*/


    return strdup(buf.c_str());
}
