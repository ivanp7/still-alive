#include "io.h"
#include "still-alive.h"

int main()
{
    InitializeScreen();

    int result = Song();

    ReleaseScreen();

    return result;
}

