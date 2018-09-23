#include <stdio.h>
#include <stdlib.h>

typedef struct Node  //structure of node
{
    int value;
    int nextIndex;
    long *child_pointer;
} node;

void Disk_Read(node *pds){}
void Disk_Write(node *pds){}

int Binary_Search(node pds, int item, int lb, int ub){
    int mid = (lb + ub)/2;
    if(pds[mid].value == item)
        return mid;
    else
    {
        if(pds[mid].value> item)
            ub = mid - 1;
        else
            lb = mid + 1;
        if(lb == ub)
           return lb;
        else
            return Binary_Search(pds,item, lb, ub);

    }
}

int Searching(node pds, int item)
{
    int lb = 1, int ub = sizeof(pds);
    int loc = Binary_Search(pds, item, lb, ub)
    if((loc <= sizeof(pds)) && (item < pds[loc].value))
    {

    }
}
void Insertion(node *pds, int item){}
void Splitting(node *pds_parent, node *pds_child, int loc){}



int main()
{
    printf("Hello world!\n");
    return 0;
}
