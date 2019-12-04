#include <iostream>
#include<pthread.h>
#include <time.h>
#include<bits/stdc++.h>
#define ll long long
using namespace std;
vector < vector<ll> > matrix1 , matrix2;
int **result;
void* multiply_two_element(void* arg){
    pair<ll,ll> p =*(pair<ll,ll>*) arg;
    ll row = matrix1[0].size();
    ll sum = 0 ;
    for(ll i=0;i<row;i++){
        sum+=(matrix1[p.first].at(i)*matrix2[i].at(p.second));
    }
    result[p.first][p.second]=sum;
}

void* multiply_row_cols(void* arg){
    pair<ll,ll> p =*(pair<ll,ll>*) arg;
    ll row = matrix1[0].size();
    ll cols = matrix2.size();
    for(ll i=0;i<row;i++){
        ll sum =0 ;
        for(ll j=0;j<cols;j++){
            result[p.first][i]+=matrix1[p.first].at(j)*matrix2[j].at(i);
        }
    }
}
void multiplication_each_element(ll n , ll m){
     ll rows = n;
     ll cols = m;
     pthread_t threads[n][m];
     for(ll i=0;i<rows;i++){
        for(ll j=0;j<cols;j++){
            pair<ll,ll> *p = new pair<ll,ll>(i,j);
            pthread_create(&threads[i][j],NULL,&multiply_two_element,p);
        }
     }
     for(ll i=0;i<rows;i++){
        for(ll j=0;j<cols;j++){
                pthread_join(threads[i][j],NULL);
        }
     }
}

void multiplication_each_row(ll n , ll m){
     ll rows = n;
     pthread_t threads[n];
     for(ll i=0;i<rows;i++){
        pair<ll,ll> *p = new pair<ll,ll>(i,0);
        pthread_create(&threads[i],NULL,&multiply_row_cols,p);
     }
     for(ll i=0;i<rows;i++){
        pthread_join(threads[i],NULL);

     }
}
int main()
{
    freopen("input5.txt","r",stdin);
    freopen("output.txt","w",stdout);
    ll n, m ;
    cin >> n >> m ;
    matrix1 = vector<vector<ll> >(n);
    for(ll i=0;i<n;i++){
        for(ll j=0;j<m;j++){
            ll a ;
            cin >> a ;
            matrix1[i].push_back(a);
        }
    }
    ll n2,m2;
    cin >> n2 >> m2 ;
    matrix2 = vector<vector<ll> >(n2);
     for(ll i=0;i<n2;i++){
        for(ll j=0;j<m2;j++){
            ll a ;
            cin >> a ;
            matrix2[i].push_back(a);
        }
    }
    result = new int*[n];
    for(int i = 0;i < n; i++){
        result[i] =new int[m2];
    }
    for(int i =0; i<n;i++){
        for(int j = 0; j < m;j++){
            result[i][j] = 0;
        }
    }
    clock_t t1 = clock();
    multiplication_each_row(n,m2);
    clock_t t2 =clock();
    double time = (t2 - t1) /(double) CLOCKS_PER_SEC;
    cout << "taken time : " << time << endl;
    for(ll i=0;i<n;i++){
        for(ll j=0;j<m2;j++){
            cout << result[i][j] << " " ;
        }
        cout << "\n--------------------------------\n" ;
    }


    return 0;
}
