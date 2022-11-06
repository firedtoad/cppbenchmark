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
        std::cout << "need " << need << " real " << sz << " over head " << sz - need << '\n';
        p    = (char *)malloc(sz);
        need = sz;
        sz   = malloc_usable_size(p);
        std::cout << "need " << need << " real " << sz << " over head " << sz - need << '\n';
    } while (sz <= 1024);

    return 0;
}