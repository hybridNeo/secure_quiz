// address: 0x804846b
int main(int argc, char **argv, char **envp) {
    int local0; 		// m[r28 - 26]
    int local10; 		// m[r28 - 36]
    int local13; 		// r24
    int local16; 		// r28

    printf("Before Encryption %s \n", &local0);
    local10 = 0;
    while (local10 <= 5) {
        local13 = *(unsigned char*)(local16 - 26 + local10);
        *(char*)(local16 + local10 - 26) = (unsigned char) local13 + 7;
        local10++;
    }
    printf("Encrypted string %s \n", &local0);
    local13 = 0;
    return local13;
}

