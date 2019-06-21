#include"CSVReader.h"
#include<iostream>
void CCSVReader::callback1(void *str, size_t length, void *data)
{
	//implentation information taught by TA Nick
	// static_cast does things like implicit conversions between types
	auto csv = static_cast<CCSVReader*>(data);
	const char* const strr = static_cast<const char* const>(str);
	csv->obj.push_back(std::string(strr,length));
}

void CCSVReader::callback2(int num, void *data)
{
	// implentation information taught by TA Nick and Professor Nitta

	auto csv = static_cast<CCSVReader*>(data);
	if(num == '\r' or num == '\n'){
		csv->myqueue.push(csv->obj);
		csv->obj.clear();
	}

}

// taught by Professor Nitta
CCSVReader::CCSVReader(std::istream &in) : in_(in)
{	
	// use the csv_init function to initialze the parser object
	csv_init(&csv_par,0);
}

CCSVReader::~CCSVReader()
{	
	//free the parser object
	csv_free(&csv_par);
}

bool CCSVReader::End() const
{	
	bool flag = false;
	if(in_.eof())
		flag = true;
	return flag;
}

bool CCSVReader::ReadRow(std::vector<std::string> &row)
{
	// taught by TA Nick and Professor Nittta
	char buffer[10000];
	bool flag = false;
	
	
	while(!in_.eof() && myqueue.empty()){
		in_.read(buffer, 10000);

		csv_parse(&csv_par, buffer, in_.gcount(), callback1, callback2, this); // "this" means data
		if(in_.eof())
		{	
			csv_fini(&csv_par, callback1, callback2,this);
		}
	}



	if(!myqueue.empty())
	{	
		row.clear();
		row = myqueue.front();
		myqueue.pop();
		flag = true;
	}

	return flag;


}
