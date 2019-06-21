#include"CSVWriter.h"

CCSVWriter::CCSVWriter(std::ostream &ou):out_(ou)
{

}

CCSVWriter::~CCSVWriter()
{

}

bool CCSVWriter::WriteRow(const std::vector<std::string> &row)
{
	if(out_.good() == false || row.size() == 0)
		return false;

	for(unsigned int i = 0;i < row.size(); i++)
	{	
		
		auto str = row[i];
		out_ << str;
		if(i < row.size() - 1)
			out_ << ",";
	}

	out_ << "\n";
	
	return true;
}
