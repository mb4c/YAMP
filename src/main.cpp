#include "YAMP.h"

int main(int argc, char** argv)
{
    YAMP yamp("YAMP", 1280, 720, true, argc, argv);
    yamp.Run();
}