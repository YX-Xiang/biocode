#ifndef __QUERY_H__
#define __QUERY_H__

#include <string>
#include <iostream>
#include <fstream>

class Query {
    public:
        std::string str;

        Query(const std::string&);
        ~Query();
};

#endif