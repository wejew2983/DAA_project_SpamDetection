#include <bits/stdc++.h>
using namespace std;

//50 spam keywords taken from the spam.csv dataset

string patterns[50]={
"win money","free offer","click here","claim prize","urgent",
"limited time","loan approved","cash reward","work from home","be your own boss",
"exclusive deal","call now","reply now","free gift","earn money",
"winner","congratulations","special offer","risk free","bonus",
"double money","act now","hurry up","subscribe now","buy now",
"discount","selected","guaranteed","cheap rate","investment",
"lottery","reward now","free cash","easy income","no cost",
"claim now","instant loan","extra income","offer ends","text now",
"visit now","password reset","verify account","bank details","miracle cure",
"no obligation","gift card","trial offer","free entry","jackpot"
};

string toLower(string s){
    for(int i=0; i<s.length(); i++)
        s[i]=tolower(s[i]);
    return s;
}

//search using RabinKarp
int rabinKarp(string text){
    int i;
    int count=0;

    text=toLower(text);

    for(i=0; i<50; i++){
        if(text.find(patterns[i])!=string::npos){
            cout<<patterns[i]<<", ";
            count++;
        }
    }

    return count;
}

//aho corasick
struct Node{
    int next[26];
    int fail;
    int output;

    Node(){
        int i;

        for(i=0; i<26; i++)
            next[i]=-1;

        fail=0;
        output=0;
    }
};

Node trie[3000];
int sizeTrie=1;

//putting in pattern
void insertWord(string s){
    int node=0;
    int i, x;

    for(i=0; i<s.length(); i++){

        if(s[i]==' ')
            continue;

        if(s[i]<'a' || s[i]>'z')
            continue;

        x=s[i]-'a';

        if(trie[node].next[x]==-1){
            trie[node].next[x]=sizeTrie;
            sizeTrie++;
        }

        node=trie[node].next[x];
    }

    trie[node].output++;
}

//building the trie
void buildTrie(){
    queue<int> q;
    int i, u, v;

    for(i=0; i<26; i++){

        if(trie[0].next[i]!=-1){
            trie[trie[0].next[i]].fail=0;
            q.push(trie[0].next[i]);
        }
        else{
            trie[0].next[i]=0;
        }
    }

    while(!q.empty()){

        u=q.front();
        q.pop();

        for(i=0; i<26; i++){

            v=trie[u].next[i];

            if(v!=-1){
                trie[v].fail=trie[trie[u].fail].next[i];
                q.push(v);
            }
            else{
                trie[u].next[i]=trie[trie[u].fail].next[i];
            }
        }
    }
}

//search text
int ahoSearch(string text){

    int node=0;
    int i, x;
    int count=0;

    text=toLower(text);

    for(i=0; i<text.length(); i++){

        if(text[i]==' ')
            continue;

        if(text[i]<'a' || text[i]>'z')
            continue;

        x=text[i]-'a';

        node=trie[node].next[x];

        count=count+trie[node].output;
    }

    return count;
}

//main
int main(){

    cout<<"C VISHWAJEET KUMAR"<<endl;
    cout<<"24BCE0341"<<endl;
    cout<<"CS-1 REAL TIME SPAM DETECTION SYSTEM"<<endl;
    cout<<"RABIN KARP VS AHO-CORASICK"<<endl;

    int i=0;
    string line;

    ifstream file("C:\\sem4 projects\\spam.csv");

    if(!file){
        cout<<"File not found"<<endl;
        return 0;
    }

    //building trie
    for(i=0; i<50; i++)
        insertWord(toLower(patterns[i]));

    buildTrie();

    int msgNo=1;

    while(getline(file,line))
    {
        cout<<"\nMessage "<<msgNo<<endl;
        cout<<"Text: "<<line<<endl;

        //measuring time for rabin karp
        clock_t start1=clock();

        cout<<"Matched Words: ";
        int rk=rabinKarp(line);

        clock_t end1=clock();

        double time1=double(end1-start1)/CLOCKS_PER_SEC;

        cout<<endl;
        cout<<"Total Matches: "<<rk<<endl;
        cout<<"Time Taken: "<<time1<<" sec"<<endl;

        //aho corasick
        clock_t start2=clock();

        int ac=ahoSearch(line);

        clock_t end2=clock();

        double time2=double(end2-start2)/CLOCKS_PER_SEC;

        cout<<"Aho Corasick Total Matches: "<<ac<<endl;
        cout<<"Time Taken: "<<time2<<" sec"<<endl;

        if(rk>0 || ac>0)
            cout<<"SPAM MESSAGE!! CAUTION !!"<<endl;
        else
            cout<<"NORMAL MESSAGE!"<<endl;

        msgNo++;

        if(msgNo>20)
            break;
    }

    file.close();

    return 0;
}