#include <Windows.h>
#include <iostream>
#include<vector>
#include<algorithm>
#include<direct.h>
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
    for (int i=0;i< 255;i++)
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
            //cout<<stmp<<"  "<<idx<<endl;
            if(p->child[idx]==NULL)
            {
            p->child[idx] = createNode(stmp);
            //cout<<"created at: "<<idx<<endl;
            }
            p = p->child[idx];
        }
    }
    p->child[calcIndex(s.substr(previd,s.length()-1))] = createNode(s.substr(previd,s.length()-1));
    p = p->child[calcIndex(s.substr(previd,s.length()-1))];
    
    //cout<<"---------> "<<p->data<<endl;
    //cout<<s.substr(previd,s.length()-1)<<"  "<<calcIndex(s.substr(previd,s.length()-1))<<endl;
    //cout<<"created at: "<<calcIndex(s.substr(previd,s.length()-1))<<endl;
    p->strend = true;
}
void searchTrie(trie *root,string s,string &path)
{
    
    int pos = ('\\'+calcIndex(s))%256;
    for (int i=0;i<255;i++)
    {
        if(root->child[pos]!=NULL)
        {
            path+="\\"+s;
            cout<<path;
            break;
        }
        if(root->child[i]!=NULL) 
        {
            path+=root->child[i]->data;
            
            searchTrie(root->child[i],s,path);
            path = path.substr(0,path.find_last_of('\\'));
        }
    }
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
                sv.push_back(fpath);
                findFiles(fpath,k+1,sv);
            }
            else 
            {
            cout<<repeatString("   ",k+1)<<findData.cFileName<<endl;
            sv.push_back(fpath);
            cout<<endl;
            }
        }
        while (FindNextFileA(handle, &findData));
        
    }
    
}
// void display(trie *root,string &ss)
// {
//     for (int i=0;i<255;i++)
//     {
//         if(root->child[i])
//         {
//             cout<<root->child[i]->data<<endl;
//             display(root->child[i],root->data);
//         }
//     }
// }

void createFolder(string filefrom, string fileto,int k)
{
    
    string folder =fileto + filefrom.substr(filefrom.find_last_of('\\'),k);
    if(mkdir(folder.c_str())!=-1)
    cout<<"file created"<<endl;
    string str = "move \""+ filefrom+ "\"" + " \""+fileto + filefrom.substr(filefrom.find_last_of('\\'),k)+"\"" ;
    const char *c = str.c_str();
    system(c); 
}
int main()
{
    string filepath = "E:\\unordered";
    string fileto = "E:\\ordered";
    string ss=""; 
    int count=1;  
    trie *root = createNode(filepath);
    findFiles(filepath,0,sv);
    //createFolder();

    for (int i=0;i<sv.size();i++)
    {
        cout<<sv[i]<<endl;
        //insertTrie(root,sv[i]);
    }
    for (int i=0;i<sv.size();i++)
    createFolder(sv[i],fileto,2);
    sv.clear();
    findFiles(fileto,0,sv);
    for (int i=0;i<sv.size();i++)
    {
        cout<<sv[i]<<endl;
        //insertTrie(root,sv[i]);
    } 
    for (int i=0;i<sv.size();i++)
    {
        if(sv[i].find('.')==std::string::npos)
        {
            int count=0;
            for (int j=i+1;j<sv.size();j++)
            {
                if(sv[j].find(sv[i])!=std::string::npos) count++; 
            }
            cout<<count<<endl;
            if(count>3)
            for (int j=i+1;j<sv.size();j++)
            {
                if(sv[j].find(sv[i])!=std::string::npos) createFolder(sv[j],sv[i],3);
            }
        }
    }
    
    //searchTrie(root,"main.txt",ss);
    return 0;
}   