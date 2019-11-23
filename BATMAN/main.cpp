#include <iostream>
#include<pthread.h>
#include<unistd.h>
#include<bits/stdc++.h>

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

using namespace std;
class BAT{
    private:
        int num;
        string dir;
    public:
        BAT(int bat_num , string bat_dir){
            this->num=bat_num;
            this->dir=bat_dir;
        }

        int get_num(){
            return num;
        }

        string get_dir(){
            return dir;
        }
};
class Monitor{
    private:
        //waiting to enter the monitor
        int waiting[4];

        // variables to dedect if there is a car in each section
        int crossing_dir[4];

        // number of cars ready for crossing
        int counter = 0;
        // conditions variables
        pthread_cond_t NorthQueue, EastQueue, SouthQueue,WestQueue,
                       NorthFirst, EastFirst, SouthFirst, WestFirst;
       // mutex for synchronisation
        pthread_mutex_t crossing;

        // hashing directions
        map<string,int> directions;

        // initalize conditions variables with default attributes
        void inti_condition_variables(){
            pthread_cond_init(&NorthQueue,NULL);
            pthread_cond_init(&EastQueue,NULL);
            pthread_cond_init(&SouthQueue,NULL);
            pthread_cond_init(&WestQueue,NULL);
            pthread_cond_init(&NorthFirst,NULL);
            pthread_cond_init(&EastFirst,NULL);
            pthread_cond_init(&SouthFirst,NULL);
            pthread_cond_init(&WestFirst,NULL);
        }



    public:
        Monitor(){
            for(int i=0;i<4;i++){
                waiting[i]=crossing_dir[i]=0;
            }
            directions["North"]=0;
            directions["East"]=1;
            directions["South"]=2;
            directions["West"]=3;
            inti_condition_variables();
            pthread_mutex_init(&crossing,NULL);
        }

        void arrive(BAT b){
            pthread_mutex_lock(&crossing);
            int dir = directions[b.get_dir()];
            if(crossing_dir[dir]!=0){
                    waiting[dir]++;
                    switch(dir){
                        case 0:
                            pthread_cond_wait(&NorthQueue,&crossing);
                            break;
                        case 1:
                            pthread_cond_wait(&EastQueue,&crossing);
                            break;
                        case 2:
                            pthread_cond_wait(&SouthQueue,&crossing);
                            break;
                        case 3:
                            pthread_cond_wait(&WestQueue,&crossing);
                            break;
                    }
                   waiting[dir]--;

            }
            crossing_dir[dir]=1;
            cout << "BAT " << b.get_num() << " from " << b.get_dir() << " arrives at crossing" << endl;
            counter++;
            pthread_mutex_unlock(&crossing);
            sleep(1);
        }

        void cross(BAT b){
             // code to check traffic from the right, use counters,condition variables etc
            pthread_mutex_lock(&crossing);
            int dir = directions[b.get_dir()];
            // check the intersection for crossing
            if(crossing_dir[(dir+3)%4]!=0){
                    switch(dir){
                        case 0:
                            pthread_cond_wait(&NorthFirst,&crossing);
                            break;
                        case 1:
                            pthread_cond_wait(&EastFirst,&crossing);
                            break;
                        case 2:
                            pthread_cond_wait(&SouthFirst,&crossing);
                            break;
                        case 3:
                            pthread_cond_wait(&WestFirst,&crossing);
                            break;
                    }
            }
            crossing_dir[dir]=0;
            cout << "BAT " << b.get_num() << " from " << b.get_dir() << " crossing" << endl;
            sleep(1); // it takes one second for a BAT to cross
            counter--;
            pthread_mutex_unlock(&crossing);
        }
        void leave(BAT b){
            pthread_mutex_lock(&crossing);
            int dir = directions[b.get_dir()];
            //cout << "BAT " << b.get_num() << " from " << b.get_dir() << " leaving crossing" << endl;
            // check left
            switch(dir){
                case 0:
                    pthread_cond_signal(&EastFirst);
                    break;
                case 1:
                    pthread_cond_signal(&SouthFirst);
                    break;
                case 2:
                    pthread_cond_signal(&WestFirst);
                    break;
                case 3:
                    pthread_cond_signal(&NorthFirst);
                    break;
            }

            // check waiting signals
            switch(dir){
                case 0:
                    pthread_cond_signal(&NorthQueue);
                    break;
                case 1:
                    pthread_cond_signal(&EastQueue);
                    break;
                case 2:
                    pthread_cond_signal(&SouthQueue);
                    break;
                case 3:
                    pthread_cond_signal(&WestQueue);
            }
            pthread_mutex_unlock(&crossing);
        }
        void check()
        {
            if(counter ==4)
              {
                     pthread_cond_signal(&NorthFirst);
                     cout << "DEADLOCK DETECT : SEND SIGNAL TO NORTH\n" ;
              }

        }
};
//global object of monitor
Monitor monitor;
void* operation(void* bat){
    BAT Bat = *(BAT*)bat;
    monitor.arrive(Bat);
    monitor.cross(Bat);
    monitor.leave(Bat);
}
void* check(void* arg){
    while(true){
         monitor.check();
         sleep(3);
    }
}
int main()
{
    string command ;//= "nnwnsewwewn";
    cin >> command;
    pthread_t th[command.size()];
    pthread_t check_thread;
    pthread_create(&check_thread,NULL,check,NULL);
    for(int i=0;i<command.size();i++){
        BAT* bat;
        if(command[i]=='n') {
            bat = new BAT(i+1,"North");
        }
        if(command[i]=='e') {
            bat = new BAT(i+1,"East");
        }
        if(command[i]=='s') {
            bat = new BAT(i+1,"South");
        }
        if(command[i]=='w') {
            bat = new BAT(i+1,"West");
        }
        pthread_create(&th[i], NULL, &operation, bat);

    }
    for (int i = 0; i < command.size(); i++) {
        pthread_join(th[i], NULL);
    }
    return 0;
}
