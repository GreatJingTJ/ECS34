#ifndef CSVREADER_H
#define CSVREADER_H

#include <istream>
#include <string>
#include <vector>
#include <csv.h>
#include<queue>

class CCSVReader{
    protected:
        std::istream &in_;
        struct csv_parser csv_par;
        std::vector<std::string> obj;
        std::queue<std::vector<std::string>> myqueue;

        static void callback1(void*str, size_t length, void *data);
        static void callback2(int num, void *data);
        
    public:
        CCSVReader(std::istream &in);
        ~CCSVReader();
        
        bool End() const;
        bool ReadRow(std::vector< std::string > &row);
};

#endif