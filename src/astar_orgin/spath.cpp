//============================================================================
// Name        : spath.cpp
// Author      : dietoad
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "AStar.hpp"
#include <iostream>
#include <list>
#include <map>
#include <random>
#include <chrono>
#include <thread>
#include <set>
#include <benchmark/benchmark.h>
#include <queue>
#include "flat_hash_map.hpp"
#include<unordered_map>
//#include <EASTL/hash_map.h>
//#include <EASTL/memory.h>
//#include <EASTL/map.h>
//#include <EASTL/vector_map.h>
//#include <EASTL/unordered_map.h>
//#include <absl/container/node_hash_map.h>
//#include <absl/container/flat_hash_map.h>
using namespace std;
static AStar::Generator generator;
static std::vector<AStar::Node*> bheap;
struct Tdata{
    int id=0;
    Tdata()=default;
    Tdata(int id_):id(id_){}
};
//ska::unordered_map<int,Tdata> tumap;
ska::flat_hash_map<int,Tdata> tumap;
//eastl::hash_map<int,Tdata> ehmap;
//eastl::unordered_map<int,Tdata> ehmap;
//absl::flat_hash_map<int,Tdata> nmap;
std::unordered_map<int,Tdata> tmap;

std::vector<Tdata> tvec;
void BM_Astar(benchmark::State& state) {
  for (auto _ : state) {
//      auto path = generator.findPath({49, 76}, {87,70});
      generator.findPath({1,1}, {499,499});
  }
}

BENCHMARK(BM_Astar);

time_t GetCurrentTick()
{
    //man clock http://man7.org/linux/man-pages/man3/clock.3.html
    //since glibc 2.18, it is implemented on top of clock_gettime(2) using the CLOCK_PROCESS_CPUTIME_ID
    struct timespec ti={0};
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ti);
    return ti.tv_sec*1000+ti.tv_nsec/CLOCKS_PER_SEC;
}

#define LOG(fmt,...) fprintf(stdout,"%s:%d:%s " fmt ,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)

struct XXX
{
    int x;
    std::map<int,int> mp{};
};
int main(int argc, char** argv)
{
    // Set 2d map size.
    generator.setWorldSize({500, 500});
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<> dis(0,10000);
    char cc[500][500]={0};
    for(auto &it:cc){
    	for(auto &cit:it){
    		if(dis(gen)<3000){
    			cit=1;
    		}
    	}
    }
//    generator.setHeuristic(AStar::Heuristic::manhattan);
    generator.setHeuristic(AStar::Heuristic::octagonal);
//    generator.setHeuristic(AStar::Heuristic::manhattan);
    generator.setDiagonalMovement(true);
    generator.SetCollision([cc](const AStar::Vec2i& cord){
    	if(cord.x<500&&cord.y<500){
    		return cc[cord.x][cord.y]>0;
    	}
        return false;
    });
    for(auto i=0;i<1000000;++i)
    {
        tumap[i]={i};
//        tmap[i]={i};
//        ehmap[i]={i};
//        nmap[i]={i};
    }
//    auto path = generator.findPath({0, 0}, {50,50});
//    for(auto &p:path)
//    {
//        cout<<p.x<<" "<<p.y<<endl;
//    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
	return 0;
}
