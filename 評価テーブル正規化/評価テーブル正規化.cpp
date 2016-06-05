// 評価テーブル正規化.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"

#define LEN 32
#define PATTERN_NUM 11
#define INDEX_NUM 6561
#define MOBILITY_NUM 1
#define POT_MOBILITY_NUM 60
#define PARITY_NUM 16

double hori_ver1[INDEX_NUM];
double hori_ver2[INDEX_NUM];
double hori_ver3[INDEX_NUM];
double dia_ver1[INDEX_NUM];
double dia_ver2[INDEX_NUM / 3];
double dia_ver3[INDEX_NUM / 9];
double dia_ver4[INDEX_NUM / 27];
//double dia_ver5[INDEX_NUM / 81];
double edge[INDEX_NUM * 9];
double corner5_2[INDEX_NUM * 9];
double corner3_3[INDEX_NUM * 3];
double triangle[INDEX_NUM * 9];
double mobility[MOBILITY_NUM];
double parity[PARITY_NUM];
//double constant;

//double mobility[MOBILITY_NUM];
//double pot_mobility[POT_MOBILITY_NUM];
//double pality[PALITY_NUM];

/* 出現頻度記録用 */
//int key_count_hori1[INDEX_NUM];
//int key_count_hori2[INDEX_NUM];
//int key_count_hori3[INDEX_NUM];
//int key_count_dia1[INDEX_NUM];
//int key_count_dia2[INDEX_NUM/3];
//int key_count_dia3[INDEX_NUM/9];
//int key_count_dia4[INDEX_NUM/27];
//int key_count_edge[INDEX_NUM * 9];
//int key_count_corner5_2[INDEX_NUM * 9];
//int key_count_triangle[INDEX_NUM * 9];
//int key_count_pality[PALITY_NUM];

void Eval_Write(char *filename)
{
	FILE *fp;

	if (fopen_s(&fp, filename, "w") != 0){
		return;
	}

	int i;
	for (i = 0; i < 6561; i++)
	{
		if (hori_ver1[i] == 0)
		{
			fprintf(fp, "0\n");
		}
		else
		{
			fprintf(fp, "%lf\n", hori_ver1[i]);
		}
	}
	for (i = 0; i < 6561; i++)
	{
		if (hori_ver2[i] == 0)
		{
			fprintf(fp, "0\n");
		}
		else
		{
			fprintf(fp, "%lf\n", hori_ver2[i]);
		}
	}
	for (i = 0; i < 6561; i++)
	{
		if (hori_ver3[i] == 0)
		{
			fprintf(fp, "0\n");
		}
		else
		{
			fprintf(fp, "%lf\n", hori_ver3[i]);
		}
	}
	for (i = 0; i < 6561; i++)
	{
		if (dia_ver1[i] == 0)
		{
			fprintf(fp, "0\n");
		}
		else
		{
			fprintf(fp, "%lf\n", dia_ver1[i]);
		}
	}
	for (i = 0; i < 2187; i++)
	{
		if (dia_ver2[i] == 0)
		{
			fprintf(fp, "0\n");
		}
		else
		{
			fprintf(fp, "%lf\n", dia_ver2[i]);
		}
	}
	for (i = 0; i < 729; i++)
	{
		if (dia_ver3[i] == 0)
		{
			fprintf(fp, "0\n");
		}
		else
		{
			fprintf(fp, "%lf\n", dia_ver3[i]);
		}
	}
	for (i = 0; i < 243; i++)
	{
		if (dia_ver4[i] == 0)
		{
			fprintf(fp, "0\n");
		}
		else
		{
			fprintf(fp, "%lf\n", dia_ver4[i]);
		}
	}

	for (i = 0; i < 59049; i++)
	{
		if (edge[i] == 0)
		{
			fprintf(fp, "0\n");
		}
		else
		{
			fprintf(fp, "%lf\n", edge[i]);
		}
	}

	for (i = 0; i < 59049; i++)
	{
		if (corner5_2[i] == 0)
		{
			fprintf(fp, "0\n");
		}
		else
		{
			fprintf(fp, "%lf\n", corner5_2[i]);
		}
	}
	for (i = 0; i < 19683; i++)
	{
		if (corner3_3[i] == 0)
		{
			fprintf(fp, "0\n");
		}
		else
		{
			fprintf(fp, "%lf\n", corner3_3[i]);
		}
	}
	for (i = 0; i < 59049; i++)
	{
		if (triangle[i] == 0)
		{
			fprintf(fp, "0\n");
		}
		else
		{
			fprintf(fp, "%lf\n", triangle[i]);
		}
	}
#if 0
	for (i = 0; i < MOBILITY_NUM; i++)
	{
		if (mobility[i] == 0)
		{
			fprintf(fp, "0\n");
		}
		else
		{
			fprintf(fp, "%lf\n", mobility[i]);
		}
	}
#endif
	for (i = 0; i < PARITY_NUM; i++)
	{
		if (parity[i] == 0)
		{
			fprintf(fp, "0\n");
		}
		else
		{
			fprintf(fp, "%lf\n", parity[i]);
		}
	}

	//fprintf(fp, "%lf\n", constant);

	fclose(fp);
}

/* 正規化する評価テーブルを読み込む */
int read_table(char *filename)
{
	FILE *fp;
	char str[32];

	strcpy_s(str, filename);

	if (fopen_s(&fp, str, "r") != 0){
		char msg[48];
		printf(msg, "%s を開けません。", filename);
		return -1;
	}

	int i;
	char buf[LEN];
	for (i = 0; i < 6561; i++)
	{
		fgets(buf, LEN, fp);
		sscanf_s(buf, "%lf", &hori_ver1[i]);
	}
	for (i = 0; i < 6561; i++)
	{
		fgets(buf, LEN, fp);
		sscanf_s(buf, "%lf", &hori_ver2[i]);
	}
	for (i = 0; i < 6561; i++)
	{
		fgets(buf, LEN, fp);
		sscanf_s(buf, "%lf", &hori_ver3[i]);
	}
	for (i = 0; i < 6561; i++)
	{
		fgets(buf, LEN, fp);
		sscanf_s(buf, "%lf", &dia_ver1[i]);
	}
	for (i = 0; i < 2187; i++)
	{
		fgets(buf, LEN, fp);
		sscanf_s(buf, "%lf", &dia_ver2[i]);
	}
	for (i = 0; i < 729; i++)
	{
		fgets(buf, LEN, fp);
		sscanf_s(buf, "%lf", &dia_ver3[i]);
	}
	for (i = 0; i < 243; i++)
	{
		fgets(buf, LEN, fp);
		sscanf_s(buf, "%lf", &dia_ver4[i]);
	}

	for (i = 0; i < 59049; i++)
	{
		fgets(buf, LEN, fp);
		sscanf_s(buf, "%lf", &edge[i]);
	}

	for (i = 0; i < 59049; i++)
	{
		fgets(buf, LEN, fp);
		sscanf_s(buf, "%lf", &corner5_2[i]);
	}
	for (i = 0; i < 19683; i++)
	{
		fgets(buf, LEN, fp);
		sscanf_s(buf, "%lf", &corner3_3[i]);
	}
	for (i = 0; i < 59049; i++)
	{
		fgets(buf, LEN, fp);
		sscanf_s(buf, "%lf", &triangle[i]);
	}
#if 0
	for (i = 0; i < MOBILITY_NUM; i++)
	{
		fgets(buf, LEN, fp);
		sscanf_s(buf, "%lf", &mobility[i]);
	}
#endif
	for (i = 0; i < PARITY_NUM; i++)
	{
		fgets(buf, LEN, fp);
		sscanf_s(buf, "%lf", &parity[i]);
	}

	/*fgets(buf, LEN, fp);
	sscanf_s(buf, "%lf", &constant);*/

	fclose(fp);

	return 0;
}

int pow_table[10] = { 1, 3, 9, 27, 81, 243, 729, 2187, 6561, 19683 };

int symmetry(int index_num)
{
	if (index_num == 0)
	{
		return 0;
	}
	return index_num;
}

/* 線対称 */
int convert_index_sym(int index_num, int num_table[])
{
	int i;
	int s_index_num = 0;
	for (i = 0; index_num != 0; i++)
	{
		s_index_num += symmetry(index_num % 3) * pow_table[num_table[i]];
		index_num /= 3;
	}

	return s_index_num;
}

int reverse(int index_num)
{
	if (index_num == 0)
	{
		return 0;
	}
	return index_num ^ 3;
}

/* 白黒反転 */
int convert_index_rev(int index_num)
{
	int i;
	int r_index_num = 0;
	for (i = 0; index_num != 0; i++)
	{
		r_index_num += reverse(index_num % 3) * pow_table[i];
		index_num /= 3;
	}

	return r_index_num;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int COUNT = 0;

	char filename[32];

	for (int count = 0; count < 60; count++){

		sprintf_s(filename, "%d.dat", count);

		if (read_table(filename) == -1)
		{
			printf("読み込みに失敗しました。\n");
			continue;
		}

		/*int triangleeee_convert_table[10] = {0,4,7,9,1,5,8,2,6,3};
		printf("59045 -> %d\n", convert_index_sym(59045, triangleeee_convert_table));
		return 0;*/


		printf("正規化します...");

		int i, j, table_num;

		int index_num_table[PATTERN_NUM] =
		{ INDEX_NUM, INDEX_NUM, INDEX_NUM, INDEX_NUM, INDEX_NUM / 3, INDEX_NUM / 9, INDEX_NUM / 27,
		INDEX_NUM * 9, INDEX_NUM * 9, INDEX_NUM * 3, INDEX_NUM * 9 };

		clock_t end_t, start_t = clock();

		//FILE *debug_fp;

		//fopen_s(&debug_fp, "converted_pattern_number.txt", "w+");

		/* horizon ver.1 */
		int hori_convert_table[8] = { 7, 6, 5, 4, 3, 2, 1, 0 };
		table_num = index_num_table[0];
		for (i = 0; i < table_num; i++)
		{
			j = convert_index_sym(i, hori_convert_table);
			/* 線対称とのインデックスとの差が0.1以上 または未評価の場合*/
			if (j > i)
			{
				/* 値が大きい方に正規化 */
				hori_ver1[j] = hori_ver1[i];
				COUNT++;
			}
		}

		/* horizon ver.2 */
		table_num = index_num_table[1];
		for (i = 0; i < table_num; i++)
		{
			j = convert_index_sym(i, hori_convert_table);
			/* 線対称とのインデックスとの差が0.1以上 または未評価の場合*/
			if (j > i)
			{
				/* 値が大きい方に正規化 */
				hori_ver2[j] = hori_ver2[i];
				COUNT++;
			}
		}

		/* horizon ver.3 */
		table_num = index_num_table[2];
		for (i = 0; i < table_num; i++)
		{
			j = convert_index_sym(i, hori_convert_table);
			/* 線対称とのインデックスとの差が0.1以上 または未評価の場合*/
			if (j > i)
			{
				/* 値が大きい方に正規化 */
				hori_ver3[j] = hori_ver3[i];
				COUNT++;
			}
		}

		/* diagram ver.1 */
		table_num = index_num_table[3];
		for (i = 0; i < table_num; i++)
		{
			j = convert_index_sym(i, hori_convert_table);
			/* 線対称とのインデックスとの差が0.1以上 または未評価の場合*/
			if (j > i)
			{
				/* 値が大きい方に正規化 */
				dia_ver1[j] = dia_ver1[i];
				COUNT++;
			}
		}

		/* diagram ver.2 */
		table_num = index_num_table[4];
		int dia2_convert_table[7] = { 6, 5, 4, 3, 2, 1, 0 };
		for (i = 0; i < table_num; i++)
		{
			j = convert_index_sym(i, dia2_convert_table);
			/* 線対称とのインデックスとの差が0.1以上 または未評価の場合*/
			if (j > i)
			{
				/* 値が大きい方に正規化 */
				dia_ver2[j] = dia_ver2[i];
				COUNT++;
			}
		}

		/* diagram ver.3 */
		table_num = index_num_table[5];
		int dia3_convert_table[6] = { 5, 4, 3, 2, 1, 0 };
		for (i = 0; i < table_num; i++)
		{
			j = convert_index_sym(i, dia3_convert_table);
			/* 線対称とのインデックスとの差が0.1以上 または未評価の場合*/
			if (j > i)
			{
				/* 値が大きい方に正規化 */
				dia_ver3[j] = dia_ver3[i];
				COUNT++;
			}
		}

		/* diagram ver.4 */
		table_num = index_num_table[6];
		int dia4_convert_table[5] = { 4, 3, 2, 1, 0 };
		for (i = 0; i < table_num; i++)
		{
			j = convert_index_sym(i, dia4_convert_table);
			/* 線対称とのインデックスとの差が0.1以上 または未評価の場合*/
			if (j > i)
			{
				/* 値が大きい方に正規化 */
				dia_ver4[j] = dia_ver4[i];
				COUNT++;
			}
		}
#if 0
		/* diagram ver.5 */
		table_num = 81;
		int dia5_convert_table[4] = { 3, 2, 1, 0 };
		for (i = 0; i < table_num; i++)
		{
			j = convert_index_sym(i, dia5_convert_table);
			/* 線対称とのインデックスとの差が0.1以上 または未評価の場合*/
			if (j > i)
			{
				/* 値が大きい方に正規化 */
				dia_ver5[j] = dia_ver5[i];
				COUNT++;
			}
		}
#endif

		/* edge */
		table_num = index_num_table[7];
		int edge_convert_table[10] = { 7, 6, 5, 4, 3, 2, 1, 0, 9, 8 };
		for (i = 0; i < table_num; i++)
		{
			j = convert_index_sym(i, edge_convert_table);
			/* 線対称とのインデックスとの差が0.1以上 または未評価の場合*/
			if (j > i)
			{
				/* 値が大きい方に正規化 */
				edge[j] = edge[i];
				COUNT++;
			}
		}
#if 0
		/* edge_cor */
		table_num = index_num_table[8];
		int edge_cor_convert_table[10] = {0,1,6,7,8,9,2,3,4,5};
		for(i = 0; i < table_num; i++)
		{
			j = convert_index_sym(i, edge_cor_convert_table);
			/* 線対称とのインデックスとの差が0.1以上 または未評価の場合*/
			if(j > i)
			{
				/* 値が大きい方に正規化 */
				edge_cor[j] = edge_cor[i];
				COUNT++;
			}
		}

		/* corner4_2 */
		table_num = index_num_table[9];
		int corner4_2_convert_table[10] =  {1,0,5,4,3,2,9,8,7,6};
		for(i = 0; i < table_num; i++)
		{
			j = convert_index_sym(i, corner4_2_convert_table);
			/* 線対称とのインデックスとの差が0.1以上 または未評価の場合*/
			if(j > i)
			{
				/* 値が大きい方に正規化 */
				corner4_2[j] = corner4_2[i];
				COUNT++;
			}
		}
#endif
		/* corner3_3 */
		table_num = index_num_table[10];
		int corner3_3_convert_table[9] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };
		for (i = 0; i < table_num; i++)
		{
			j = convert_index_sym(i, corner3_3_convert_table);
			/* 線対称とのインデックスとの差が0.1以上 または未評価の場合*/
			if (j > i)
			{
				/* 値が大きい方に正規化 */
				corner3_3[j] = corner3_3[i];
				COUNT++;
			}
		}

		table_num = index_num_table[10];
		int triangle_convert_table[10] = { 0, 4, 7, 9, 1, 5, 8, 2, 6, 3 };
		for (i = 0; i < table_num; i++)
		{
			j = convert_index_sym(i, triangle_convert_table);
			if (j > i)
			{
				/* 値が大きい方に正規化 */
				triangle[j] = triangle[i];
				COUNT++;
			}
		}

		end_t = clock();

		Eval_Write(filename);

		printf("\n正規化を完了しました！\n");
		printf("正規化した総パターン数 %d個, かかった時間 %.3f秒\n", COUNT, (double)(end_t - start_t) / CLOCKS_PER_SEC);
	}
	return 0;
}

