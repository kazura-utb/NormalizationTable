/***************************************************************************
* Name  : fio.cpp
* Brief : ファイルIO関連の処理を行う
* Date  : 2016/02/03
****************************************************************************/

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "zlib.h"

//#include "cpu.h"
#include "fio.h"

#define BOOK_DECODE_DATA_SIZE 18160640
#define BOOK_DATA_SIZE 10485760
#define EVAL_DECODE_DATA_SIZE 1572864
#define EVAL_DATA_SIZE 2097152
#define TREE_SIZE_ADDR 8
#define ENCODE_SIZE_ADDR 4
#define DECODE_SIZE_ADDR 0

#define NODE_SIZE 256

#define EVAL_N             226234
#define EVAL_FEATURE_SIZE (EVAL_N * sizeof(INT16))

typedef struct
{
	UINT32 size;
	UINT32 decompSize;
	UINT8  compData[512 * 1024];
} st_eval_data;

st_eval_data g_evalData;

UINT8 g_outBuf[1024 * 512];

char charSet[] = "0123456789abcdefgh\n.-;";

int convertChar2Index(char chr)
{
	int i;
	for (i = 0; i < 22; i++)
	{
		if (charSet[i] == chr)
		{
			break;
		}
	}

	return i;

}

int CreateHuffmanTree(TreeNode *nodes, int nodeNum)
{
	int freeNode;
	int min1, min2;

	nodes[NODE_SIZE - 1].occurrence = 100000000;          /* 番兵 */

	for (freeNode = nodeNum + 1;; freeNode++) {
		min1 = min2 = NODE_SIZE - 1;
		for (int i = NODE_SIZE - 2; i >= 0; i--){
			if (nodes[i].occurrence > 0) {
				if (nodes[i].occurrence < nodes[min1].occurrence) {
					min2 = min1;
					min1 = i;
				}
				else if (nodes[i].occurrence < nodes[min2].occurrence)
					min2 = i;
			}
		}
		if (min2 == NODE_SIZE - 1) {
			break;
		}
		nodes[freeNode].left = min1;
		nodes[freeNode].right = min2;
		nodes[freeNode].occurrence = nodes[min1].occurrence + nodes[min2].occurrence;
		nodes[min1].parent = freeNode;
		nodes[min2].parent = freeNode;
		nodes[min1].occurrence = 0;
		nodes[min2].occurrence = 0;
	}

	return min1;
}

/* C#版では使う事がない */
int writeEncodeData(UCHAR *encodeData, TreeNode *nodes, int root,
	CodeInfo *codeInfo, UCHAR *data, int dataLen){

	int index, div;
	int bitCounter = 0, encodeCounter = 0;
	char bit[16] = { 0 };

	// ハフマン木を保存
	int nodesLen = sizeof(TreeNode) * (root + 1);
	encodeData[encodeCounter++] = (nodesLen & 0xFF00) >> 8;
	encodeData[encodeCounter++] = nodesLen & 0xFF;

	memcpy_s(&encodeData[encodeCounter], dataLen, nodes, nodesLen);
	encodeCounter += nodesLen;

	for (int i = 0; i < dataLen; i++)
	{
		index = convertChar2Index(data[i]);

		div = codeInfo[index].code;
		for (int j = bitCounter + codeInfo[index].codeSize - 1; j >= bitCounter; j--){

			bit[j] = div % 2;
			div /= 2;
		}

		bitCounter += codeInfo[index].codeSize;

		if (bitCounter >= 8){
			encodeData[encodeCounter++] = (bit[0] << 7) + (bit[1] << 6) + (bit[2] << 5) +
				(bit[3] << 4) + (bit[4] << 3) + (bit[5] << 2) + (bit[6] << 1) + bit[7];
			bitCounter -= 8;
			for (int j = 0; j < bitCounter; j++){
				bit[j] = bit[8 + j];
			}
		}
	}

	// bit余り
	memset(&bit[bitCounter], 0, 8 - bitCounter);
	encodeData[encodeCounter++] = (bit[0] << 7) + (bit[1] << 6) + (bit[2] << 5) +
		(bit[3] << 4) + (bit[4] << 3) + (bit[5] << 2) + (bit[6] << 1) + bit[7];

	return encodeCounter;

}

int generateCode(CodeInfo *codeInfo, TreeNode *node, int nodeIndex, int code, int codeSize) {

	if (node[nodeIndex].left == 0 && node[nodeIndex].right == 0) {
		codeInfo[nodeIndex].code = code;
		codeInfo[nodeIndex].codeSize = codeSize;

		return 1;

	}
	else {
		int count;
		count = generateCode(codeInfo, node, node[nodeIndex].left, code << 1, codeSize + 1);
		count += generateCode(codeInfo, node, node[nodeIndex].right, (code << 1) + 1, codeSize + 1);

		return count;
	}
}

int encode(UCHAR *encodeData, CodeInfo *codeInfo, UCHAR *data, int dataLen) {

	// 各バイト値の発生回数を数える
	TreeNode nodes[NODE_SIZE];

	memset(nodes, 0, sizeof(nodes));

	for (int i = 0; i < 22; i++)
	{
		nodes[i].chr = charSet[i];
	}

	for (int i = 0; i < dataLen; i++) {

		nodes[convertChar2Index(data[i])].occurrence++;
	}

	// ハフマン木を作成
	int root = CreateHuffmanTree(nodes, 22 - 1);
	// 深さ優先探索でバイト値→符号情報を作成
	int codeInfoNum = generateCode(codeInfo, nodes, root, 0, 0);

	if (codeInfoNum > 22)
	{
		// charSetで指定されている文字以外が含まれたデータを読み込もうとしている
		return -1;
	}

	// 圧縮データをメモリに書き込み
	int encodeDataLen = writeEncodeData(encodeData, nodes, root, codeInfo, data, dataLen);

	return encodeDataLen;
}


BOOL do_compress(FILE *fp, UINT8 *compData, UINT32 *compDataSize, UINT32 *fileSize)
{
	INT32     count, flush, status;
	UINT8    *inBuf;
	UINT32    inSize;
	z_stream  z;

	/* すべてのメモリ管理をライブラリに任せる */
	z.zalloc = Z_NULL;
	z.zfree = Z_NULL;
	z.opaque = Z_NULL;

	/* 初期化 */
	/* 第2引数は圧縮の度合。0～9 の範囲の整数で，0 は無圧縮 */
	/* Z_DEFAULT_COMPRESSION (= 6) が標準 */
	if (deflateInit(&z, Z_DEFAULT_COMPRESSION) != Z_OK) {
		printf("deflateInit: %s\n", (z.msg) ? z.msg : "???");
		return FALSE;
	}

	/* 通常は deflate() の第2引数は Z_NO_FLUSH にして呼び出す */
	z.avail_in = 0;
	z.next_out = g_outBuf; /* 出力バッファ残量を元に戻す */
	z.avail_out = sizeof(g_outBuf); /* 出力ポインタを元に戻す */
	flush = Z_NO_FLUSH;

	inSize = 1024 * 512;
	inBuf = (UCHAR *)malloc(inSize);

	status = Z_OK;
	while (status == Z_OK)
	{
		if (z.avail_in == 0)
		{  /* 入力が尽きれば */
			z.next_in = inBuf;  /* 入力ポインタを入力バッファの先頭に */
			z.avail_in = fread(&inBuf[4], 1, inSize, fp); /* データを読み込む */
			if(z.avail_in != 0)
			{
				inBuf[0] = (UCHAR)(z.avail_in >> 24);
				inBuf[1] = (UCHAR)(z.avail_in >> 16);
				inBuf[2] = (UCHAR)(z.avail_in >> 8);
				inBuf[3] = (UCHAR)z.avail_in;
				z.avail_in += 4;
				*fileSize = z.avail_in;
			}
			if (z.avail_in < inSize) flush = Z_FINISH;
		}
		do
		{
			status = deflate(&z, flush); /* 圧縮する */
			if (status == Z_STREAM_END) break; /* 完了 */
			if (status != Z_OK)
			{   /* エラー */
				printf("deflate: %s\n", (z.msg) ? z.msg : "???");
				break;
			}
		} while (status == Z_OK && z.avail_in > 0);
		
	}

	if ((count = sizeof(g_outBuf) - z.avail_out) != 0)
	{
		memcpy_s(compData, *compDataSize, g_outBuf, count);
		*compDataSize = count;
	}

	free(inBuf);

	if (deflateEnd(&z) != Z_OK) {
		printf("inflateEnd: %s\n", (z.msg) ? z.msg : "???");
		return FALSE;
	}

	return TRUE;
}


UINT32 marshalEvalData(st_eval_data *evalData, UINT8 *outBuf, UINT32 outBufSize)
{
	UINT32 cnt;

	cnt = 0;

	outBuf[cnt++] = (UINT8)(evalData->size >> 24);
	outBuf[cnt++] = (UINT8)(evalData->size >> 16);
	outBuf[cnt++] = (UINT8)(evalData->size >> 8);
	outBuf[cnt++] = (UINT8 )evalData->size;

	outBuf[cnt++] = (UINT8)(evalData->decompSize >> 24);
	outBuf[cnt++] = (UINT8)(evalData->decompSize >> 16);
	outBuf[cnt++] = (UINT8)(evalData->decompSize >> 8);
	outBuf[cnt++] = (UINT8)evalData->decompSize;

	memcpy_s(&outBuf[cnt], outBufSize - cnt, evalData->compData, evalData->size);
	cnt += evalData->size;

	return cnt;
}


// Asciiデータからテーブル作成を行う時のみ使用する
BOOL makeData()
{
	FILE     *fp, *wfp;
	char     filePath[64];
	BOOL     ret;
	UINT32   fileSize;

	if (fopen_s(&wfp, "table\\new\\eval.bin", "wb") != 0) {
		exit(1);
	}

#if 1

	for (int i = 0; i < 61; i++)
	{
		sprintf_s(filePath, sizeof(filePath), "table\\new\\%d.dat", i);
		if (fopen_s(&fp, filePath, "rb") != 0) {
			break;
		}

		g_evalData.size = sizeof(g_evalData.compData);
		ret = do_compress(fp, g_evalData.compData, &g_evalData.size, &g_evalData.decompSize);
		if (ret == FALSE) break;

		fileSize = marshalEvalData(&g_evalData, g_outBuf, sizeof(g_outBuf));
		fwrite(&fileSize, sizeof(UINT8), 4, wfp);
		fwrite(g_outBuf, sizeof(UINT8), fileSize, wfp);

		fclose(fp);
	}

	fclose(wfp);

	return TRUE;
}

#endif

INT64 decode(UCHAR *decodeData, INT64 maxLen, UCHAR *data, INT64 dataLen, TreeNode *nodes, int root)
{
	int nodeIndex = root;
	int cnt = 0, byteCounter = 0;

	while (byteCounter < dataLen){

		for (int i = 7; i >= 0; i--){

			if (data[byteCounter] & (1 << i))
			{
				nodeIndex = nodes[nodeIndex].right;
			}
			else
			{
				nodeIndex = nodes[nodeIndex].left;
			}

			if (nodes[nodeIndex].chr != 0 && cnt < maxLen)
			{
				decodeData[cnt++] = nodes[nodeIndex].chr;
				nodeIndex = root;
			}
		}
		byteCounter++;
	}

	return cnt;

}

UCHAR *DecodeBookData(INT64 *decodeDataLen_p, char *filename)
{
	INT64 readSize, decodeDataLen;
	FILE *fp;

	if (fopen_s(&fp, filename, "rb") != 0 || fp == NULL){
		return NULL;
	}

	UCHAR *data = (UCHAR *)malloc(BOOK_DATA_SIZE);

	if (data == NULL) return NULL;

	// open books
	readSize = fread(data, sizeof(UCHAR), 2 * sizeof(int) + 2, fp);
	if (readSize < 2 * sizeof(int) + 2){
		fclose(fp);
		free(data);
		return NULL;
	}

	// 木データのサイズ
	int nodesLen = (data[TREE_SIZE_ADDR] << 8) 
		+ data[TREE_SIZE_ADDR + 1];
	// 圧縮データのサイズ
	int evalDataLen = (data[ENCODE_SIZE_ADDR] << 24) 
		+ (data[ENCODE_SIZE_ADDR + 1] << 16) 
		+ (data[ENCODE_SIZE_ADDR + 2] << 8) 
		+ data[ENCODE_SIZE_ADDR + 3] - nodesLen - 2;
	// 解凍データのサイズ
	decodeDataLen = (data[DECODE_SIZE_ADDR] << 24) 
		+ (data[DECODE_SIZE_ADDR + 1] << 16) 
		+ (data[DECODE_SIZE_ADDR + 2] << 8) + data[3];

	TreeNode nodes[43];

	memset(nodes, 0, sizeof(nodes));

	readSize = fread(nodes, sizeof(UCHAR), nodesLen, fp);
	readSize = fread(data, sizeof(UCHAR), evalDataLen, fp);

	int root = nodesLen / sizeof(TreeNode) - 1;

	/* 復号化 */
	UCHAR *decodeData = (UCHAR *)malloc(BOOK_DECODE_DATA_SIZE);
	decodeDataLen = decode(decodeData, decodeDataLen, data, readSize, nodes, root);

	fclose(fp);
	free(data);

	*decodeDataLen_p = decodeDataLen;
	return decodeData;

}

UCHAR *DecodeEvalData(INT64 *decodeDataLen_p, char *filename)
{
	int i;
	INT64 readSize, decodeDataLen, decodeDataLenSum;
	UCHAR *data;
	UCHAR *decodeData;
	FILE *fp;
	TreeNode nodes[43];

	data = (UCHAR *)malloc(EVAL_DATA_SIZE);
	if (data == NULL) return NULL;

	decodeData = (UCHAR *)malloc(EVAL_DECODE_DATA_SIZE * 61);
	if (decodeData == NULL) return NULL;

	if (fopen_s(&fp, filename, "rb") != 0 || fp == NULL){
		free(data);
		free(decodeData);
		return NULL;
	}

	i = 0;
	decodeDataLenSum = 0;

	while (i < 60){
		readSize = fread(data, sizeof(UCHAR), 2 * sizeof(int) + 2, fp);
		// 木データのサイズ
		int nodesLen = (data[TREE_SIZE_ADDR] << 8)
			+ data[TREE_SIZE_ADDR + 1];
		// 圧縮データのサイズ
		int evalDataLen = (data[ENCODE_SIZE_ADDR] << 24)
			+ (data[ENCODE_SIZE_ADDR + 1] << 16)
			+ (data[ENCODE_SIZE_ADDR + 2] << 8)
			+ data[ENCODE_SIZE_ADDR + 3] - nodesLen - 2;
		// 解凍データのサイズ
		decodeDataLen = (data[DECODE_SIZE_ADDR] << 24)
			+ (data[DECODE_SIZE_ADDR + 1] << 16)
			+ (data[DECODE_SIZE_ADDR + 2] << 8) + data[3];

		memset(nodes, 0, sizeof(nodes));

		readSize = fread(nodes, sizeof(UCHAR), nodesLen, fp);
		readSize = fread(data, sizeof(UCHAR), evalDataLen, fp);

		int root = nodesLen / sizeof(TreeNode) - 1;
		/* 復号化 */
		decodeDataLenSum += decode(decodeData + decodeDataLenSum,
			decodeDataLen, data, readSize, nodes, root);

		i++;
	}

	fclose(fp);
	free(data);

	*decodeDataLen_p = decodeDataLenSum;

	return decodeData;

}

