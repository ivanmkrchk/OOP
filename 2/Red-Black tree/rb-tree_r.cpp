// ������-������ ������
// (�) Proteus  (lawnmower-man@mail.ru)

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<memory.h>

//! ���������� (����� ������� ��� ��������� ��������)
struct stat_st{
	struct st{
		int max_count;		//!< ��������� ���������� ��������� �� ���
		int sum_count;		//!< �������� ����� ���������
		int sum_divider;	//!< ���������� ��������, ��� ���������� ��������
	};
	int max_depth;		//!< ������� ������������� ����
	int black_depth;	//!< ������ ������� ������
	int nodes_count;	//!< ���������� ������ � ������
	int turn_count;		//!< ���������� ���������� �� ��� ��������� (������� ���������)
	st insert;			//!< ���������� �������
	st remove;			//!< ���������� ��������
}stat;

// value - ��������
// p1,p2 - ����� ������ �����
// red   - ���� (true - ���� �������)
struct node_st{node_st *p1,*p2; int value; bool red;}; // ��������� ����

node_st *tree_root=0;	//!< ������

// ������� �������
void Show();
void Clear();
void Insert(int);
void Remove(int);


//===== ��������� ������� =======================================


//! ��������� ����� ������
node_st *NewItem(int value)
{
	stat.nodes_count++;
	node_st *node=new node_st;
	node->value=value;
	node->p1=node->p2=0;
	node->red=true;
	return node;
}

//! �������� �������
void DelItem(node_st *node)
{
	stat.nodes_count--;
	delete node;
}

//! ���� ������ (����������� �����)
void Clear(node_st *node)
{
	if(!node) return;
	Clear(node->p1);
	Clear(node->p2);
	DelItem(node);
}

//! ����� ������, ����������� �����
//! \param node ����
//! \param depth �������
//! \param dir   ������
//! \code Show(root,0,'*'); \endcode
void Show(node_st *node,int depth,char dir)
{
	int n;
	if(!node) return;
	for(n=0; n<depth; n++) putchar(' ');
	printf("%c[%d] (%s)\n",dir,node->value,node->red?"red":"black");
	Show(node->p1,depth+2,'-');
	Show(node->p2,depth+2,'+');
}


//! �������� �����
//! \param index ������ �������
//! \result ����� ������� ������
node_st *Rotate21(node_st *node)
{
	stat.turn_count++;
	node_st *p2 =node->p2;
	node_st *p21=p2->p1;
	p2->p1=node;
	node->p2=p21;
	return p2;
}

//! �������� ������
//! \param index ������ �������
//! \result ����� ������� ������
node_st *Rotate12(node_st *node)
{
	stat.turn_count++;
	node_st *p1 =node->p1;
	node_st *p12=p1->p2;
	p1->p2=node;
	node->p1=p12;
	return p1;
}


//! ������������ �������
void BalanceInsert(node_st **root)
{
	node_st *p1,*p2,*px1,*px2;
	node_st *node=*root;
	if(node->red) return;
	p1=node->p1;
	p2=node->p2;
	if(p1 && p1->red) {
		px2=p1->p2;				// ������ ����� ��� ����� ������� ������� �������
		if(px2 && px2->red) p1=node->p1=Rotate21(p1);
		px1=p1->p1;
		if(px1 && px1->red) {
			node->red=true;
			p1->red=false;
			if(p2 && p2->red) {	// ���������� ����������� ������
				px1->red=true;
				p2->red=false;
				return;
			}
			*root=Rotate12(node);
			//return;
		}
	}
	// ���� ����� � ������ �������
	if(p2 && p2->red) {
		px1=p2->p1;				// ������ ����� ��� ����� ������� ������� �������
		if(px1 && px1->red) p2=node->p2=Rotate12(p2);
		px2=p2->p2;
		if(px2 && px2->red) {
			node->red=true;
			p2->red=false;
			if(p1 && p1->red) {	// ���������� ����������� ������
				px2->red=true;
				p1->red=false;
				return;
			}
			*root=Rotate21(node);
			//return;
		}
	}
}



bool BalanceRemove1(node_st **root)
{
	node_st *node=*root;
	node_st *p1=node->p1;
	node_st *p2=node->p2;
	if(p1 && p1->red) {
		p1->red=false;return false;
	}
	if(p2 && p2->red) { // ������ 1
		node->red=true;
		p2->red=false;
		node=*root=Rotate21(node);
		if(BalanceRemove1(&node->p1)) node->p1->red=false;
		return false;
	}
 	unsigned int mask=0;
	node_st *p21=p2->p1;
	node_st *p22=p2->p2;
	if(p21 && p21->red) mask|=1;
	if(p22 && p22->red) mask|=2;
	switch(mask)
	{
	case 0:		// ������ 2 - if((!p21 || !p21->red) && (!p22 || !p22->red))
		p2->red=true;
		return true;
	case 1:
	case 3:		// ������ 3 - if(p21 && p21->red)
		p2->red=true;
		p21->red=false;
		p2=node->p2=Rotate12(p2);
		p22=p2->p2; 
	case 2:		// ������ 4 - if(p22 && p22->red)
		p2->red=node->red;
		p22->red=node->red=false;
		*root=Rotate21(node);
	}
	return false;
}

bool BalanceRemove2(node_st **root)
{
	node_st *node=*root;
	node_st *p1=node->p1;
	node_st *p2=node->p2;
	if(p2 && p2->red) {p2->red=false;return false;}
	if(p1 && p1->red) { // ������ 1
		node->red=true;	
		p1->red=false;
		node=*root=Rotate12(node);
		if(BalanceRemove2(&node->p2)) node->p2->red=false;
		return false;
	}
 	unsigned int mask=0;
	node_st *p11=p1->p1;
	node_st *p12=p1->p2;
	if(p11 && p11->red) mask|=1;
	if(p12 && p12->red) mask|=2;
	switch(mask) {
	case 0:		// ������ 2 - if((!p12 || !p12->red) && (!p11 || !p11->red))
		p1->red=true;
		return true;
	case 2:
	case 3:		// ������ 3 - if(p12 && p12->red)
		p1->red=true;
		p12->red=false;
		p1=node->p1=Rotate21(p1);
		p11=p1->p1;
	case 1:		// ������ 4 - if(p11 && p11->red)
		p1->red=node->red;
		p11->red=node->red=false;
		*root=Rotate12(node);
	}
	return false;
}


//! ����������� ����� �������
//! \result true ���� ��������� ������ ��� ������������ � ������ ������� �� �����
bool Insert(int value,node_st **root)
{
	node_st *node=*root;
	if(!node) *root=NewItem(value);
	else {
		if(value==node->value) return true; 
		if(Insert(value,value<node->value?&node->p1:&node->p2)) return true;
		BalanceInsert(root);
	}
	return false;
}


//! ����� � ������ ������������ ���� ���������
//! \param root ������ ������ � ������� ���� ����� �������
//! \retval res �������� ������� ��� �����
//! \result true ���� ����� ������
bool GetMin(node_st **root,node_st **res)
{
	node_st *node=*root;
	if(node->p1) {
		if(GetMin(&node->p1,res)) return BalanceRemove1(root);
	} else {
		*root=node->p2;
		*res=node;
		return !node->red;
	}
	return false;
}


//! ����������� ����� ��������
//! \result true ���� ����� ������
bool Remove(node_st **root,int value)
{
	node_st *t,*node=*root;
	if(!node) return false;
	if(node->value<value) {
		if(Remove(&node->p2,value))	return BalanceRemove2(root);
	} else if(node->value>value) {
		if(Remove(&node->p1,value))	return BalanceRemove1(root);
	} else {
		bool res;
		if(!node->p2) {
			*root=node->p1;
			res=!node->red;
			DelItem(node);
			return res;
		}
		res=GetMin(&node->p2,root);
		t=*root;
		t->red=node->red;
		t->p1=node->p1;
		t->p2=node->p2;
		DelItem(node);
		if(res) res=BalanceRemove2(root);
		return res;
	}
	return 0;
}


//===== ������� ������� =========================================

//! ���������� ���������� �������/��������
void UpdateStat(stat_st::st &item)
{
	if(item.max_count<stat.turn_count) item.max_count=stat.turn_count;
	item.sum_count+=stat.turn_count; item.sum_divider++;
}


//! ����� ������
void Show()
{
	printf("[tree]\n");
	Show(tree_root,0,'*');
}

//! ������� �������
void Insert(int value)
{
	stat.turn_count=0;
	Insert(value,&tree_root);
	if(tree_root) tree_root->red=false;
	UpdateStat(stat.insert);
}

//! �������� ����
void Remove(int value)
{
	stat.turn_count=0;
	Remove(&tree_root,value); 
	UpdateStat(stat.remove);
}

//! ���� ������
void Clear()
{
	Clear(tree_root);
	tree_root=0;
}


//===== �������� ================================================

// ���� ������
#define ERROR_STRUCT	1
#define ERROR_BALANCE	2

//! �������� ������ (����������� �����)
//! \param tree ������
//! \param b_d  ������� ������ �������
//! \param d    ������� ������� �������
//! \param h    ��������� ������ �������
//! \result 0 ��� ��� ������
int Check(node_st *tree,int b_d,int d)
{
	int n;
	if(!tree) {
		// ���������� ������ ������ �� ����� ���� ����������
		if(stat.max_depth<d) stat.max_depth=d;
		if(stat.black_depth<0) stat.black_depth=b_d;
		return stat.black_depth==b_d?0:ERROR_BALANCE;
	}
	node_st *p1=tree->p1;
	node_st *p2=tree->p2;
	// ������� ������� ������ ����� ������ ��������
	if(tree->red && (p1 && p1->red || p2 && p2->red)) return ERROR_STRUCT;
	if(p1 && tree->value<p1->value || p2 && tree->value>p2->value) return ERROR_STRUCT;
	if(!tree->red) b_d++;
	n=Check(p1,b_d,d+1); if(n) return n;
	return Check(p2,b_d,d+1);
}


//! �������� ������
int Check()
{
	int d= 0;
	stat.black_depth=-1;
	if(!tree_root) return 0;
	if(tree_root->red) return ERROR_STRUCT;
	return Check(tree_root,d,0);
}

//! ����� ������ � ������ �������� � �������� (����������� �����)
//! \param node  ������ ������
//! \param array ������ ��� ������
//! \param size  ������ �������
bool TreeWalk(node_st *node,bool *array,int size)
{
	if(!node) return false;
	int value=node->value;
	if(value<0 || value>=size || !array[value]) return true;
	array[value]=false;
	return TreeWalk(node->p1,array,size) || TreeWalk(node->p2,array,size);
}

//! ����� ������ � ������ �������� � ��������
//! \param array ������ ��� ������
//! \param size  ������ �������
bool TreeWalk(bool *array,int size)
{
	if(TreeWalk(tree_root,array,size)) return true;
	for(int n=0; n<size; n++) if(array[n]) return true;
	return false;
}


//================================================================

#define SIZE 1000	//!< ������ ������� �������



//! ����� ����������� �������� 
int main()
{
	int n,i;
	bool array[SIZE]; 
	srand(time(0));
	memset(&stat,0,sizeof(stat)); stat.black_depth=-1;
	memset(array,false,sizeof(array));
	for(n=0; n<SIZE*100; n++) {
		printf("pass: %d of %d\r",n+1,SIZE*100);
		i=rand()%SIZE; array[i]=true;  Insert(i);
		i=rand()%SIZE; array[i]=false; Remove(i);
	}
	putchar('\n');
	switch(Check()) {
	case ERROR_STRUCT:   printf("*** structure error\n\a"); break;
	case ERROR_BALANCE:  printf("*** balance error\n\a");   break;
	default:
		printf("insert - max turn count: %d\n",stat.insert.max_count);
		printf("insert - average turn count: %g\n",(double)stat.insert.sum_count/stat.insert.sum_divider);
		printf("remove - max turn count: %d\n",stat.remove.max_count);
		printf("remove - average turn count: %g\n",(double)stat.remove.sum_count/stat.remove.sum_divider);
		printf("nodes count: %d\n",stat.nodes_count);
		printf("black depth: %d\n",stat.black_depth);
		printf("max   depth: %d\n",stat.max_depth);
		break;
	}
	if(TreeWalk(array,SIZE)) printf("*** context error\n\a");
	Clear();
	getchar();
	return 0;
}



