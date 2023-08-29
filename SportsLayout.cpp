#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <random>

using namespace std;

#include "SportsLayout.h"

    SportsLayout::SportsLayout(string inputfilename)
    {

        readInInputFile(inputfilename);
        mapping= new int[z];

    }

    bool SportsLayout::check_output_format()
    {

        vector<bool> visited(l,false);
        for(int i=0;i<z;i++)
        {
            if((mapping[i]>=1 && mapping[i]<=l))
            {
                if(!visited[mapping[i]-1])
                visited[mapping[i]-1]=true;
                else
                {
                    cout<<"Repeated locations, check format\n";
                    return false;
                }
            }
            else
            {
                cout<<"Invalid location, check format\n";
                return false;
            }
        }

        return true;

    }

    // void SportsLayout::readOutputFile(string output_filename)
    // {
    //         fstream ipfile;
	//         ipfile.open(output_filename, ios::in);
    //         if (!ipfile) {
    //             cout << "No such file\n";
    //             exit( 0 );
    //         }
    //         else {

    //             vector<int> ip;

    //             while (1) {
    //                 int t;
    //                 ipfile >> t;
    //                 ip.push_back(t);
    //                 if (ipfile.eof())
    //                     break;

    //             }

    //         if(ip.size()!=z)
    //         {
    //             cout<<"number of values not equal to number of zones, check output format\n";
    //             exit(0);
    //         }
    //         for(int i=0;i<z;i++)
    //         mapping[i]=ip[i];
    //     ipfile.close();

    //     if(!check_output_format())
    //         exit(0);
    //     cout<<"Read output file, format OK"<<endl;
    //         }
    // }


    long long SportsLayout::cost_fn(){
        long long cost=0;
        for(int i=0;i<z;i++)
        {
           for(int j=0;j<z;j++)
           {
                cost+=(long long)N[i][j]*(long long)T[mapping[i]-1][mapping[j]-1];
           }
        }
        return cost;
    }

    long long SportsLayout::getCost(vector<int> v)
    {
        long long cost=0;
        for(int i=0;i<z;i++)
        {
           for(int j=0;j<z;j++)
           {
                cost+=(long long)N[i][j]*(long long)T[v[i]-1][v[j]-1];
           }
        }
        return cost;
    }

    void SportsLayout::readInInputFile(string inputfilename)
    {
        fstream ipfile;
        ipfile.open(inputfilename, ios::in);
        if (!ipfile) {
            cout << "No such file\n";
            exit( 0 );
        }
        else {
            ipfile>> time;
            ipfile >> z;
            ipfile >> l;

            if(z>l){
                cout<<"Number of zones more than locations, check format of input file\n";
                exit(0);
            }

            int **tempT;
            int **tempN;

            tempT = new int*[l];
            for (int i = 0; i < l; ++i)
                tempT[i] = new int[l];

            tempN = new int*[z];

            for (int i = 0; i < z; ++i)
                tempN[i] = new int[z];

            for(int i=0;i<z;i++){
                for(int j=0;j<z;j++)
                ipfile>>tempN[i][j];
            }

            for(int i=0;i<l;i++){
                for(int j=0;j<l;j++)
                ipfile>>tempT[i][j];
            }

            ipfile.close();
            T= tempT;
            N= tempN;
        }
    }

    void SportsLayout::write_to_file(string outputfilename){
         // Open the file for writing
        ofstream outputFile(outputfilename);
        // Check if the file is opened successfully
        if (!outputFile.is_open()) {
            cerr << "Failed to open the file for writing." << std::endl;
            exit(0);
        }
        for(int i=0;i<z;i++)
            outputFile<<mapping[i]<<" ";
        // Close the file
        outputFile.close();
        cout << "Allocation written to the file successfully." << endl;
    }

    vector<int> SportsLayout::restart_state(vector<pair<int,int>> tcounts, vector<pair<int,int>> zcounts)
    {
         // Create a random number generator
        random_device rd;
        mt19937 gen(rd()); // Mersenne Twister engine
        uniform_int_distribution<> zrand(0, z-1);
        uniform_int_distribution<> lrand(0, l-1);
        uniform_real_distribution<> triangular_dist(0, 1);
        int swap_count = static_cast<int>(z * (1 - sqrt(triangular_dist(gen)))) + 2; // generates a random number between [0,z] biased towards 0 and gradually decreasing towards N
        // cout << swap_count -2 <<endl;
        while(swap_count)
        {
            int i1 = zrand(gen);
            int i2 = lrand(gen);
            swap(tcounts[i1], tcounts[i2]);
            swap_count--;
        }
        vector<int> res(z, 0);
        for(int i=0;i<z;i++)
            {res[zcounts[i].second-1]=tcounts[i].second;}
        return res;
        
    }

    long long SportsLayout::compute_cost(long long cur_cost,int index1, int index2, int location1, int location2){
        
        long long extra_cost = 0;
        for(int i=0;i<z;++i){
            if(i==index1 || i==index2)continue;
            extra_cost+=(N[i][index1]-N[i][index2])*(T[mapping[i]-1][location2-1]-T[mapping[i]-1][location1-1]);
            extra_cost+=(N[index1][i]-N[index2][i])*(T[location2-1][mapping[i]-1]-T[location1-1][mapping[i]-1]);
        }
        return cur_cost+extra_cost;
    }

    long long SportsLayout::compute_cost_outside(long long cur_cost,int index1, int location2, int location1){
        
        long long extra_cost = 0;
        for(int i=0;i<z;++i){
            if(i==index1) continue;
            extra_cost+=(N[i][index1]+N[index1][i])*(T[mapping[i]-1][location2-1]-T[mapping[i]-1][location1-1]);
        }
        return cur_cost+extra_cost;
    }



    pair<vector<int>,long long> SportsLayout::get_best_nbr(long long cur_cost, vector<int> temp){
        bool included[l+1] = {0}; //mapping is 0 based indexing but its y-values are 1-based
        for(int i=0;i<z;++i){
            included[temp[i]] = 1;
            cout<<temp[i]<<" ";
        }
        cout<<endl;
        vector<int> not_vis;
        for(int i=1; i<=l; i++)
        {
            if(!included[i]) {not_vis.push_back(i); cout << i<<", ";}
        }
        

        long long best_cost = LONG_MAX;
        int index1 = 0,index2 = 0;
        for(int i=0;i<z;++i){
            for(int j=i+1;j<z;++j){
                // cout<<i<<j<<temp[i]<<temp[j]<<endl;
                long long possible_cost = compute_cost(cur_cost,i,j, temp[i], temp[j]);
                // cout <<i<< " " << j << " " << possible_cost <<endl;
                if(possible_cost<best_cost){
                    best_cost = possible_cost;
                    index1 = i;
                    index2 = j;
                }
            }
        }
        swap(temp[index1],temp[index2]);
        index1 = -1;
        int loc = -1;
        for(int i=0; i<z; i++)
        {
            for(auto j: not_vis)
            {
                int location1 = temp[i];
                long long possible_cost = compute_cost_outside(cur_cost,i,j, location1);
                // cout << i<< " " << j<< " "<<possible_cost<<endl;
                if(possible_cost<best_cost){
                    best_cost = possible_cost;
                    index1 = i;
                    loc = j;
                }
            }
        }
        if(index1 != -1)
        {
            temp[index1] = loc;
        }
        return {temp,best_cost};
    }

    void SportsLayout::compute_allocation()
    {
        // time_t start, end;
        // time(&start);
        vector<pair<int,int>> zcounts(z ,{0,0});
        for(int i=0; i<z; i++)
        {
            zcounts[i].second = i+1;
            for(int j = 0; j<z; j++)
            {
                if(i==j) continue;
                zcounts[i].first += N[i][j];
                zcounts[j].first += N[i][j];
            }
        }
        sort(zcounts.begin(), zcounts.end(), greater<pair<int,int>>());
        vector<pair<int,int>> tcounts(l, {0,0});
        for(int i=0; i<l; i++)
        {
            tcounts[i].second = i+1;
            for(int j = 0; j<l; j++)
            {
                tcounts[i].first += T[i][j];
            }
        }
        sort(tcounts.begin(), tcounts.end());
        for(int i=0;i<z;i++)
            mapping[zcounts[i].second-1]=tcounts[i].second;

        //initial state ready

        long long initial_cost = cost_fn();

        vector<int> best_nbr;
        for(int i=0; i<z; i++) best_nbr.push_back(mapping[i]);
        long long best_cost = initial_cost;

        pair <vector<int>,long long> greedy_nbr = get_best_nbr(initial_cost, best_nbr);
        while(greedy_nbr.second < initial_cost) {
            initial_cost = greedy_nbr.second;
            vector<int> nbr = greedy_nbr.first;
            for(int i=0;i<z;++i){
                mapping[i]=nbr[i];
            }
            greedy_nbr = get_best_nbr(initial_cost, nbr);
        }
        best_cost = initial_cost;
        // reached local max here
        //get random permutation with heuristic as new start state
        // keep finding best 
        int iters = 0;
        while(iters < 2)
        {
            vector<int> start_state = restart_state(tcounts, zcounts);
            cout<<"RESTART: \n";
            for(auto x: start_state) cout<<x<<" ";
            cout<<'\n';
            long long cur_cost = getCost(start_state);
            cout << cur_cost << endl;
            
            pair <vector<int>,long long> greedy_nbr = get_best_nbr(cur_cost, start_state);
            cout << "nbr_cost = " << greedy_nbr.second<<endl;
            // break;
            while(greedy_nbr.second < cur_cost) {
                cur_cost = greedy_nbr.second;
                vector<int> nbr = greedy_nbr.first;
                
                if(cur_cost > best_cost)
                {
                    for(int i=0;i<z;++i){
                        mapping[i]=nbr[i];
                    }
                    best_cost = cur_cost;
                }
                cout << "cur_cost = "<< cur_cost << endl;
                greedy_nbr = get_best_nbr(cur_cost, nbr);
                cout << "nbr_cost = " << greedy_nbr.second<<endl;
                if(cur_cost < -100) break;
            }
            iters++;
        }



        vector<int> rest = restart_state(tcounts, zcounts);
        // cout<<"RESTART: \n";
        // for(auto x: rest) cout<<x<<" ";

        //.....final assignment.......
        for(int i=0;i<z;++i){
                mapping[i]=best_nbr[i];
        }


        for(int i=0;i<z;++i){
            cout<<mapping[i]<<" ";
        }
        cout<<endl;

    }
