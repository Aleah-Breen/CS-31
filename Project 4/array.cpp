#include<iostream>
#include<string>
#include<cctype>
#include <cassert>

using namespace std;
//simply concatenate the string to itself at each element.
//O(n)
int reduplicate(string a[], int n){
    if(n<0){
        return -1;
    }
    for(int i=0;i<n;i++){
        a[i]+=a[i];
    }
    return n;
};
//loop through until a[i] is the target 
//O(n)
int locate(string a[], int n, string target){
    if(n<0){
        return -1;
    }
    for(int i=0;i<n;i++){
        if(a[i]==target){
            return i;
        }
    }
    return -1;
};
//for each element look at all the elements after it. 
//Count how many after are greater than it and if that number is 0 return that position
//unsure of effeciency maybe nlogn or something weird. < O(n)^2
int locationOfMax(const string a[], int n){
    if(n<0){
        return -1;
    }
    for(int i=0;i<n;i++){
        int numLess=0;
        for(int j=0;j<n;j++){
            if(i!=j){
                if(a[i]<a[j]){
                    numLess++;
                }

            }
        }
        if(numLess==0){
            return i;
        }
    }
    return -1;
};
//store the first element you are removing then set each element equal to the element on its left
//at the very end set the last element in the subarray to the first one we stored
int circleLeft(string a[], int n, int pos){
    if(pos > n-1){
        return -1;
    }
    if(n<0){
        return -1;
    }
    string temp = a[pos];
    for(int i=pos;i<n-1;i++){
        a[i]=a[i+1];
    }
    a[n-1]=temp;
    return pos;
};
//O(n)
//if the current element doesnt equal the last element increment consecs and change what the last element was.
//could maybe be written using just a[i]!=a[i-1]???
int enumerateRuns(const string a[], int n){
    if(n<0){
        return -1;
    }
    if(n==0){
        return 0;
    }
    string current=a[0];
    int consecs=1;
    for(int i=1;i<n;i++){
        if(a[i]!=current){
            current=a[i];
            consecs++;
        }
    }
    return consecs;
};
//do intDiv and doubleDiv then compare. This tells me which loop to use regarding odd or even number of elements
//O(n/2) 
int flip(string a[], int n){
    if(n<0){
        return -1;
    }
    int intdiv=0;
    double dubdiv=0.0;
    intdiv=((n+1)/2.0)-1;
    dubdiv=((n+1)/2.0)-1;
    bool equal = intdiv==dubdiv ;
    if(equal){
        for(int i=0;i<intdiv;i++){
            string temp=a[i];
            a[i]=a[(n-i)-1];
            a[(n-i)-1]=temp;
        }
    }
    else{
        for(int i=0;i<=intdiv;i++){
            string temp=a[i];
            a[i]=a[(n-i)-1];
            a[(n-i)-1]=temp;
        }
    }
    return n;
};
//simply loop using n1 and n2 as the max iteration value until they are not equal O(n)
int locateDifference(const string a1[], int n1, const string a2[], int n2){
    if(n1<0 || n2<0){
        return -1;
    }
    for(int i=0;i<n1 && i<n2;i++){
        if(a1[i]!=a2[i]){
            return i;
        }
    }
    if(n1>n2){
        return n2;
    }
    return n1;
};

int subsequence(const string a1[], int n1, const string a2[], int n2){
    if(n1<0 || n2<0){
        return -1;
    }
    if(n2==0){
        return 0;
    }
    //loop through a1 until I see the first item of a2, then loop until the end of either a2 or a1. 
    //If the loop finishes and correct remains true it is a sequence
    //O(n)^2
    for(int i=0;i<n1;i++){
        if(a1[i]==a2[0]){

            bool correct = true;
            for(int j=0;j<n2 && j<n1;j++){
                if(a1[i+j]!=a2[j]){
                    correct=false;
                }
            }
            if(correct){
                return i;
            }
        }
    }
    return -1;
};

//for every string in a1, loop through a2 to see if it exits. O(n)^2
int locateAny(const string a1[], int n1, const string a2[], int n2){
    for(int i=0;i<n1;i++){
        for(int j=0;j<n2;j++){
            if(a1[i]==a2[j]){
                return i;
            }
        }
    }
    return -1;
};

int divide(string a[], int n, string divider){
    //logically if sorted alphabetically then it doesnt matter what divider is it will be correctly ordered
    //apply bubble sort O(2n)
    //later parse through a to find first index larger than divider, or return n
    for(int i=0; i<n;i++){
        for(int j=i+1;j<n;j++){
            if(a[i]>a[j]){
                string temp = a[i];
                a[i]=a[j];
                a[j]=temp;
            }
        }
    }
    for(int i=0;i<n;i++){
        if(a[i]>=divider){
            return i;
        }
    }
    return n;
};

int main() {
    string a[7] = { "a",    "b",    "d",    "",    "v",    "c",    "q"};
    assert(reduplicate(a, 4) == 4 && a[0] == "aa" && a[3] == "");
    assert(reduplicate(a, 0) ==  0);
    assert(reduplicate(a, -3) ==  -1);
    string b[7] = { "a",    "b",    "d",    "",    "v",    "c",    "q"};
    assert(locate(b, 7, "c") == 5);
    assert(locate(b, 7, "d") == 2);
    assert(locate(b, 2, "d") == -1);
    assert(locate(b, 1, "a") == 0);
    string c[7] = { "a",    "b",    "d",    "",    "v",    "v",    "q"};
    assert(locationOfMax(c, 7) == 4);
    assert(locationOfMax(c, 0) == -1);
    assert(locationOfMax(c, -4) == -1);
    string d[7] = { "a",    "b",    "d",    "",    "v",    "c",    "q"};
    assert(circleLeft(d, 4, 1) == 1 && d[1] == "d" && d[3] == "b");
    assert(circleLeft(d, 7, 1) == 1 && d[1] == "" && d[6] == "d");
    string e[7] = { "a",    "b",    "d",    "",    "v",    "c",    "q" };
    assert(circleLeft(e, 7, 2) == 2 && e[2] == "" && e[5] == "q");
    assert(circleLeft(e, 7, -1) ==  -1);
    assert(circleLeft(e, 0, 0) ==  -1);
    string f[7] = { "a",    "b",    "b",    "b",    "v",    "v",    "q" };
    assert(enumerateRuns(f, 7) == 4);
    string g[7] = { "a",    "a",    "a",    "a",    "a",    "a",    "a" };
    assert(enumerateRuns(g, 7) == 1);
    assert(enumerateRuns(g, 0) == 0);
    assert(enumerateRuns(g, -1) == -1);
    string h[7] = { "a",    "b",    "d",    "",    "v",    "c",    "q"};
    assert(flip(h, 3) == 3 && h[0] == "d" && h[2] == "a");
    assert(flip(h, 0) == 0);
    assert(flip(h, -1) == -1);
    string folks[6] = { "donald", "tim", "", "chris", "nikki", "donald" };
    string group[5] = { "donald", "tim", "donald", "", "chris" };
    int r = locateDifference(folks, 6, group, 5);
    int s = locateDifference(folks, 2, group, 1);
    string names[10] = { "nikki", "ron", "tim", "vivek", "doug" };
    string names1[10] = { "ron", "tim", "vivek" };
    int aa = subsequence(names, 5, names1, 3);
    string names2[10] = { "nikki", "vivek" };
    int bb = subsequence(names, 4, names2, 2);
    string name[10] = { "nikki", "ron", "tim", "vivek", "doug" };
    string set1[10] = { "donald", "doug", "vivek", "ron" };
    int cc = locateAny(name, 5, set1, 4);
    string set2[10] = { "chris", "asa" };
    int dd = locateAny(name, 5, set2, 2);
    string cand[6] = { "donald", "asa", "ron", "vivek", "chris", "nikki" };
    int ee = divide(cand, 6, "doug");
    string cand2[4] = { "ron", "vivek", "asa", "tim" };
    int ff = divide(cand2, 4, "tim");
}
