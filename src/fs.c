
#include "fs.h"
#include "disk.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>

/* editado por mim */
#include <iostream>
using namespace std;

#define FS_MAGIC           0xf0f03410
#define INODES_PER_BLOCK   128
#define POINTERS_PER_INODE 5
#define POINTERS_PER_BLOCK 1024

struct fs_superblock {
	int magic;
	int nblocks;
	int ninodeblocks;
	int ninodes;
};

struct fs_inode {
	int isvalid;
	int size;
	int direct[POINTERS_PER_INODE];
	int indirect;
};

union fs_block {
	struct fs_superblock super;
	struct fs_inode inode[INODES_PER_BLOCK];
	int pointers[POINTERS_PER_BLOCK];
	char data[DISK_BLOCK_SIZE];
};

/* carrega o seguinte inode da memoria */
void inode_load( int inumber, struct fs_inode *inode_ler ) {
	/* inode alocado */
	inode_ler = new struct fs_inode;
	/* bloco de leitura */
	union fs_block leitura;
	
	/* A FAZER ABAIXO: Verificar se inode varia de 0-127 ou de 1-128. Se for a primeira tem que mudar o numero de bloco e posicao */
	
	/* \/\/ */
	
	/* Acha o bloco em que o inode se encontra. Soma 1 pra tirar o superbloco */
	int numbloco = inumber/INODES_PER_BLOCK + 1;
	/* Acha a posicao no bloco */
	int posicao = inumber%INODES_PER_BLOCK;
	
	/* /\/\ */
	
	/* efetua a leitura do bloco */
	disk_read(numbloco,leitura.data);
	/* aponta inode_ler para a posicao no bloco  */
	inode_ler = leitura.inode+posicao;
	
}
void inode_save( int inumber, struct fs_inode &inode_esc ) {
	/* bloco de escrita */
	union fs_block escrita;
	
	/* A FAZER ABAIXO: Verificar se inode varia de 0-127 ou de 1-128. Se for a primeira tem que mudar o numero de bloco e posicao */
	
	/* \/\/ */
	
	/* Acha o bloco em que o inode se encontra. Soma 1 pra tirar o superbloco */
	int numbloco = inumber/INODES_PER_BLOCK + 1;
	/* Acha a posicao no bloco */
	int posicao = inumber%INODES_PER_BLOCK;
	
	/* /\/\ */
	
	/* efetua a leitura do bloco */
	disk_read(numbloco,escrita.data);
	
	/* atribui o valor desse inode no bloco */
	escrita.inode[posicao] = inode_esc;
	
	/* escreve o dado na memoria */
	disk_write(numbloco,escrita.data);
	
	/* libera o inode */
	delete &inode_esc;
}

/*
 * Cria um novo sistema de arquivos no disco, destruindo qualquer dado que estiver presente. Reserva
 * dez por cento dos blocos para inodos, libera a tabela de inodos, e escreve o superbloco. Retorna um para
 * sucesso, zero caso contrario. Note que formatar o sistema de arquivo n~ao faz com que ele seja montado.
 * Tambem, uma tentativa de formatar um disco que ja foi montado n~ao deve fazer nada e retornar falha.
 * 
 */
int fs_format()
{
	int tamanho = disk_size();
	/* tamanho para inodos */
	int tamanho_inodos = tamanho/10;
	union fs_block bloco0;
	disk_read(0,bloco0.data);
	int magico = bloco0.super.magic;
	/* Fazer todos os i-nodes serem invalidos */
	
	return 0;
}

/*
 * Varre um sistema de arquivo montado e reporta como os inodos e os blocos est~ao organizados. Se
 * voc^e conseguir escrever esta func~ao, voc^e ja ganhou metade da batalha! Assim que voc^e conseguir varrer
 * e reportar as estruturas do sistema de arquivos, o resto e facil. Sua saida do fs debug deve ser similar ao
 * seguinte:
 * 
 */
void fs_debug()
{
	/* Bloco inicial */
	union fs_block block;
	/* vetor com os outros blocos na memoria */
	union fs_block *block_vec;
	/* le a parte zero que e o superbloco */
	disk_read(0,block.data);

	printf("superblock:\n");
	printf("    %d blocks\n",block.super.nblocks);
	printf("    %d inode blocks\n",block.super.ninodeblocks);
	printf("    %d inodes\n",block.super.ninodes);
	
	
	/* Aloca os espacos dos blocos para o vetor de blocos de inodes */
	block_vec = new union fs_block[block.super.ninodeblocks];
	int i,j,k;
	int num_inodos_percorridos = 0;
	/* percorre todos os blocos que contem inodes. Comeca do 1 pq o 0 e o superbloco */
	for (i = 0; i < block.super.ninodeblocks; i++) {
		/* Coloca o valor do disco no vetor de blocos */
		/* le a porcao i+1 pq ja foi lida a porcao 0 */
		disk_read(i+1,block_vec[i].data);
		
		/* percorre todos os inodos do bloco */
		for (j = 0; j < INODES_PER_BLOCK; j++) {
			/* Verifica se o inodo e valido */
			if (block_vec[i].inode[j].isvalid) {
				cout << "inode " << num_inodos_percorridos << ": " << endl;
				/* Escreve o tamanho */
				cout << "\tsize: " << block_vec[i].inode[j].size << " bytes" << endl;
				
				/* Escreve quais sao os blocos diretos */
				cout << "\tdirect blocks:";
				for (k = 0; k < POINTERS_PER_INODE; k++) {
					/* Se o numero for zero não exibe. */
					if (block_vec[i].inode[j].direct[k] != 0) cout << " " << block_vec[i].inode[j].direct[k];
				}
				cout << endl;
				
				/* Aqui comeca a parte que pega um pouco. Tenho que achar os blocos indiretos. */
				/* Se indirect for 0 entao nao tem. Mas se nao for isso sera o numero do bloco */
				/* com ponteiros para os outros blocos */
				
				/* Verifica se existem blocos indiretos */
				if (block_vec[i].inode[j].indirect != 0) {
					/* bloco de indirecao */
					union fs_block bloco_indirecao;
					/* Efetua a leitura */
					disk_read(block_vec[i].inode[j].indirect,bloco_indirecao.data);
					
					/* Numero de ponteiros validos */
					int num_ponteiros = 0;
					/* Agora trata bloco_indirecao como campo de ponteiros e conta os ponteiros diferentes de zero */
					for (k = 0; k < POINTERS_PER_BLOCK; k++) {
						/* Sera ponteiro valido se for diferente de zero */
						if (bloco_indirecao.pointers[k] > 0) num_ponteiros++;
					}
					
					/* Mostra qual e o bloco de indirecao */
					cout << "\tindirect block: " << block_vec[i].inode[j].indirect << endl;
					
					/* So exibe o proximo se bloco de indirecao tiver um ponteiro valido */
					if (num_ponteiros > 0) {
						cout << "\tindirect data blocks:";
						/* Exibe agora os numeros diferentes de zero */
						for (k = 0; k < POINTERS_PER_BLOCK; k++) {
							/* Sera ponteiro valido se for diferente de zero */
							if (bloco_indirecao.pointers[k] != 0) cout << " " << bloco_indirecao.pointers[k];
						}
						cout << endl;
					}
				}  
				
			}
			num_inodos_percorridos++;
			if (num_inodos_percorridos == block.super.ninodes) break;
		}
		if (num_inodos_percorridos == block.super.ninodes) break;
	}
	
	delete block_vec;
	
}

/*
 * Examina o disco para um sistema de arquivo. Se um esta presente, l^e o superbloco, constroi um
 * bitmap de blocos livres, e prepara o sistema de arquivo para uso. Retorna um em caso de sucesso, zero
 * caso contrario. Note que uma montagem bem-sucedida e um pre-requisito para as outras chamadas.
 * 
 */
int fs_mount()
{
	return 0;
}

int fs_create()
{
	return 0;
}

int fs_delete( int inumber )
{
	return 0;
}

int fs_getsize( int inumber )
{
	return -1;
}

int fs_read( int inumber, char *data, int length, int offset )
{
	return 0;
}

int fs_write( int inumber, const char *data, int length, int offset )
{
	return 0;
}
