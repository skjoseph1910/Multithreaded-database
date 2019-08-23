#include <iostream>
#include <sys/wait.h>
#include <pthread.h>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <queue>
using namespace std;

//compile with g++ -std=c++11 Sungmin_Kim.cpp -lpthread -o main

int groupnumber, other, n, og, og2, groupcount=0, positionarray[10], sumi[4]={0, 0, 0, 0};
static pthread_mutex_t semB[2];
pthread_cond_t condition[10], groupie=PTHREAD_COND_INITIALIZER;

struct ohmygod
{
    int user, group, position, time, time2;
};

void *yupyup(void *x_void_ptr);
void itslit(queue<ohmygod> q);
queue<ohmygod> read(queue<ohmygod> a);

int main()
{
    queue<ohmygod> que;
    que=read(que);
    itslit(que);
}

void *yupyup(void *x_void_ptr)
{
    struct ohmygod *x = (struct ohmygod *)x_void_ptr;
    sleep((*x).time > 4 ? 4 : (*x).time); int i=0;
    pthread_mutex_lock(&semB[0]);
    cout<<"User "<<(*x).user<<" from Group "<<(*x).group<<" arrives to the DBMS"<<endl;
    pthread_mutex_unlock(&semB[0]);
    if ((*x).group!=groupnumber && og2!=0)
    {
        pthread_mutex_lock(&semB[0]);
        cout<<"User "<<(*x).user<<" is waiting due to its group"<<endl;
        sumi[2]++;
        pthread_cond_wait(&groupie, &semB[0]);
        pthread_mutex_unlock(&semB[0]);
    }
    
    if (positionarray[x->position-1]!=0)
    {
        pthread_mutex_lock(&semB[1]);
        cout<<"User "<<x->user<<" is waiting: position "<<x->position<<" of the database is being used by user "<<positionarray[x->position-1]<<endl;
        sumi[3]++;
        pthread_cond_wait(&condition[x->position-1], &semB[1]);
        pthread_mutex_unlock(&semB[1]);
    }
    pthread_mutex_lock(&semB[1]);
    positionarray[x->position-1]=x->user;
    cout<<"User "<<x->user<<" is accessing the position "<<x->position<<" of the database for "<<x->time2<<" second(s)"<<endl;
    pthread_mutex_unlock(&semB[1]);
    sleep(x->time2);
    pthread_mutex_lock(&semB[1]);
    cout<<"User "<<x->user<<" finished its execution"<<endl;
    groupcount--;
    positionarray[x->position-1]=0;
    pthread_cond_signal(&condition[x->position-1]);
    pthread_mutex_unlock(&semB[1]);
    
    if (sumi[0]==0 || sumi[1]==0)
        i=2;
    if (groupcount==0 && i==0)
    {
        cout<<endl<<"All users from Group "<<groupnumber<<" finished their execution"<<endl<<"The users from Group "<<other<<" start their execution"<<endl<<endl;
        i=2;
        for (int i=0;i<og;i++)
        {
            pthread_mutex_lock(&semB[0]);
            pthread_cond_signal(&groupie);
            pthread_mutex_unlock(&semB[0]);
        }
    }
    return NULL;
}

void itslit(queue<ohmygod> q)
{
    ohmygod use;
    pthread_t tid[n];
    for (int i=0;i<2;i++)
        pthread_mutex_init(&semB[i], NULL);
    for (int i=0;i<10;i++)
    {
        condition[i] = PTHREAD_COND_INITIALIZER;
        positionarray[i]=0;
    }
    for (int i=0;i<n;i++)
    {
        pthread_create(&tid[i], NULL, yupyup, (void *)&q.front());
        q.pop();
    }
    for (int i = 0; i < n; i++)
        pthread_join(tid[i], NULL);
    cout<<"\nTotal Requests:"<<endl<<"\tGroup 1: "<<sumi[0]<<endl<<"\tGroup 2: "<<sumi[1]<<"\n\nRequested that waited:\n\tDue to its group: "<<sumi[2]<<"\n\tDue to a locked position: "<<sumi[3]<<endl;
}

queue<ohmygod> read(queue<ohmygod> a)
{
    ohmygod use; string line,token;
    getline(cin, line);
    groupnumber=stoi(line);
    if (groupnumber==1)
        other=2;
    else
        other=1;
    int sum=0, count = 1, arr[4];
    while (getline(cin, line))
    {
        int i=0;
        istringstream ss(line);
        while (getline(ss, token,' '))
        {
            arr[i]=stoi(token);
            i++;
        }
        sum+=(arr[2]);
        if ((arr[0])==groupnumber)
            groupcount++;
        if ((arr[0])==1)
            sumi[0]++;
        else
            sumi[1]++;
        use.user = count; use.group=(arr[0]); use.position=(arr[1]); use.time=sum; use.time2=(arr[3]);
        a.push(use); count++;
    }
    n=a.size(); og=n-groupcount; og2=groupcount;
    
    return a;
}
