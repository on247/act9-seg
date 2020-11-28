#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sodium.h>

// 1024 bits.
#define KEY_BYTES 256

off_t fsize(char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1; 
}

void print_hex(char * str){
    while (*str != 0) {
        printf("%02x ", (unsigned char) *str);
        str++;
    }
    printf("\n");
}



  
int main(int argc,char **argv)
{
   char *filename = argv[1];
   if(argc<2){
       printf("./act9 archivo");
       return 1;
   }
   FILE *file;

   

   file = fopen(filename,"rb");

   if(file){
       off_t size = fsize(filename);
       char *message = (char *) malloc(sizeof(char) * size);
       size_t bytesread = fread(message,sizeof(char),size,file);
       unsigned long long messageSize = (unsigned long long) bytesread;
       printf("%lu bytes leidos\n",bytesread);
       printf("Generando clave privada / publica: \n");
       char privkey[KEY_BYTES];
       char pubkey[KEY_BYTES];
       if (sodium_init() < 0) return 1;
       crypto_sign_keypair(pubkey, privkey);
       printf("PRIV:\n");
       print_hex(privkey);
       printf("PUB:\n");
       print_hex(pubkey);
       printf("Firmando mensaje\n");
        unsigned long long signedMessageSize = (unsigned long long)messageSize + KEY_BYTES;
        char *signedMessage= (char *) malloc(sizeof(char) * signedMessageSize  );
        crypto_sign(signedMessage, &signedMessageSize,  message , messageSize , privkey);


        char *verifiedMessage= (char *) malloc(sizeof(char) * size);

        if(crypto_sign_open(verifiedMessage,&messageSize,signedMessage,signedMessageSize,pubkey)>-1){
            printf("Firma verificada\n");
        }
        else{
            printf("Firma no valida\n");
        }
   }

   else{
       printf("Error al abrir archivo");
   }
  
   return 0;
}