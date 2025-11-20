#include "csharper.h"

int main(void) {
    String string = Strings.New("Hello world!");

    Console.WriteLine("String Value : %s", string);
    Console.WriteLine("String Length: %zu", string.Length);

    return 0;
}