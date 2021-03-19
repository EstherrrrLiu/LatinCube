//
//  main.cpp
//  generatecube
//
//  Created by LoonyClown on 2021/3/17.
//  Copyright © 2021 LoonyClown. All rights reserved.
//

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>
 
template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v)
{
    auto it = v.cbegin();
    auto end = v.cend();
 
    os << '[';
    if (it != end)
    {
        os << *it;
        it = std::next(it);
    }
    while (it != end)
    {
        os << ", ";
        os << *it;
        it = std::next(it);
    }
    return os << ']';
}
 
void printSquare(const std::vector<std::vector<int>> &latin)
{
    for (auto &row : latin)
    {
        std::cout << row << '\n';
    }
    std::cout << '\n';
}
 
void latinSquare(int n,std::vector<std::vector<int>> &latin)
{
    if (n <= 0)
    {
        std::cout << "[]\n";
        return;
    }
 
    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto g = std::default_random_engine(seed);
 
    for (int i = 0; i < n; ++i)
    {
        std::vector<int> inner;
        for (int j = 0; j < n; ++j)
        {
            inner.push_back(j);
        }
        latin.push_back(inner);
    }
    
    // first row
    std::shuffle(latin[0].begin(), latin[0].end(), g);
 
    // middle row(s)
    for (int i = 1; i < n - 1; ++i)
    {
        bool shuffled = false;
 
        while (!shuffled)
        {
            std::shuffle(latin[i].begin(), latin[i].end(), g);
            for (int k = 0; k < i; ++k)
            {
                for (int j = 0; j < n; ++j)
                {
                    if (latin[k][j] == latin[i][j])
                    {
                        goto shuffling;
                    }
                }
            }
            shuffled = true;
 
        shuffling: {}
        }
    }
 
    // last row
    for (int j = 0; j < n; ++j)
    {
        std::vector<bool> used(n, false);
        for (int i = 0; i < n - 1; ++i)
        {
            used[latin[i][j]] = true;
        }
        for (int k = 0; k < n; ++k)
        {
            if (!used[k])
            {
                latin[n - 1][j] = k;
                break;
            }
        }
    }
 
    //printSquare(latin);
}

void latinCube(int n,std::vector<std::vector<std::vector<int>>> cube)
{
    if (n <= 0)
    {
        std::cout << "[]\n";
        return;
    }
    
    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto g1 = std::default_random_engine(seed);
    
    // first cross section
    std::vector<std::vector<int>> plane;
    latinSquare(n,plane);
    cube.push_back(plane);
    
    //initialization
    for (int i = 1; i < n; ++i)
    {
        std::vector<std::vector<int>> square;
        for (int j = 0; j < n; ++j)
        {
            std::vector<int> row;
            for (int k=0;k<n;k++)
            {
                row.push_back(k);
            }
            square.push_back(row);
        }
        cube.push_back(square);
    }
    
    
    // first tangent plane
    for (int i = 1; i < n - 1; ++i)
    {
           bool shuffled = false;
    
           while (!shuffled)
           {
               std::shuffle(cube[i][0].begin(), cube[i][0].end(), g1);
               for (int k = 0; k < i; ++k)
               {
                   for (int j = 0; j < n; ++j)
                   {
                       if (cube[k][0][j] == cube[i][0][j])
                       {
                           goto shuffling0;
                       }
                   }
               }
               shuffled = true;
    
           shuffling0: {}
           }
       }
    
restart:{}
    //middle cross section(s)
    for(int i=1;i<n-1;++i)
    {
        for(int j=1;j<n;++j)
        {
            bool shuffled=false;
            double start,stop,durationtime;
            start=clock();
        
            while(!shuffled)
            {

                std::shuffle(cube[i][j].begin(), cube[i][j].end(), g1);
                //checking parallel row(s)
                for(int k=0;k<i;++k)
                {
                    for(int l=0;l<n;++l)
                    {
                        if(cube[k][j][l]==cube[i][j][l])
                        {
                            goto shuffling1;
                        }
                    }
                }
                for(int m=0;m<j;++m)
                {
                    for(int h=0;h<n;++h)
                    {
                        if(cube[i][m][h]==cube[i][j][h])
                        {
                            goto shuffling1;
                        }
                    }
                }
                
                shuffled=true;
                
            shuffling1: {}
                stop=clock();
                durationtime=((double)(stop-start))/CLK_TCK;
                if(durationtime>=2){goto restart;}
            }

        }
    }
    
    // last cross section
    for(int k=0;k<n;++k)
    {
        for (int j = 0; j < n; ++j)
        {
            std::vector<bool> used(n, false);
            for (int i = 0; i < n - 1; ++i)
            {
                used[cube[k][j][i]] = true;
            }
            for (int l = 0; l < n; ++l)
            {
                if (!used[l])
                {
                cube[n - 1][j][k]= l;
                break;
                }
             }
        }
    }
    
    for(int i=0;i<n;++i)
    {
        std::cout<<"level"<<i+1<<":\n";
        printSquare(cube[i]);
    }

}

int main()
{
    std::vector<std::vector<std::vector<int>>> cube;
    latinCube(5,cube);
    return 0;
}
