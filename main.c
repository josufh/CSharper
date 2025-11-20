#include "csharpier.h"

int main(void) {
    String string = Strings.New("こんにちは");

    Bytes bytes = Encoding.GetBytes(string);

    String base64 = Convert.ToBase64(bytes);

    Console.WriteLine("%s", string);
    Console.WriteLine("%s", base64);

    return 0;
}