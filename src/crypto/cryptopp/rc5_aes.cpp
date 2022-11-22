// Copyright 2020 The Division Authors.
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
#include <cryptopp/aes.h>
#include <cryptopp/chacha.h>
#include <cryptopp/des.h>
#include <cryptopp/modes.h>
#include <cryptopp/rc5.h>
#include <cryptopp/rc6.h>
#include <cryptopp/salsa.h>
#include <numeric>
#include <vector>

static unsigned char str_key[16];
template <typename T, size_t kLen = 16> static void BM_CRYPTO_NO_IV(benchmark::State &state)
{
    T enc(str_key, kLen);
    std::vector<uint8_t> in(state.range(0));
    for (auto _ : state)
    {
        std::vector<uint8_t> out(state.range(0));
        enc.ProcessData(out.data(), in.data(), in.size());
    }
}

template <typename T,size_t kLen = 16> static void BM_CRYPTO(benchmark::State &state)
{
    uint8_t iv[kLen];
    T enc(str_key, kLen, iv);
    std::vector<uint8_t> in(state.range(0));
    for (auto _ : state)
    {
        std::vector<uint8_t> out(state.range(0));
        enc.ProcessData(out.data(), in.data(), in.size());
    }
}

BENCHMARK_TEMPLATE(BM_CRYPTO_NO_IV, CryptoPP::ECB_Mode<CryptoPP::RC5>::Encryption)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::CBC_Mode<CryptoPP::RC5>::Encryption)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::CFB_Mode<CryptoPP::RC5>::Encryption)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::OFB_Mode<CryptoPP::RC5>::Encryption)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::CBC_CTS_Mode<CryptoPP::RC5>::Encryption)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::CTR_Mode<CryptoPP::RC5>::Encryption)->Range(16, 1024);

BENCHMARK_TEMPLATE(BM_CRYPTO_NO_IV, CryptoPP::ECB_Mode<CryptoPP::RC6>::Encryption)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::CBC_Mode<CryptoPP::RC6>::Encryption)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::CFB_Mode<CryptoPP::RC6>::Encryption)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::OFB_Mode<CryptoPP::RC6>::Encryption)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::CBC_CTS_Mode<CryptoPP::RC6>::Encryption)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::CTR_Mode<CryptoPP::RC6>::Encryption)->Range(16, 1024);

BENCHMARK_TEMPLATE(BM_CRYPTO_NO_IV, CryptoPP::ECB_Mode<CryptoPP::DES>::Encryption,8)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::CBC_Mode<CryptoPP::DES>::Encryption,8)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::CFB_Mode<CryptoPP::DES>::Encryption,8)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::OFB_Mode<CryptoPP::DES>::Encryption,8)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::CBC_CTS_Mode<CryptoPP::DES>::Encryption,8)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::CTR_Mode<CryptoPP::DES>::Encryption,8)->Range(16, 1024);

BENCHMARK_TEMPLATE(BM_CRYPTO_NO_IV, CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::OFB_Mode<CryptoPP::AES>::Encryption)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::CBC_CTS_Mode<CryptoPP::AES>::Encryption)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption)->Range(16, 1024);

BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::ChaCha::Encryption)->Range(16, 1024);
BENCHMARK_TEMPLATE(BM_CRYPTO, CryptoPP::Salsa20::Encryption)->Range(16, 1024);

int main(int argc, char **argv)
{
    srand(time(nullptr));
    std::iota(std::begin(str_key),std::end(str_key),rand());
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}