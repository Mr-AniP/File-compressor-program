#include<stdio.h>
#include<stdlib.h>

struct pq_node{
    char c;
    unsigned int priority;
    struct pq_node*left,*right;
};

struct node{
    char c;
    int mycode[8];
    int top;
};

struct minh1{
    int size;
    struct node **arr;
};

struct minh{
    int size;
    struct pq_node **arr;
};

struct nest{
    char c;
    struct nest*left,*right;
};

void swap(struct pq_node **a, struct pq_node **b){
    struct pq_node *t=*a;
    *a=*b;
    *b= t;
}

void heapify(int i,struct minh *H){
    if(i>(*H).size || (*H).size==1 || (*H).size==0)
    return;
    int large=i;
    int l=1+(2*i);
    int r=2+(2*i);
    if(l<(*H).size && (*((*H).arr + i))->priority > (*((*H).arr +l))->priority){
        large=l;
    }
    if(r<(*H).size && (*((*H).arr + i))->priority >(*((*H).arr +r))->priority){
        large=r;
    }
    if(large!=i){
        swap(((*H).arr + i),((*H).arr + large));
        heapify(large,H);
    }
}

struct pq_node *extractmin(struct minh *H){
    struct pq_node *temp=((*H).arr[0]);
    (*H).arr[0]=(*H).arr[(*H).size-1];
    (*H).size--;
    heapify(0,H);
    return temp;
}

struct pq_node *new(char ch){
    struct pq_node *node=(struct pq_node *)malloc(sizeof(struct pq_node));
    node->c=ch;
    node->priority=1;
    node->left=NULL;
    node->right=NULL;
    return node;
}

void insert(struct minh *H,struct pq_node *node){
    (*H).size++;
    int j=((*H).size)-1;
    while(j!=0 && node->priority < (*H).arr[(j-1)/2]->priority){
        (*H).arr[j]=(*H).arr[(j-1)/2];
        j=(j-1)/2;
    }
    (*H).arr[j]=node;
}

void minheapify(struct minh *H){
    int i=((*H).size-2)/2;
    while (i>=0){
        heapify(i,H);
        i--;
    }
}

struct pq_node* hoffmantree(struct minh *H){
    minheapify(H);
    struct pq_node *node1,*node2,*topsify;
    while((*H).size>1){
        node1=extractmin(H);
        node2=extractmin(H);
        topsify=new(EOF);
        topsify->priority=node1->priority+node2->priority;
        topsify->left=node1;
        topsify->right=node2;
        insert(H,topsify);
    }
    return extractmin(H);
}

void Hcode(struct pq_node *node,int count,int ar[],struct minh1 *H1){
    if(node->left!=NULL){
        ar[count]=0;
        Hcode(node->left,count+1,ar,H1);
    }
    if(node->right!=NULL){
        ar[count]=1;
        Hcode(node->right,count+1,ar,H1);
    }
    if(node->left==NULL && node->right==NULL){
        struct node* node1=(struct node *)malloc(sizeof(struct node));
        node1->top=count;
        node1->c=node->c;
        printf("%c -> ",node->c);
        for(int i=0;i<count;i++){
            printf("%d",ar[i]);
            node1->mycode[i]=ar[i];
        }
        printf("\n");
        (*H1).arr[(*H1).size++]=node1;
    }
}

int power_f(int x,int n){
	if(n==0)
	return 1;
	if(n==1)
	return x;
	if(n%2==0){
		int p=power_f(x,n/2);
		return (p*p);
	}
	int p=power_f(x,(n-1)/2);
	return (x*p*p);
}

int deciboy(int buf[]){
    int a=0;
    for(int i=0;i<8;i++){
        a+=buf[i]*power_f(2,7-i);
    }
    return a;
}

void compress(char in[],char out[]){
    struct pq_node *tree;
    struct minh H;
    H.size=0;
    H.arr=(struct pq_node ** )malloc(127*sizeof(struct pq_node *));
    FILE *fs=fopen(in,"r"),*ft;
    if(fs==NULL){
        printf("Cannot open %s\n",in);
        exit(0);
    }
    while(1){
        char ch=getc(fs);
        if(ch==EOF){
            break;
        }
        else if(H.size==0){
            tree=new(ch);
            insert(&H,tree);
        }
        else{
            int i=0;
            for(;i<H.size;i++){
                if(H.arr[i]->c==ch){
                    H.arr[i]->priority++;
                    break;
                }
            }
            if(i==H.size){
                tree=new(ch);
                insert(&H,tree);
            }
        }
    }
    int a =H.size;
    int ar[9];
    struct minh1 H1;
    H1.size=0;
    H1.arr=(struct node ** )malloc(H.size*sizeof(struct node *));
    tree=hoffmantree(&H);
    free(H.arr);
    if(a>1){
        Hcode(tree,0,ar,&H1);
    }
    else if(a==1){
        struct node* node1;
        node1->mycode[0]=0;
        node1->top=1;
        node1->c=tree->c;
        H1.arr[H1.size++]=node1;
        printf("%c -> ",node1->c);
        printf("0\n");
    }
    else{
        printf("Generating %s\n",out);
        ft=fopen(out,"wb");
        if(ft==NULL){
            printf("Cannot open %s\n",out);
            exit(0);
        }
        fclose(ft);
        exit(0);
    }
    printf("Generating %s\n",out);
    ft=fopen(out,"w");
    if(ft==NULL){
        printf("Cannot open %s\n",out);
        exit(0);
    }
    char acg;
    for(int i=0;i<H1.size;i++){
        fprintf(ft,"%c -> ",H1.arr[i]->c);
        for(int j=0;j<H1.arr[i]->top;j++){
            if(H1.arr[i]->mycode[j]==0)
            acg='0';
            else
            acg='1';
            fprintf(ft,"%c",acg);
        }
        fprintf(ft,"\n");
    }
    rewind(fs);
    int buer[8],nx=0;
    while(1){
        char ch=getc(fs);
        if(ch==EOF){
            if(nx==8){
                int aghu=deciboy(buer);
                fprintf(ft,"%c",aghu);
                fprintf(ft,"\n%d",(8-nx));
            }
            else if(nx!=0){
                for(int i=nx;i<8;i++)
                buer[i]=0;
                int aghu=deciboy(buer);
                fprintf(ft,"%c",aghu);
                fprintf(ft,"\n%d",(8-nx));
            }
            break;
        }
        else{
            for(int i=0;i<H1.size;i++){
                if(ch==H1.arr[i]->c){
                    for(int j=0;j<H1.arr[i]->top;j++){
                        if(nx==8){
                            nx=0;
                            int aghu=deciboy(buer);
                            fprintf(ft,"%c",aghu);
                        }
                        buer[nx]=H1.arr[i]->mycode[j];
                        nx++;
                    }
                    break;
                }
            }
        }
    }
    fclose(ft);
    fclose(fs);
}

struct nest* newnodel(){
    struct nest* temp=(struct nest *)malloc(sizeof(struct nest));
    temp->left=NULL;
    temp->right=NULL;
    temp->c='\0';
    return temp;
}

void insertom(struct nest **head, char c ,int arr[],int top,int i){
    if((*head)==NULL){
        (*head)=newnodel();
    }
    struct nest *temp=(*head);
    while(i<top){
        if(arr[i]==0){
            if(temp->left==NULL){
                temp->left=newnodel();
            }
            temp=temp->left;
        }
        else if(arr[i]==1){
            if(temp->right==NULL){
                temp->right=newnodel();
            }
            temp=temp->right;
        }
        i++;
    }
    temp->c=c;
}

void decompress(char in[],char out[]){
    struct nest *head=NULL,*pointme;
    struct ch{
    	char a;
    	char b;
    	char c;
    	char d;
    	char e;
    };
    struct ch ch1;
    char c;
    int top=0,ar[9],flag=0,k=0,nx;
    FILE *fs=fopen(in,"rb"),*ft;
    if(fs==NULL){
        printf("Cannot open %s\n",in);
        exit(0);
    }
    while(fread(&ch1,sizeof(ch1),1,fs)==1){
        if(ch1.b!=' '|| ch1.c!='-'|| ch1.d!='>'|| ch1.e!=' '){
            break;
        }
        k++;  
        while(1){
            c=getc(fs);
            if(c=='\n'){
            	break;
            }
            else{
                if(c=='0')
                ar[top++]=0;
                else if(c=='1')
                ar[top++]=1;
                else if(c=='\r'){
                	continue;
                }
                else{
                	flag=1;
                	break;
                }
            }
        }
        if(flag==1)
        break;
        c=ch1.a;
        insertom(&head,c,ar,top,0);
        top=0;
    }
    flag=0;
    long int posi=ftell(fs),capm;
    fseek(fs,-(sizeof(int)+sizeof(char)+sizeof(char)),SEEK_END);
    capm=ftell(fs);
    fseek(fs,-(sizeof(int)),SEEK_END);
    while(fread(&nx,sizeof(int),1,fs)==1)
    break;
    fseek(fs,posi,SEEK_SET);
    printf("Generating %s\n",out);
    ft=fopen(out,"w");
    if(ft==NULL){
        printf("Cannot open %s\n",out);
        exit(0);
    }
    if(k==0){
        fclose(ft);
        exit(0);
    }
    pointme=head;
    while(fread(&c, sizeof(char),1,fs)==1){
       posi=ftell(fs);
        if(flag==1){
            for(int i =7; i>= nx; i--){
                k=(c>>i)&1;
                if(k==0){
                    pointme=pointme->left;
                    if(pointme->left==NULL && pointme->right==NULL){
                        fprintf(ft,"%c",pointme->c);
                        pointme=head;
                    }
                }
                else if(k==1){
                    pointme=pointme->right;
                    if(pointme->left==NULL && pointme->right==NULL){
                        fprintf(ft,"%c",pointme->c);
                        pointme=head;
                    }
                }
            }
            break;
        }
        else{
            if(posi==capm)
            flag=1;
            for(int i =7; i>= 0; i--){
                k=(c>>i)&1;
                if(k==0){
                    pointme=pointme->left;
                    if(pointme->left==NULL && pointme->right==NULL){
                        fprintf(ft,"%c",pointme->c);
                        pointme=head;
                    }
                }
                else if(k==1){
                    pointme=pointme->right;
                    if(pointme->left==NULL && pointme->right==NULL){
                        fprintf(ft,"%c",pointme->c);
                        pointme=head;
                    }
                }
            }
        }
    }
    fclose(ft);
    fclose(fs);
}

int main(){
    int mode;
    char in[10],out[10];
    printf("Enter the input file \n");
    scanf("%s",in);
    printf("Enter the output file \n");
    scanf("%s",out);
    printf("Enter mode (0 for compression, 1 for decompression) \n");
    scanf("%d",&mode);
    if(mode==0){
        compress(in,out);
    }
    else{
        decompress(in,out);
    }
    return 0;
}
