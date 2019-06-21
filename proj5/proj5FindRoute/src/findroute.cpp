#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <cctype>
#include "CSVReader.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include "CSVWriter.h"
#include "MapRouter.h"
#include <iomanip>
#include <ncurses.h>

using TNodeID = unsigned long;
using TStopID = unsigned long;
using TLocation = std::pair<double, double>;
using TPathStep = std::pair<std::string, TNodeID>;

std::vector<std::string> menu = {"help", "exit", "count", "node", "fastest", "shortest", "save", "print"};

std::string helpInfor =  "findroute [--data=path | --resutls=path]\n"
                         "------------------------------------------------------------------------\n"
                         "help     Display this help menu\n"
                         "exit     Exit the program\n"
                         "count    Output the number of nodes in the map\n"
                         "node     Syntax \"node [0, count)\" \n"
                         "         Will output node ID and Lat/Lon for node\n"
                         "fastest  Syntax \"fastest start end\" \n"
                         "         Calculates the time for fastest path from start to end\n"
                         "shortest Syntax \"shortest start end\" \n"
                         "         Calculates the distance for the shortest path from start to end\n"
                         "save     Saves the last calculated path to file\n"
                         "print    Prints the steps for the last calculated path\n"
			 "UP       Use Up and down keys to see what have been entered\n";
std::vector<unsigned long> getID(std::string str)
{   
    int break1;
    bool flag = false;
    std::vector<unsigned long> vec;
    for(unsigned int i = 0; i < str.size();i++)
    {   
        if(isalpha(str[i]))
        {
            flag = false;
            break;
        }

        if(isspace(str[i]))
        {
            break1 = i;
            flag = true;
            for(int j = break1; j < (int)str.size(); j++)
                if(isalpha(str[j]))
                {
                    flag = false;
                    break;
                }
            break;
        }
    }

    if(flag)
    {
        vec.push_back(std::stoul(str.substr(0,break1)));
        vec.push_back(std::stoul(str.substr(break1+1)));
    }
    return vec;
}

// cite from https://stackoverflow.com/questions/15306463/getchar-returns-the-same-value-27-for-up-and-down-arrow-keys
void history(std::vector<std::string> vec)
{
        int ch;
        int i = vec.size()-1;
        const char *a;
        initscr();
        raw();
        keypad(stdscr, TRUE);

        printw("Press E to Exit\n");

        while((ch = getch()) != 'E')
        {
                switch(ch)
                {
                case KEY_UP:
                        i--;
                        if(i < 0)
                        {
                                i = 0;
                                break;
                        }

                        a = vec[i].c_str();
                        printw("%s\n",a);
                        break;
                case KEY_DOWN:
                        i++;
                        if(i >= (int)vec.size())
                        {
                                i = vec.size() - 1;
                                break;
                        }
                        a = vec[i].c_str();
                        printw("%s\n",a);

                        break;
                default:
                        break;
                }
        }
         endwin();

}


void converttime(double time)
{
	int hour = time;
	int min = (double)(time-hour) * 60;
	double seconds = ((double)(time-hour) * 60 - min)* 60;
	
	std::cout << std::setprecision(4);
	if(hour != 0)
		std::cout << " " << hour << " hour ";
	if(min != 0)
		std::cout << min << "min";
         std::cout <<  " " << seconds << "sec\n";
}

int checkspace(std::string str1)
{

	for(int i = 0; i < (int)str1.size();i++)
	{
		if(isspace(str1[i])){
			if(i + 1 <(int) str1.size())
				if(isspace(str1[i+1]))
					return 1;
		}
	}
	return 0;

}
	


int main(int argc, char * argv[])
{
	if(argc >= 2)
	{
		std::cout << "Syntax Error: findroute [--data=path | --resutls=path]\n";
		exit(1);
	}
	
	std::ifstream a("../data/davis.osm");
	std::ifstream b("../data/stops.csv");
	std::ifstream c("../data/routes.csv");
	
	CMapRouter obj;
	


	if(not obj.LoadMapAndRoutes(a,b,c) ){
		std::cout << "cannot open the file\n";
		exit(1);
	}
	
	int count = 0;
	std::string userDecision = "";
	std::vector<std::string>path_step;
	std::vector<TNodeID> path_;
	std::vector<TPathStep >path;
	std::vector<std::string> vec;
	while(userDecision != "exit")
	{
		std::cout << ">";
		std::getline(std::cin,userDecision);
		vec.push_back(userDecision);
		if(userDecision == "help")
			std::cout << helpInfor;
		else if(userDecision == "exit")
			break;
		else if(userDecision == "count")
			std::cout << obj.NodeCount() << " nodes" << "\n";
		else if(userDecision.substr(0,4) == "node")
		{
			 if(userDecision.size() <= 5)
                        {
                                std::cout << "Invalid node command, see help.\n";
                                continue;
                        }

			for(int i = 5; i < (int)userDecision.size();i++)
				if(!isdigit(userDecision[i]))
				{
					std::cout << "Invalid node parameter, see help.\n";
					continue;
				}
			int SortID = std::stoi(userDecision.substr(5));
			
			if(SortID >= (int)obj.NodeCount())
				std::cout << "Invalid node parameter, see help.\n";
			else{
				std::cout << "Node " << SortID << " id = " << obj.GetSortedNodeIDByIndex(SortID) << " is at ";
				auto a = obj.GetSortedNodeLocationByIndex(SortID);
				std :: cout << a.first << ", " << a.second << "\n";
			}
			
		}
		else if(userDecision.substr(0,8) == "shortest")
		{
			 if(userDecision.size() <= 10 || checkspace(userDecision))
                        {
                                std::cout << "Invalid shortest command, see help.\n";
                                continue;
                        }

			auto vec = getID(userDecision.substr(9));
			path_.clear();
			if(vec.size() != 2)
				std::cout << "Invalid shortest command, see help.\n";
			else
			{	
				count = 1;
				auto id = vec[0],id_ = vec[1];
				if(obj.FindShortestPath(id, id_, path_) and obj.FindShortestPath(id, id_, path_) < 9999)
				{
						std::cout << "Shortest path is " << std::setprecision(4)<< obj.FindShortestPath(id, id_, path_) << "miles\n";


				}
				else{
					std::cout << "length is " << obj.FindShortestPath(id, id_, path_) << "\n";
					std::cout << "Unable to find shortest path from " << id << " to " << id_ << "\n";

			
				}
			}	
		}
		else if(userDecision.substr(0,7) ==  "fastest")
		{
			if(userDecision.size() <= 9 || checkspace(userDecision))
			{
				std::cout << "Invalid fastest command, see help.\n";
				continue;
			}
						
			
			auto vec = getID(userDecision.substr(8));
                        path.clear();
                        if(vec.size() != 2)
                                std::cout << "Invalid fastest command, see help.\n";
                        else
                        {
                                auto id = vec[0],id_ = vec[1];
				count = 2;
                                if(obj.FindFastestPath(id, id_, path))
				{
					if(!obj.GetPathDescription(path, path_step))
						 std::cout << "Unable to find fastest path from " << id << " to " << id_ << "\n";
					else
					{
						std::cout << "Fastest path takes ";
						converttime(obj.FindFastestPath(id, id_, path));	
					}
				}
                                else 
                                        std::cout << "Unable to find fastest path from " << id << " to " << id_ << "\n";

			}
		}

		else if (userDecision.substr(0,4) == "save")
		{
			std::ofstream file("route.text");

			if(file.is_open() && path_step.size() != 0)
			{
				for(int i = 0; i < (int)path_step.size();i++)
					file << path_step[i] << "\n";
				file.close();
				std::cout << "route.text was saved successfully\n";
			}
			
			else
				std::cout << "Failed to save path.\n";

			
		}
		else if(userDecision.substr(0,5) == "print")
		{	
			if(path_.size() != 0 && count == 2)
			{	
				path_step.clear();
				for(int i = 0; i < (int)path_.size();i++)
				{	
					TPathStep a;
					a.first = "Walk"; a.second = path_[i];
					path.push_back(a);
					
				}
				if(!obj.GetPathDescription(path, path_step))
				{
					std::cout << "Unable to print\n";
					continue;	
				}
			}

			
			
			if(path_step.size()!= 0)
			{
				
				for(int i = 0; i < (int)path_step.size();i++)
					std::cout << path_step[i] << "\n";
		
			}
			else
				std::cout << "Unable to print\n";
		}

		else if(userDecision == "UP")
			history(vec);


		else
			std::cout << "Unknown command \"" << userDecision << "\" type help for help.\n";

	}

}
