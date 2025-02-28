#include <benchmark/benchmark.h>
#include "hashset.h"
#include <unordered_set>
#include <string>
#include <vector>
#include <iostream>

static void BM_CustomHashSet_Insert(benchmark::State &state)
{
    HashSet hashset;
    std::vector<std::string> keys(state.range(0), "test");

    for (auto _ : state)
    {
        for (auto &key : keys)
        {
            hashset.insert(&key);
        }
    }
}
BENCHMARK(BM_CustomHashSet_Insert)->Range(8, 1 << 8);

static void BM_StdUnorderedSet_Insert(benchmark::State &state)
{
    std::unordered_set<std::string *> hashset;
    std::vector<std::string> keys(state.range(0), "test");
    for (auto _ : state)
    {
        for (auto &key : keys)
        {
            hashset.insert(&key);
        }
    }
}
BENCHMARK(BM_StdUnorderedSet_Insert)->Range(8, 1 << 8);

static void BM_CustomHashSet_Lookup(benchmark::State &state)
{
    HashSet hashset;
    std::vector<std::string> keys(state.range(0), "test");
    for (auto &key : keys)
    {
        hashset.insert(&key);
    }
    for (auto _ : state)
    {
        for (auto &key : keys)
        {
            benchmark::DoNotOptimize(hashset.lookup(&key));
        }
    }
}
BENCHMARK(BM_CustomHashSet_Lookup)->Range(8, 1 << 8);

static void BM_StdUnorderedSet_Lookup(benchmark::State &state)
{
    std::unordered_set<std::string *> hashset;
    std::vector<std::string> keys(state.range(0), "test");
    for (auto &key : keys)
    {
        hashset.insert(&key);
    }
    for (auto _ : state)
    {
        for (auto &key : keys)
        {
            benchmark::DoNotOptimize(hashset.find(&key) != hashset.end());
        }
    }
}
BENCHMARK(BM_StdUnorderedSet_Lookup)->Range(8, 1 << 8);

static void BM_CustomHashSet_Remove(benchmark::State &state)
{
    HashSet hashset;
    std::vector<std::string> keys(state.range(0), "test");
    for (auto &key : keys)
    {
        hashset.insert(&key);
    }
    for (auto _ : state)
    {
        for (auto &key : keys)
        {
            hashset.remove(&key);
        }
    }
}
BENCHMARK(BM_CustomHashSet_Remove)->Range(8, 1 << 8);

static void BM_StdUnorderedSet_Remove(benchmark::State &state)
{
    std::unordered_set<std::string *> hashset;
    std::vector<std::string> keys(state.range(0), "test");
    for (auto &key : keys)
    {
        hashset.insert(&key);
    }
    for (auto _ : state)
    {
        for (auto &key : keys)
        {
            hashset.erase(&key);
        }
    }
}
BENCHMARK(BM_StdUnorderedSet_Remove)->Range(8, 1 << 8);

BENCHMARK_MAIN();
