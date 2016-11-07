#include "SamProcess.h"
#include "SamParser.h"
int main(int argc, char const *argv[])
{
    /* code */
    SamParser a=SamParser();
    a.parser("data/MH0010.1.sam","data/MH0010.2.sam");
    return 0;
}