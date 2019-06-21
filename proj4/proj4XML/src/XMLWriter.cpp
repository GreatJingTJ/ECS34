#include "XMLWriter.h"
#include<iostream>

CXMLWriter::CXMLWriter(std::ostream &os) : os_(os)
{

}

CXMLWriter::~CXMLWriter()
{

}

bool CXMLWriter::Flush()
{
        //idea from TA Nick and Professor Nitta
        SXMLEntity entity;
        bool flag = false;
        while(!mystack.empty())
        {
                entity = mystack.top();
                std::string str = "</" + entity.DNameData;
                str += entity.DNameData;
                str +=  ">";
                os_ << str;
                mystack.pop();
                flag = true;
        }

        return flag;
}

bool CXMLWriter::WriteEntity(const SXMLEntity &entity)
{
        bool flag = true;

        //idea from TA Nick and Professor Nitta
        if(entity.DType == SXMLEntity::EType::StartElement or entity.DType == SXMLEntity::EType::CompleteElement)
        {
                std::string str = "<" + entity.DNameData;
		
		for(auto temp : entity.DAttributes)
		{
			str += " " + temp.first + "=" +  '"';
			str += temp.second + '"';

		}

                if(entity.DType == SXMLEntity::EType::StartElement)
                {
                        str += ">";
                        mystack.push(entity);
                }
                else
                        str += "/>";
                os_ << str;
        }
        else if(entity.DType == SXMLEntity::EType::EndElement)
        {
                std::string str = "</" + entity.DNameData;
                str +=  ">";
		if(mystack.top().DType == SXMLEntity::EType::EndElement)
			mystack.pop();
		
                os_ << str;
        }
        else if(entity.DType == SXMLEntity::EType::CharData)
        {
                os_ << entity.DNameData;
        }
        else
                flag = false;


        return flag;
}

