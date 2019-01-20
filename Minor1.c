#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
typedef struct Node
{
    int value;
    int limit;
    int nextIndex;
    int is_leaf;
    struct Node *c[3];
}node;
typedef struct o_node
{
    int value[2];
    int limit;
    int is_leaf;
    struct o_node *c[3];
}o_node;
int limit=2;
struct o_node *intialise_onode(void);
node *global[3]={NULL,NULL,NULL};
o_node *global_b=NULL;
//-----------------------------------------------------------------------------------------------------------------------------
int b_search(node *pds[], int item, int lb, int ub){
    int mid = (lb + ub)/2;
    if(lb==ub)
    {
        if(pds[lb]->value>=item)
        {
            return (lb-1);
        }
        return lb;
    }
    else
    {
        if(pds[mid]->value>item)
        {
            if(mid==1)
            {
               ub=mid;
            }
            else
            {
               ub=mid-1;
            }
        }
        else
        {
            lb=mid+1;
        }
    }
    b_search(pds,item,lb,ub);
}
//---------------------------------------------------------------------------------
void Disk_Write(node *pds[],o_node *btree)
{
    btree->limit=pds[0]->limit;
    btree->is_leaf=pds[0]->is_leaf;
    int y=0;
    int i=0;
    while(y<pds[0]->limit)
    {
        btree->value[y]=pds[pds[i]->nextIndex]->value;
        i=pds[i]->nextIndex;
        y++;
    }
    y=0;
    int replace=-1;
    for(i=1;i<=pds[0]->limit;i++)
    {
        pds[i]->value=btree->value[i-1];
        pds[i]->nextIndex=i+1;
        if(pds[0]->is_leaf==0)
        {
            if(i==pds[i-1]->nextIndex)
            {
                if(replace==1)
                {
                    y=0;
                    for(y=0;y<3;y++)
                    {
                        pds[2]->c[y]=global[y];
                    }
                }
                else
                {
                    copy_children(pds,pds,i,pds[i-1]->nextIndex);
                }
            }
            else
            {
                for(y=0;y<3;y++)
                {
                    global[y]=pds[1]->c[y];
                }
                copy_children(pds,pds,i,pds[i-1]->nextIndex);
                replace=1;
            }
        }
    }
    pds[0]->nextIndex=1;
}
//-------------------------------------------------------------------------------------------------------------------------
int insertion(node *pds[],node *pds_parent[], int item,o_node *btree,o_node *btree_parent){
    int decision=0;
    if(pds[0]->limit==0)
    {
        pds[1]->value=item;
        pds[0]->limit++;
        pds[1]->nextIndex=(pds[0]->limit+1);
        Disk_Write(pds,btree);                            //DISK WRITE FUNCTION
        return 1;
    }
    int loc=b_search(pds,item,1,pds[0]->limit);
    if(pds[0]->is_leaf==1)
    {
        if(pds[0]->limit==2)
        {
            return insert_non_leaf(pds_parent,loc,item,pds,btree,btree_parent);
        }
        else
        {
            pds[(pds[0]->limit+1)]->value=item;
            printf("location is %d\n",loc);
            if(loc==limit)
            {
               pds[(pds[0]->limit+1)]->nextIndex=-1;
            }
            else
            {
               pds[(pds[0]->limit+1)]->nextIndex=loc+1;
               pds[loc]->nextIndex=pds[0]->limit+1;
            }
            pds[0]->limit++;
            Disk_Write(pds,btree);                          // DISK WRITE FUNCTION
            return 1;
        }
    }
    else
    {
        if(pds[loc]->value>item)
        {
            decision=insertion(pds[loc]->c,pds,item,btree->c[loc],btree);
        }
        else
        {
            decision=insertion(pds[loc]->c,pds,item,btree->c[loc],btree);
        }
        if(decision==1)
        {
            return 1;
        }
        else
        {
           decision=insert_non_leaf(pds_parent,loc,decision,pds,btree,btree_parent);
           return decision;
        }
    }
}
//------------------------------------------------------------------------------------------------------------
int insert_non_leaf(node *pds_parent[],int loc,int item, node *pds[],o_node *btree,o_node *btree_parent)
{
    int value=0;
    node *new_node[3];
    intialise_node(new_node);
    o_node *new_btree;
    new_btree=intialise_onode();
    //int location=b_search(pds_parent,value,1,pds_parent[0]->limit);
    value=split_node(pds,new_node,pds_parent,loc,item);
    Disk_Write(pds,btree);                              // DISK WRITE FUNCTION
    Disk_Write(global,new_btree);                     // DISK WRITE FUNCTION
    if(pds_parent==NULL)
    {
        node *new_root[3];
        intialise_node(new_root);
        o_node *new_btree_root;
        new_btree_root=intialise_onode();
        new_root[0]->limit++;
        new_root[0]->is_leaf=0;
        new_btree_root->is_leaf=0;
        //new_root[0]->c=pds;
        make_children(new_root,pds,0);
        make_children_btree(new_btree_root,btree,0);                // MAKE CHILDREN BTREE
        new_root[1]->value=value;
        new_root[1]->nextIndex=-1;
        //new_root[1]->c=global;
        make_children(new_root,global,1);
        make_children_btree(new_btree_root,new_btree,1);           // MAKE CHILDREN BTREE
        if(global[0]->c[0]!=NULL)
        {
            pds[0]->is_leaf=0;
            global[0]->is_leaf=0;
        }
        Disk_Write(new_root,new_btree_root);                    // DISK WRITE FUNCTION
        int i=0;
        for(i=0;i<3;i++)
        {
            global[i]=new_root[i];
        }
        global_b=new_btree_root;
        return 1;
    }
    else if(pds_parent[0]->limit==2)  // PARENT NODE HAS TO BE SPLIT RECURSIVELY
    {
        return value;
    }
    else
    {
        int locp=b_search(pds_parent,value,1,pds[0]->limit);
        pds_parent[(pds_parent[0]->limit+1)]->value=value;
            if(locp==limit)
            {
               pds_parent[(pds_parent[0]->limit+1)]->nextIndex=-1;
            }
            else
            {
               pds_parent[(pds_parent[0]->limit+1)]->nextIndex=locp+1;
               pds_parent[locp]->nextIndex=(pds_parent[0]->limit+1);
            }
            make_children(pds_parent,global,(pds_parent[0]->limit+1));
            pds_parent[0]->limit++;
            Disk_Write(pds_parent,btree_parent);
            if(locp==pds_parent[0]->limit)
            {
                make_children_btree(btree_parent,new_btree,(pds_parent[0]->limit+1));
            }
            else
            {
                copy_children_btree(btree_parent,btree_parent,2,1);
                make_children_btree(btree_parent,new_btree,locp+1);
            }
            return 1;
    }
}
//------------------------------------------------------------------------------------
int split_node(node *pds[],node *new_node[],node *pds_parent[],int loc,int item)
{
    int value=0;
    if(loc==limit/2)
    {
        printf("location is = limit/2\n");
        int i;
        for(i=1;i<=limit/2;i++)
        {
            new_node[i]->value=pds[i+limit/2]->value;
            //new_node[i]->c=pds[i+limit/2]->c;
            copy_children(new_node,pds,i,(i+limit/2));
            new_node[i]->nextIndex=i+1;
        }
        value=item;
        if(pds[0]->is_leaf!=1)
        {
            //new_node[0]->c=global;
            make_children(new_node,global,0);
        }
    }
    else if(loc<limit/2)
    {
        printf("location is < limit/2\n");
        value=pds[(limit/2)]->value;
        int i;
        for(i=1;i<=limit/2;i++)
        {
            new_node[i]->value=pds[i+limit/2]->value;
            //new_node[i]->c=pds[i+limit/2]->c;
            copy_children(new_node,pds,i,(i+limit/2));
            new_node[i]->nextIndex=i+1;
        }
        //new_node[0]->c=pds[limit/2]->c;
        copy_children(new_node,pds,0,(limit/2));
        pds[limit/2]->value=item;
        pds[(limit/2)]->nextIndex=loc+1;
        pds[loc]->nextIndex=limit/2;
        if(pds[0]->is_leaf!=1)
        {
            //pds[limit/2]->c=global;
            make_children(pds,global,limit/2);
        }
    }
    else if(loc>limit/2)
    {
        printf("location is > limit/2\n");
        value=pds[(limit/2)+1]->value;
        int i=0;
        for(i=1;i<=limit/2;i++)
        {
            if(loc==(i+limit/2))
            {
                new_node[i]->value=item;
                //new_node[i-1]->c=pds[i+limit/2]->c;
                copy_children(new_node,pds,i-1,(i+limit/2));
                //new_node[i]->c=global;
                make_children(new_node,global,i);
                new_node[i]->nextIndex=i+1;
            }
            else
            {
                new_node[i]->value=pds[i+limit/2]->value;
                //new_node[i]->c=pds[i+limit/2]->c;
                copy_children(new_node,pds,i,(i+limit/2));
                new_node[i]->nextIndex=i+1;
            }
        }
       // new_node[0]->c=pds[limit/2]->c;
        copy_children(new_node,pds,0,(limit/2));
        if(new_node[0]->c[0]==NULL)
        {
            printf("null son in loop 5\n");
        }
        else
        {
            printf("not null son in loop 5\n");
        }
    }
    if(pds[0]->is_leaf==0)
    {
        new_node[0]->is_leaf=0;
    }
    int i=0;
    for(i=0;i<3;i++)
    {
       global[i]=new_node[i];
    }
    pds[0]->limit=limit/2;
    new_node[0]->limit=limit/2;
    printf("global value %d\n",global[1]->value);
    printf("vlaue returned from split_node %d\n",value);
    return value;
}
//--------------------------------------------------------------------------------------------------------------------------
void make_children(node *parent[],node *child[],int loc)
{
    int i=0;
    for(i=0;i<3;i++)
    {
       parent[loc]->c[i]=child[i];
    }
}
//-------------------------------------------------------------
void make_children_btree(o_node *parent,o_node *child,int loc)
{
       parent->c[loc]=child;
}
//------------------------------------------------------------------------------------
void copy_children_btree(o_node *node1,o_node *node2,int loc1,int loc2)
{
       node1->c[loc1]=node2->c[loc2];
}
//-------------------------------------------------------------------------------------------------------------------------
void copy_children(node *node1[],node *node2[],int loc1,int loc2)
{
    int i=0;
    for(i=0;i<3;i++)
    {
       node1[loc1]->c[i]=node2[loc2]->c[i];
    }
}
//-----------------------------------------------------------------------------------------------
void intialise_node(node *pds[])
{
    int i=0;
    int j=0;
    for (i = 0; i < 3; i++) {
        pds[i] = (node *)malloc(sizeof(node));
        for(j=0;j<3;j++)
        {
            pds[i]->c[j]=NULL;
        }
    }
    pds[0]->limit=0;
    pds[0]->nextIndex=1;
    pds[0]->value=999999999;
    pds[0]->is_leaf=1;

}
//----------------------------------------------------------------------------------------------------------------------------
struct o_node *intialise_onode(void)
{
     struct o_node *btree=(o_node *)malloc(sizeof(o_node));
    btree->limit=0;
    btree->value[0]=0;
    btree->value[1]=0;
    btree->is_leaf=1;
    int i=0;
    for(i=0;i<3;i++)
    {
        btree->c[i]=NULL;
    }
    return btree;
}
void main()
{
    int i=0;
    node *pds[3];
    intialise_node(pds);
    printf("pds value %d\n",pds[0]->value);
    o_node *btree;
    btree=intialise_onode();
    printf("---------------------------------------\n");
    insertion(pds,NULL,200,btree,NULL);
    printf("pds value %d\n",pds[1]->value);
    printf("pds limit %d\n",pds[0]->limit);
    printf("btree's value in main  %d\n",btree->value[0]);
    printf("btree limit %d\n",btree->limit);
    for(i=0;i<3;i++)
    {
        global[i]=NULL;
    }
    printf("---------------------------------------\n");
    /*insertion(pds,NULL,100,btree,NULL);
    printf("pds value %d\n",pds[2]->value);
    printf("pds limit %d\n",pds[0]->limit);
    printf("btree value %d\n",btree->value[1]);
    printf("btree limit %d\n",btree->limit);
    i=0;
    for(i=0;i<3;i++)
    {
        global[i]=NULL;
    }*/
    printf("---------------------------------------\n");
    /*insertion(pds,NULL,300,btree,NULL);
    btree=global_b;
    printf("new parent value is %d\n",global[1]->value);
    printf("children's value %d\n",global[0]->c[1]->value);
    printf(" new btree's value is %d\n",btree->value[0]);
    printf(" new btree's limit is %d",btree->limit);
    printf("btree's child value is %d",btree->c[1]->value[0]);
    i=0;
    for(i=0;i<3;i++)
    {
        pds[i]=global[i];
        global[i]=NULL;
    }*/
    printf("---------------------------------------\n");
    /*insertion(pds,NULL,150,btree,NULL);
    printf("children's value %d\n",pds[0]->c[2]->value);
    printf("children's limit %d\n",pds[0]->c[0]->limit);
    printf(" btree's children's value is %d\n",btree->c[0]->value[1]);
    i=0;
    for(i=0;i<3;i++)
    {
        global[i]=NULL;
    }*/
    printf("---------------------------------------\n");
    /*insertion(pds,NULL,160,btree,NULL);
    printf("parents's value %d\n",pds[1]->value);
    printf("parents's limit %d\n",pds[0]->limit);
    printf(" pds children's value %d\n",pds[2]->c[1]->value);
    printf(" btree's value is %d\n",btree->value[0]);
    printf("btree's limit is %d",btree->limit);
    printf(" btree's children's value is %d\n",btree->c[2]->value[0]);
    i=0;
    for(i=0;i<3;i++)
    {
        global[i]=NULL;
    }*/
    printf("---------------------------------------\n");
    /*insertion(pds,NULL,170,btree,NULL);
    printf(" pds children's value %d\n",pds[1]->c[2]->value);
    printf(" btree's children's value is %d\n",btree->c[1]->value[0]);
        printf(" btree's children's value is %d\n",btree->c[1]->value[1]);
        printf(" btree's children's limit is %d\n",btree->c[1]->limit);
    i=0;
    for(i=0;i<3;i++)
    {
        global[i]=NULL;
    }*/
    printf("---------------------------------------\n");
}
