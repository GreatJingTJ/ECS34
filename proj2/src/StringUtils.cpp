#include "StringUtils.h"
#include <algorithm> 
#include <cctype>
#include <cstdio>
#include<sstream>

int min(int a, int b, int c)
{

    return std::min(std::min(a,b),c);
}

namespace StringUtils{
    
std::string Slice(const std::string &str, ssize_t start, ssize_t end){
    // Your code goes here
    int length = 0;
    int str_length = str.size();

    if(start > str_length)
        return "";
   
    if(!end)
    {   
        return str.substr(start);
    }
    else
    {   
        if((start < 0 || end < 0) && (start*end <= 0))
            length = str_length - (abs(start) + abs(end));
        else
            length = end - start;
        if(start >= 0)
            return str.substr(start, length);
        else
            return str.substr(str.length()+start,length);
    }
    
}

std::string Capitalize(const std::string &tmp){
    // Your code goes here
    std::string str = tmp;

    if(isspace(str[0]))
      for(unsigned int i = 0; i < str.length(); i++)
          str[i] = tolower(str[i]);
    else
    {
      str[0] = toupper(str[0]);
      if(str.length() > 1)
        for(unsigned int i = 1; i< str.length();i++)
          str[i] = tolower(str[i]);
    }
    return str;
}

std::string Title(const std::string &tmp){
    // Your code goes here
    std::string str = tmp;

    for(unsigned int i = 1; i< str.length();i++)
        str[i] = tolower(str[i]);
    
    str[0] = toupper(str[0]);
        
    for(unsigned int i = 0; i < str.length();i++)
    {
        if(isspace(str[i]))
        {
                if(i + 1 < str.length() && islower(str[i+1]))
                    str[i+1] = toupper(str[i+1]);
        }
        else
        {
            if(!isalpha(str[i]))
                if(i+1 < str.length() && islower(str[i+1]))
                    str[i+1] = toupper(str[i+1]);
        }
    }
    
    return str;
}

std::string LStrip(const std::string &tmp){
    // Your code goes here
    std::string str = tmp;
    std::string val = "";
    for(unsigned  int i = 0; i < str.length();i++)
    {
      if(isspace(str[i]))
          val = str.substr(i+1);
      else
          break;
    }
    return val;
}

std::string RStrip(const std::string &tmp){
    // Your code goes here
    std::string str = tmp, val = "";

    std::reverse(str.begin(), str.end());


    for(unsigned int i = 0; i < tmp.length();i++)
    {
        if(isspace(str[i]))
            val = str.substr(i+1);
        else
            break;
    }
    std::reverse(val.begin(), val.end());


    return val;
}

std::string Strip(const std::string &str){
    return StringUtils::RStrip(StringUtils::LStrip(str));
}

std::string Center(const std::string &str, int width, char fill){
    // Your code goes here
    std::string add_one = "";
    std::string add_two = "";
    int length1 = (width-str.length())/2;
    int length2 = width - length1 - str.length();
  
    for(int i = 0; i < length1; i++)
        add_one += fill;

    for(int i = 0; i < length2; i++)
        add_two += fill;

    return add_one + str + add_two;
}

std::string LJust(const std::string &str, int width, char fill){
    // Your code goes here
    std::string add_one = "";
  
    for(unsigned int i = 0; i < width - str.length(); i++)
        add_one += fill;
    return str+add_one;
}

std::string RJust(const std::string &str, int width, char fill){
    // Your code goes here
    std::string add_one = "";
  
    for(unsigned int i = 0; i < width-str.length(); i++)
        add_one += fill;
    return add_one+str;
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep){
    // Your code goes here
    std::string tmp = str;
    size_t found = str.find(old);

    while(found != std::string::npos)
    {
        tmp = tmp.substr(0,found) + rep + tmp.substr(found + old.length());
        found = tmp.find(old,found+rep.length());
    }
    return tmp;
}

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

std::string Join(const std::string &str, const std::vector< std::string > &vect){
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

std::string ExpandTabs(const std::string &tmp, int tabsize){
    // Your code goes here
    std::string str = tmp,added = "";
    size_t found = str.find("\t");
    unsigned int z = tabsize, n = 0;
    
    while(found != std::string::npos)
    {   
        if(tabsize)
        {
            added += str.substr(0,found);
            if(str.substr(0,found).size() >= z)
            {
                n = str.substr(0,found).size() / z;
                z = (n + 1) * z - str.substr(0,found).size();
            }
            else
                z -= str.substr(0,found).size();
    
            for(unsigned int i = 0; i < z; i++)
            {
                added += " ";
            }
    
            str = str.substr(found+1);
            found = str.find("\t");
            z = tabsize;
        }
        else
        {
            added += str.substr(0,found);
            str = str.substr(found+1);
            found = str.find("\t");
        }
    }   
    added += str;

    return added;
}


int EditDistance(const std::string &left, const std::string &right, bool ignorecase){
    // Your code goes here
    //code from https://www.geeksforgeeks.org/edit-distance-dp-5/
    std::string str1 = left, str2 = right;

    if(ignorecase)
    {   
        str1 = "";
        str2 = "";
        for(unsigned int i = 0; i < left.length();i++)
            str1 += tolower(str1[i]);
        for(unsigned int i = 0; i < right.length();i++)
            str2 += tolower(str2[i]);
    }
    int m = left.length();
    int n = right.length();
    int dp[m+1][n+1];
  

    for(int i = 0; i <= m;i++)
        for(int j = 0; j <= n; j++)
        {
            if(i == 0)
                dp[i][j] = j;
            else if(j == 0)
                dp[i][j] = i;
            else if(str1[i-1] == str2[j-1])
                dp[i][j] = dp[i-1][j-1];
            else
                dp[i][j] = 1 + min(dp[i][j-1],dp[i-1][j],dp[i-1][j-1]);
        }
    return dp[m][n];
}

}
