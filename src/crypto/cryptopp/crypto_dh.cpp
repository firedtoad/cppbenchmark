// Copyright 2023 The Division Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// Author dietoad@gmail.com && firedtoad@gmail.com

#include <benchmark/benchmark.h>
#include <cryptopp/dh.h>
#include <cryptopp/dh2.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/osrng.h>
#include <iostream>
#include <numeric>
#include <vector>

static unsigned char str_key[16];

static void BM_DH(benchmark::State &state)
{
    using namespace CryptoPP;
    AutoSeededRandomPool prng;
    Integer p, q, g;
    PrimeAndGenerator pg;
    pg.Generate(1, prng, 512, 511);
    p = pg.Prime();
    q = pg.SubPrime();
    g = pg.Generator();
    DH dh(p, q, g);
    for (auto _ : state)
    {
        SecByteBlock clientPri(dh.PrivateKeyLength()), clientPub(dh.PublicKeyLength());
        dh.GenerateKeyPair(prng, clientPri, clientPub);
        SecByteBlock serverPri(dh.PrivateKeyLength()), serverPub(dh.PublicKeyLength());
        dh.GenerateKeyPair(prng, serverPri, serverPub);
        SecByteBlock clientShare(dh.AgreedValueLength()), serverShare(dh.AgreedValueLength());
        dh.Agree(clientShare, clientPri, serverPub);
        dh.Agree(serverShare, serverPri, clientPub);
    }
}

BENCHMARK(BM_DH);

static void BM_EasyDH(benchmark::State &state)
{
#define P 0xffffffffffffffc5ull
#define G 5

    using namespace CryptoPP;
    AutoSeededRandomPool prng;

    for (auto _ : state)
    {
        Integer BP(Integer::POSITIVE,0,P);
        Integer BG(Integer::POSITIVE,0,G);
        Integer clientPri;
        Integer serverPri;
        clientPri.Randomize(prng, 64);
        serverPri.Randomize(prng, 64);
        Integer clientPub = a_exp_b_mod_c(BG, clientPri, BP);
        Integer serverPub = a_exp_b_mod_c(BG, serverPri, BP);

        auto sharedClient = a_exp_b_mod_c(serverPub, clientPri, BP);
        auto sharedServer = a_exp_b_mod_c(clientPub, serverPri, BP);
    }
}

BENCHMARK(BM_EasyDH);

int main(int argc, char **argv)
{
    {
#define P 0xffffffffffffffc5ull
#define G 5

        using namespace CryptoPP;
        AutoSeededRandomPool prng;
        Integer BP(Integer::POSITIVE,0,P);
        Integer BG(Integer::POSITIVE,0,G);
        Integer clientPri;
        Integer serverPri;
        clientPri.Randomize(prng, 64);
        serverPri.Randomize(prng, 64);
        Integer clientPub = a_exp_b_mod_c(BG, clientPri, BP);
        Integer serverPub = a_exp_b_mod_c(BG, serverPri, BP);

        auto sharedClient = a_exp_b_mod_c(serverPub, clientPri, BP);
        auto sharedServer = a_exp_b_mod_c(clientPub, serverPri, BP);

//        std::cout << clientPri << ' ' << clientPub << '\n';
//        std::cout << serverPri << ' ' << serverPub << '\n';
//        std::cout << sharedClient << ' ' << sharedServer << '\n';
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}