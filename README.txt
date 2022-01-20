# C++ Benchmark

basic c++ benchmarks  which may help in making the right decision.

any complement is welcome.

# Some Results  

May differ from different compilers and optimization options and CPU.

| Category   | Cases                                   |                             Link                             | extra                             |
| :--------- | --------------------------------------- | :----------------------------------------------------------: | --------------------------------- |
| Astar      | Astar Path Find                         |    https://quick-bench.com/q/E1yfZb9S3wOBIeQCQFefdkTTa1I     | may failed when no path to target |
| Arithmetic | divide uint32 uint64 float and double   |    https://quick-bench.com/q/9PmcSCQu3O8Q--K7a-frTUTHy1o     |                                   |
| Arithmetic | multiply uint32 uint64 float and double |    https://quick-bench.com/q/CvbF8q4VTjmvgtSRMw2vXn_OzFo     |                                   |
| Containers | list insert                             |               https://godbolt.org/z/o7odbr34s                | quick bench do not support boost  |
| Containers | list remove                             |               https://godbolt.org/z/x9nhWeaq1                |                                   |
| Containers | list range                              |               https://godbolt.org/z/TocGzosj1                |                                   |
| Containers | map decay                               |    https://quick-bench.com/q/0wiiO6s1aAlUrFl4r9nro_jNnA0     |                                   |
| Containers | ordered map find                        |               https://godbolt.org/z/Gc4T8MTxP                | quick bench code length limit     |
| Containers | ordered map range                       |               https://godbolt.org/z/45T6x5Eo8                | quick bench code length limit     |
| Containers | unordered map reserve insert            |    https://quick-bench.com/q/5lXbGFD_YsOCOhcIRiN2mU-_BnY     |                                   |
| Containers | unordered map find integer              |               https://godbolt.org/z/5cq45731s                | quick bench code length limit     |
| Containers | unordered map find string               | https://godbolt.org/z/4xdeqfx3M<br /> https://godbolt.org/z/o6eY4eW5K | Ce time limit                     |
| Containers | unordered map range integer             |               https://godbolt.org/z/qbcKazhex                | Ce time limit                     |
| Containers | unordered map range string              | https://godbolt.org/z/qbcKazhex<br />https://godbolt.org/z/q4TY1G86r | Ce time limit                     |
| Containers | ordered set find                        |               https://godbolt.org/z/WYYznWGff                |                                   |
| Containers | ordered set range                       |               https://godbolt.org/z/bq78o5GTK                |                                   |
| Containers | unordered set reserve insert            |    https://quick-bench.com/q/NIedSNHc-B5hHpBJNpDc38Ixxoc     |                                   |
| Containers | unordered set find integer              |               https://godbolt.org/z/xbz18Wx9b                |                                   |
| Containers | unordered set find string               | https://godbolt.org/z/1sfaP8z45<br />https://godbolt.org/z/7c4KvEj18 | Ce time limit                     |
| Containers | unordered set range int                 |               https://godbolt.org/z/rhrK1aeK4                |                                   |
| Containers | unordered set range string              | https://godbolt.org/z/qch8nPhrs<br />https://godbolt.org/z/j95hfheo9 | Ce time limit                     |
| Containers | vector remove                           |    https://quick-bench.com/q/uXLuQoi7w7Vhyc0LTR6DSRh7Q9s     |                                   |
| Containers | vector reserve                          |    https://quick-bench.com/q/7M73A9azMmow_hnxUZoaYQJi8cI     |                                   |
| Containers | vector vs inline vector                 | https://godbolt.org/z/Wzn497z7q<br />https://godbolt.org/z/seEeY49nT | Ce time limit                     |
| CPU        | cache line                              |    https://quick-bench.com/q/lLBAcIL6w_KOJL89xy7eaxFeo_I     |                                   |
| CPU        | false share                             |    https://quick-bench.com/q/ViRanL6G0REXG1yDD4WMOunR1aQ     |                                   |
| CPU        | predict                                 |    https://quick-bench.com/q/1GHqNmWEF72XGzmT4u7BEJk_R3Q     |                                   |
| CPU        | row vs col                              |    https://quick-bench.com/q/ozANPViyS8FBhat90CJ3IjxMqO0     |                                   |
| Handler    | array vs function                       |    https://quick-bench.com/q/kEmsJb5ysMAh6YujQCjfDSf0KB0     |                                   |
| Handler    | map unordered_map  array                |    https://quick-bench.com/q/NSoyzIDxsHIzYjCNDcMKwOjxRSI     |                                   |
| Spinlock   | vary spinlock compare                   |               https://godbolt.org/z/G6TcxTT86                | Ce limit error                    |



# License

>  Copyright [dvison] [dietoad@gmail.com]
>
>    Licensed under the Apache License, Version 2.0 (the "License");
>    you may not use this file except in compliance with the License.
>    You may obtain a copy of the License at
>
>    http://www.apache.org/licenses/LICENSE-2.0
>
>    Unless required by applicable law or agreed to in writing, software
>    distributed under the License is distributed on an "AS IS" BASIS,
>    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
>    See the License for the specific language governing permissions and
>    limitations under the License.