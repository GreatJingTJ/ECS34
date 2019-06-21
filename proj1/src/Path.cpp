#include "Path.h"
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include<iostream>
#include<cctype>
#include "StringUtils.h"

struct CPath::SImplementation{
    // Your code goes here
    std::string path;
};

CPath::CPath() : DImplementation(std::make_unique< SImplementation >()){
    // Your code goes here
    this->DImplementation->path = "";
}

CPath::CPath(const CPath &path1) : DImplementation(std::make_unique< SImplementation >()){
    // Your code goes here
    this->DImplementation->path = path1.DImplementation->path;
}

CPath::CPath(const std::string &path1): DImplementation(std::make_unique< SImplementation >()) {
    // Your code goes here
    this->DImplementation->path = path1;
}

CPath::~CPath(){
    // Your code goes here
}

CPath &CPath::operator=(const CPath &path){
    // Your code goes here
    this->DImplementation->path = path.DImplementation->path;
    return *this;
}

CPath CPath::operator+(const CPath &path) const{
    // Your code goes here
    std::string str = this->DImplementation->path + "/"+path.DImplementation->path;
    CPath value(str);
    return value;
}

bool CPath::operator==(const CPath &path) const{
    // Your code goes here
    std:: string str1 = this->DImplementation->path, str2 = path.DImplementation->path;

    if(!str1.compare(str2))
        return true;
    else
        return false;
}

bool CPath::operator!=(const CPath &path) const{
    // Your code goes here
    std:: string str1 = this->DImplementation->path, str2 = path.DImplementation->path;
    if(str1.compare(str2))
        return true;
    else
        return false;
}

CPath CPath::Directory() const{
    // Your code goes here
    std:: string str = this->DImplementation->path,str2 = "";
   
    if(str.empty())
        str2 = "";
    size_t found = str.find("/");

    if(str.substr(found+1).find("/"))
    {   

        found += str.substr(found+1).find("/");
    }
    str2 = str.substr(0,found+1);

    CPath value(str2);
    return value;  
}


std::string CPath::BaseName() const{
    // Your code goes here
    std:: string str = this->DImplementation->path,str2;
    if(str.empty())
        str2 = "";
    size_t found = str.find("/");
    if(str.substr(found+1).find("/"))
    {   
        str = str.substr(found+1);
        found = str.find("/");
    }
    str2 = str.substr(found+1);
    return str2;
}

std::string CPath::Extension() const{
   // Your code goes here
    std::string str = this->DImplementation->path;
    std::string str2 = str;

    std::reverse(str.begin(),str.end());
    bool flag = true,found = false;

    for(unsigned int i = 0; i < str.size(); i++)
    {
        if(str[i] == '.')
        {   
            found = true;
            for(unsigned int j = 0; j < i;j++)
            {
                if(!isalpha(str[j]))
                    flag = false;
            }
            if(flag)
                str2 = str2.substr(str.size()-i-1);
            else
                str2 = "";
            break;
        }

        if( i == str.size() - 1 && !found)
            str2 = "";
    }
    return str2;
}

bool CPath::IsAbsolute() const{
    // Your code goes here
    std::string str = this->DImplementation->path;

    if(str[0] == '/')
        return true;
    else
        return false;
}

std::string CPath::ToString() const{
    // Your code goes here
    return this->DImplementation->path;
}

CPath::operator std::string() const{
    // Your code goes here
    return this->DImplementation->path;
}

CPath CPath::AbsolutePath() const{
    // Your code goes here
    std::string str = this->DImplementation->path;
    CPath value(str);
    if(value.IsAbsolute())
        str = value.NormalizePath().DImplementation->path;
    else
        str = GetCWD() + str;

    CPath result(str);
    return result.NormalizePath();

}

CPath CPath::CommonPath(const CPath &path) const{
    // Your code goes here
    std::string str1 = this->DImplementation->path, str2 = path.DImplementation->path, short_str = "", long_str = "";
    std::string commonstring = "";

    int length1 = str1.length(), length2 = str2.length();

    if(length1 > length2)
    {
        short_str = str2;
        long_str = str1;
    }
    else
    {
        short_str = str1;
        long_str = str2;
    }

    size_t found = short_str.find("/"), mark_pos;
    mark_pos = found;

    while(found != std::string::npos)
    {
        found = short_str.find("/", found + 1);

        if(short_str.substr(mark_pos, found) == long_str.substr(mark_pos, found))
        {
            commonstring = short_str.substr(mark_pos, found);
        }
        else
            break;

    }

    CPath value(commonstring);

    return value;
}

CPath CPath::NormalizePath() const{
    // Your code goes here
    std::string str = this->DImplementation->path, str_return = "";
   
    std::vector<std::string> vec_str;
    vec_str = StringUtils::Split(str, "/");
    
    for(unsigned int i = 0; i < vec_str.size();i++)
    {   
        if(vec_str[i] == ".")
            vec_str.erase(vec_str.begin() + i);
        else if(vec_str[i] == "..")
        {
            if(i == 0)
               continue;
            else
            {   
                vec_str.erase(vec_str.begin() + i);
                vec_str.erase(vec_str.begin() + i-1);
            }
        }
    }

    for(unsigned int i = 0; i < vec_str.size() - 1; i++)
    {   
        if(i == 0 && vec_str[i].empty())
        {
            str_return += "/";
        }
        else if(!vec_str[i].empty())
        {
            str_return += vec_str[i];
            str_return += "/";
        }
        else if(vec_str[i].empty() && i != 0)
        {
            continue;
        }

    }    
    str_return += vec_str[vec_str.size()-1];

    CPath value(str_return);


    return value;
}

CPath CPath::RelativePathTo(const CPath &path) const{
    // Your code goes here
    int i = 0;
    std::string str = this->DImplementation->path, str2 = path.DImplementation->path,str_copy = "";
    std::string str3 = "", tmp = "";

    CPath path1(str), path2(str2);

    str = path1.NormalizePath().DImplementation->path;
    str2 = path2.NormalizePath().DImplementation->path;
    str_copy = str;

    CPath Path1(str), Path2(str2);

    if(str.size() < str2.size())
    {
        tmp = str;
        str = str2;
        str2 = tmp;
    }


    if(str.substr(0,str2.size()) == str2)
        str3 = "." + str.substr(str2.size());

    else
    {
        CPath path3 = Path2.CommonPath(Path1);
        
        str3 = path3.DImplementation->path;
        int length = str3.size();
        if(str2.find("/", length+1) == std::string::npos)
            str3 = "../" + str.substr(length+1);

        else
        {            
            size_t found = str3.find("/");

            while(found != std::string::npos)
            {
                i++;
                found = str3.find("/", found + 1);
            }
    
            str3 = "";
    
            for(int j = 0; j < i; j++)
                str3 += "../";
            str3 += str.substr(length + 1);
    
        }
    }
    CPath value(str3);
    return value;
}

// Helper function provided to get the current working directory
std::string CPath::GetCWD(){
    std::vector< char > CurrentPathName;

    CurrentPathName.resize(1024);
    while(NULL == getcwd(CurrentPathName.data(), CurrentPathName.size())){
        CurrentPathName.resize(CurrentPathName.size() * 2);
    }
    return CurrentPathName.data();
}

// Helper funciton provided to get the home directory
std::string CPath::GetHome(){
    return std::getenv("HOME");
}

CPath CPath::AbsolutePath(const CPath &path){
    std::string str = path.DImplementation->path,str2 = "";

    CPath value(str);

    return  value.AbsolutePath();
}

CPath CPath::CurrentPath(){
    // Your code goes here
    std::string str = GetCWD();
    CPath value(str);

    return value;
    
}

CPath CPath::CommonPath(const CPath &path1, const CPath &path2){
    // Your code goes here
    CPath path3 = path1.CommonPath(path2);
    return path3;
}

CPath CPath::ExpandUserPath(const CPath &path){
    // Your code goes here

    if(path.DImplementation->path[0] != '~')
        return path;
    std::string str = path.HomePath() + path.DImplementation->path;
    std::string tmp = "";
    std::string old = "//", rep = "/";
    for(unsigned int i  = 0; i < str.size(); i++)
    {
        if(str[i] != '~' &&  !isspace(str[i]) )
            tmp += str[i];
    }

    size_t found = tmp.find(old);

    while(found != std::string::npos)
    {
        tmp = tmp.substr(0,found) + rep +tmp.substr(found+old.size());
        found = tmp.find(old,found + rep.size());
    }
    CPath result(tmp);
    return result;
}

CPath CPath::HomePath(){
    // Your code goes here
    std::string str2 = GetHome();

    CPath result(str2);

    return result;
}

CPath CPath::NormalizePath(const CPath &path1){

    std::string str = path1.DImplementation->path, tmp = "";
   
    
    CPath value(str);

    return value.NormalizePath();

}

CPath CPath::RelativePath(const CPath &path, const CPath &startpath){
    // Your code goes here
    std::string str = startpath.DImplementation->path, str2 = path.DImplementation->path;
    
    CPath path1(str), path2(str2);

    str = path1.NormalizePath().DImplementation->path;
    str2 = path2.NormalizePath().DImplementation->path;

    CPath Path1(str), Path2(str2);

    CPath value = Path1.RelativePathTo(Path2);
    
    return value;
}
