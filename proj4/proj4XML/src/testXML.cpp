#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "CSVReader.h"
#include "CSVWriter.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include "../include/XMLEntity.h"
void ReadTest(std::vector<std::string>name, std::vector<std::string> data, int times, int ele_status, std::string str, std::string DnameData, bool skip = false, bool end = false);
void ReadTest(std::vector<std::string>name, std::vector<std::string> data, int times, int ele_status, std::string str, std::string DnameData, bool skip, bool end)
{
	std::stringstream  ss(str);
	CXMLReader obj(ss);

	SXMLEntity entity;

	for(int i = 1; i <= times; i ++)
		EXPECT_TRUE(obj.ReadEntity(entity,skip));

	if( ele_status == 1)
    {
		EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
        EXPECT_EQ(entity.DNameData,DnameData);

    }
	else if(ele_status == 2)
    {
		EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
        EXPECT_EQ(entity.DNameData,DnameData);
    }
	else if(ele_status == 3)
    {
		EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
        EXPECT_EQ(entity.DNameData,DnameData);
    }
    else
    {   
        std::cout << "Wrong ele_status" << std::endl;
        exit(1);
    
    }


	for(unsigned int i = 0; i < name.size(); i++)
	{
		
		EXPECT_EQ(entity.AttributeValue(name[i]),data[i]);
       
	}

	if(!end)
		EXPECT_EQ(obj.End(),false);
	else
		EXPECT_EQ(obj.End(),true);
}
void clearVec(std::vector<std::string> &a1,std::vector<std::string> &a11)
{
    a1.clear(); a11.clear();
}
TEST(XMLTest, TEST)
{
	std::string ss = "<course SUBJ=\"ABC\" CRSE=\"114\" SEC=\"2\">\n""    <student SEAT=\"1\" SID=\"409337710\" SURNAME=\"White\" PREFNAME=\"Victoria\" LEVEL=\"GR\" UNITS=\"12\" CLASS=\"D1\" MAJOR=\"ABCG\" GRADE=\"S\" STATUS=\"RE\" EMAIL=\"victoria.b.white@fakeu.edu\"></student>""</course>";

	//EXPECT_TRUE(obj.ReadEntity(entity));
	std::vector<std::string> name,data,empty;
    name = {"SUBJ", "CRSE", "SEC"};
    data = {"ABC", "114", "2"};
	ReadTest(name, data,1,1,ss, "course");
    
    //ReadTest(name, data,2,2,ss, "\n");
	clearVec(name,data);
	ReadTest(name, data,3,2,ss, "    ");


	name = {"SEAT", "SID", "SURNAME", "PREFNAME", "LEVEL", "UNITS", "CLASS", "MAJOR", "GRADE", "STATUS", "EMAIL"};
	data = {"1", "409337710", "White", "Victoria", "GR", "12", "D1", "ABCG", "S", "RE", "victoria.b.white@fakeu.edu"};

	ReadTest(name, data,4,1,ss, "student");
    clearVec(name,data);
    ReadTest(name, data, 5,3,ss, "student");

    name = {"SEAT", "SID", "STATUS", "EMAIL"};
    data = {"1", "409337710", "RE", "EMAIL", "victoria.b.white@fakeu.edu"};

    clearVec(name,data);
    ReadTest(name, data, 6,3,ss, "course",false,true);
}

TEST(XMLTest, TEST2)
{
    std::string ss = "<course SUBJ=\"ABC\" CRSE=\"204\" SEC=\"2\">\n"
    "<student SEAT=\"1\" SID=\"679358754\" SURNAME=\"Rose\" PREFNAME=\"Sarah\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O268\" GRADE=\"C+\" STATUS=\"RE\" EMAIL=\"sarah.o.rose@fakeu.edu\"/>\n"
    "<student SEAT=\"2\" SID=\"493845023\" SURNAME=\"Freeman\" PREFNAME=\"Alexander\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O239\" GRADE=\"A+\" STATUS=\"RE\" EMAIL=\"alexander.b.freeman@fakeu.edu\"/>\n"
    "<student SEAT=\"3\" SID=\"992212797\" SURNAME=\"Keller\" PREFNAME=\"Olivia\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"OT35\" GRADE=\"C+\" STATUS=\"RE\" EMAIL=\"olivia.v.keller@fakeu.edu\"/>\n"
    "<student SEAT=\"4\" SID=\"540708735\" SURNAME=\"Daniel\" PREFNAME=\"Noah\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"ABC1\" GRADE=\"B-\" STATUS=\"RE\" EMAIL=\"noah.a.daniel@fakeu.edu\"/>\n"
    "<student SEAT=\"5\" SID=\"411409681\" SURNAME=\"O'brien\" PREFNAME=\"Mason\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O224\" GRADE=\"C+\" STATUS=\"RE\" EMAIL=\"mason.z.o'brien@fakeu.edu\"/>\n"
    "<student SEAT=\"6\" SID=\"196501315\" SURNAME=\"Scott\" PREFNAME=\"Brianna\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"ABC1\" GRADE=\"A-\" STATUS=\"RE\" EMAIL=\"brianna.a.scott@fakeu.edu\"/>\n"
    "<student SEAT=\"7\" SID=\"288442650\" SURNAME=\"Kidd\" PREFNAME=\"Aaliyah\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"ABC2\" GRADE=\"C\" STATUS=\"RE\" EMAIL=\"aaliyah.a.kidd@fakeu.edu\"/>\n"
    "<student SEAT=\"8\" SID=\"721595509\" SURNAME=\"Wood\" PREFNAME=\"Sofia\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O240\" GRADE=\"C+\" STATUS=\"RE\" EMAIL=\"sofia.a.wood@fakeu.edu\"/>\n"
    "<student SEAT=\"9\" SID=\"689493411\" SURNAME=\"Leon\" PREFNAME=\"Isabella\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O160\" GRADE=\"B+\" STATUS=\"RE\" EMAIL=\"isabella.v.leon@fakeu.edu\"/>\n"
    "<student SEAT=\"10\" SID=\"239763519\" SURNAME=\"Ray\" PREFNAME=\"Ivan\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O244\" GRADE=\"B+\" STATUS=\"RE\" EMAIL=\"ivan.k.ray@fakeu.edu\"/>\n"
    "<student SEAT=\"11\" SID=\"793868457\" SURNAME=\"Robertson\" PREFNAME=\"Benjamin\" LEVEL=\"GR\" UNITS=\"4\" CLASS=\"GM\" MAJOR=\"O272\" GRADE=\"B\" STATUS=\"RE\" EMAIL=\"benjamin.b.robertson@fakeu.edu\"/>\n"
    "<student SEAT=\"12\" SID=\"976734390\" SURNAME=\"Peters\" PREFNAME=\"Hailey\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O224\" GRADE=\"B\" STATUS=\"RE\" EMAIL=\"hailey.v.peters@fakeu.edu\"/>\n"
    "<student SEAT=\"13\" SID=\"515177515\" SURNAME=\"Boyd\" PREFNAME=\"Mateo\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"JR\" MAJOR=\"O224\" GRADE=\"A-\" STATUS=\"RE\" EMAIL=\"mateo.k.boyd@fakeu.edu\"/>\n"
    "<student SEAT=\"14\" SID=\"779755376\" SURNAME=\"Potts\" PREFNAME=\"Eva\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"JR\" MAJOR=\"ABC1\" GRADE=\"A-\" STATUS=\"RE\" EMAIL=\"eva.g.potts@fakeu.edu\"/>\n"
    "<student SEAT=\"15\" SID=\"688078119\" SURNAME=\"Young\" PREFNAME=\"Emma\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"ABC1\" GRADE=\"B-\" STATUS=\"RE\" EMAIL=\"emma.l.young@fakeu.edu\"/>\n"
    "<student SEAT=\"16\" SID=\"673925205\" SURNAME=\"Ryan\" PREFNAME=\"Eric\" LEVEL=\"GR\" UNITS=\"4\" CLASS=\"D1\" MAJOR=\"O184\" GRADE=\"A-\" STATUS=\"RE\" EMAIL=\"eric.f.ryan@fakeu.edu\"/>\n"
    "<student SEAT=\"17\" SID=\"163390175\" SURNAME=\"Smith\" PREFNAME=\"Sofia\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O224\" GRADE=\"B-\" STATUS=\"RE\" EMAIL=\"sofia.e.smith@fakeu.edu\"/>\n"
    "<student SEAT=\"18\" SID=\"490720476\" SURNAME=\"Leonard\" PREFNAME=\"Sophia\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"JR\" MAJOR=\"ABC2\" GRADE=\"A\" STATUS=\"RE\" EMAIL=\"sophia.w.leonard@fakeu.edu\"/>\n"
    "<student SEAT=\"19\" SID=\"781564117\" SURNAME=\"Gutierrez\" PREFNAME=\"Valeria\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"ABC1\" GRADE=\"C-\" STATUS=\"RE\" EMAIL=\"valeria.a.gutierrez@fakeu.edu\"/>\n"
    "<student SEAT=\"20\" SID=\"504980191\" SURNAME=\"Carpenter\" PREFNAME=\"Dylan\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"JR\" MAJOR=\"ABC1\" GRADE=\"B+\" STATUS=\"RE\" EMAIL=\"dylan.d.carpenter@fakeu.edu\"/>\n"
    "<student SEAT=\"21\" SID=\"746877527\" SURNAME=\"Cook\" PREFNAME=\"Brandon\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"ABC1\" GRADE=\"B+\" STATUS=\"RE\" EMAIL=\"brandon.j.cook@fakeu.edu\"/>\n"
    "<student SEAT=\"22\" SID=\"379128764\" SURNAME=\"Whitney\" PREFNAME=\"Mason\" LEVEL=\"UG\" UNITS=\"4\" CLASS=\"SR\" MAJOR=\"O160\" GRADE=\"B+\" STATUS=\"RE\" EMAIL=\"mason.j.whitney@fakeu.edu\"/>\n"
"</course>\n";

    std::vector<std::string> name,data;

    name = {"SUBJ", "CRSE", "SEC"};
    data = {"ABC", "204", "2"};

    ReadTest(name,data,1,1,ss,"course");
    clearVec(name,data);
    
    name = {"SEAT", "SID", "SURNAME", "PREFNAME", "LEVEL", "UNITS", "CLASS", "MAJOR", "GRADE", "STATUS", "EMAIL"};
    data = {"1", "679358754", "Rose", "Sarah", "UG", "4", "SR", "O268", "C+", "RE", "sarah.o.rose@fakeu.edu"}; 
    ReadTest(name,data,2,1,ss,"student",true);
    
    clearVec(name, data);
    ReadTest(name,data,3,3,ss,"student",true);
    
    name = {"SEAT", "SID", "SURNAME", "PREFNAME", "LEVEL", "UNITS", "CLASS", "MAJOR", "GRADE", "STATUS", "EMAIL"};
    data = {"2","493845023", "Freeman", "Alexander", "UG", "4", "SR", "O239", "A+", "RE", "alexander.b.freeman@fakeu.edu"};
    ReadTest(name, data,4,1,ss,"student",true);
    
    clearVec(name, data);
    name = {"SEAT", "SID", "SURNAME", "PREFNAME", "LEVEL", "UNITS", "CLASS", "MAJOR", "GRADE", "STATUS", "EMAIL"};
    data = {"22", "379128764", "Whitney", "Mason", "UG", "4", "SR", "O160", "B+", "RE", "mason.j.whitney@fakeu.edu"};

    ReadTest(name,data,44,1,ss,"student",true);
    
    clearVec(name,data);
    ReadTest(name,data,45,3,ss,"student",true);
    ReadTest(name,data,46,3,ss,"course",true,true);

   //ReadTest()
}
void WriteTest(std::string str, int looptimes, std::string str2)
{
    std::stringstream ss1;
    std::stringstream ss2(str);
    CXMLReader read(ss2);
    CXMLWriter write(ss1);
    SXMLEntity entity;

    for(int i = 0; i < looptimes; i++){
        EXPECT_TRUE(read.ReadEntity(entity, true));
        EXPECT_TRUE(write.WriteEntity(entity));
    }

    EXPECT_TRUE(read.End());
    
    EXPECT_EQ(ss1.str(),str2);
}


TEST(XMLTest, 01)
{   
    // idea from Professor Nitta
    std::string str = "<course SUBJ=\"ABC\" CRSE=\"114\" SEC=\"2\">\n""    <student SEAT=\"1\" SID=\"409337710\" SURNAME=\"White\" PREFNAME=\"Victoria\" LEVEL=\"GR\" UNITS=\"12\" CLASS=\"D1\" MAJOR=\"ABCG\" GRADE=\"S\" STATUS=\"RE\" EMAIL=\"victoria.b.white@fakeu.edu\"></student>""</course>";
    
    std::stringstream  ss(str);
    CXMLReader obj(ss);

    SXMLEntity entity;

    EXPECT_TRUE(obj.ReadEntity(entity));


    EXPECT_EQ(obj.End(),false);
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "course");

    if(entity.AttributeExists("SUBJ"))
        EXPECT_EQ(entity.AttributeValue("SUBJ"), "ABC");
    else
        EXPECT_FALSE(entity.AttributeExists("SUBJ"));

    if(entity.AttributeExists("SEC"))
            EXPECT_EQ(entity.AttributeValue("SEC"), "2");
    else
        EXPECT_FALSE(entity.AttributeExists("SEC"));
    
    if(entity.AttributeExists("CRSE"))
        EXPECT_EQ(entity.AttributeValue("CRSE"), "114");
    else
        EXPECT_FALSE(entity.AttributeExists("CRSE"));

    EXPECT_TRUE(obj.ReadEntity(entity));
    EXPECT_EQ(obj.End(), false);
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "\n");
    

    EXPECT_TRUE(obj.ReadEntity(entity));
    EXPECT_EQ(obj.End(), false);
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "    ");
    

    EXPECT_TRUE(obj.ReadEntity(entity));
    EXPECT_FALSE(obj.End());
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "student");

    if(entity.AttributeExists("SEAT"))
        EXPECT_EQ(entity.AttributeValue("SEAT"), "1");
    else
        EXPECT_FALSE(entity.AttributeExists("SEAT"));

    if(entity.AttributeExists("SID"))
        EXPECT_EQ(entity.AttributeValue("SID"), "409337710");
    else
        EXPECT_FALSE(entity.AttributeExists("SID"));

    if(entity.AttributeExists("STATUS"))
        EXPECT_EQ(entity.AttributeValue("STATUS"), "RE");
    else
        EXPECT_FALSE(entity.AttributeExists("STATUS"));
    
    if(entity.AttributeExists("EMAIL"))
        EXPECT_EQ(entity.AttributeValue("EMAIL"), "victoria.b.white@fakeu.edu");
    else
        EXPECT_FALSE(entity.AttributeExists("EMAIL"));
    
    EXPECT_TRUE(obj.ReadEntity(entity));
    EXPECT_EQ(obj.End(), false);
    
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "student");

    EXPECT_TRUE(obj.ReadEntity(entity));
    EXPECT_TRUE(obj.End());
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "course");

    EXPECT_TRUE(obj.End());

}

TEST(XMLTest,2) {
    std::string str = "<course SUBJ=\"ABC\" CRSE=\"114\" SEC=\"2\">\n" "    <student SEAT=\"1\" SID=\"900667130\" SURNAME=\"Stein\" PREFNAME=\"Ava\" LEVEL=\"UG\" UNITS=\"2\" CLASS=\"FR\" MAJOR=\"DEF2\" GRADE=\"I\" STATUS=\"RE\" EMAIL=\"ava.s.stein@fakeu.edu\"/>\n"
                             "</course>";
    std::string str2 = "<course SUBJ=\"ABC\" CRSE=\"114\" SEC=\"2\">""<student SEAT=\"1\" SID=\"900667130\" SURNAME=\"Stein\" PREFNAME=\"Ava\" LEVEL=\"UG\" UNITS=\"2\" CLASS=\"FR\" MAJOR=\"DEF2\" GRADE=\"I\" STATUS=\"RE\" EMAIL=\"ava.s.stein@fakeu.edu\"></student>""</course>";

    WriteTest(str,4, str2);
}

