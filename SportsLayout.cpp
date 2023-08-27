#include <fstream>
#include <iostream>
#include <bits/stdc++.h>

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

    long long SportsLayout::compute_cost(long long cur_cost,int index1, int index2){
        int location1 = mapping[index1];
        int location2 = mapping[index2];
        long long extra_cost = 0;
        for(int i=0;i<z;++i){
            if(i==index1 || i==index2)continue;
            extra_cost+=(N[i][index1]-N[i][index2])*(T[mapping[i]-1][location2-1]-T[mapping[i]-1][location1-1]);
            extra_cost+=(N[index1][i]-N[index2][i])*(T[location2-1][mapping[i]-1]-T[location1-1][mapping[i]-1]);
        }
        return cur_cost+extra_cost;
    }

    long long SportsLayout::compute_cost_outside(long long cur_cost,int index1, int location2){
        int location1 = mapping[index1];
        long long extra_cost = 0;
        for(int i=0;i<z;++i){
            if(i==index1) continue;
            extra_cost+=(N[i][index1]+N[index1][i])*(T[mapping[i]-1][location2-1]-T[mapping[i]-1][location1-1]);
        }
        return cur_cost+extra_cost;
    }



    pair<vector<int>,long long> SportsLayout::get_best_nbr(long long cur_cost){
        vector<int> temp;
        bool included[l+1] = {0}; //mapping is 0 based indexing but its y-values are 1-based
        for(int i=0;i<z;++i){
            temp.push_back(mapping[i]);
            included[mapping[i]] = 1;
        }
        vector<int> not_vis;
        for(int i=1; i<=l; i++)
        {
            if(!included[i]) not_vis.push_back(i);
        }

        long long best_cost = LONG_MAX;
        int index1 = 0,index2 = 0;
        for(int i=0;i<z;++i){
            for(int j=i+1;j<z;++j){
                long long possible_cost = compute_cost(cur_cost,i,j);
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
                long long possible_cost = compute_cost_outside(cur_cost,i,j);
                cout << i<< " " << j<< " "<<possible_cost<<endl;
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

        long long initial_cost = cost_fn();
        pair <vector<int>,long long> best_nbr_pair = get_best_nbr(initial_cost);
        while(best_nbr_pair.second<initial_cost){
            initial_cost = best_nbr_pair.second;
            vector<int> nbr = best_nbr_pair.first;
            for(int i=0;i<z;++i){
                mapping[i]=nbr[i];
            }
            best_nbr_pair = get_best_nbr(initial_cost);
        }
        // for(int i=0;i<z;++i){
        //     cout<<mapping[i]<<" ";
        // }
        // cout<<endl;

    }