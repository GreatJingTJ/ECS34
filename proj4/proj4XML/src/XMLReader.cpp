#include "XMLReader.h"
#include<iostream>

/*
need to use functions from libexpat from piazza
XML_SetUserData((XML_Parser p, void *userData));
XML_SetElementHandlerXML_Parser p,XML_StartElementHandler start,XML_EndElementHandler end);
XML_SetCharacterDataHandler(void *userData,const XML_Char *s,int len);                        
*/

CXMLReader::CXMLReader(std::istream &is): is_(is)
{
        //taught by Professor Nitta and online information from http://www.vivtek.com/xml/using_expat.html
    XMLPar = XML_ParserCreate(NULL);
    XML_SetUserData(XMLPar, this);
    XML_SetElementHandler(XMLPar, callback1, callback2);
    XML_SetCharacterDataHandler(XMLPar,callback3);
}


void CXMLReader::callback1(void *data, const XML_Char *name, const XML_Char **atts)
{       
        //idea from TA Nick and Professor Nitta
        auto CXML = static_cast<CXMLReader*>(data);

        SXMLEntity entity_;
        entity_.DNameData = std::string(name);
        entity_.DType = SXMLEntity::EType::StartElement;
        int i = 0;
        while(atts[i])
        {
                entity_.SetAttribute(std::string(atts[i]), std::string(atts[i+1]));
                i += 2;
//        	std::cout<<"In "<<__FILE__<<" @ "<<__LINE__<<std::endl;
            }

        CXML->myqueue.push(entity_);

}
void CXMLReader::callback2(void *data, const XML_Char *name)
{   
    //idea from TA Nick and Professor Nitta
        auto CXML = static_cast<CXMLReader*>(data);

        SXMLEntity entity_;

        entity_.DType = SXMLEntity::EType::EndElement;
        entity_.DNameData = std::string(name);
//	std::cout<<"In "<<__FILE__<<" @ "<<__LINE__<<std::endl;
        CXML->myqueue.push(entity_);
}

void CXMLReader::callback3(void *data, const XML_Char *s, int len)
{   
    //idea from TA Nick and Professor Nitta
        auto CXML = static_cast<CXMLReader*>(data);

        SXMLEntity entity_;

        entity_.DType = SXMLEntity::EType::CharData;

        entity_.DNameData = std::string(s,len);
//	std::cout<<"In "<<__FILE__<<" @ "<<__LINE__<<std::endl;
        CXML->myqueue.push(entity_);
}

CXMLReader::~CXMLReader()
{
//online information from http://www.vivtek.com/xml/using_expat.html    
    XML_ParserFree(XMLPar);
}

bool CXMLReader::End()
{   
    //idea from TA Nick and Professor Nitta
    bool flag = false;
    if(myqueue.empty() && XMLPar)
        flag = true;
    return flag;
}
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata)
{
        //XML_SetElementHandler(XMLPar, 0, is_.gcount());

        char buffer[1000];
        bool flag = true;
        SXMLEntity entity_;
    // idea from Professor Nitta and TA Nick
	while(!myqueue.empty() && myqueue.front().DType ==SXMLEntity::EType::CharData && skipcdata)
		myqueue.pop();
	
	while(!is_.eof() && myqueue.empty())
        {
                is_.read(buffer,1000);
                XML_Parse(XMLPar, buffer, is_.gcount(), is_.eof());

        	while(!myqueue.empty() && myqueue.front().DType ==SXMLEntity::EType::CharData && skipcdata)
               		 myqueue.pop();
	}
	if(myqueue.empty() == false){

		entity = myqueue.front();
		myqueue.pop();
	}
	return flag;

        // idea from TA Nick
}


