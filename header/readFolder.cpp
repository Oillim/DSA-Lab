#include <Windows.h>
#include <iostream>
#include<vector>
#include<algorithm>
#include<direct.h>
using namespace std;
vector<string> sv, sfolder;
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
//Cách tạo cây trie (coi video của mấy anh Ấn chứ đ biết giải thích)
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


string searchTrie(trie *root,string s,string &path)
{
    //đầu tiên tìm vị trí của thư mục cần tìm được đánh số như 1 hằm băm
    int pos = ('\\'+calcIndex(s))%256;
    for (int i=0;i<255;i++)
    {
        //tại root nếu ptu ở vị trí pos là Null tức là thư mục kh nằm ở vị trí này, nếu kh là null tức là tồn tại và in ra màn hình
        if(root->child[pos]!=NULL)
        {
            path+="\\"+s;
            return path;
        }
        //duyệt từng vị trí i nếu bắt gặp 1 child mà tại đó kh là null tức là tồn tại 1 thư mục. Khi đó tiến hành nối xâu thư mục vào path
        //tiếp tục đệ quy, duyệt tiếp các child ở root có child[i]. khi kết thúc đệ quy mà không bị break ở phía trên
        //tức là đến cuối cùng vẫn kh có thư mục cần tìm. Quay về reset lại path và tiếp tục với root có child[i+1],child[i+2]...
        //cho tới khi tòn tại đường dẫn dẫn đến thư mục đó
        if(root->child[i]!=NULL) 
        {
            path+=root->child[i]->data;
            
            return searchTrie(root->child[i],s,path);
            path = path.substr(0,path.find_last_of('\\'));
        }
    }
}
//Hàm để in ra cho đẹp giống yêu cầu của thầy 
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


//Hàm để duyệt và in ra tất cả các thư mục con có trong thư mục cha và lưu từng đường dẫn vào vector sv
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

bool createFolder(string filefrom, string fileto,int k)
{
    bool check = true;
    string folder =fileto + filefrom.substr(filefrom.find_last_of('\\'),k);
    if(mkdir(folder.c_str())!=-1)
    {
    cout<<"file created"<<endl;
    }else check = false;
    string str = "move \""+ filefrom+ "\"" + " \""+fileto + filefrom.substr(filefrom.find_last_of('\\'),k)+"\"" ;
    const char *c = str.c_str();
    system(c);
    return check;
}




void checkFolder(string fileto,int k)
{
    sfolder.clear();
    int countFolder = 0;
    findFiles(fileto,0,sfolder); 
    for (int i=0;i<sfolder.size();i++)
    {
        //ktra do nếu là folder thì kh có đuôi như .png, .txt.Nếu đúng thì qua bước tiếp theo
        if(sfolder[i].find('.')==std::string::npos)
        {
            
            //ở bước tiếp theo đếm các thư mục con có trong folder hiện tại
            int count=0;
            for (int j=i+1;j<sfolder.size();j++)
            {
                //do vector chứa đường dẫn nên nếu thư mục con có đường dẫn của thư mục cha thì nghĩa là nó nằm trong thư mục cha
                //tăng count
                if(sfolder[j].find(sfolder[i])!=std::string::npos) count++; 
            }
            //theo đề bài, nếu count lớn hơn 3 thì ta phải tiến hành duyệt lần nữa
            if(count>3)
            {
            for (int j=i+1;j<sfolder.size();j++)
            {
                // tạo folder trong folder
                if(sfolder[j].find(sfolder[i])!=std::string::npos) 
                {
                createFolder(sfolder[j],sfolder[i],k+1);
                }
            }
            checkFolder(sfolder[i],k+1);
            }
        }
    }  
}








int main()
{
    string filepath;
    string fileto="E:\\ordered";
    string ss="",ff;   
    trie *root = createNode(filepath);

    // //Chức năng 1: in ra toàn bộ thư mục con có trong filepath (filepath nhập vào)
    // findFiles(filepath,0,sv);





    // //Chức năng 2: tìm đường dẫn tới thư mục ("main.txt")
    // for (int i=0;i<sv.size();i++)
    // {
    //     cout<<sv[i]<<endl;
    //     //tạo cây trie
    //     insertTrie(root,sv[i]);
    // }
    // searchTrie(root,"main.txt",ss);



    // findFiles(fileto,0,sfolder); 

    // //Chức năng 3: sắp xếp các thư mục bỏ vào các thư mục con tự tạo
    // //Đầu tiên duyệt thư mục ngoài cùng và tiến hành sắp xếp tạm.
    // //VD: thư mục E:\unordered có các thư mục abc, abd, abe, ace, bac, bc, be (tất cả file này đều là file text document)
    // for (int i=0;i<sv.size();i++)
    // createFolder(sv[i],fileto,2);
    // //Sau khi duyệt lần đầu, ta sẽ được ở thư mục E:\ordered có 2 thư mục con được đánh dấu theo chữ cái đầu
    // // a: abc, abd, abe, ace
    // // b: bac, bc, be
    // //xóa vector ban đầu để tạo vector mới gồm tất cả thư mục con mang đường dẫn của E:\ordered
    

    // //này in ra để dễ nhìn lỗi sai để fix
    // for (int i=0;i<sfolder.size();i++)
    // {
    //     cout<<sfolder[i]<<endl;
    //     //insertTrie(root,sv[i]);
    // }
    // checkFolder(fileto,2);


cout<<"Vui long nhap vao duong dan: ";
cin>>filepath;
string output;

while(1)
{
    int chooseMenu;
    cout<<"Chon menu: "; 
    cin>>chooseMenu;
    switch(chooseMenu)
    {
        case 1:
        sv.clear();
        cout<<"Xu ly chuc nang 1..."<<endl;
        findFiles(filepath,0,sv);
        break;

        case 2:
        sv.clear();
        ss="";
        cout<<"Xu ly chuc nang 2..."<<endl;
        findFiles(filepath,0,sv);
        cout<<"Nhap vao thu muc can tim"<<endl;
        cin>>ff;
        for (int i=0;i<sv.size();i++)
        {
            //tạo cây trie
            insertTrie(root,sv[i]);
        }
        output = searchTrie(root,ff,ss);
        if(output.length()==ff.length()+1) cout<<"File no exist"<<endl;
        else cout<<output<<endl;
        break;

        case 3:
        cout<<"Vui long nhap vao duong dan: ";
        cin>>filepath;
        cout<<"Xu ly chuc nang 3..."<<endl;
        findFiles(filepath,0,sfolder);
        for (int i=0;i<sfolder.size();i++)
            createFolder(sfolder[i],fileto,2);
        checkFolder(fileto,2);
        
        break;

        case 0:
        return 0;
    }
}
    return 0;
}   