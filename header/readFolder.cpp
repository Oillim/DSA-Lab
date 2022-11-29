#include <Windows.h>
#include <iostream>
#include <queue>
using namespace std;
queue<std::string> qFolders;

// void findFiles(string Path)
// {
//     WIN32_FIND_DATAA findData;
//     HANDLE handle = NULL;

//     if (qFolders.size() > 0)
//     {
//         std::string tempFolder = Path;
//         tempFolder.append("\\*.*");
//         handle = FindFirstFileA(tempFolder.c_str(), &findData);
        
//         cout << qFolders.front() << endl;
//         do
//         {
//             if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//             {
//                 if (strcmp(".",findData.cFileName) == 0 || strcmp("..", findData.cFileName) == 0)
//                 {
//                     continue;
//                 }
//                 tempFolder = qFolders.front();
//                 tempFolder.append("\\").append(findData.cFileName);
//                 qFolders.push(tempFolder);
//                 qFolders.pop();
//                 findFiles(qFolders.front());
//             }
//             else if (findData.cFileName)
//             cout<<" "<<findData.cFileName<<endl;
            
//         } while (FindNextFileA(handle, &findData));
//         qFolders.pop();
//         if(!qFolders.empty())
//         {
//             findFiles(qFolders.front());
//         }
//     }
//     if (handle)
//     {
//         FindClose(handle);
//         handle = NULL;
//     }
// }
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
void findFiles(string path,int k)
{
    WIN32_FIND_DATAA findData;
    HANDLE handle;

    string tempFolder = path + "\\*.*";
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
                findFiles(fpath,k+1);
            }
            else 
            {
            cout<<repeatString("   ",k+1)<<findData.cFileName<<endl;
            cout<<endl;
            }
        }
        while (FindNextFileA(handle, &findData));
        
    }
    
}

int main()
{
    qFolders.push("E:\\txt");
    findFiles("E:\\txt",0);
    return 0;
}