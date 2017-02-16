#include<stdio.h>

int main(int argc, char* argv[]){
    char ciphertext = argv[1][0];
    char plaintext = ciphertext + 7;
    for (int i = 0; i < 10; i++){
      plaintext = plaintext + 2;
    }
    printf("%c\n", plaintext);
}
