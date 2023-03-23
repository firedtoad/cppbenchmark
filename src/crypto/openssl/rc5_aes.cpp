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
#include <numeric>
#include <openssl/aes.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/rc5.h>
#include <openssl/rc4.h>
#include <vector>

static unsigned char str_key[16];

static void BM_OPEN_RC4(benchmark::State &state)
{
    RC4_KEY key{};
    RC4_set_key(&key, 16, str_key);
    for (auto _ : state)
    {
        uint8_t out[16];
        RC4(&key,16,out,out);
        benchmark::DoNotOptimize(out);
    }
}

BENCHMARK(BM_OPEN_RC4);

static void BM_RC4_EVP(benchmark::State &state)
{
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    uint8_t iv[16];
    EVP_EncryptInit_ex(&ctx, EVP_rc4_40(), NULL,str_key, iv);
    uint8_t in[16];
    for (auto _ : state)
    {
        uint8_t out[16];
        int l;
        EVP_EncryptUpdate(&ctx, out, &l, in, 16);
        benchmark::DoNotOptimize(out);
    }
}
BENCHMARK(BM_RC4_EVP);

static void BM_OPEN_RC5(benchmark::State &state)
{
    RC5_32_KEY key{};
    RC5_32_set_key(&key, 16, str_key, 12);
    for (auto _ : state)
    {
        RC5_32_INT d[2];
        RC5_32_encrypt(d, &key);
        benchmark::DoNotOptimize(d);
    }
}

BENCHMARK(BM_OPEN_RC5);

static void BM_RC5_EVP(benchmark::State &state)
{
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    uint8_t iv[16];
    EVP_EncryptInit_ex(&ctx, EVP_rc5_32_12_16_cbc(), NULL,str_key, iv);
    uint8_t in[16];
    for (auto _ : state)
    {
        uint8_t out[16];
        int l;
        EVP_EncryptUpdate(&ctx, out, &l, in, 16);
        benchmark::DoNotOptimize(out);
    }
}
BENCHMARK(BM_RC5_EVP);


static void BM_AES(benchmark::State &state)
{
    AES_KEY key{};
    uint8_t iv[16];
    AES_set_encrypt_key(str_key, 128, &key);
    uint8_t in[16];
    for (auto _ : state)
    {
        uint8_t out[16];
        AES_cbc_encrypt(in, out, AES_BLOCK_SIZE, &key, iv, AES_ENCRYPT);
        benchmark::DoNotOptimize(out);
    }
}
BENCHMARK(BM_AES);

static void BM_AES_EVP(benchmark::State &state)
{
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    uint8_t iv[16];
    EVP_EncryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL,str_key, iv);

    uint8_t in[16];
    for (auto _ : state)
    {
        uint8_t out[16];
        int l;
        EVP_EncryptUpdate(&ctx, out, &l, in, 16);
        benchmark::DoNotOptimize(out);
    }
}
BENCHMARK(BM_AES_EVP);

int main(int argc, char **argv)
{
    srand(time(nullptr));
    std::iota(std::begin(str_key),std::end(str_key),rand());
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}