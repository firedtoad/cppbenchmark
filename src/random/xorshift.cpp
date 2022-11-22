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
// This file is from  https://gist.github.com/t-mat/8b2c183ae50480c7998f4d9ab2271b1d
// Thanks to  Takayuki Matsuoka

// Find all 8-bit and 16-bit xorshift triples.
//
//  // 8-bit Xorshift
//  #include <stdint.h>
//  #include <stdio.h>
//  const int a = 1, b = 1, c = 2;      // triple: { 1, 1, 2}
//  uint8_t f(uint8_t y) { y ^= y << a; y ^= y >> b; y ^= y << c; return y; }
//  int main() { uint8_t y = 1; for(int i = 0; i < 256; ++i) { printf("%d, ", y); y = f(y); } }
//
//  // 16-bit Xorshift
//  #include <stdint.h>
//  #include <stdio.h>
//  const int a = 1, b = 1, c = 15;     // triple: { 1, 1,15}
//  uint16_t f(uint16_t y) { y ^= y << a; y ^= y >> b; y ^= y << c; return y; }
//  int main() { uint16_t y = 1; for(int i = 0; i < 65536; ++i) { printf("%d, ", y); y = f(y); } }
//
// Result:
//  static const int xorshift8_triples[12][3] = {
//   { 1, 1, 2}, { 1, 1, 3}, { 1, 7, 3}, { 1, 7, 6}, { 1, 7, 7},
//   { 2, 5, 5}, { 3, 1, 5}, { 3, 5, 4}, { 3, 5, 5}, { 3, 5, 7},
//   { 5, 3, 6}, { 5, 3, 7},
//  };
//  static const int xorshift16_triples[30][3] = {
//   { 1, 1,14}, { 1, 1,15}, { 1, 5, 2}, { 1, 7, 4}, { 1, 7,11},
//   { 1,11, 3}, { 1,15, 6}, { 1,15, 7}, { 2, 5,13}, { 2, 5,15},
//   { 2, 7,13}, { 2, 7,15}, { 3, 1,12}, { 3, 1,15}, { 3, 5,11},
//   { 3,11,11}, { 3,13, 9}, { 4, 3, 7}, { 4,11,11}, { 5, 7,14},
//   { 5, 9, 8}, { 5,11, 6}, { 5,11,11}, { 6, 7,13}, { 7, 1,11},
//   { 7, 9, 8}, { 7, 9,13}, { 9, 7,13}, {11, 3,13}, {12, 3,13},
//  };
#include <stdio.h>
#include <inttypes.h>
#include <vector>
#include <bitset>
#include <algorithm>

struct Xorshift8 {
    int a, b, c;
    using State = uint8_t;

    State step(State y) const {
        y ^= (y << a);
        y ^= (y >> b);
        y ^= (y << c);
        return y;
    }

    // With given triples, if RNG state can forward (2^N)-1 steps without
    // collision, we can say it's not bad N-bit Xorshift triple, basically.
    bool good() const {
        std::bitset<256> q;
        State y = 1;
        for(size_t i = 1; i < q.size(); ++i) {
            if(q[y]) {
                return false;   // collision detected
            }
            q[y] = true;
            y = step(y);
        }
        return true;
    }

    static auto findAllGoodTriples() {
        std::vector<Xorshift8> xs;
        for(int i = 0; i < 8*8*8; ++i) {
            const int a = (i >> (3*0)) % 8;
            const int b = (i >> (3*1)) % 8;
            const int c = (i >> (3*2)) % 8;
            if(a >= c) { continue; }
            Xorshift8 x { .a = a, .b = b, .c = c };
            if(x.good()) {
                xs.push_back(x);
            }
        }
        return xs;
    }
};


struct Xorshift16 {
    int a, b, c;
    using State = uint16_t;

    State step(State y) const {
        y ^= (y << a);
        y ^= (y >> b);
        y ^= (y << c);
        return y;
    }

    bool good() const {
        std::bitset<65536> q;
        State y = 1;
        for(size_t i = 1; i < q.size(); ++i) {
            if(q[y]) {
                return false;   // collision detected
            }
            q[y] = true;
            y = step(y);
        }
        return true;
    }

    static auto findAllGoodTriples() {
        std::vector<Xorshift16> xs;
        for(int i = 0; i < 16*16*16; ++i) {
            const int a = (i >> (4*0)) % 16;
            const int b = (i >> (4*1)) % 16;
            const int c = (i >> (4*2)) % 16;
            if(a >= c) { continue; }
            Xorshift16 x { .a = a, .b = b, .c = c };
            if(x.good()) {
                xs.push_back(x);
            }
        }
        return xs;
    }
};


int main(int argc,char** argv) {
    const auto print = [](const auto& xs, const char* arrayName) {
        size_t i = 0;
        printf("static const int %s[%d][3] = {\n", arrayName, static_cast<int>(xs.size()));
        for(const auto& x : xs) {
            printf(" {%2d,%2d,%2d},", x.a, x.b, x.c);
            ++i;
            if(i % 5 == 0 || i == xs.size()) {
                printf("\n");
            }
        }
        printf("};\n");
    };

    const auto sortXs = [](const auto& xs) {
        auto vs = xs;
        std::sort(vs.begin(), vs.end(), [](const auto& l, const auto& r) {
                      if(l.a != r.a) { return l.a < r.a; }
                      if(l.b != r.b) { return l.b < r.b; }
                      return l.c < r.c;
                  });
        return vs;
    };

    const auto x8s = sortXs(Xorshift8::findAllGoodTriples());
    print(x8s, "xorshift8_triples");

    const auto x16s = sortXs(Xorshift16::findAllGoodTriples());
    print(x16s, "xorshift16_triples");
    return 0;
}