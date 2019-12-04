#include <iostream>
#include <pthread.h>
#include <vector>
#define ll int
using namespace std;
class merge_sort_arg{
public:
    ll left;
    ll right;

    merge_sort_arg(ll left2,ll right2){
        left = left2;
        right = right2;
    }
};
vector<ll> arr;
void Merge(ll low , ll high){
        ll mid = (low+high)/2;
        ll left = low ;
        ll right = mid+1;
        ll b[high-low+1];
        ll cur=0;
        while(left<=mid&&right<=high){
            if(arr[left] > arr[right])
                b[cur++]=arr[right++];
            else
                b[cur++]=arr[left++];
        }
        while(left<=mid) b[cur++]=arr[left++];
        while(right<=high) b[cur++]=arr[right++];
        for(ll i=0;i<high-low+1;i++) arr[low+i]=b[i];
}

void* mergeSort(void *arg){
    merge_sort_arg p = *(merge_sort_arg *)arg;
    ll left = p.left;
    ll right = p.right;
    ll mid = (left+right)/2;


    if(left<right){
        merge_sort_arg *left_pair = new merge_sort_arg(left,mid);
        merge_sort_arg *right_pair = new merge_sort_arg(mid+1,right);
        pthread_t left_thread , right_thread ;
        pthread_create(&left_thread, NULL, mergeSort,left_pair);
        pthread_create(&right_thread, NULL, mergeSort,right_pair);
        pthread_join(left_thread,NULL);
        pthread_join(right_thread,NULL);
        Merge(left,right);
    }

}
int main()
{
    freopen("input.txt","r",stdin);
    ll n ;
    cin >> n ;
    arr = vector<ll>(n);
    for(ll i=0;i<n;i++){
         cin >> arr[i];
    }
    merge_sort_arg* p = new merge_sort_arg(0,n-1);
    mergeSort(p);
    for(ll i=0;i<n;i++){
        cout << arr[i] << " " ;
    }
    return 0;
}
