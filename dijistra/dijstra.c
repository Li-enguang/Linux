//#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>  
#include<stdlib.h>  

#define NOTEXIST -1
#define MAXVEX 100  
#define INFINITY 65535  
#define TRUE 1  
#define FALSE 0   
#define isalpha(a) ((((a)>='a')&&((a)<='z')) || (((a)>='A')&&((a)<='Z')))


int path[MAXVEX];//存路径
int dist[MAXVEX]; //存距离
int known[MAXVEX]; 

typedef struct Graph {
	char vex[MAXVEX]; //顶点
	int edge[MAXVEX][MAXVEX]; //有向边的权重
	int vex_num; //顶点数
	int edge_num; //边数
}Graph;

char read_char() 
{
	char ch;
	do {
		ch = getchar();
	} while (!isalpha(ch));
	return ch;
}

int get_pos(Graph g, char ch) //得到下标
{
	int i;
	for (i = 0; i < g.vex_num; i++) {
		if (g.vex[i] == ch)
			return i;
	}
	return -1;
}

void create_graph(Graph *g)
{
	int i, j, k;
	printf("请输入顶点数与边数:\n");
	scanf("%d%d", &g->vex_num, &g->edge_num);

	for (i = 0; i < g->vex_num; i++) {
		for (j = 0; j < g->vex_num; j++) {
			if (i == j) {
				g->edge[i][j] = 0;
			}
			else
				g->edge[i][j] = INFINITY;
		}
	}
	printf("请输入顶点信息:\n");
	for (i = 0; i < g->vex_num; i++) {
		g->vex[i] = read_char();
	}

	printf("请输入边的信息:\n");
	char c1, c2;
	int p1, p2, w;
	for (k = 0; k < g->edge_num; k++)
	{
		c1 = read_char();
		c2 = read_char();
		scanf("%d", &w);
		p1 = get_pos(*g, c1);
		p2 = get_pos(*g, c2);
		g->edge[p1][p2] = w;//有向边的权重  
	}
}

void init_graph(Graph *g, int start)
{
	int i;
	for (i = 0; i < g->vex_num; i++) {
		path[i] = NOTEXIST;
		known[i] = FALSE;
		dist[i] = INFINITY;
	}

	dist[start] = 0;
}
// // 遍历G.vexnum；每次找出一个顶点的最短路径。
int find_min(Graph g) 
{
	int min;
	min = INFINITY;
	int i;
	int index;
	for (i = 0; i < g.vex_num; i++) {
		if (known[i] == FALSE && dist[i] < min) {
			index = i;
			min = dist[i];
		}
	}
	if (min == INFINITY)
		return -1;
	else
		return index;
}
// 修正当前最短路径和前驱顶点
// 即，当已知"顶点k的最短路径"之后，更新"未获取最短路径的顶点的最短路径和前驱顶点"。
void Dijkstra(Graph g)
{
	int v;
	int w;
	while (1) {
		v = find_min(g); //从起点开始 第一次找到的一定是起点（dist【i】=0）
		if (v == NOTEXIST)
			break;
		known[v] = TRUE;  
		for (w = 0; w < g.vex_num; w++) {
			//已经在以求得集合中 就不用看这个w了 加入TRUE集合以后 就需要更新距离
			if (known[w] == FALSE && (dist[v] + g.edge[v][w] < dist[w])) { 
				dist[w] = dist[v] + g.edge[v][w];
				path[w] = v;
			}
		}
	}
}


void print_graph(Graph g){
	int i, j;
	printf("\n带权有向图如下：\n");
	for (i = 0; i < g.vex_num; i++) {
		for (j = 0; j < g.vex_num; j++) {
			if (g.edge[i][j] == INFINITY)
				printf("%5c", '*');
			else {
				printf("%5d", g.edge[i][j]);
			}
		}
		printf("\n");
	}
}

void print_path2(Graph g, int end)//这里 直接传递最后位置的索引 
{
	if (path[end] != -1) {
		print_path2(g, path[end]);
		printf("->");
	}
	printf("%c ", g.vex[end]);
}

int main()
{
	Graph g;
	int start, end;
	char c1;
	create_graph(&g);
	printf("请输入一个起始点:\n");
	c1 = read_char();
	start = get_pos(g, c1);
	init_graph(&g, start);
	Dijkstra(g);
	for (int i = 0; i < g.vex_num; ++i)
	{
		end = i;
		if (dist[end] == INFINITY)
			printf("\n %c -> %c 该两点间无路径\n",g.vex[start],g.vex[end]);
		else {
			printf("\n %c -> %c 最短路径为: ",g.vex[start], g.vex[end]);
			print_path2(g, end);
			printf("  最小花费 : %d\n", dist[end]);
		}
	}
	print_graph( g);
	return 0;
}
