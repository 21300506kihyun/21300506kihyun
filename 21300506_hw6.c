#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define MAX_STR_SIZE 100

typedef struct {
	int bits_for_page_number;
	int bits_for_offset;
	int page_table_length;
	int *frame_number;

// The following fields should be computed by ReadPageTable() from the above fields.
	int page_size;
	int limit; 
} PageTable;


int ReadPageTable(const char *filename, PageTable *pt);
void DisplayPageTable(PageTable *pt);
void DestroyPageTable(PageTable *pt);
int Translate(int logical_addr, PageTable *pt);

int main(int argc, char *argv[])
{
	if(argc < 2){
		printf("Usage: %s <pagetable_file>\n", argv[0]);
		return 0;
	}

	PageTable pt = { 0 };
	int ret = ReadPageTable(argv[1], &pt);
	if(ret == FALSE){
		printf("Failed to read page table from %s\n", argv[1]);
		return -1;
	}

	DisplayPageTable(&pt);

	int addr_limit = pt.limit;

	int step = 1;
	if(addr_limit > 256)
		step = addr_limit / 256;

	for(int logical_addr = 0; logical_addr < addr_limit; logical_addr += step){
		int physical_addr = Translate(logical_addr, &pt);
		printf("%04d (0x%04x) ==> %04d (0x%04x)\n", logical_addr, logical_addr, physical_addr, physical_addr);
	}

	DestroyPageTable(&pt);

	return 0;
}
int ReadPageTable(const char *filename, PageTable *pt)
{
	FILE *fp = fopen(filename, "r");
	if(fp == NULL){
		printf("Failed to open file %s\n", filename);
		return FALSE;
	}
	char str[MAX_STR_SIZE];
	char arr[3][MAX_STR_SIZE];
	int count = 0;
	int page_num = 0;
	int offset = 0;
	int table_len = 0;
	while(fgets(str,MAX_STR_SIZE,fp)){
    //printf("num : %d, line :%s\n", count,str);
    strcpy(arr[count],str); // string을 array에 넣어줄때에는 '=' 가 아니라 strcpy를 써야한다
    count++;
    //printf("num : %d, line2 :%s\n", count,str);
  	}
	
	sscanf(arr[0], "%*s %d", &page_num);
	sscanf(arr[1], "%*s %d", &offset);
	sscanf(arr[2], "%*s %d", &table_len);
	pt->bits_for_page_number = page_num;
	pt->bits_for_offset = offset;
	pt->page_table_length = table_len;

	
	pt->frame_number = (int *)malloc(sizeof(int) * table_len);
	//pt->frame_number[0] = 3;
	//f_arr[0] = 4;

	//pt->frame_number[0] = 3;
	int arr2[table_len];
	for(int k = 0; k < table_len; k++){
		arr2[k] = atoi(arr[k+3]);
		//printf("%d\n", arr2[k]); 
	}
	for(int i = 0; i < table_len; i++){  
		pt->frame_number[i] = atoi(arr[i+3]);  // 왜 이것만 하면 중간에 이상한 값이 들어오는 걸까 ?
		//printf("%d\n", atoi(arr[i+3]));
	}
	//printf("%d\n", pt->frame_number[0]);

	unsigned char num = 1;
	pt->page_size = num << pt->bits_for_offset;
	pt->limit = (num << pt->bits_for_offset) * pt->page_table_length;
	
	return TRUE;
}

void DisplayPageTable(PageTable *pt)
{
	printf("BITS_FOR_PAGE_NUMBER %d (maximum # of pages = %d)\n",
		pt->bits_for_page_number, 1 << pt->bits_for_page_number);
	printf("BITS_FOR_OFFSET %d (page_size = %d)\n",
		pt->bits_for_offset, pt->page_size);
	printf("PAGE_TALBLE_LENGTH %d (address limit = %d)\n", pt->page_table_length, pt->limit);

	for(int i = 0; i < pt->page_table_length; i++)
		printf("%3d: %d\n", i, pt->frame_number[i]);
}

void DestroyPageTable(PageTable *pt)
{
	free(pt->frame_number);
}

int Translate(int logical_addr, PageTable *pt)
{
	int page_num = logical_addr >> pt->bits_for_offset; //page_num구하기
	int page_offset = logical_addr%pt->page_size; // page의 offset구하기
	int frame_num = pt->frame_number[page_num]; //page_table안에서 각 page에 mapping되어있는 physical_addr의 index값


	int physical_num = frame_num << pt->bits_for_offset; // physical_num값 구하기 
	int physical_offset = page_offset; // physical_addr의 offset
	int physical_addr = physical_num | physical_offset;

	return physical_addr;
	// TO DO: Implement this function.
}

