#include "uncompress.h"
#include <bzlib.h>
#include <fstream>
#include "assert.h"

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    bz_stream s;
    BZ2_bzDecompressInit(&s, 0, 0);

    std::ifstream wikipedia;
    wikipedia.open("enwiki-latest-pages-articles.xml.bz2", std::ios::in);
    assert(wikipedia.is_open() && "wiki file did not open");
    while(!wikipedia.eof())
    {

    }


    BZ2_bzDecompressEnd(&s);


    return 0;
}

