#include <global.h>

void Java_ora_ch2_KVMNative_printMessage()
{
    STRING_INSTANCE stringInstance = popStackAsType(STRING_INSTANCE);
    INSTANCE thisPtr = popStackAsType(INSTANCE);
    char *string = getStringContents(stringInstance);
    if (string != (char *)0 && *string != (char)0) {
        printf("Message is %s\n", string);
    } else {
        printf("No message\n");
    }
}