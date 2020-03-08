// 評価テーブル正規化.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "string.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "fio.h"

#define LEN 32
#define PATTERN_NUM 11
#define INDEX_NUM 6561
#define MOBILITY_NUM 36
#define POT_MOBILITY_NUM 60
#define PARITY_NUM 16
#define STAGE_NUM 61

INT16 hori_ver1[3][INDEX_NUM];
INT16 hori_ver2[3][INDEX_NUM];
INT16 hori_ver3[3][INDEX_NUM];
INT16 dia_ver1[3][INDEX_NUM];
INT16 dia_ver2[3][INDEX_NUM / 3];
INT16 dia_ver3[3][INDEX_NUM / 9];
INT16 dia_ver4[3][INDEX_NUM / 27];
//INT16 dia_ver5[INDEX_NUM / 81];
INT16 edge[3][INDEX_NUM * 9];
INT16 corner5_2[3][INDEX_NUM * 9];
INT16 corner3_3[3][INDEX_NUM * 3];
INT16 triangle[3][INDEX_NUM * 9];
INT16 mobility[MOBILITY_NUM];
INT16 parity[PARITY_NUM];
INT16 constant;

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

void Eval_Write(char *filename, int stage)
{
	FILE *fp;

	if (fopen_s(&fp, filename, "wb") != 0){
		return;
	}

	int i;
	for (i = 0; i < 6561; i++)
	{
		fwrite(&hori_ver1[stage][i], sizeof(UINT16), 1, fp);
	}
	for (i = 0; i < 6561; i++)
	{
		fwrite(&hori_ver2[stage][i], sizeof(UINT16), 1, fp);
	}
	for (i = 0; i < 6561; i++)
	{
		fwrite(&hori_ver3[stage][i], sizeof(UINT16), 1, fp);
	}
	for (i = 0; i < 6561; i++)
	{
		fwrite(&dia_ver1[stage][i], sizeof(UINT16), 1, fp);
	}
	for (i = 0; i < 2187; i++)
	{
		fwrite(&dia_ver2[stage][i], sizeof(UINT16), 1, fp);
	}
	for (i = 0; i < 729; i++)
	{
		fwrite(&dia_ver3[stage][i], sizeof(UINT16), 1, fp);
	}
	for (i = 0; i < 243; i++)
	{
		fwrite(&dia_ver4[stage][i], sizeof(UINT16), 1, fp);
	}

	for (i = 0; i < 59049; i++)
	{
		fwrite(&edge[stage][i], sizeof(UINT16), 1, fp);
	}

	for (i = 0; i < 59049; i++)
	{
		fwrite(&corner5_2[stage][i], sizeof(UINT16), 1, fp);
	}
	for (i = 0; i < 19683; i++)
	{
		fwrite(&corner3_3[stage][i], sizeof(UINT16), 1, fp);
	}
	for (i = 0; i < 59049; i++)
	{
		fwrite(&triangle[stage][i], sizeof(UINT16), 1, fp);
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
			fprintf(fp, "%d\n", mobility[i]);
		}
	}
	for (i = 0; i < PARITY_NUM; i++)
	{
		if (parity[i] == 0)
		{
			fprintf(fp, "0\n");
		}
		else
		{
			fprintf(fp, "%d\n", (int)(parity[i] * 10000.0));
		}
	}
#endif
	fwrite(&constant, sizeof(UINT16), 1, fp);

	fclose(fp);
}

/* 正規化する評価テーブルを読み込む */
int read_table(char *filename, int stage)
{
	FILE *fp;
	char str[32];

	strcpy_s(str, sizeof(str), filename);

	if (fopen_s(&fp, str, "rb") != 0){
		char msg[48];
		printf(msg, "%s を開けません。", filename);
		return -1;
	}

	int i;
	for (i = 0; i < 6561; i++)
	{
		fgets(str, sizeof(str), fp);
		sscanf_s(str, "%hd", &hori_ver1[stage][i]);
		//if (fread(&hori_ver1[stage][i], sizeof(INT16), 1, fp) != 1) return -1;
	}
	for (i = 0; i < 6561; i++)
	{
		fgets(str, sizeof(str), fp);
		sscanf_s(str, "%hd", &hori_ver2[stage][i]);
	}
	for (i = 0; i < 6561; i++)
	{
		fgets(str, sizeof(str), fp);
		sscanf_s(str, "%hd", &hori_ver3[stage][i]);
	}
	for (i = 0; i < 6561; i++)
	{
		fgets(str, sizeof(str), fp);
		sscanf_s(str, "%hd", &dia_ver1[stage][i]);
	}
	for (i = 0; i < 2187; i++)
	{
		fgets(str, sizeof(str), fp);
		sscanf_s(str, "%hd", &dia_ver2[stage][i]);
	}
	for (i = 0; i < 729; i++)
	{
		fgets(str, sizeof(str), fp);
		sscanf_s(str, "%hd", &dia_ver3[stage][i]);
	}
	for (i = 0; i < 243; i++)
	{
		fgets(str, sizeof(str), fp);
		sscanf_s(str, "%hd", &dia_ver4[stage][i]);
	}

	for (i = 0; i < 59049; i++)
	{
		fgets(str, sizeof(str), fp);
		sscanf_s(str, "%hd", &edge[stage][i]);
	}

	for (i = 0; i < 59049; i++)
	{
		fgets(str, sizeof(str), fp);
		sscanf_s(str, "%hd", &corner5_2[stage][i]);
	}
	for (i = 0; i < 19683; i++)
	{
		fgets(str, sizeof(str), fp);
		sscanf_s(str, "%hd", &corner3_3[stage][i]);
	}
	for (i = 0; i < 59049; i++)
	{
		fgets(str, sizeof(str), fp);
		sscanf_s(str, "%hd", &triangle[stage][i]);
	}

#if 0
	for (i = 0; i < MOBILITY_NUM; i++)
	{
		fgets(buf, LEN, fp);
		sscanf_s(buf, "%d", &mobility[i]);
	}

	for (i = 0; i < PARITY_NUM; i++)
	{
		fgets(buf, LEN, fp);
		sscanf_s(buf, "%d", &parity[i]);
	}
#endif

	fgets(str, sizeof(str), fp);
	sscanf_s(str, "%hd", &constant);

	if(fread(&constant, sizeof(INT16), 1, fp) != 0) 
	{
		printf("data error!!\n");
		return -1;
	}

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
	int COUNT_SMG = 0;

	char filename[32];

	for (int count = 0; count < STAGE_NUM; count++){

		sprintf_s(filename, sizeof(filename), "table\\%d.txt", count);

		if (read_table(filename, 1) == -1)
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
				hori_ver1[1][j] = hori_ver1[1][i];
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
				hori_ver2[1][j] = hori_ver2[1][i];
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
				hori_ver3[1][j] = hori_ver3[1][i];
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
				dia_ver1[1][j] = dia_ver1[1][i];
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
				dia_ver2[1][j] = dia_ver2[1][i];
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
				dia_ver3[1][j] = dia_ver3[1][i];
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
				dia_ver4[1][j] = dia_ver4[1][i];
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
				edge[1][j] = edge[1][i];
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
		table_num = index_num_table[9];
		int corner3_3_convert_table[9] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };
		for (i = 0; i < table_num; i++)
		{
			j = convert_index_sym(i, corner3_3_convert_table);
			/* 線対称とのインデックスとの差が0.1以上 または未評価の場合*/
			if (j > i)
			{
				/* 値が大きい方に正規化 */
				corner3_3[1][j] = corner3_3[1][i];
				COUNT++;
			}
		}

		/* triangle3_3 */
		table_num = index_num_table[10];
		int triangle_convert_table[10] = { 0, 4, 7, 9, 1, 5, 8, 2, 6, 3 };
		for (i = 0; i < table_num; i++)
		{
			j = convert_index_sym(i, triangle_convert_table);
			if (j > i)
			{
				/* 値が大きい方に正規化 */
				triangle[1][j] = triangle[1][i];
				COUNT++;
			}
		}

#if 0
		parity[2] = parity[1];
		parity[4] = parity[1];
		parity[8] = parity[1];

		parity[5] = parity[3];
		parity[6] = parity[3];
		parity[9] = parity[3];
		parity[10] = parity[3];
		parity[12] = parity[3];

		parity[11] = parity[7];
		parity[13] = parity[7];
		parity[14] = parity[7];
		
		/* 前のステージによるスムージング */
		sprintf_s(filename, "%d.dat", count - 1);
		if (count - 1 >= 0)
		{
			if (read_table(filename, 0) == -1) continue;
			printf("正規化します...(前のスムージング)\n");

			/* horizon ver.1 */
			table_num = index_num_table[0];
			for (i = 0; i < table_num; i++)
			{
				if (hori_ver1[1][i] == 0 && hori_ver1[0][i] != 0)
				{
					hori_ver1[1][i] = hori_ver1[0][i];
					COUNT_SMG++;
				}
			}

			/* horizon ver.2 */
			table_num = index_num_table[1];
			for (i = 0; i < table_num; i++)
			{
				if (hori_ver2[1][i] == 0 && hori_ver2[0][i] != 0)
				{
					hori_ver2[1][i] = hori_ver2[0][i];
					COUNT_SMG++;
				}
			}

			/* horizon ver.3 */
			table_num = index_num_table[2];
			for (i = 0; i < table_num; i++)
			{
				if (hori_ver3[1][i] == 0 && hori_ver3[0][i] != 0)
				{
					hori_ver3[1][i] = hori_ver3[0][i];
					COUNT_SMG++;
				}
			}

			/* diagram ver.1 */
			table_num = index_num_table[3];
			for (i = 0; i < table_num; i++)
			{
				if (dia_ver1[1][i] == 0 && dia_ver1[0][i] != 0)
				{
					dia_ver1[1][i] = dia_ver1[0][i];
					COUNT_SMG++;
				}
			}

			/* diagram ver.2 */
			table_num = index_num_table[4];
			for (i = 0; i < table_num; i++)
			{
				if (dia_ver2[1][i] == 0 && dia_ver2[0][i] != 0)
				{
					dia_ver2[1][i] = dia_ver2[0][i];
					COUNT_SMG++;
				}
			}

			/* diagram ver.3 */
			table_num = index_num_table[5];
			for (i = 0; i < table_num; i++)
			{
				if (dia_ver3[1][i] == 0 && dia_ver3[0][i] != 0)
				{
					dia_ver3[1][i] = dia_ver3[0][i];
					COUNT_SMG++;
				}
			}

			/* diagram ver.4 */
			table_num = index_num_table[6];
			for (i = 0; i < table_num; i++)
			{
				if (dia_ver4[1][i] == 0 && dia_ver4[0][i] != 0)
				{
					dia_ver4[1][i] = dia_ver4[0][i];
					COUNT_SMG++;
				}
			}

			/* edge */
			table_num = index_num_table[7];
			for (i = 0; i < table_num; i++)
			{
				if (edge[1][i] == 0 && edge[0][i] != 0)
				{
					edge[1][i] = edge[0][i];
					COUNT_SMG++;
				}
			}

			/* corner3_3 */
			table_num = index_num_table[9];
			for (i = 0; i < table_num; i++)
			{
				if (corner3_3[1][i] == 0 && corner3_3[0][i] != 0)
				{
					corner3_3[1][i] = corner3_3[0][i];
					COUNT_SMG++;
				}
			}

			/* triangle3_3 */
			table_num = index_num_table[10];
			for (i = 0; i < table_num; i++)
			{
				if (triangle[1][i] == 0 && triangle[0][i] != 0)
				{
					triangle[1][i] = triangle[0][i];
					COUNT_SMG++;
				}
			}
		}


		/* 後ろのステージによるスムージング */
		sprintf_s(filename, "%d.dat", count + 1);
		if (count + 1 < 60)
		{
			if (read_table(filename, 2) == -1) continue;
			printf("正規化します...(後ろのスムージング)\n");

			/* horizon ver.1 */
			table_num = index_num_table[0];
			for (i = 0; i < table_num; i++)
			{
				if (hori_ver1[1][i] == 0 && hori_ver1[2][i] != 0)
				{
					hori_ver1[1][i] = hori_ver1[2][i];
					COUNT_SMG++;
				}
			}

			/* horizon ver.2 */
			table_num = index_num_table[1];
			for (i = 0; i < table_num; i++)
			{
				if (hori_ver2[1][i] == 0 && hori_ver2[2][i] != 0)
				{
					hori_ver2[1][i] = hori_ver2[2][i];
					COUNT_SMG++;
				}
			}

			/* horizon ver.3 */
			table_num = index_num_table[2];
			for (i = 0; i < table_num; i++)
			{
				if (hori_ver3[1][i] == 0 && hori_ver3[2][i] != 0)
				{
					hori_ver3[1][i] = hori_ver3[2][i];
					COUNT_SMG++;
				}
			}

			/* diagram ver.1 */
			table_num = index_num_table[3];
			for (i = 0; i < table_num; i++)
			{
				if (dia_ver1[1][i] == 0 && dia_ver1[2][i] != 0)
				{
					dia_ver1[1][i] = dia_ver1[2][i];
					COUNT_SMG++;
				}
			}

			/* diagram ver.2 */
			table_num = index_num_table[4];
			for (i = 0; i < table_num; i++)
			{
				if (dia_ver2[1][i] == 0 && dia_ver2[2][i] != 0)
				{
					dia_ver2[1][i] = dia_ver2[2][i];
					COUNT_SMG++;
				}
			}

			/* diagram ver.3 */
			table_num = index_num_table[5];
			for (i = 0; i < table_num; i++)
			{
				if (dia_ver3[1][i] == 0 && dia_ver3[2][i] != 0)
				{
					dia_ver3[1][i] = dia_ver3[2][i];
					COUNT_SMG++;
				}
			}

			/* diagram ver.4 */
			table_num = index_num_table[6];
			for (i = 0; i < table_num; i++)
			{
				if (dia_ver4[1][i] == 0 && dia_ver4[2][i] != 0)
				{
					dia_ver4[1][i] = dia_ver4[2][i];
					COUNT_SMG++;
				}
			}

			/* edge */
			table_num = index_num_table[7];
			for (i = 0; i < table_num; i++)
			{
				if (edge[1][i] == 0 && edge[2][i] != 0)
				{
					edge[1][i] = edge[2][i];
					COUNT_SMG++;
				}
			}

			/* corner3_3 */
			table_num = index_num_table[9];
			for (i = 0; i < table_num; i++)
			{
				if (corner3_3[1][i] == 0 && corner3_3[2][i] != 0)
				{
					corner3_3[1][i] = corner3_3[2][i];
					COUNT_SMG++;
				}
			}

			/* triangle3_3 */
			table_num = index_num_table[10];
			for (i = 0; i < table_num; i++)
			{
				if (triangle[1][i] == 0 && triangle[2][i] != 0)
				{
					triangle[1][i] = triangle[2][i];
					COUNT_SMG++;
				}
			}
		}
#endif
		end_t = clock();

		sprintf_s(filename, sizeof(filename), "table\\new\\%d.dat", count);
		Eval_Write(filename, 1);

		printf("\n正規化を完了しました！\n");
		printf("正規化した総パターン数 %d個, スムージングした総パターン数 %d個, かかった時間 %.3f秒\n", COUNT, COUNT_SMG, (double)(end_t - start_t) / CLOCKS_PER_SEC);

	}

	printf("\n評価テーブルを作成しています...\n");
	makeData();

	printf("\n評価テーブルを作成しました！\n");

	return 0;
}

