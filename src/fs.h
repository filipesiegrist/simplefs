#ifndef FS_H
#define FS_H

/*	
 * Emulador de Disco
 * 
 * Nos fornecemos a voc^e um emulador de disco no qual armazenar seu sistema de arquivo. Este \disco" na
 * verdade armazenado como um grande arquivo no sistema de arquivo, assim voc^e pode salvar dados em uma
 * imagem de disco e ent~ao recupera-los depois. Alem disto, nos vamos fornecer a voc^e alguns exemplos de imagens
 * de disco com os quais voc^e pode experimentar para testar seu sistema de arquivo.
 * Tal como um disco real, o emulador apenas permite operac~oes em blocos inteiros de disco de 4KB
 * (DISK BLOCK SIZE). Voc^e n~ao pode ler ou escrever nenhuma unidade menor do que isto. O principal desafio
 * de construir um sistema de arquivo e converter as operac~oes requisitadas pelo usuario em quantidades arbitrarias
 * de dados para operac~oes em tamanhos fixos de blocos.
 *
 */


/*
 * Varre um sistema de arquivo montado e reporta como os inodos e os blocos est~ao organizados. Se
 * voc^e conseguir escrever esta func~ao, voc^e ja ganhou metade da batalha! Assim que voc^e conseguir varrer
 * e reportar as estruturas do sistema de arquivos, o resto e facil. Sua saida do fs debug deve ser similar ao
 * seguinte:
 * 
 */
void fs_debug();

/*
 * Cria um novo sistema de arquivos no disco, destruindo qualquer dado que estiver presente. Reserva
 * dez por cento dos blocos para inodos, libera a tabela de inodos, e escreve o superbloco. Retorna um para
 * sucesso, zero caso contrario. Note que formatar o sistema de arquivo n~ao faz com que ele seja montado.
 * Tambem, uma tentativa de formatar um disco que ja foi montado n~ao deve fazer nada e retornar falha.
 * 
 */
int  fs_format();

/*
 * Examina o disco para um sistema de arquivo. Se um esta presente, l^e o superbloco, constroi um
 * bitmap de blocos livres, e prepara o sistema de arquivo para uso. Retorna um em caso de sucesso, zero
 * caso contrario. Note que uma montagem bem-sucedida e um pre-requisito para as outras chamadas.
 * 
 */
int  fs_mount();

/*
 * Cria um novo inodo de comprimento zero. Em caso de sucesso, retorna o inumero (positivo). Em
 * caso de falha, retorna zero. (Note que isto implica que zero n~ao pode ser um inumero valido.)
 * 
 */
int  fs_create();

/*
 * Deleta o inodo indicado pelo inumero. Libera todo o dado e blocos indiretos atribuidos a este
 * inodo e os retorna ao mapa de blocos livres. Em caso de sucesso, retorna um. Em caso de falha, returna
 * 0.
 * 
 */
int  fs_delete( int inumber );

/*
 * Retorna o tamanho logico do inodo especificado, em bytes. Note que zero e um tamanho logico
 * valido para um inodo! Em caso de falha, retorna -1.
 * 
 */
int  fs_getsize( int inumber );

/*
 * L^e dado de um inodo valido. Copia \length" bytes do inodo para dentro do ponteiro \data",
 * comecando em \oset" no inodo. Retorna o numero total de bytes lidos. O Numero de bytes efetivamente
 * lidos pode ser menos que o numero de bytes requisitados, caso o fim do inodo seja alcancado. Se o inumero
 * dado for invalido, ou algum outro erro for encontrado, retorna 0.
 * 
 */
int  fs_read( int inumber, char *data, int length, int offset );

/*
 * Escreve dado para um inodo valido. Copia \length" bytes do ponteiro \data" para o inodo
 * comecando em \offset" bytes. Aloca quaisquer blocos diretos e indiretos no processo. Retorna o numero
 * de bytes efetivamente escritos. O numero de bytes efetivamente escritos pode ser menor que o numero de
 * bytes requisitados, caso o disco se torne cheio. Se o inumero dado for invalido, ou qualquer outro erro for
 * encontrado, retorna 0.
 * 
 */
int  fs_write( int inumber, const char *data, int length, int offset );





#endif
