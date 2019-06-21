#ifndef XMLREADER_H
#define XMLREADER_H

#include "XMLEntity.h"
#include <istream>
#include <expat.h>
#include <queue>
class CXMLReader{
    private:
        std::istream &is_;
        std::queue<SXMLEntity> myqueue;
        SXMLEntity entity_;
        XML_Parser XMLPar;
        static void callback1(void *data, const XML_Char *name, const XML_Char **atts);
        static void callback2(void *data, const XML_Char *name);
        static void callback3(void *data, const XML_Char *s, int len);
    public:
        CXMLReader(std::istream &is);
        ~CXMLReader();

        bool End();
        bool ReadEntity(SXMLEntity &entity, bool skipcdata = false);
};

#endif

