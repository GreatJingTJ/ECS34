#include <iostream>
#include "DirectoryListing.h"
#include "FileSystemTree.h"
#include <sstream>
#include "StringUtils.h"
#include<algorithm>
// source from: https://www.geeksforgeeks.org/program-extract-words-given-string/
std::vector< char > TempData;

std::vector< std::string > Split(const std::string &str, const std::string &split){
    // Your code goes here
    //source: https://www.geeksforgeeks.org/program-extract-words-given-string/
    std::vector<std::string>vec_str;
    std::string tmp = str,tmp2 = "";
    size_t found = tmp.find(split);

    if(split == "")
    {   

        std::stringstream getWords(tmp);

        while(getWords >> tmp2)
            vec_str.push_back(tmp2);
    }
    else
    {
        while(found != std::string::npos)
        {
            vec_str.push_back(tmp.substr(0,found));
            tmp = tmp.substr(found+1);
            found = tmp.find(split);
        }
        vec_str.push_back(tmp);
    }
    return vec_str;
    
}


std::string extractString(std::string line)
{
	std::string str = "", tmp = "";

	std::stringstream get(line);

	while(get >> tmp)
		str += tmp;
	return str;
}

std::string extracttwoString(std::string line)
{
    std::string str = "", tmp = "";

    std::stringstream get(line);

    while(get >> tmp)
    {
        if(tmp[0] == '/')
            tmp = tmp.substr(1);
        str += tmp + "+";
    }
    

    return str.substr(0,str.size()-1);
}


std::string Join(std::string str,std::vector< std::string > vect){
    // Your code goes here
    std::string tmp = "";

    for(unsigned int i = 0;i < vect.size() - 1; i++)
    {
        tmp += vect[i];
        tmp += str;
    }
    tmp += vect[vect.size()-1];
    return tmp;
}
std::string reWritePath(std::string line)
{	
	std::string extractLine = "", result = "";
	extractLine = extractString(line);
    if(line[0] == '/')
        line = line.substr(1);
    std::vector<std::string> path = Split(extractLine, "/");
    result = Join("/", path);

    return result;
}


int main(int argc, char *argv[]){
    // Your code here
    // Dr.CjNitta's discusstion
    CFileSystemTree tree;
    auto iter = tree.Root().begin();

    if(argc >= 2)
    {
    	for(int i = 0; i < argc;i++)
    	{
    		std::cout << argv[i] << std::endl;
    	}
    }

    bool Done = false;

    std::string CurrentDir = "/";

    while(!Done)
    {
    	std::string line, path = "";
    	std::cout << "> ";
    	std::getline(std::cin, line);

    	if(line == "exit")
    		Done = true;
    	else if (line == "pwd")
        {
            std::cout << "/";
    		std::cout << iter->FullPath() << std::endl;
    	}
        else if(line.substr(0,2) == "cd")
    	{	
            if(line.size() == 2)
            {
                iter = tree.Root().begin();
                continue;
            }
           // auto fulpath = iter->FullPath();
            /*std::reverse(fulpath.begin(), fulpath.end());
            for(int i = 0; i < fulpath.size();i++)
                if(fulpath[i] == '/')
                {
                    fulpath = fulpath.substr(i+1);
                    break;
                }
            std::reverse(fulpath.begin(), fulpath.end());
            std::cout << "ls path:" <<  fulpath << std::endl;*/
            //iter = tree.Find(iter->FullPath());
           // std::cout << "path" << std::endl;
    		path = reWritePath(line.substr(3));
    
            auto check = tree.Find(path);
            //std::cout << "cd path" << path << std::endl;
    		if(check == tree.NotFound())
    			std::cout << "Unknown directory: " << path << std::endl;
    		else
    		{
    			iter = check;
    		}
            iter = iter->begin();
    	}
    	else if(line.substr(0,2) == "ls")
    	{   
            /*std::string fullpath = iter->FullPath();

            std::cout << "fullpath before: " << fullpath << std::endl;

            auto tmp = tree.Root().begin();
            if(fullpath != "")
            {   
                fullpath += "/";
                std::reverse(fullpath.begin(), fullpath.end());
                for(int i = 0; i < fullpath.size();i++)
                    if(fullpath[i] == '/')
                    {
                        fullpath = fullpath.substr(i+1);
                        break;
                    }
                std::reverse(fullpath.begin(), fullpath.end());
                std::cout << "fullpath after: " << fullpath << std::endl;
                tmp = tree.Find(fullpath);
                tmp = tmp->begin();
            }*/

            //std::cout << "fullpath after: " << fullpath << std::endl;

            if(line.size() == 2)
            {   
                auto tmp = tree.Root().begin();
                if(iter->Parent().Name().empty() == true){
                    tmp = tree.Root().begin();
                }
                else
                {   
                    auto fullpath = iter->FullPath();
                    std::reverse(fullpath.begin(), fullpath.end());
                    for(unsigned int i = 0; i < fullpath.size();i++)
                        if(fullpath[i] == '/')
                        {
                            fullpath = fullpath.substr(i+1);
                            break;
                        }
                    std::reverse(fullpath.begin(), fullpath.end());
                    //std::cout << "ls path:" <<  fullpath << std::endl;
                    tmp = tree.Find(fullpath)->begin();
                    //tmp = iter;
                }
                
                while(!tmp->Name().empty())
                {   
                    std::cout << tmp->Name() << std::endl;
                    tmp++;
                }
                
                


            }
            else
            {   
    		    path = reWritePath(line.substr(3));
                auto tmp = tree.Find(path);
                if(tmp != tree.NotFound())
                {
                    while(tmp != tmp->end())
                    {   
                        std::cout << tmp->Name() << std::endl;
                        tmp++;
                    }
                }
                else
                    std::cout << "Unknown directory: " << path << std::endl;
            }
    	}
    	else if(line.substr(0,3) == "cat")
    	{
    		if(line.size() == 3)
                std::cout << "Error cat: missing parameter" << std::endl;
            else
            {
                path = reWritePath(line.substr(4));
            }

    	}
    	else if(line.substr(0,5) == "mkdir")
    	{  
            bool flag = false;
    		path = reWritePath(line.substr(6));

            if(line.size() ==  5)
            {
                std::cout << "Error mkdir: missing parameter" << std::endl;
                continue;
            }

            for(unsigned int i = 6; i < line.size();i++)
            {
                if(!isspace(line[i]))
                    flag = true;
            }

            if(!flag)
            {
                std::cout << "Error mkdir: failed to make directory " << path << std::endl;
                continue;
            }


            //std::cout << "path: " << path << std::endl;
            if(!iter->FullPath().empty()){
                auto fullpath = iter->FullPath();
            
                std::reverse(fullpath.begin(), fullpath.end());
                for(unsigned int i = 0; i < fullpath.size();i++)
                    if(fullpath[i] == '/')
                    {
                        fullpath = fullpath.substr(i+1);
                        break;
                    }
                std::reverse(fullpath.begin(), fullpath.end());
                
                //std::cout << "fullpath after: " << fullpath << std::endl;
                path = fullpath + "/" + path;
            }

    
            //std::cout << "path: " << path << std::endl;
            if(path[0]== '/')
                path = path.substr(1);

            if(path.find("/"))
                flag = true;
            else
                flag  = false;

    		if(!tree.Root().AddChild(path, flag))
    		{
    			std::cout << "Error mkdir2: failed to make directory " << path << std::endl;
    		}

    	}
        else if(line.substr(0,2) == "rm")
        {
            auto tmp = tree.Root().begin();
            if(iter->Parent().Name().empty() == true){
                    iter = tree.Root().begin();
            }
            else
            {   
                auto fullpath = iter->FullPath();
                std::reverse(fullpath.begin(), fullpath.end());
                for(unsigned int i = 0; i < fullpath.size();i++)
                    if(fullpath[i] == '/')
                    {
                        fullpath = fullpath.substr(i+1);
                        break;
                    }
                    std::reverse(fullpath.begin(), fullpath.end());
                    std::cout << "ls path:" <<  fullpath << std::endl;
                    tmp = tree.Find(fullpath)->begin();
                    //tmp = iter;
            }
                path = reWritePath(line.substr(3));
                //std::cout << :
                //std::cout << "rm Path: " << path << std::endl;
                tmp = tmp->Find(path);

                if(tmp != tree.NotFound())
                    iter->RemoveChild(path);
                else
                    std::cout << "Unknown file or directory: " << path << std::endl;

            
        }
        else if (line.substr(0,2) == "cp")
        {   
            std::string src = "", dest = "";

            line = extracttwoString(line.substr(3));
            std::vector<std::string> path = Split(line, "+");

            if(path.size() <= 1)
                std::cout << "Error cp: missing parameter(s)\n";
            else if (path.size()>= 3)
                std::cout << "Unknown file or directory: " << path[0] << std::endl;
            else
            {
                src = path[0];
                dest = path[1];

                if (iter->Find(src) == tree.NotFound() or iter->Find(src) == tree.NotFound())
                {
                    std::cout << "Error cp: failed to copy " << src;
                    std::cout << " to " << dest << std::endl;
                }
                //else
                // check whether the src or des is a file // it both use getdata func form src
                // add a new file the same name addchild func and say 
            }
        }   
        else if(line.substr(0,2) == "mv")
        {
            // should do the samething as cp but need to deleted the src file
        }

        else if(line.substr(0,4) == "tree")
        {

            if(line.size() == 4)
            {
                std::cout << std::string(tree) << "\n";
                continue;
            }
            line = extractString(line.substr(5));

            /*line = extractString(line.substr(5));
            auto iter1 = iter->Find(line);

            if(iter1 == tree.NotFound())
                std::cout << "Unknown directory: " << line;*/
        }
        else
        {
            std::cout << "Unknown command: " << line << "\n";
            continue;
        }


    }


    return EXIT_SUCCESS;
}

