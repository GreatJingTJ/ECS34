#include "FileSystemTree.h"
#include<iostream>
#include"StringUtils.h"
#include<algorithm>
#include<string>
//infor: addchildren test and empty test work
// cannot use begin() and end()implementing the begin func
// you may just seach thisisbegin/thisisend/thisis++/thisisnameto find where my function is 

CFileSystemTree::CEntry empty;
CFileSystemTree::CEntryIterator Empty;

class Node:std::enable_shared_from_this<Node>{
    private:
        std::string val;
    public:
      Node()
      {
        val = "";
      }
      Node(std::string data)
      {
          val = data;
      }
      void setVal(std::string data)
      {
          val = data;
      }    
      std::string name = val;
      std::vector<std::shared_ptr<Node>>Children;
      std::vector<char> data;
      std::weak_ptr<Node> Parent;
      CFileSystemTree::CEntry entry;
};
std::vector<std::shared_ptr<Node>> emptyVec;

bool Compare(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
{
    return left->name < right->name;
}
// from nitta
std::string toStringHelper(std::shared_ptr<Node> node, std::vector<bool> ancestor)
{
    std::string str = "";

    if(ancestor.size())
    {
        for(unsigned int i = 0; i < ancestor.size() - 1; i++)
        {
            if(ancestor[i])
                str += "|  ";
            else
                str += "   ";
        }
        if(ancestor.back())
            str += "|--";
        else
            str += "`--";
    }

    str += node->name;

    str +="\n";

    for(unsigned int i = 0;  i < node->Children.size();i++)
    {
        if(i != node->Children.size() - 1)
            ancestor.push_back(true);
        else
            ancestor.push_back(false);
        str += toStringHelper(node->Children[i],ancestor);
        ancestor.pop_back();
    }
    return str;
}

struct CFileSystemTree::SImplementation{
    // You implementation here
    std::shared_ptr<Node> root;
};

struct CFileSystemTree::CEntry::SImplementation{
    // You implementation here
   std::weak_ptr<Node> weakPtr;
};

struct CFileSystemTree::CEntryIterator::SImplementation{
    // You implementation here
    std::shared_ptr<Node> iteratorPtr;
    std::shared_ptr<Node> Previous;
    unsigned int index;
    bool isEnd = false;
};

struct CFileSystemTree::CConstEntryIterator::SImplementation{
    // You implementation here
    std::shared_ptr<Node> iteratorPtr;
    std::shared_ptr<Node> Previous;
    unsigned int index;
    bool isEnd = false;
};

CFileSystemTree::CEntry::CEntry() : DImplementation(std::make_unique< SImplementation >()){
    // You code here

}

CFileSystemTree::CEntry::CEntry(const CEntry &entry) : DImplementation(std::make_unique< SImplementation >()){
    // You code here
    DImplementation->weakPtr = entry.DImplementation->weakPtr.lock();
}

CFileSystemTree::CEntry::~CEntry(){
    // You code here
}

CFileSystemTree::CEntry &CFileSystemTree::CEntry::operator=(const CEntry &entry){
    // You code here

    DImplementation->weakPtr = entry.DImplementation->weakPtr.lock();
    return *this;

}

bool CFileSystemTree::CEntry::Valid() const{
    // You code here
    bool flag = false;
    if(auto tmpPtr = DImplementation->weakPtr.lock())
        flag = true;
    return flag;
}

std::string CFileSystemTree::CEntry::Name() const{
    // You code here
    //thisisname
    if(auto tmp = DImplementation->weakPtr.lock())
        return tmp->name;
    else
        return "";
}

std::string CFileSystemTree::CEntry::FullPath() const{
    // You code here
    std::string fullpath = "";
   
    if(auto tmp = DImplementation->weakPtr.lock())
    {   
        fullpath = tmp->name;
        while(1)
        {   
            if(auto Parent = tmp->Parent.lock())
            {
                fullpath = Parent->name + "/" +fullpath;
                if(tmp = Parent->Parent.lock())
                {   
                    tmp = Parent;
                    continue;
                }
                else
                    break;
            }
            else
                break;
        }
    }
    fullpath = "/" + fullpath;

    if(fullpath[0] == '/' && fullpath[1] == '/')
        fullpath = fullpath.substr(1);

    return fullpath;


}

std::string CFileSystemTree::CEntry::ToString() const{
    // You code here
    std::string str = "/";
    if(auto tmp = DImplementation->weakPtr.lock())
    {
        std::vector<bool> v;
        str += toStringHelper(tmp,v);
    }
    return str.substr(0,str.size()-1);
}

CFileSystemTree::CEntry::operator std::string() const{
    // You code here
    std::string str = "/";
    if(auto tmp = DImplementation->weakPtr.lock())
    {
        std::vector<bool> v;
        str += toStringHelper(tmp,v);
    }
    return str.substr(0,str.size()-1);
}

bool CFileSystemTree::CEntry::Rename(const std::string &path){
    // You code here
    bool flag = true;
    if(auto tmpPtr = DImplementation->weakPtr.lock())
    {
        if(auto Parent = tmpPtr->Parent.lock())
        {
            for(auto child: Parent->Children){
                if(child->name == path){
                    flag = false;
                }
            }
        }
        tmpPtr->name = path;
        if(auto Parent = tmpPtr->Parent.lock())
        {
            std::sort(Parent->Children.begin(), Parent->Children.end(),Compare);
        }
    }
    return flag;

}

size_t CFileSystemTree::CEntry::ChildCount() const{
    // You code here
    size_t count = 0;
    if(auto tmpPtr = DImplementation->weakPtr.lock())
    {
        count = tmpPtr->Children.size();
    }
    return count;

}

bool CFileSystemTree::CEntry::SetChild(const std::string &name, CEntryIterator &iter){
    // You code here
    bool flag = true;

    std::string copy_path  = iter->FullPath();
    //std::string current. = fullpath;
    std::reverse(copy_path.begin(), copy_path.end());

    for(unsigned int i = 0; i < copy_path.size(); i++)
    {
        if(copy_path[i] == '/')
        {
            copy_path = copy_path.substr(i + 1);
            break;
        }
    }
    std::reverse(copy_path.begin(), copy_path.end());

    //std::cout << "Path: " << fullpath << "\n";
    auto root = DImplementation->weakPtr.lock();

    if(auto tmpPtr = DImplementation->weakPtr.lock())
    {
        while(1)
        {
            if(auto Parent = tmpPtr->Parent.lock())
            {
                root = Parent;
                if(auto tmp = Parent->Parent.lock())
                    tmpPtr = Parent;
                else
                    break;
            }
            else
                break;
        }
    }

    auto iterTemp = root->entry.Find(copy_path);
    
    if(iterTemp != Empty)
    {   
        if(!iterTemp->RemoveChild(iter->Name()))
            flag = false;
    }
    else
        flag = false;
    
    if(auto tmp = DImplementation->weakPtr.lock())
    {
        for(unsigned int i = 0; i < tmp->Children.size();i++)
        {
            if(tmp->Children[i]->name == name)
            {
                flag = false;
                break;
            }
        }
        if(flag)
        {   
            CEntry a;
            std::shared_ptr<Node>ptr = std::make_shared<Node>();
            //create ptr
            ptr->name = name;
            tmp->Children.push_back(ptr);
            //push the ptr
            a.DImplementation->weakPtr = tmp->Children[tmp->Children.size() - 1];
            tmp->Children[tmp->Children.size() - 1]->Parent = tmp;
            tmp->Children[tmp->Children.size() - 1]->entry = a;
            iter.DImplementation->iteratorPtr->Parent = tmp->Children[tmp->Children.size() - 1];
            
            std::string name = tmp->Children[tmp->Children.size() - 1]->entry.FullPath() + "/";
           
            auto current = iter.DImplementation->iteratorPtr;
            for(unsigned int i = 0; i < iter.DImplementation->iteratorPtr->Children.size();i++)
            {
                root->entry.AddChild(name + iter.DImplementation->iteratorPtr->Children[i]->name, true);
            }
        }         
    }



    return flag;
}

bool CFileSystemTree::CEntry::AddChild(const std::string &name, bool addall){
    // You code here
    unsigned int size = 0, count = -1;
     if(auto tmpPtr = DImplementation->weakPtr.lock())
            size = tmpPtr->Children.size();
    
    if(addall == false)
    {   
        //std:: cout << "1" << std::endl;
        bool flag = true;

        std::shared_ptr<Node>ptr = std::make_shared<Node>();
        ptr->name = name;

        for(unsigned int i = 0; i < size;i++)
        {   
            if(auto tmpPtr = DImplementation->weakPtr.lock())
                if (tmpPtr->Children[i]->name == name)
                {
                        flag = false;
                        break;
                }
        }
        if(flag)    
            if(auto tempPtr = DImplementation->weakPtr.lock())
            {   
                if(tempPtr->data.size())
                    return false;
                tempPtr->Children.push_back(ptr);
                std::sort(tempPtr->Children.begin(), tempPtr->Children.end(),Compare);
                ptr->Parent = tempPtr; 
                ptr->entry = *this;
            }


        return flag;
    }
    else 
    {   
        //std:: cout << "2" << std::endl;
        bool found = false, added = false;
        std::string str = name;
        unsigned int times = 0;
        
        if(str[0] == '/')
            str = str.substr(1);

        std::vector<std::string>vec_str = StringUtils::Split(str, "/");
        
        if(!vec_str.size())
            return false;

        for(unsigned int i = 0; i < vec_str.size();i++)
        {
            if(vec_str[i] == "..")
            {
                times++;
                vec_str[i] = "";
            }
        }
            
        if(times)
            if(auto node = DImplementation->weakPtr.lock())
            {      
                std::vector<std::string> v;
                for(unsigned int i = 0; i < vec_str.size();i++)
                {
                    if(!vec_str[i].empty())
                        v.push_back(vec_str[i]);
                }

                while(times)
                {
                    if(auto t = node->Parent.lock())
                        node = t;
                    else
                        return false;
                    times--;
                }
                for(unsigned int i = 0; i < size;i++)
                {
                    if(node->Children[i]->name == v[0])
                        return false;
                }
                std::shared_ptr<Node>ptr = std::make_shared<Node>();
                ptr->name = v[0];
                node->Children.push_back(ptr);
                return true;
            }

        for(unsigned int i = 0; i < size; i++)
        {
            if(auto tempPtr = DImplementation->weakPtr.lock())
            {   

                if(tempPtr->Children[i]->name == vec_str[0])
                {   
                    count = i;
                    found = true;
                    break;
                }
            }
        }
        if(found)
        {
            if(count >= 0)
            {   
                
                if(auto tmpPtr = DImplementation->weakPtr.lock())
                {
                    auto current = std::make_shared<Node>();
                    current = tmpPtr->Children[count];// pounsigned int to home
            
                    for(unsigned int j = 1; j < vec_str.size();j++)
                    {   
                        bool flag = false;
                        if(!current->Children.size())
                        {   
                            if(current->data.size())
                                return false;
                            CEntry a;
                            std::shared_ptr<Node>ptr = std::make_shared<Node>();
                            ptr->name = vec_str[j];
                            //ptr->Parent = current;
                            current->Children.push_back(ptr);
                            //std::cout << "Added(Line 189) : " << vec_str[j] << std::endl;
                            ptr->Parent = current;
                            //current->entry = a;
                            current = current->Children[0];
                            a.DImplementation->weakPtr = current;
                            current->entry = a;
                            added = true;
                            std::sort(tmpPtr->Children.begin(), tmpPtr->Children.end(),Compare);

                        }
                        else
                        {   
                            unsigned int k = 0;
                            for(; k < current->Children.size(); k++)
                            {
                                if(current->Children[k]->name == vec_str[j])
                                {   
                                    //std::cout << "find directory: " << vec_str[j] << std::endl;
                                    flag = true;
                                    break;
                                }
                            }

                            if(flag)
                            {    
                                if(j + 1 == vec_str.size() ){
                                    return false;
                                }
                                current = current->Children[k];
                            }
                            else
                            {      
                                if(current->data.size())
                                    return false;
                                CEntry a;
                                std::shared_ptr<Node>ptr = std::make_shared<Node>();
                                ptr->name = vec_str[j];

                                current->Children.push_back(ptr);

                                ptr->Parent = current;
                                current = current->Children[0];
                                a.DImplementation->weakPtr = current;
                                current->entry = a;
                                added = true;
                                std::sort(tmpPtr->Children.begin(), tmpPtr->Children.end(),Compare);
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(auto tmpPtr = DImplementation->weakPtr.lock())
            {   
                auto current = std::make_shared<Node>();
                current = tmpPtr;
                for(unsigned int i = 0; i < vec_str.size();i++)
                {   
                    CEntry a;
                    auto ptr = std::make_shared<Node>();
                    ptr->name = vec_str[i];
                    current->Children.push_back(ptr);
                    ptr->Parent = current;

                    current = current->Children[current->Children.size()-1];
                    a.DImplementation->weakPtr = current;
                    current->entry = a;
                    added = true;
                    std::sort(tmpPtr->Children.begin(), tmpPtr->Children.end(),Compare);
                }
            }
        }
    return added;
    }
}

bool CFileSystemTree::CEntry::RemoveChild(const std::string &path){
    // You code here
    unsigned int size = 0;

    bool flag = false;

    if(auto tmpPtr = DImplementation->weakPtr.lock())
        size = tmpPtr->Children.size();

    for(unsigned int i = 0; i < size; i++)
        if(auto tmpPtr = DImplementation->weakPtr.lock())
        {   
            //std::cout << tmpPtr->Children[i]->name << std::endl;
            if(tmpPtr->Children[i]->name == path)
            {   
                tmpPtr->Children.erase(tmpPtr->Children.begin()+i);
                flag = true; 
                break;
            }
        }
    return flag;
}

bool CFileSystemTree::CEntry::SetData(const std::vector< char > &data){
    // You code here
    bool flag = false;
    if(auto tmp = DImplementation->weakPtr.lock())
    {
        if(!tmp->Children.size())
        {
            if(!tmp->data.size())
            {
                flag = true;
                tmp->data = data;
            }
        }
    }
    return flag;

}

bool CFileSystemTree::CEntry::GetData(std::vector< char > &data) const{
    // You code here
    bool flag = false;

    if(auto tmp = DImplementation->weakPtr.lock())
    {
        if(tmp->data.size())
        {
            data = tmp->data;
            flag = true;
        }
    }
    return flag;

}

CFileSystemTree::CEntry &CFileSystemTree::CEntry::Parent(){
    // You code here
    if(auto tmp = DImplementation->weakPtr.lock())
    {
        if(auto Parent = tmp->Parent.lock())
            return Parent->entry;
    }

    return empty;
}

const CFileSystemTree::CEntry &CFileSystemTree::CEntry::Parent() const{
    // You code here
    if(auto tmp = DImplementation->weakPtr.lock())
        if(auto Parent = tmp->Parent.lock())
            return Parent-> entry;
    return empty;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::Find(const std::string &name){
    // You code here
    bool found = false;
    std::string str = name;
    if(str[0] == '/')
            str = str.substr(1);
    std::vector<std::string> find = StringUtils::Split(str, "/");
    
    auto current = std::make_shared<Node>();
    // CEntry Empty
    if (auto tempPtr = DImplementation->weakPtr.lock()){
        current = tempPtr;
        while(find.size() > 0){
            for(unsigned int i = 0; i < current->Children.size(); i++){
                if (find[0] == current->Children[i]->name){
                        current = current->Children[i];
                        find.erase(find.begin());
                        found = true;
                        break;
                }
            }
            if(!found)        
                return Empty;
        }
    }
    CEntryIterator iter;
    iter.DImplementation->iteratorPtr = current;
    return iter;

}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::Find(const std::string &name) const{
    // You code here
    bool found = false;
    std::string str = name;
    if(str[0] == '/')
            str = str.substr(1);
    std::vector<std::string> find = StringUtils::Split(str, "/");
    
    auto current = std::make_shared<Node>();
    // CEntry Empty
    if (auto tempPtr = DImplementation->weakPtr.lock()){
        current = tempPtr;
        while(find.size() > 0){
            for(unsigned int i = 0; i < current->Children.size(); i++){
                if (find[0] == current->Children[i]->name){
                        current = current->Children[i];
                        find.erase(find.begin());
                        found = true;
                        break;
                }
            }
            if(!found)        
                return Empty;
        }
    }
    CEntryIterator iter;
    iter.DImplementation->iteratorPtr = current;
    return iter;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::begin(){
    // You code here
    CEntryIterator iter;
    if(auto tmpPtr = DImplementation->weakPtr.lock())
        if(tmpPtr->Children.size() != 0)
        {
            iter.DImplementation->iteratorPtr = tmpPtr->Children[0];
            iter.DImplementation->index = 0;
            //std::cout << "bgein's name: " << iter.DImplementation->iteratorPtr->name << "\n";
        }
    
    return iter;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::begin() const{
    // You code here
    CConstEntryIterator iter;
    if(auto tmpPtr = DImplementation->weakPtr.lock())
        if(tmpPtr->Children.size() != 0)
        {
            iter.DImplementation->iteratorPtr = tmpPtr->Children[0];
            iter.DImplementation->index = 0;
            //std::cout << "bgein's name: " << iter.DImplementation->iteratorPtr->name << "\n";
        }
    
    return iter;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cbegin() const{
    // You code here
    CConstEntryIterator iter;
    if(auto tmpPtr = DImplementation->weakPtr.lock())
        if(tmpPtr->Children.size() != 0)
        {
            iter.DImplementation->iteratorPtr = tmpPtr->Children[0];
            iter.DImplementation->index = 0;
            //std::cout << "bgein's name: " << iter.DImplementation->iteratorPtr->name << "\n";
        }
    
    return iter;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntry::end(){
    //You code here
    CEntryIterator it;

    it.DImplementation->iteratorPtr = nullptr;
    it.DImplementation->index = DImplementation->weakPtr.lock()->Children.size();
    return it;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::end() const{
    CConstEntryIterator it;

    it.DImplementation->iteratorPtr = nullptr;
    it.DImplementation->index = DImplementation->weakPtr.lock()->Children.size();
    return it;

}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CEntry::cend() const{
    // You code here
    CConstEntryIterator it;

    it.DImplementation->iteratorPtr = nullptr;
    it.DImplementation->index = DImplementation->weakPtr.lock()->Children.size();
    return it;
}

CFileSystemTree::CEntryIterator::CEntryIterator() : DImplementation(std::make_unique< SImplementation >()){
   
}

CFileSystemTree::CEntryIterator::CEntryIterator(const CEntryIterator &iter) : DImplementation(std::make_unique< SImplementation >()){
    // You code here
    DImplementation->iteratorPtr = iter.DImplementation->iteratorPtr;
}

CFileSystemTree::CEntryIterator::~CEntryIterator(){
    // You code here
}

CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator=(const CEntryIterator  &iter){
    // You code here

    DImplementation->iteratorPtr = iter.DImplementation->iteratorPtr;

    return *this;
}

bool CFileSystemTree::CEntryIterator::operator==(const CEntryIterator &iter) const{
    // You code here
    if(DImplementation->iteratorPtr == iter.DImplementation->iteratorPtr)
        return true;
    else
        return false;
}

bool CFileSystemTree::CEntryIterator::operator!=(const CEntryIterator &iter) const{
    // You code here
    if(DImplementation->iteratorPtr != iter.DImplementation->iteratorPtr)
        return true;
    else
        return false;
}

CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator++(){
   // You code here
    if(auto Parent =DImplementation->iteratorPtr->Parent.lock())
    {
        DImplementation->index += 1;
        
        if(DImplementation->index == Parent->Children.size())
        {
            
            DImplementation->Previous = DImplementation->iteratorPtr;
            DImplementation->iteratorPtr = nullptr;
        }
        
        DImplementation->Previous = DImplementation->iteratorPtr;
        DImplementation->iteratorPtr = Parent->Children[DImplementation->index];
            
        //if(DImplementation->index == Parent->Children.size())

    }
    return *this;
}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator++(int)
{   
    if(auto Parent =DImplementation->iteratorPtr->Parent.lock())
    {
        DImplementation->index += 1;
        
        if(DImplementation->index == Parent->Children.size())
        {
            
            DImplementation->Previous = DImplementation->iteratorPtr;
            DImplementation->iteratorPtr = nullptr;
            return *this;
        }
        
        DImplementation->Previous = DImplementation->iteratorPtr;
        DImplementation->iteratorPtr = Parent->Children[DImplementation->index];
            
        //if(DImplementation->index == Parent->Children.size())

    }
    return *this;
    
}

CFileSystemTree::CEntryIterator& CFileSystemTree::CEntryIterator::operator--(){
    // You code here
    //std::string str = DImplementation->iteratorPtr->name;
    if(DImplementation->index == DImplementation->Previous->Parent.lock()->Children.size())
    {
        DImplementation->index -= 1;
        DImplementation->iteratorPtr = DImplementation->Previous;
    }
    else
    {
        if(auto Parent = DImplementation->iteratorPtr->Parent.lock())
        {
            DImplementation->index -= 1;
            DImplementation->iteratorPtr = Parent->Children[DImplementation->index];
            DImplementation->Previous = Parent->Children[DImplementation->index - 1];
        }
    }
    return *this;

}

CFileSystemTree::CEntryIterator CFileSystemTree::CEntryIterator::operator--(int){
   
    if(DImplementation->index == DImplementation->Previous->Parent.lock()->Children.size())
    {
        DImplementation->index -= 1;
        DImplementation->iteratorPtr = DImplementation->Previous;
    }
    else
    {
        if(auto Parent = DImplementation->iteratorPtr->Parent.lock())
        {
            DImplementation->index -= 1;
            DImplementation->iteratorPtr = Parent->Children[DImplementation->index];
            DImplementation->Previous = Parent->Children[DImplementation->index - 1];
        }
    }
    return *this;
}

CFileSystemTree::CEntry &CFileSystemTree::CEntryIterator::operator*() const{
    // You code here
    return DImplementation->iteratorPtr->entry;
}

CFileSystemTree::CEntry *CFileSystemTree::CEntryIterator::operator->() const{
    // You code here
    //std::cout << "not use ->" << std::endl;
    //return &DImplementation->iteratorPtr->entry;
}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator() : DImplementation(std::make_unique< SImplementation >()){

}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator(const CConstEntryIterator &iter) : DImplementation(std::make_unique< SImplementation >()){
    // You code here
    DImplementation->iteratorPtr = iter.DImplementation->iteratorPtr;
}

CFileSystemTree::CConstEntryIterator::CConstEntryIterator(const CEntryIterator &iter) : DImplementation(std::make_unique< SImplementation >()){
    // You code here
     DImplementation->iteratorPtr = iter.DImplementation->iteratorPtr;
}

CFileSystemTree::CConstEntryIterator::~CConstEntryIterator(){
    // You code here
}

CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator=(const CConstEntryIterator &iter){
    // You code here
    DImplementation->iteratorPtr = iter.DImplementation->iteratorPtr;
    return *this;
}

bool CFileSystemTree::CConstEntryIterator::operator==(const CConstEntryIterator &iter) const{
    // You code here
    if(DImplementation->iteratorPtr == iter.DImplementation->iteratorPtr)
        return true;
    else
        return false;
}

bool CFileSystemTree::CConstEntryIterator::operator!=(const CConstEntryIterator &iter) const{
    // You code here
    if(DImplementation->iteratorPtr != iter.DImplementation->iteratorPtr)
        return true;
    else
        return false;
}

CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator++(){
    // You code here
    if(auto Parent =DImplementation->iteratorPtr->Parent.lock())
    {
        DImplementation->index += 1;
        
        if(DImplementation->index == Parent->Children.size())
        {
            
            DImplementation->Previous = DImplementation->iteratorPtr;
            DImplementation->iteratorPtr = nullptr;
            return *this;
        }
        
        DImplementation->Previous = DImplementation->iteratorPtr;
        DImplementation->iteratorPtr = Parent->Children[DImplementation->index];
            
        //if(DImplementation->index == Parent->Children.size())

    }
    return *this;

}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator++(int){
    // You code here
   if(auto Parent =DImplementation->iteratorPtr->Parent.lock())
    {
        DImplementation->index += 1;
        
        if(DImplementation->index == Parent->Children.size())
        {
            
            DImplementation->Previous = DImplementation->iteratorPtr;
            DImplementation->iteratorPtr = nullptr;
            return *this;
        }
        
        DImplementation->Previous = DImplementation->iteratorPtr;
        DImplementation->iteratorPtr = Parent->Children[DImplementation->index];
            
        //if(DImplementation->index == Parent->Children.size())

    }
    return *this;
}

CFileSystemTree::CConstEntryIterator& CFileSystemTree::CConstEntryIterator::operator--(){
    // You code here
   if(DImplementation->index == DImplementation->Previous->Parent.lock()->Children.size())
    {
        DImplementation->index -= 1;
        DImplementation->iteratorPtr = DImplementation->Previous;
    }
    else
    {
        if(auto Parent = DImplementation->iteratorPtr->Parent.lock())
        {
            DImplementation->index -= 1;
            DImplementation->iteratorPtr = Parent->Children[DImplementation->index];
            DImplementation->Previous = Parent->Children[DImplementation->index - 1];
        }
    }
    return *this;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::CConstEntryIterator::operator--(int){
    // You code here
    if(DImplementation->index == DImplementation->Previous->Parent.lock()->Children.size())
    {
        DImplementation->index -= 1;
        DImplementation->iteratorPtr = DImplementation->Previous;
    }
    else
    {
        if(auto Parent = DImplementation->iteratorPtr->Parent.lock())
        {
            DImplementation->index -= 1;
            DImplementation->iteratorPtr = Parent->Children[DImplementation->index];
            DImplementation->Previous = Parent->Children[DImplementation->index - 1];
        }
    }
    return *this;
}

const CFileSystemTree::CEntry &CFileSystemTree::CConstEntryIterator::operator*() const{
    // You code here
    return DImplementation->iteratorPtr->entry;
}

const CFileSystemTree::CEntry *CFileSystemTree::CConstEntryIterator::operator->() const{
    //return &DImplementation->iteratorPtr->entry;
    // You code here
}

CFileSystemTree::CFileSystemTree() : DImplementation(std::make_unique< SImplementation >()){
    // You code here
    DImplementation->root = std:: make_shared<Node>();
    DImplementation->root->entry.DImplementation->weakPtr = DImplementation->root;
}

CFileSystemTree::CFileSystemTree(const CFileSystemTree &tree) : DImplementation(std::make_unique< SImplementation>()){
    // You code here
    DImplementation->root = tree.DImplementation->root;
}

CFileSystemTree::~CFileSystemTree(){
    // You code here
}

CFileSystemTree &CFileSystemTree::operator=(const CFileSystemTree &tree){
    // You code here
    DImplementation->root = tree.DImplementation->root;
    return *this;
}

CFileSystemTree::CEntry &CFileSystemTree::Root(){
    // You code here
    return DImplementation->root->entry;
}

const CFileSystemTree::CEntry &CFileSystemTree::Root() const{
    // You code here
    return DImplementation->root->entry;
}

std::string CFileSystemTree::ToString() const{
    // You code here
    return DImplementation->root->entry.ToString();
}

CFileSystemTree::operator std::string() const{
    // You code here
    return DImplementation->root->entry.ToString();

}

CFileSystemTree::CEntryIterator CFileSystemTree::Find(const std::string &name){
    
    return DImplementation->root->entry.Find(name);
    
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::Find(const std::string &name) const{
    // You code here
    
    return DImplementation->root->entry.Find(name);
}

CFileSystemTree::CEntryIterator CFileSystemTree::NotFound(){
    // You code here
    return Empty;
}

CFileSystemTree::CConstEntryIterator CFileSystemTree::NotFound() const{
    // You code here
    return Empty;
}
