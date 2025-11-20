#include "csharper.h"

int main(void) {
    String string = Strings.New("こんにちは");

    Bytes bytes = Encoding.GetBytes(string);

    String base64 = Convert.ToBase64(bytes);

    Console.WriteLine("%s: %zu", string, string.Length);
    Console.WriteLine("%s", base64);

    Console.LineBreak();

    Bytes back = Convert.FromBase64(base64);
    Console.WriteLine("Decoded back: %s", back);
    Console.WriteLine("Length: %zu", back.Length);

    return 0;
}