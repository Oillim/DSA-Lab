#include <Windows.h>
#include <iostream>
#include<vector>
using namespace std;
vector<string> sv;
struct trie{
    trie *child[256];
    string data;
    bool strend;
};
trie* createNode(string data){
    trie *p = new trie;
    
    p->data = data;
    for (int i=0;i< 256;i++)
        p->child[i] = NULL;
    p->strend = false;
    return p;
}
int calcIndex(string s)
{
    int sum=0;
    for (int i=0;i<s.length();i++)
    {
        sum+=s[i];
    }
    return sum%256;
}

void insertTrie(trie *root, string s)
{
    int previd=0,idx=0;
    string stmp;
    trie *p = root;
    for (int i=0;i<s.length();i++)
    {   
        
        if (s[i]=='\\')
        {
            stmp = s.substr(previd,i-previd);
            previd = i;
            idx = calcIndex(stmp);
            cout<<stmp<<"  "<<idx<<endl;
        }
        if(p->child[idx]==NULL)
        {
        p->child[idx] = createNode(stmp);
        
        }
        p = p->child[idx];
    }
    p->child[calcIndex(s.substr(previd,s.length()-1))] = createNode(s.substr(previd,s.length()-1));
    p = p->child[calcIndex(s.substr(previd,s.length()-1))];
    cout<<"---------> "<<p->data<<endl;
    cout<<s.substr(previd,s.length()-1)<<"  "<<calcIndex(s.substr(previd,s.length()-1))<<endl;
    p->strend = true;
}
bool searchTrie(trie *root,string s)
{
    trie *tmp =root;
    for (int i=0;i<s.length();i++)
    {
        int idx = s[i];
        if(tmp->child[idx]==NULL)
        return false;
        tmp = tmp->child[idx];
    }
    return (tmp->strend);
}
string repeatString(string s,int k)
{
    string rp;
    if (k!=0)
    {
    for (int i=0;i<k-1;i++)
    rp+=s;
    return rp+"|_ ";
    }
    else return "";
}
void findFiles(string path,int k,vector<string> &sv)
{
    WIN32_FIND_DATAA findData;
    HANDLE handle;

    string tempFolder = path + "\\*";
    handle = FindFirstFileA(tempFolder.c_str(), &findData);
    if (k!=1)
    cout<<repeatString("   ",k)<<path.substr(path.find_last_of("\\")+1)<<endl;
    else 
   cout<<repeatString("|_ ",k)<<path.substr(path.find_last_of("\\")+1)<<endl;

    cout<<endl;
    if (handle != INVALID_HANDLE_VALUE)
    {
        do 
        {
            string fpath  = path + "\\" + findData.cFileName;
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
            {
                if(strcmp(".",findData.cFileName)==0 || strcmp("..",findData.cFileName)==0)
                continue;         
                sv.push_back(tempFolder.erase(tempFolder.length()-1));
                findFiles(fpath,k+1,sv);
            }
            else 
            {
            cout<<repeatString("   ",k+1)<<findData.cFileName<<endl;
            if (tempFolder[tempFolder.length()-1]=='*')
            sv.push_back(tempFolder.erase(tempFolder.length()-1)+findData.cFileName);
            else sv.push_back(tempFolder + findData.cFileName);
            cout<<endl;
            }
        }
        while (FindNextFileA(handle, &findData));
        
    }
    
}

int main()
{
    string filepath = "E:\\txt";
       
    
    trie *root = createNode(filepath);
    findFiles(filepath,0,sv);
    for (int i=0;i<sv.size();i++)
    {
        //cout<<sv[i]<<endl;
        insertTrie(root,sv[i]);
    }
    cout<<searchTrie(root,"\\c.txt");
    return 0;
}