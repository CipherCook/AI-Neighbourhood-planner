
#ifndef SPORTSLAYOUT_H
#define	SPORTSLAYOUT_H
#define ll long long
#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

class SportsLayout{

    private:
    ll z,l;
    ll** T;
    ll **N;
    ll time;
    ll *mapping;

    public:
    SportsLayout(string inputfilename);

    bool check_output_format();

    // void readOutputFile(string output_filename);
    
    long long cost_fn();

    void write_to_file(string outputfilename);

    void readInInputFile(string inputfilename);

    long long compute_cost(long long cur_cost,ll index1, ll index2, vector<ll> temp );
    long long compute_cost_outside(long long cur_cost,ll index1, ll location2, vector<ll> temp);
    pair<vector<ll>,long long> get_best_nbr(long long cur_cost, vector<ll> temp);
    vector<ll> restart_state(vector<pair<ll,ll>> tcounts, vector<pair<ll,ll>> zcounts);
    long long getCost(vector<ll> v);
    void compute_allocation();


};


#endif