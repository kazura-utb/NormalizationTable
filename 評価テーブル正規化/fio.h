/***************************************************************************
* Name  : fio.h
* Brief : ファイルIO関連の処理を行う
* Date  : 2016/02/01
****************************************************************************/

#include "stdafx.h"

#pragma once

typedef struct {
	int code;
	int codeSize;
}CodeInfo;

typedef struct{
	char chr;
	int occurrence;
	int parent;
	int left;
	int right;
}TreeNode;

BOOL makeData();
