#include<stdio.h>
#include<stdlib.h>
typedef enum{true,false}bool;
//typedef enum{true,false,mid}new_bool;
char Array[1024];
typedef struct node
{
    int Actual_size;
    int size;
    struct node *left,*right;
    bool Left_free,Right_free,Overall;
}node;

node* Split(int data,int size,node *Head,int *done)
{
    node *temp;
    if((data > (size/2)) && (data <= size) && *done == 0)
    {
        temp = (node*)malloc(sizeof(node));
        temp->size = size;
        temp->Actual_size = data;
        //temp->Address = Array+size; 
        temp->left = temp->right = NULL;
        temp->Left_free = false;
        temp->Right_free = false;
        temp->Overall = false;
        Head->left = temp;
        Head=Head->left;
        //printf("node :%d\n",temp->size);
        *done = 1;
    }
    else
    {
        if(*done == 0)
        {
            temp = (node*)malloc(sizeof(node));
            temp->size = size/2;
            temp->Actual_size = temp->size;
            //temp->Address = Array+(size/2);
            temp->left = temp->right = NULL;
            //temp->Left_free = true;
            temp->Left_free = true;
            temp->Right_free = true;
            temp->Overall = true;
            Head->left = temp;
            //Head = Head->left;
            //printf("left_split :%d\n",temp->size);
            Head->left = Split(data,(size/2),Head->left,done);

            // for buddy
            temp = (node*)malloc(sizeof(node));
            temp->size = size/2;
            temp->Actual_size = temp->size;
            //temp->Address = Array+(size);
            temp->left = temp->right = NULL;
            temp->Left_free = true;
            temp->Left_free = true;
            temp->Overall = true;
            Head->right = temp;
            //Head = Head->right;
            //printf("right_split :%d\n",temp->size);
            Head->right = Split(data,size/2,Head->right,done);
        }  
    }
    return Head;
}

node* Second_third(int data,int size,node *Head,int *done)
{
    if(Head==NULL)
    {
        return Head;
    }
    if(data <= (size/2))
    {
        Head->left = Second_third(data,size/2,Head->left,done);
    }
    if(data < (size/2))
    {
        Head->right = Second_third(data,size/2,Head->right,done);
    }
    
    if(Head->Overall == true && (data > (size/2)) && (data <= size) && *done == 0)//node is free 
    {
        //printf("Found : %d address %p\n",Head->size,Head->Address);
        //Head->Address = Array+size;
        Head->Actual_size = data;
        Head->left = Head->right = NULL;
        Head->Overall = false;
        *done = 1;
    }
    else if(Head->Overall == true && (data > (size/2)) && (data <= size) && *done == 0)//node is free 
    {
        //printf("Found : %d address %p\n",Head->size,Head->Address);
        //Head->Address = Array+size;
        Head->Actual_size = data;
        Head->left = Head->right = NULL;
        Head->Overall = false;
        *done = 1;
    }
    
    if(*done == 0 && Head!=NULL && Head->Overall == true)
    {
        //printf("The is me:\n");
        Head = Split(data,(Head->size),Head,done);
        *done = 1;
    }
    else if(*done == 0 && Head!=NULL && Head->Overall == true)
    {
        Head = Split(data,(Head->size),Head,done);
        *done = 1;
    }
    return Head;
}
void SetValue_for_alloc(int Actual_size,node *Head,int *d1)
{
    if(Head->left != NULL && Head->right != NULL && *d1==0)
    {
        SetValue_for_alloc(Actual_size,Head->left,d1);
        SetValue_for_alloc(Actual_size,Head->right,d1);
        if(Head->Actual_size == Actual_size && *d1 == 0)
        {
            Head->Overall = false;
            *d1 = 1;
        }
        if(Head->left->Overall == false || Head->right->Overall == false)
        {
            Head->Overall = false;
        }
        else
        {
            Head->Overall = true;
        }
    }
    if(Head->Actual_size == Actual_size && *d1 == 0)
    {   
        Head->Left_free = false;
        Head->Right_free = false;
        Head->Overall = false;
        *d1 = 1;
    }
}
void SetValue_for_free(int Actual_size,node* Head)
{
    if(Head->left != NULL && Head->right != NULL)
    {
        SetValue_for_free(Actual_size,Head->left);
        SetValue_for_free(Actual_size,Head->right);
        if(Head->Actual_size == Actual_size)
        {
            Head->Overall = true;
            Head->Actual_size = Head->size;
        }
        if((Head->left->Overall == false || Head->right->Overall == false))
        {
            Head->Overall = false;
        }
        else
        {
            Head->Overall = true;
        }           
    }
    if(Head->Actual_size == Actual_size)
    {
        Head->Overall = true;
        Head->Actual_size = Head->size;
    }
}

void Del(node* root)
{
    if(root->left!=NULL && root->right!=NULL)
    {
        Del(root->left);
        Del(root->right);
        if(root->left->Overall == true && root->right->Overall == true)
        {
            // if both child are free then delete both
            free(root->left);
            free(root->right);
            root->left = NULL;
            root->right = NULL;
        }
    }
}
/*
void Collapse(node* root)
{
    if(root->left == NULL && root->right==NULL)
    {
        return;
    }
    else
    {
        Collapse(root->left);
        Collapse(root->right);
        if(root->left->free == true && root->right->free == true)
        {
            free(root->left);
            free(root->right);
        }
    }
}

working
void Merge(node *root)
{
    if(root!=NULL)
    {
        Merge(root->left);
        Merge(root->right);
        if(root->free == true)
        {
            printf("Deleting : %d\n",root->size);
            free(root);
        }
        /*
        if(root->left && root!= NULL)
        {
            root->left = NULL;
        }
        if(root->right && root!= NULL)
        {
            root->right = NULL;   
        }
        
    }   
}

void Merge(node *root)
{
    if(root!=NULL)
    {
        Merge(root->left);
        Merge(root->right);
        if(root->free == true && root->size != 1024)
        {
            root->right = NULL;
            root->left = NULL;
            printf("Deleting : %d\n",root->size);
            free(root);
        }
    }  
}
*/
void Delete(node** Node_ref)
{
    Del(*Node_ref);
    //*Node_ref = NULL;
}

void Print(node* Head)
{
    if(Head==NULL)
    {
        return;
    }
    Print(Head->left);
    Print(Head->right);
    if(Head!=NULL)
    {
        printf("size: %d Actual Size: %d status: %d\n",Head->size,Head->Actual_size,Head->Overall);
    }
    
}
node* Initialize(node* Head)
{
    int size = 1024;
    Head = (node*)malloc(sizeof(node));
    Head->left = Head->right = NULL;
    Head->size = size;
    Head->Actual_size = size;
    return Head;
}
void main()
{
    node *Head;
    int size = 1024;
    int done = 0,i,num,data,d1 = 0;
    Head = Initialize(Head);
    printf("Enter The Number of times you want to do: \n");
    scanf("%d",&i);
    while (i>0)
    {
        printf("Enter 1 for Allocate And 2 for Free: \n");
        scanf("%d",&num);
        if(num == 1)
        {
            printf("**********-**********\n");
            printf("Enter The data: \n");
            scanf("%d",&data);
            Head = Second_third(data,size,Head,&done);
            SetValue_for_alloc(data,Head,&d1);
            Print(Head);
            done = 0;
            d1 = 0;
        }
        else
        {
            printf("**********-**********\n");
            printf("Enter The Size you want to delete: \n");
            scanf("%d",&data);
            SetValue_for_free(data,Head);
            Print(Head);
            //Head->free = false;
            Delete(&Head);
            /*
            if(Head == NULL)
            {
                Head = Initialize(Head);
            }
            */
            printf("After Deletion:\n");
            Print(Head);   
        }
        i--;
    }
    
}