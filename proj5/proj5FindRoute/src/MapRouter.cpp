#include "MapRouter.h"
#include <cmath>
#include "CSVReader.h"
#include "XMLReader.h"
#include <unordered_map> 
#include<string>
#include<map>
#include<queue>
#include "XMLEntity.h"
#include <climits>
#include<iostream>
#include<algorithm>
#include "../include/csv.h"
#include<fstream>
#include<iomanip>

const CMapRouter::TNodeID CMapRouter::InvalidNodeID = -1;

void Dijkstra(int n, unsigned int v, std::vector<double> &dist, std::vector<unsigned int> &pre, std::vector<std::vector<double>> &d);

std::string convertMinSec(double latlon)
    {
        int degree =  0;
        degree = (int)latlon;
        double minutes = 0;
        minutes = (latlon - (int)latlon) * 60;
        double seconds = 0; 
        seconds = (minutes - (int)minutes) * 60;
        
        std::string str = std::to_string(degree) + "d " ;
        str += std::to_string((int)minutes) +  "' " + std::to_string((int)seconds)  + "\" ";
        return str;
    }

bool checkfunc(int id, int num)
{
	if(id < 1 or id > num)
		return false;
	return true;
}

std::string getBearing(double bearing)
{
	bearing = std::round(bearing);
	
	if(bearing == 0)
		return "N";
	else if(bearing > 0 and  bearing < 90)
		return "NE";
	else if(bearing == 90)
		return "E";
	else if(bearing > 90 and bearing < 180)
		return "SE";
	else if(bearing == 180)
		return "S";
	else if(bearing < 0 and bearing > -90)
		return "WE";
	else if(bearing == -90)
		return "W";
	else if(bearing < -90 and bearing > -180)
		return "SW";
	else if(bearing == -180)
		return "S";
	else
		return "";
}


class CMapRouter::CImplementation{

public:
    class edgeTo{
        public:
            int index;
            double distance;
            double time;
            edgeTo(int index, double distance, double time)
            { 
                index = index;
                distance = distance;
                time = time;
                
            }
            edgeTo()
            { 
                    index = -1;
                    distance = 0;
                    time = 0;
            }
            double get_weight(bool dist)
            {
                if (dist)
                {
                        return distance;
                }
                return time;

            }
    };
    struct Node{
        TNodeID id;
        unsigned int IDFromOne;
        TLocation myLatLon;
        std::vector<edgeTo> myadjecList;
    };
    struct Way{
        std::string name;
        bool oneway;
        double maxspeed;
        std::vector<TNodeID> refVec;
    };
   
    std::vector<TStopID> mystopVec; 
    std::vector<std::string> myrouteVec;
    std::unordered_map<TStopID, std::string> myroutemap;
    std::unordered_map<TStopID, TNodeID> mystopsmap;
    std::vector<TPathStep> myPathVec;

    std::unordered_map<TNodeID,int> mymap;
    std::vector<Node> myNodeVec;    
    std::vector<TNodeID> sortIDVec;
    
    std::vector<std::vector<double>>Time;
    std::vector<std::vector<double>>Distance;
    int expandSize = 0;

    // the reading part from Professor Nitta
    bool LoadMapAndRoutes(std::istream &osm, std::istream &stops, std::istream &routes)
    {
	 CXMLReader XML_(osm);
        SXMLEntity entity_;

        //std::cout<<"In "<<__FILE__<<"@ "<<__LINE__<<std::endl;// cannot output this line here
        XML_.ReadEntity(entity_);
        //std::cout<<"In "<<__FILE__<<"@ "<<__LINE__<<std::endl;// cannot output this line here   

	

        /*if(entity_.DType != SXMLEntity::EType::StartElement or entity_.DNameData != "osm")
        {
            ////std::cout << "Cannot read the file\n";
            return false;
        }*/
        
        ////std::cout << "Going to read the loop\n";
	
        CCSVReader obj(routes), obj_(stops);
        std::vector<std::string> row;
	
	 //std::cout << "Reading stops" << "\n";
        obj_.ReadRow(row);
	
        while(obj_.ReadRow(row))
        {
            auto stopID = std::stoul(row[0]);
            auto NodeID = std::stoul(row[1]);
            mystopsmap[stopID] = NodeID;
            ////std::cout <<"NodeID"  << NodeID << "\n";
       
	}


       //std::cout << "Reading routes" << "\n";
            obj.ReadRow(row);
    
        while(obj.ReadRow(row))
        {
            auto stopID = std::stoul(row[1]);
            //std::cout << "StopID: " << stopID << "\n";
	    mystopVec.push_back(stopID);
            myroutemap[stopID] = row[0];
            myrouteVec.push_back(row[0]);
        }
    	int count = 0;
        //read XML Part
        while(!XML_.End())
        {   
            //std::cout << "Reading XML\n";
            XML_.ReadEntity(entity_,true);

            if(entity_.DType == SXMLEntity::EType::StartElement)
            {
                Node node;
                Way way;
		way.maxspeed = 25;
		way.oneway = false;
                    if(entity_.DNameData == "node")
                    {
                        node.id = std::stoul(entity_.AttributeValue("id"));
                        auto value = std::stod(entity_.AttributeValue("lat")); 
			node.myLatLon.first = value;
			value = std::stod(entity_.AttributeValue("lon"));
                        node.myLatLon.second = value;
                        //std::cout << "paid<" << node.myLatLon.first << "," << node.myLatLon.second << ">" << "\n";
                        mymap[node.id] = myNodeVec.size() + 1;
            		node.IDFromOne = myNodeVec.size() + 1;
                        myNodeVec.push_back(node);
                        //std::cout << "add node: " << node.id << "\n";
                        sortIDVec.push_back(node.id);
                 	count++;   	
		    }

                    else if(entity_.DNameData == "way")
                    {   
                        ////std::cout << "In way\n";
                        size_t NodeNum = myNodeVec.size();

                        if(expandSize == 0)
                        {
                            Distance.resize(NodeNum + 9999);
                            
                            for(auto &vec : Distance)
                                vec.resize(NodeNum + 9999, 9999);
                            
                            Time.resize(NodeNum + 9999);
                            for(auto &vec : Time)
                                vec.resize(NodeNum + 9999, 9999);

                             expandSize++;
                             /*//std::cout << "------------------\n";
                             //std::cout << "expand size here\n";
                             //std::cout << "NodeNum:" << NodeNum << "\n";
                             //std::cout << "------------------\n";*/
                         }


                        while(!XML_.End())
                        {   
                            XML_.ReadEntity(entity_,true);
                            if(entity_.DType == SXMLEntity::EType::StartElement)
                            {
                                if(entity_.DNameData == "nd")
                                {
                                    auto lookup = mymap.find(std::stoul(entity_.AttributeValue("ref")));
                                    if(mymap.end() != lookup)
                                    {
				        auto num = std::stoul(entity_.AttributeValue("ref"));
                                        way.refVec.push_back(num);
                                        //std::cout << "refID" << num << "\n";
                                    }
                                }

                                if(entity_.DNameData == "tag")
                                {   
                                    if(entity_.AttributeValue("k") == "maxspeed")
                                    {
                                        way.maxspeed = std::stod(entity_.AttributeValue("v"));
                                        ////std::cout << "way.maxspeed"  << way.maxspeed << "\n";
                                    }

                                    if(entity_.AttributeValue("k") == "oneway")
                                    {
                                        way.oneway  = true;

                                        ////std::cout << "way.oneway" << "true" << "\n";

				    }
                                }
                            }
                            

			    if(entity_.DType == SXMLEntity::EType::EndElement)
                                if(entity_.DNameData == "way")
                                    break;
                        }

                        ////std::cout << "Here read all the refvec\n";
                        int size = way.refVec.size() - 1;
			std::vector<double>myDistanceVec;
			for(int i = 0; i < size ;i++)
                        {
                            if(mymap.find(way.refVec[i]) != mymap.end() && mymap.find(way.refVec[i+1]) != mymap.end())
                            {   
                                auto index = mymap[way.refVec[i]] - 1;// set the index from 1 instead of 0
                                auto index_ = mymap[way.refVec[i+1]] - 1;

                                double distance = HaversineDistance(myNodeVec[index].myLatLon.first, myNodeVec[index].myLatLon.second, myNodeVec[index_].myLatLon.first, myNodeVec[index_].myLatLon.second);                   
                                edgeTo a(index_, distance,distance / way.maxspeed);
                                myNodeVec[index].myadjecList.push_back(a);
                                myDistanceVec.push_back(distance);
				////std::cout << "Here is the infor\n";
                                ////std::cout << "< " << myNodeVec[index].id  << " to "<<myNodeVec[index_].id << "," << distance << "," << distance / way.maxspeed << "\n";
                                ////std::cout << "index:" << index << "\n";
                                ////std::cout << "index_:" << index_ << "\n";
                                Distance[myNodeVec[index].IDFromOne][myNodeVec[index_].IDFromOne] =  distance;
                                //std::cout << "Distance from " << index << " to " << index_  << " is " << distance << "\n";
                                // if two nodes are on the same line like A,B then take bus
                                //auto route = OnTheSameRoute(myNodeVec[index].id, myNodeVec[index_].id);

                               // if(route == "Walk")
                                    Time[myNodeVec[index].IDFromOne][myNodeVec[index_].IDFromOne] =  distance / 3;
                                //else
                                   // Time[myNodeVec[index].IDFromOne][myNodeVec[index_].IDFromOne] = distance / 25;
                                if(!way.oneway)
                                {   
                                     //revserse
                                    edgeTo a(index, distance,distance / way.maxspeed);
                                    myNodeVec[index_].myadjecList.push_back(a);
                                    Distance[myNodeVec[index_].IDFromOne][myNodeVec[index].IDFromOne] =  distance;
				    Time[myNodeVec[index_].IDFromOne][myNodeVec[index].IDFromOne]=  distance / 3;
				
				}
                            }   
                        }
			//std:: cout << "set distances between two existed nodes" << "\n";
			//std:: cout << " way.refVec.size() - 2: " <<  way.refVec.size() - 2 << "\n";
			size =  way.refVec.size() - 2;
			for( int i = 0; i <= size;i++)
			{	
				//std::cout << "Here is the fill part\n";
				auto index1 = mymap[way.refVec[i]] - 1;
				for(int j = i + 2; j < size + 2;j++)
				{
					//auto index_ = mymap[way.refVec[j]] - 1;
					//auto a  = myNodeVec[index].IDFromOne, c = myNodeVec[index_].IDFromOne, b =  myNodeVec[indexx].IDFromOne;
					auto index2 =  mymap[way.refVec[j - 1]] - 1, index3 =  mymap[way.refVec[j]] - 1;
					auto a = myNodeVec[index1].IDFromOne, b = myNodeVec[index2].IDFromOne, c = myNodeVec[index3].IDFromOne;
					if(a < c)
						Distance[a][c] = Distance[a][b] + Distance[b][c];
					else if (a > c)
						Distance[a][c] = Distance[c][a];
						
				}
				//std::cout << "finished here\n";

			}
			//0 1  2  3  4
			/*1 11 3  7 12
		        [1,3] = [1,11] + [11,3]
			[1,7] = [1,3] + [3,7]
			[1,12] = [1,11] + [11,3] + [3,7] + [7,12]*/
			// [0,2] = [0,1] + [1,2]
			// [0,3] = [0,2] + [2,3]
			// [0,4] = [0,3] + [0,4]
			/*for(int i = 0; i <= size; i ++)
			{
				auto index = mymap[way.refVec[i]] - 1;
                                auto indexx = mymap[way.refVec[i + 1]] - 1;
				
				auto a  = myNodeVec[index].IDFromOne, auto b  = myNodeVec[indexx].IDFromOne;

				if(a > b)
					Distance[a][b] = Distance[b][a];
				else
					Distance[a][b] = 
			}*/
			way.refVec.clear();

				
		    
		    }      
                }
        }
        // set time while taking bus
        for(unsigned int i = 0; i <  mystopVec.size();i++) 
	{	
		auto find = mystopsmap.find(mystopVec[i]);
		for(unsigned int j = 0; j <  mystopVec.size();j++)
		{	
			auto find_ = mystopsmap.find( mystopVec[j]);
			if(find != mystopsmap.end() and find_ != mystopsmap.end() and j > i)
			{
				////std::cout << find->second << " to " << find_->second << "\n";
				auto index = 0, index_ = 0;
				if(mymap.find(find->second) != mymap.end() && mymap.find(find_->second) != mymap.end())
					index = mymap.find(find->second)->second-1, index_ = mymap.find(find_->second)->second-1;
				else
					break;
				double distance = Distance[myNodeVec[index].IDFromOne][myNodeVec[index_].IDFromOne];
				Time[myNodeVec[index].IDFromOne][myNodeVec[index_].IDFromOne] =  distance/25 + (double)30/3600;
				//std::cout << "The distance is " << distance << "\n";
			}
		}
	}
     	//std::cout << "count: " << count << "\n";
	 //std::cout << "last id: " <<  myNodeVec[count-2].id << "\n"; 
	std::sort(sortIDVec.begin(), sortIDVec.end());
        std::sort(myrouteVec.begin(), myrouteVec.end());
        return true;
    }
   
    size_t NodeCount() const
    {
        return myNodeVec.size();
    }
    unsigned long GetSortedNodeIDByIndex(size_t index) const
    {   
       return sortIDVec[index];
    }
    std::pair<double, double>  GetSortedNodeLocationByIndex(size_t index) const
    {   
    
        if(index >= sortIDVec.size())
            return std::make_pair(180,360);
        else
        {
        auto ID = sortIDVec[index];
                   
        return myNodeVec[mymap.find(ID)->second-1].myLatLon;
        }

    }
    std::pair <double, double> GetNodeLocationByID(TNodeID ID) const
    {
        if(mymap.find(ID) == mymap.end())
            return std::make_pair(180,360);
        else
        {
            return myNodeVec[mymap.find(ID)->second-1].myLatLon;
        }   
    }

    unsigned long GetNodeIDByStopID(TStopID stopid) const
    {
        auto find =  mystopsmap.find(stopid);
        if(find != mystopsmap.end())
            return find->second;
        else
            return -1;
    }

    size_t RouteCount() const
    {   
        size_t i = 0;
    
        if(myrouteVec.size())
            i = 1;
	if(myrouteVec.size() == 0)
	{
		//std::cout << "Vec_size: 0" << "\n";
		return 0;
	}	
        std::string  prev = myrouteVec[0];
        for(auto value : myrouteVec)
        {
            if(value != prev)
            {	
		//std::cout << "value:" << value << "\n";
                prev = value;
                i++;
            }
        }
        return i;
    }

    std::string GetSortedRouteNameByIndex(size_t index)
    {
        if(index >= myrouteVec.size())
            return "outOfbound";
        else
            return myrouteVec[index];
    }

    bool GetRouteStopsByRouteName(const std::string &route, std::vector< TStopID > &stops)
    {
        bool flag = false;

        for(auto value : myrouteVec)
        {
            if(value == route)
            {
                flag = true;
                break;
            }
        }

        if(flag)
        {
            stops.clear();
            for(auto value : myroutemap)
                if(value.second == route)
                    stops.push_back(value.first);
        }

        std::sort(stops.begin(), stops.end());
        return flag;

    }

    //std::unordered_map<TStopID, std::string> myroutemap;
    //std::unordered_map<TStopID, TNodeID> mystopsmap;
    std::string OnTheSameRoute(TNodeID src_id, TNodeID dest_id)
    {
        std::string route = "Walk";
        
	if(src_id == dest_id)
		return route;
        
	
	std::vector<TStopID>mySrcVec, myStopVec;

        for(auto value : mystopsmap)
        {
            if(value.second == src_id)
            {
                mySrcVec.push_back(value.first);
            }

            if(value.second == dest_id)
            {
                myStopVec.push_back(value.first);
            }
        }

        if(mySrcVec.size() && myStopVec.size())
        {
            for(unsigned i = 0; i < mySrcVec.size();i++)
            {   
                auto find = myroutemap.find(mySrcVec[i]);
                for(unsigned j = 0; j < myStopVec.size();j++)
                {   
                    auto find_ = myroutemap.find(myStopVec[i]);
                    if(find != myroutemap.end() and find_ != myroutemap.end())
                    {
                        if(find->second == find_->second)
                        {   
                            route = "Bus ";
                            route += find->second;
                            //std::cout << "find route: " << route << " between ID " << src_id << " and " << "ID " << dest_id << "\n";
                            break;
                        }
                    }
                }
            }
        }

        return route;
    }

    // cite here:  https://blog.csdn.net/mimi9919/article/details/51219176
    double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path)
    {
        if (src == dest)
        {
            path.push_back(src);
            return 0.0;
        }
	//std::cout << "Load here" << "\n";
	size_t TotalNodes = myNodeVec.size();
        double distance = 0;
	
	if(mymap.find(src) == mymap.end() ||mymap.find(dest) == mymap.end())
        {
                return 0;
        }
	//std::cout << "ID found \n"<< "\n";
	

        Node startNode =  myNodeVec[mymap.find(src)->second - 1];
        Node endNode =  myNodeVec[mymap.find(dest)->second - 1];
        
        
	auto LastID = endNode.IDFromOne, StartID = startNode.IDFromOne,ep = endNode.IDFromOne;
	

	//std::cout << " LastID " << LastID << " StartID " <<  StartID << "\n";
        std::vector<double> myDistanceVec(TotalNodes + 9999, 9999);
        std::vector<unsigned int> myPreVec(TotalNodes+9999, 0);

        Dijkstra(TotalNodes, startNode.IDFromOne, myDistanceVec, myPreVec, Distance);
	//std::cout << "after Dijkstra\n";
        
	if(myDistanceVec.size() == 0 || myPreVec.size() == 0 )
                return 0;

	
	Node before,cur;
        
        before.id = -1;
        cur.id =-1;
	
	 if(myDistanceVec.size() == 0 || myPreVec.size() == 0 )
                return 0;

	
        for(int i = 0; i < INT_MAX; i++)
        {   
            if(LastID != StartID)
            {	
	        //std::cout << "in the loop StartID "<< StartID << "End ID " << LastID <<  "\n";
            	if(!checkfunc(StartID,TotalNodes) || !checkfunc(LastID, TotalNodes))
                {	
			//std::cout << "break" << "StartID "<< StartID << "End ID " << LastID <<  "\n";
		       	break;
		}
	       //std::cout<<"In "<<__FILE__<<"@ "<<__LINE__<<std::endl;
               cur = myNodeVec[LastID - 1];
	       //std::cout<<"In "<<__FILE__<<"@ "<<__LINE__<<std::endl;
               path.push_back(cur.id);
	       //std::cout<<"In "<<__FILE__<<"@ "<<__LINE__<<std::endl;

               if((int)before.id != -1 &&(int) cur.id != -1 )
                   distance += Distance[cur.IDFromOne][before.IDFromOne];
	       //std::cout<<"In "<<__FILE__<<"@ "<<__LINE__<<std::endl;
	       if( myPreVec[ep] == 9999 )
		       return 0;
	        //std::cout<<"In "<<__FILE__<<"@ "<<__LINE__<<std::endl;
               LastID = myPreVec[ep];
	       //std::cout<<"In "<<__FILE__<<"@ "<<__LINE__<<std::endl;
               ep = LastID;
	       //std::cout<<"In "<<__FILE__<<"@ "<<__LINE__<<std::endl;
               before = cur;
	       //std::cout<<"In "<<__FILE__<<"@ "<<__LINE__<<std::endl;
            }
            else
                break;
        }
        path.push_back(src);
          
        distance += Distance[startNode.IDFromOne][before.IDFromOne];
        std::reverse(path.begin(),path.end());
        return distance;
    }
	// cite here:  https://blog.csdn.net/mimi9919/article/details/51219176
    double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TPathStep > &path)
    {   
        std::vector<TStopID>path_;
        if (src == dest)
        {
            path_.push_back(src);
            return 0.0;
        }
        size_t TotalNodes = myNodeVec.size();
        double time = 0;
        
	if(mymap.find(src) == mymap.end() ||mymap.find(dest) == mymap.end()) 
	{
		return 0;
	}
	
	Node startNode =  myNodeVec[mymap.find(src)->second - 1];
        Node endNode =  myNodeVec[mymap.find(dest)->second - 1];
        
        auto LastID = endNode.IDFromOne, StartID = startNode.IDFromOne,ep = endNode.IDFromOne;

        std::vector<double> myDistanceVec(TotalNodes + 9999, 9999);
        std::vector<unsigned int> myPreVec(TotalNodes+9999, 0);

	// therom: P(i,j)={Vi....Vk..Vs...Vj} is the shortest path from i to j, k and s are the nodes in the P(i,j), then  P(k,s) must be shortest path from k to s
	// since if there exists other shortest pah between k and s, then P(i,j) will not include node k and s 
        Dijkstra(TotalNodes, startNode.IDFromOne, myDistanceVec, myPreVec, Time);

        Node before,cur;
        
        before.id = -1;
        cur.id =-1;
	if(myDistanceVec.size() == 0 || myPreVec.size() == 0 )
		return 0;
        for(int i = 0; i < INT_MAX; i++)
        {   
            if(LastID != StartID) // they are keeping swaping 
            {  
               
	       if(!checkfunc(StartID,TotalNodes) || !checkfunc(LastID,TotalNodes))
	       {
                        break;

		 }
	       cur = myNodeVec[LastID - 1];
               path_.push_back(cur.id);
               if((int)before.id != -1 &&(int) cur.id != -1 )
                   time += Time[cur.IDFromOne][before.IDFromOne];
	       if(myPreVec[ep] == 9999)
		       return 0;
               LastID = myPreVec[ep]; //prevec will save all the node's from 1 to n 
               ep = LastID;
               before = cur;
	    }
            else
                break;
        }
        path_.push_back(src);
          
        time += Time[startNode.IDFromOne][before.IDFromOne];
        
        std::reverse(path_.begin(),path_.end());
	
	std::string str = "Walk",last = str;
	
	TPathStep temp;
	temp.first = str;
	temp.second = path_[0];
	path.push_back(temp);

       for(unsigned int i = 1; i < path_.size() - 1;i++)
        {   
		TPathStep pathStep;
		if(OnTheSameRoute(path_[i],path_[i+1]) == "Walk")
		{
			pathStep.first = last;
			pathStep.second = path_[i];
			path.push_back(pathStep);
			last = str;
		}
		else
		{	
			auto node = myNodeVec[mymap.find(path_[i])->second-1], node_ =  myNodeVec[mymap.find(path_[i+1])->second-1];
			std::string route = OnTheSameRoute(path_[i],path_[i+1]);
			pathStep.first = last;
			pathStep.second = path_[i];
			 path.push_back(pathStep);

			for(int j = node.IDFromOne + 1; j <(int) node_.IDFromOne;j++)
			{
				if (Distance[j][j+1] != 9999)
				{	
					pathStep.first = route;	
					pathStep.second =  myNodeVec[j - 1].id;
					 path.push_back(pathStep);
				}
			}
			last = route;
			
		}

        }

       if(last == "Walk")
       {	
	        temp.second = path_[path_.size() - 1];
		path.push_back(temp);	
       }
       else
       {
	       temp.first = last;
	       temp.second = path_[path_.size() - 1];
	       path.push_back(temp);
       }
        
        return time;
    }

    bool GetPathDescription(const std::vector< TPathStep > &path, std::vector< std::string > &desc) const
    {	   
	     
	     for(auto value :  path)
		     if(mymap.find(value.second) == mymap.end())
			     return false;

	     double a = myNodeVec[mymap.find(path[0].second)->second-1].myLatLon.first;
	     double b = myNodeVec[mymap.find(path[0].second)->second-1].myLatLon.second;
	     std::string str = "Start at " + convertMinSec(a) + "N, " +  convertMinSec(b) + "E";
	     desc.push_back(str);
	   
	  	 
	   for (unsigned int i = 1; i < path.size()- 1; i++) 
	    {
        	auto node = myNodeVec[mymap.find(path[i].second)->second-1];
	        auto node_ = myNodeVec[mymap.find(path[i+1].second)->second-1];
       	    	auto bearing = CalculateBearing(node.myLatLon.first, node.myLatLon.second, node_.myLatLon.first, node_.myLatLon.second);
	        double aa = node.myLatLon.first, bb = node.myLatLon.second;
	    	
		if(path[i].first == "Walk")
		{	
			auto str = "Walk " + getBearing(bearing);
			str += " to ";	
			desc.push_back(str + convertMinSec(aa) + "N, " +  convertMinSec(bb) + "E");		
	        }
	    	else
		{	
			auto route =  path[i].first;
			std::string strr =  "Take " + path[i].first + " and get off at stop ";
			while(i < path.size() - 1)
			{	
				if(path[i].first == route)
					i++;
				else
				{	
					//std::cout << "Break when i is " << i << "\n";
					break;	
			
				}
			}

			if(i >= path.size())
			{
				//std::cout << "Out of bound";
				break;
			}
			i =  i  - 1;
		
			for(auto value :  mystopsmap)
			{
				if(value.second == path[i].second)
				{	
					strr += std::to_string(value.first);
					desc.push_back(strr);
					break;
				}
			}

		}		
	    }
	    a  = myNodeVec[mymap.find(path[path.size()-1].second)->second-1].myLatLon.first;
	    b = myNodeVec[mymap.find(path[path.size()- 1].second)->second-1].myLatLon.second;
	    int i = path.size()-2;
	    auto node = myNodeVec[mymap.find(path[i].second)->second-1];
            auto node_ = myNodeVec[mymap.find(path[i+1].second)->second-1];
            auto bearing = CalculateBearing(node.myLatLon.first, node.myLatLon.second, node_.myLatLon.first, node_.myLatLon.second);
	    
	    if(path[path.size() - 1].first == "Walk")
            {
            	auto str = "Walk " + getBearing(bearing);
                str += " to ";  
                desc.push_back(str + convertMinSec(a) + "N, " +  convertMinSec(b) + "E");             
            }

	    
	    str = "End at " + convertMinSec(a) + "N, " +  convertMinSec(b) + "E";
	    desc.push_back(str);
	    desc.push_back(str);

       return true; 
    }



};



CMapRouter::CMapRouter() : DImplementation(std::make_unique<CImplementation>()){

}

CMapRouter::~CMapRouter(){

}

double CMapRouter::HaversineDistance(double lat1, double lon1, double lat2, double lon2){
    auto DegreesToRadians = [](double deg){return M_PI * (deg) / 180.0;};
    double LatRad1 = DegreesToRadians(lat1);
    double LatRad2 = DegreesToRadians(lat2);
    double LonRad1 = DegreesToRadians(lon1);
    double LonRad2 = DegreesToRadians(lon2);
    double DeltaLat = LatRad2 - LatRad1;
    double DeltaLon = LonRad2 - LonRad1;
    double DeltaLatSin = sin(DeltaLat/2);
    double DeltaLonSin = sin(DeltaLon/2);   
    double Computation = asin(sqrt(DeltaLatSin * DeltaLatSin + cos(LatRad1) * cos(LatRad2) * DeltaLonSin * DeltaLonSin));
    const double EarthRadiusMiles = 3959.88;
    
    return 2 * EarthRadiusMiles * Computation;

}       

double CMapRouter::CalculateBearing(double lat1, double lon1,double lat2, double lon2){
    auto DegreesToRadians = [](double deg){return M_PI * (deg) / 180.0;};
    auto RadiansToDegrees = [](double rad){return 180.0 * (rad) / M_PI;};
    double LatRad1 = DegreesToRadians(lat1);
    double LatRad2 = DegreesToRadians(lat2);
    double LonRad1 = DegreesToRadians(lon1);
    double LonRad2 = DegreesToRadians(lon2);
    double X = cos(LatRad2)*sin(LonRad2-LonRad1);
    double Y = cos(LatRad1)*sin(LatRad2)-sin(LatRad1)*cos(LatRad2)*cos(LonRad2-LonRad1);
    return RadiansToDegrees(atan2(X,Y));
}

bool CMapRouter::LoadMapAndRoutes(std::istream &osm, std::istream &stops, std::istream &routes){
    // Your code HERE
   return  DImplementation->LoadMapAndRoutes(osm, stops, routes);
}

size_t CMapRouter::NodeCount() const{
    // Your code HERE
    return DImplementation->NodeCount();
}


CMapRouter::TNodeID CMapRouter::GetSortedNodeIDByIndex(size_t index) const{
    // Your code HERE
    return DImplementation-> GetSortedNodeIDByIndex(index);
}

CMapRouter::TLocation CMapRouter::GetSortedNodeLocationByIndex(size_t index) const{
    // Your code HERE
    return DImplementation->GetSortedNodeLocationByIndex(index);
}

CMapRouter::TLocation CMapRouter::GetNodeLocationByID(TNodeID nodeid) const{
    // Your code HERE
    return DImplementation->GetNodeLocationByID(nodeid);
}

CMapRouter::TNodeID CMapRouter::GetNodeIDByStopID(TStopID stopid) const{
    // Your code HERE
    return DImplementation->GetNodeIDByStopID(stopid);
}

size_t CMapRouter::RouteCount() const{
    // Your code HER
        return DImplementation->RouteCount();
}

std::string CMapRouter::GetSortedRouteNameByIndex(size_t index) const{
    // Your code HERE
    return DImplementation->GetSortedRouteNameByIndex(index);
}
bool CMapRouter::GetRouteStopsByRouteName(const std::string &route, std::vector< TStopID > &stops){
    // Your code HER
     return DImplementation->GetRouteStopsByRouteName(route, stops);
}

double CMapRouter::FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path){
    // Your code HERE
    return DImplementation->FindShortestPath(src, dest, path);

}

double CMapRouter::FindFastestPath(TNodeID src, TNodeID dest, std::vector< TPathStep > &path){
    // Your code HERE
    return DImplementation->FindFastestPath(src, dest,path);
}

bool CMapRouter::GetPathDescription(const std::vector< TPathStep > &path, std::vector< std::string > &desc) const{
    // Your code HERE
	return DImplementation->GetPathDescription(path, desc);
}


// cite here:  https://blog.csdn.net/mimi9919/article/details/51219176
void Dijkstra(int n, unsigned int v, std::vector<double> &dist, std::vector<unsigned int> &pre,std::vector<std::vector<double>> &d)
{
    int maxdist = 9999;
    std::vector<bool> s(n+1); // this is used to set nodes  has been visited or not
    for (int i = 1; i <= n;i++)
    {
        dist[i] = d[v][i];
        if (dist[i] < maxdist) // find valid distance in s 
            pre[i] = v; // 
        else
            pre[i] = 0;
    }

    dist[v] = 0;
    s[v] = true; // start node is visited 
    for (int i = 2; i <= n;i++)
    {
        int best = v;
        int temp = maxdist;
        for (int j = 1; j <= n;j++) // go throught all the nodes and need to see the minimum distance from 1 to n 
        {
            if (!s[j]&&dist[j]<temp)  // if it is not and distance is  < temp_best, then set it is equal to temp best 
            {
                temp = dist[j];
                best = j;
            }
        }
        s[best] = true; // set we've found the best/smallest that has minimum cost 
        for (int j = 1; j <= n;j++)
        {   
            if (!s[j] && d[best][j] != maxdist) // make sure edge exists
            {
                int newdist = dist[best] + d[best][j]; // rest is keeping the minimum cost 
                if (newdist<dist[j])
                {
                    dist[j] = newdist;
                    pre[j] = best;
                }

            }
        }
    }
}





