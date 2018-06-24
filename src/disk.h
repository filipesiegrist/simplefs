#ifndef DISK_H
#define DISK_H

#define DISK_BLOCK_SIZE 4096

/*
 * Antes de realizar qualquer operac~ao no disco, voc^e deve chamar disk init e especificar uma imagem de
 * disco (real) para armazenar os dados de disco, e o numero de blocos em um disco simulado. Se esta func~ao e
 * chamada em uma imagem de disco que ja existe, o dado contido n~ao sera alterado.
 */
int  disk_init( const char *filename, int nblocks );

/*
 * Assim que o disco for inicializado, voc^e pode chamar disk size() para descobrir o numero de blocos no
 * disco.
 */
int  disk_size();

/*
 * Como o nome sugere, disk read() e disk write() l^e e escreve um bloco de dada no disco. Note que
 * o primeiro argumento e um numero de bloco, para que a chamada para disk read(0,data) leia os primeiros
 * 4KB de dado no disco, e disk read(1,data) leia o proximo bloco de dado de 4KB no disco. Toda vez que voc^e
 * invocar um read ou write, voc^e deve se assegurar de que data aponte para 4KB completos de memoria.
 * 
 */
void disk_read( int blocknum, char *data );
void disk_write( int blocknum, const char *data );

/*
 * Quando voc^e tiver terminado
 * de usar o disco, chame disk close para liberar o arquivo.
 * 
 */
void disk_close();


#endif
