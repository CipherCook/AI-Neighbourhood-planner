#ifndef SPORTSLAYOUT_H
#define	SPORTSLAYOUT_H

#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

class SportsLayout{

    private:
    int z,l;
    int** T;
    int **N;
    int time;
    int *mapping;

    public:
    SportsLayout(string inputfilename);

    bool check_output_format();

    // void readOutputFile(string output_filename);
    
    long long cost_fn();

    void write_to_file(string outputfilename);

    void readInInputFile(string inputfilename);

    long long compute_cost(long long cur_cost,int index1, int index2);
    long long compute_cost_outside(long long cur_cost,int index1, int location2);
    pair<vector<int>,long long> get_best_nbr(long long cur_cost);
    vector<int> restart_state(vector<pair<int,int>> tcounts, vector<pair<int,int>> zcounts);
    void compute_allocation();


};


#endif
