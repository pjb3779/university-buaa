#include <stdio.h>
#include <string.h>

#define MAXSTRLEN 41
#define INF 0x3f3f3f3f
#define MAXSTATNUM 500

typedef struct _Edge
{
	int adjseq;
	int weight;
	int lineseq;
	struct _Edge* next;
} Edge;

typedef struct _Station
{
	char name[MAXSTRLEN];
	int trans;
	int lineseq;
	int statseq;
	Edge* link;
} Station;

typedef struct _Line
{
	int size;
	int seq;
	int isloop;
	struct _Station* list;
} Line;

typedef struct _Railway
{
	int size;
	struct _Station list[MAXSTATNUM];
} Railway;

void insert_Edge(Station* head, int end, int lineseq)
{
	Edge* rear = head->link;
	Edge* newEdge = (Edge*)malloc(sizeof(Edge));
	if (newEdge == NULL)
	{
		printf("allocation failure");
		exit(1);
	}

	newEdge->weight = 1;
	newEdge->lineseq = lineseq;
	newEdge->adjseq = end;
	newEdge->next = NULL;
	if (rear == NULL)
	{
		head->link = newEdge;
	}
	else
	{
		while (rear->next != NULL) rear = rear->next; 
		rear->next = newEdge;					
	}
}

void input_Station(Railway* railway, Line line, int seq_inline, int seq_inrail)
{
	strcpy(railway->list[seq_inrail].name, line.list[seq_inline].name);
	railway->list[seq_inrail].trans = line.list[seq_inline].trans;
	railway->list[seq_inrail].lineseq = line.seq;
	railway->list[seq_inrail].statseq = seq_inrail;
	railway->list[seq_inrail].link = NULL;
	railway->size++;

	if (seq_inline == 0)
	{
		if (line.isloop)
		{
			insert_Edge(&(railway->list[seq_inrail]), line.list[1].statseq, line.seq);
			insert_Edge(&(railway->list[seq_inrail]), line.list[line.size - 2].statseq, line.seq);
		}
		else
		{
			insert_Edge(&(railway->list[seq_inrail]), line.list[1].statseq, line.seq);
		}
	}
	else if (seq_inline == line.size - 1 - line.isloop)
	{
		if (line.isloop)
		{
			insert_Edge(&(railway->list[seq_inrail]), line.list[0].statseq, line.seq);
			insert_Edge(&(railway->list[seq_inrail]), line.list[line.size - 3].statseq, line.seq);
		}
		else
		{
			insert_Edge(&(railway->list[seq_inrail]), line.list[line.size - 2].statseq, line.seq);
		}
	}
	else
	{
		insert_Edge(&(railway->list[seq_inrail]), line.list[seq_inline + 1].statseq, line.seq);
		insert_Edge(&(railway->list[seq_inrail]), line.list[seq_inline - 1].statseq, line.seq);
	}
}

void update_Station(Railway* railway, Line line, int seq_inline, int seq_inrail)
{
	if (seq_inline == 0)
	{
		if (line.isloop)
		{
			insert_Edge(&(railway->list[seq_inrail]), line.list[1].statseq, line.seq);
			insert_Edge(&(railway->list[seq_inrail]), line.list[line.size - 2].statseq, line.seq);
		}
		else
		{
			insert_Edge(&(railway->list[seq_inrail]), line.list[1].statseq, line.seq);
		}
	}
	else if (seq_inline == line.size - 1 - line.isloop)
	{
		if (line.isloop)
		{
			insert_Edge(&(railway->list[seq_inrail]), line.list[0].statseq, line.seq);
			insert_Edge(&(railway->list[seq_inrail]), line.list[line.size - 3].statseq, line.seq);
		}
		else
		{
			insert_Edge(&(railway->list[seq_inrail]), line.list[line.size - 2].statseq, line.seq);
		}
	}
	else
	{
		insert_Edge(&(railway->list[seq_inrail]), line.list[seq_inline + 1].statseq, line.seq);
		insert_Edge(&(railway->list[seq_inrail]), line.list[seq_inline - 1].statseq, line.seq);
	}
}

int in_Rail(Railway railway, char name[])
{
	int iter_rail;
	for (iter_rail = 0; iter_rail < railway.size; iter_rail++)
	{
		if (strcmp(railway.list[iter_rail].name, name) == 0)
			return iter_rail;
	}

	return -1; 
}


void input_Line(Railway* railway, Line line)
{
	int seq_inline, seq_inrail;
	if (railway->size == 0)
	{
		for (seq_inline = 0; seq_inline < line.size; seq_inline++)
		{
			railway->list[seq_inline].link = NULL;
			input_Station(railway, line, seq_inline, seq_inline);
		}
	}
	else
	{
		int seq_fromsize = railway->size;
		for (seq_inline = 0; seq_inline < line.size - line.isloop; seq_inline++)
		{
			seq_inrail = in_Rail(*railway, line.list[seq_inline].name);
			if (seq_inrail != -1)
			{
				update_Station(railway, line, seq_inline, seq_inrail);
			}
			else
			{
				input_Station(railway, line, seq_inline, seq_fromsize);
				seq_fromsize++;
			}
		}
	}
}

void process_Line(Railway* railway, Line* line)
{
	int seq_inline, seq_inrail;
	if (railway->list == NULL)
	{
		for (seq_inline = 0; seq_inline < line->size; seq_inline++)
			line->list[seq_inline].statseq = seq_inline;
	}
	else
	{
		int seq_fromsize = railway->size;
		for (seq_inline = 0; seq_inline < line->size; seq_inline++)
		{
			seq_inrail = in_Rail(*railway, line->list[seq_inline].name);
			if (seq_inrail != -1)
			{
				line->list[seq_inline].statseq = seq_inrail;
			}
			else
			{
				line->list[seq_inline].statseq = seq_fromsize;
				seq_fromsize++;
			}
		}
	}
}

void Dijkstra(Railway* railway, char startname[], char destname[], int path[])
{
	int start = in_Rail(*railway, startname);
	int dest = in_Rail(*railway, destname);
	int* dist = (int*)malloc(railway->size * sizeof(int));
	memset(dist, INF, railway->size * sizeof(int));
	dist[start] = 0;

	Edge* link = railway->list[start].link;
	while (link != NULL)
	{
		dist[link->adjseq] = link->weight;
		path[link->adjseq] = start;
		link = link->next;
	}

	while (dist[dest] == INF)
	{
		int min = INF, minstat = dest, iter;
		for (iter = 0; iter < railway->size; iter++)
		{
			if (dist[iter] != 0 && dist[iter] < min)
			{
				min = dist[iter];
				minstat = iter;
			}
		}
		link = railway->list[minstat].link;
		if (link == NULL) exit(1);
		while (link != NULL)
		{
			if (dist[minstat] + link->weight < dist[link->adjseq])
			{
				dist[link->adjseq] = dist[minstat] + link->weight;
				path[link->adjseq] = minstat;
			}
			link = link->next;
		}
		dist[minstat] = 0;
	}
	free(dist);
}

int get_Lineseq(Railway railway, int stat1, int stat2)
{
	Edge* link = railway.list[stat1].link;
	while (link != NULL)
	{
		if (link->adjseq == stat2)
			return link->lineseq;
		link = link->next;
	}
	return -1; 
}

void depict_Path(Railway* railway, int start, int dest, int path[])
{
	int* track = (int*)malloc(railway->size * sizeof(int));
	memset(track, INF, railway->size * sizeof(int));
	int iter = 0;
	while (dest != start)
	{
		track[iter++] = dest;
		dest = path[dest];
	}
	track[iter] = start;

	int i = 0, j = iter;
	while (i < j)
	{
		int tmp;
		tmp = track[j];
		track[j] = track[i];
		track[i] = tmp;
		i++, j--;
	}



	int currstats = 1;
	printf("%s-", railway->list[track[0]].name);
	for (i = 1; i < iter; i++)
	{
		if (get_Lineseq(*railway, railway->list[track[i - 1]].statseq, railway->list[track[i]].statseq)
			!= get_Lineseq(*railway, railway->list[track[i + 1]].statseq, railway->list[track[i]].statseq))
		{
			printf("%d(%d)-%s-", get_Lineseq(*railway, railway->list[track[i - 1]].statseq,
				railway->list[track[i]].statseq), currstats, railway->list[track[i]].name);
			currstats = 1;
		}
		else
		{
			currstats++;
		}
	}
	printf("%d(%d)-%s\n", get_Lineseq(*railway, railway->list[track[i - 1]].statseq,
		railway->list[track[i]].statseq), currstats, railway->list[track[iter]].name);
}

int main()
{
	FILE* fin;
	fin = fopen("bgstations.txt", "r");
	if (fin == NULL) exit(1);

	int linenum, lineseq, statnum, trans;
	int i, j;
	char statname[MAXSTRLEN];
	Railway* beijing = (Railway*)malloc(sizeof(Railway));
	beijing->size = 0;
	Line* newline = (Line*)malloc(sizeof(Line));
	fscanf(fin, "%d\n", &linenum);

	for (i = 1; i <= linenum; i++)
	{
		fscanf(fin, "%d %d\n", &lineseq, &statnum);
		newline->seq = lineseq;
		newline->size = statnum;
		newline->isloop = 0;
		newline->list = (Station*)malloc(statnum * sizeof(Station));
		for (j = 0; j < statnum; j++)
		{
			fscanf(fin, "%s %d\n", statname, &trans);
			newline->list[j].lineseq = i;
			strcpy(newline->list[j].name, statname);
			newline->list[j].trans = trans;
		}
		if (strcmp(newline->list[0].name, statname) == 0)
			newline->isloop = 1;
		process_Line(beijing, newline);
		input_Line(beijing, *newline);
		free(newline->list);
		fscanf(fin, "\n");
	}

	char startname[MAXSTRLEN], destname[MAXSTRLEN];
	scanf("%s", startname);
	scanf("%s", destname);
	int* path = (int*)malloc(beijing->size * sizeof(int));
	Dijkstra(beijing, startname, destname, path);

	depict_Path(beijing, in_Rail(*beijing, startname), in_Rail(*beijing, destname), path);

	free(newline);
	free(path);
	fclose(fin);
	return 0;
}



