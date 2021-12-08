/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"
int main()
{
  int n = 0;
	int i, j, temp;
	PrintString("Nhap so luong phan tu: ");
	//So luong phan tu
  n = ReadInt();
	if (n > 0 && n <= 100)
	{
    int A[100];
		//Lan luot nhap cac phan tu
		for (i = 0; i < n; i++)
		{
      PrintString("Nhap phan tu thu[");
      PrintInt(i);
      PrintString("]: ");
			A[i] = ReadInt();
		}
    PrintString("Dang sap xep...\n");
		//Sap xep mang bang bubble sort.
		for (i = 0; i < n - 1; i++)
			for (j = i + 1; j < n; j++)
				if (A[i] > A[j]) 
				{	
					temp = A[j];
					A[j] = A[i];
					A[i] = temp;
				}

		//In mang sau khi duoc sap xep.
		PrintString("Mang sau khi duoc sap xep:\n");
		for (i = 0; i < n; i++)
		{
			PrintInt(A[i]);
			PrintString("\t");
		}
		PrintString("\n");
	} else {
    PrintString("Vui long nhap mot so phu hop nho hon 100!!");
  }
}
