#include <iostream>
#include <malloc.h>
int main(int argc, char **argv)
{
    auto sz = 0;
    do
    {
        auto p    = (char *)malloc(sz + 1);
        auto need = sz + 1;
        sz        = malloc_usable_size(p);
        auto real = *(size_t *)(p - 8);
        std::cout << "need " << need << " real " << real - 1 << " over head " << real - need - 1 << " usable " << sz << '\n';

    } while (sz <= 1024);

    return 0;
}