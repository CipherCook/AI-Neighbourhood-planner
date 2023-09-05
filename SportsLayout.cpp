#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <random>
#include <chrono>
// #define int long long
using namespace std;

#define bug(...)       __f (#__VA_ARGS__, __VA_ARGS__)
#define print(a)       for(auto x : a) cout << x << " "; cout << endl
#define ll long long
template <typename Arg1>
void __f (const char* name, Arg1&& arg1) { cout << name << " : " << arg1 << endl; }
template <typename Arg1, typename... Args>
void __f (const char* names, Arg1&& arg1, Args&&... args)
{
    const char* comma = strchr (names + 1, ',');
    cout.write (names, comma - names) << " : " << arg1 << " | "; __f (comma + 1, args...);
}

#include "SportsLayout.h"

    SportsLayout::SportsLayout(string inputfilename)
    {

        readInInputFile(inputfilename);
        mapping= new ll[z];

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

    long long SportsLayout::getCost(vector<ll> v)
    {
        long long cost=0;
        for(ll i=0;i<z;i++)
        {
           for(ll j=0;j<z;j++)
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

            ll **tempT;
            ll **tempN;

            tempT = new ll*[l];
            for (ll i = 0; i < l; ++i)
                tempT[i] = new ll[l];

            tempN = new ll*[z];

            for (ll i = 0; i < z; ++i)
                tempN[i] = new ll[z];

            for(ll i=0;i<z;i++){
                for(ll j=0;j<z;j++)
                ipfile>>tempN[i][j];
            }

            for(ll i=0;i<l;i++){
                for(ll j=0;j<l;j++)
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

    vector<ll> SportsLayout::restart_state(vector<pair<ll,ll>> tcounts, vector<pair<ll,ll>> zcounts)
    {
         // Create a random number generator
        random_device rd;
        mt19937 gen(rd()); // Mersenne Twister engine
        uniform_int_distribution<> zrand(0, z-1);
        uniform_int_distribution<> lrand(0, l-1);
        uniform_real_distribution<> triangular_dist(0, 1);
        ll swap_count = static_cast<ll>(z * (1 - sqrt(triangular_dist(gen)))) + 2; // generates a random number between [0,z] biased towards 0 and gradually decreasing towards N
        // cout << swap_count -2 <<endl;
        while(swap_count)
        {
            ll i1 = zrand(gen);
            ll i2 = lrand(gen);
            swap(tcounts[i1], tcounts[i2]);
            swap_count--;
        }
        vector<ll> res(z, 0);
        for(ll i=0;i<z;i++)
            {res[zcounts[i].second-1]=tcounts[i].second;}
        return res;
        
    }

    long long SportsLayout::compute_cost(long long cur_cost,ll index1, ll index2, vector<ll> temp){
        ll location1 = temp[index1];
        ll location2 = temp[index2];
        long long extra_cost = 0;
        for(ll i=0;i<z;++i){
            if(i==index1 || i==index2)continue;
            extra_cost+=(N[i][index1]-N[i][index2])*(T[temp[i]-1][location2-1]-T[temp[i]-1][location1-1]);
            extra_cost+=(N[index1][i]-N[index2][i])*(T[location2-1][temp[i]-1]-T[location1-1][temp[i]-1]);
        }
        return cur_cost+extra_cost;
    }

    long long SportsLayout::compute_cost_outside(long long cur_cost,ll index1, ll location2, vector<ll> temp){
        ll location1 = temp[index1];
        long long extra_cost = 0;
        for(ll i=0;i<z;++i){
            if(i==index1) continue;
            extra_cost+=(N[i][index1]+N[index1][i])*(T[temp[i]-1][location2-1]-T[temp[i]-1][location1-1]);
        }
        return cur_cost+extra_cost;
    }



    pair<vector<ll>,long long> SportsLayout::get_best_nbr(long long cur_cost, vector<ll> temp){
        bool included[l+1] = {0}; //mapping is 0 based indexing but its y-values are 1-based
        for(ll i=0;i<z;++i){
            included[temp[i]] = 1;
            
        }
        vector<ll> not_vis;
        for(ll i=1; i<=l; i++)
        {
            if(!included[i]) {not_vis.push_back(i); }
        }
        
        long long best_cost = LONG_MAX;
        ll index1 = 0,index2 = 0;
        ll jumps = 0;
        for(ll i=0;i<z;++i){
            for(ll j=i+1;j<z;++j){
                long long possible_cost = compute_cost(cur_cost,i,j, temp);
                if(possible_cost<best_cost){
                    best_cost = possible_cost;
                    index1 = i;
                    index2 = j;
                    jumps++;
                }
                if(jumps>7 && (l>=600 || z*(l-z) > 250000)){  break;}
                else if(jumps>12 && (l>=300 || z*(l-z) > 50000)){  break;}
                else if(jumps>12 && l>=100) {break;}
            }
        }
        swap(temp[index1],temp[index2]);
        cur_cost = getCost(temp);
        
        index1 = -1;
        ll loc = -1;
        for(ll i=0; i<z; i++)
        {
            for(auto j: not_vis)
            {
                long long possible_cost = compute_cost_outside(cur_cost,i,j, temp);
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
        const ll targetDuration = time*60 - 2;
        auto startTime = chrono::system_clock::now();
        auto endTime = chrono::system_clock::now();
        chrono::duration<double> duration = endTime - startTime;
        if(duration.count()>=targetDuration)return;

        vector<pair<ll,ll>> zcounts(z ,{0,0});
        for(ll i=0; i<z; i++)
        {
            zcounts[i].second = i+1;
            for(ll j = 0; j<z; j++)
            {
                if(i==j) continue;
                zcounts[i].first += N[i][j];
                zcounts[j].first += N[i][j];
            }
        }
        sort(zcounts.begin(), zcounts.end(), greater<pair<ll,ll>>());
        vector<pair<ll,ll>> tcounts(l, {0,0});
        for(ll i=0; i<l; i++)
        {
            tcounts[i].second = i+1;
            for(ll j = 0; j<l; j++)
            {
                tcounts[i].first += T[i][j];
            }
        }
        sort(tcounts.begin(), tcounts.end());
        for(ll i=0;i<z;i++)
            mapping[zcounts[i].second-1]=tcounts[i].second;

        //initial state ready

        long long initial_cost = cost_fn();
        vector<ll> greedy_start;
        for(ll i=0; i<z; i++) greedy_start.push_back(mapping[i]);
        long long best_cost = initial_cost;
        long long c1 = getCost(greedy_start);
        // bug(c1, initial_cost);

        pair <vector<ll>,long long> greedy_nbr = get_best_nbr(initial_cost, greedy_start);
        // greedy_nbr.second = getCost(greedy_nbr.first);
        while(greedy_nbr.second < initial_cost) {
            endTime = chrono::system_clock::now();
            duration = endTime - startTime;
            if(duration.count()>=targetDuration)return;
            initial_cost = greedy_nbr.second;
            bug(greedy_nbr.second);
            vector<ll> nbr = greedy_nbr.first;
            for(ll i=0;i<z;++i){
                mapping[i]=nbr[i];
            }
            // bug(c, getCost(nbr));
            greedy_nbr = get_best_nbr(initial_cost, nbr);
            // greedy_nbr.second = getCost(greedy_nbr.first);
        }
        best_cost = initial_cost;
        endTime = chrono::system_clock::now();
        duration = endTime - startTime;
        if(duration.count()>=targetDuration)return;

        while(true)
        {
            endTime = chrono::system_clock::now();
            duration = endTime - startTime;
            if(duration.count()>=targetDuration)return;
            vector<ll> start_state = restart_state(tcounts, zcounts);
            cout<<"RESTART\n";
            long long cur_cost = getCost(start_state);
            pair <vector<ll>,long long> greedy_nbr = get_best_nbr(cur_cost, start_state);
            // greedy_nbr.second = getCost(greedy_nbr.first);
            while(greedy_nbr.second < cur_cost) {
                endTime = chrono::system_clock::now();
                duration = endTime - startTime;
                if(duration.count()>=targetDuration)return;
                cur_cost = greedy_nbr.second;
                vector<ll> nbr = greedy_nbr.first;
                bug(cur_cost);
                if(cur_cost < best_cost)
                {
                    for(ll i=0;i<z;++i){
                        mapping[i]=nbr[i];
                    }
                    best_cost = cur_cost;
                }
                bug(best_cost);
                greedy_nbr = get_best_nbr(cur_cost, nbr);
                // greedy_nbr.second = getCost(greedy_nbr.first);
            }
        }
    }