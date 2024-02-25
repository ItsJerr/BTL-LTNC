#include<bits/stdc++.h>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int rnd(int l, int r) {
	return l + rng() % (r - l + 1);
}


const int Rows=35,Columns=55;
// Actual numbers are 30 & 50
pair<int,int> Size=make_pair(Rows,Columns);
int Layout[Rows][Columns];
const int Rooms_Count=8;//5 6 7 8 9 ??
struct Room
{
	pair<int,int> Up_Left,Down_Right;
}a[Rooms_Count];

int Layer[3]; // Avoid room collisions
// Layer0 = 1-->10
// Layer1 = 11-->20
// Layer2 = 21-->30

int Rooms_Created=0;
void Generate_Rooms(int _Layer)
{
	int End=0;
	for(int i=0;i<Layer[_Layer];++i)
	{
		int Rooms_behind=Layer[_Layer]-i;
		int Length=rnd(3,9),Width=rnd(3,9);
		a[Rooms_Created].Down_Right=make_pair(rnd((1+_Layer)*10-(10-Width),(1+_Layer)*10),rnd(End+1+Length,50-Rooms_behind*10-1));
		a[Rooms_Created].Up_Left=make_pair(a[Rooms_Created].Down_Right.first-Width+1,a[Rooms_Created].Down_Right.second-Length+1);
		End=a[Rooms_Created].Down_Right.second;
		++Rooms_Created;
	}
	return;
}
// 1 --> Up
// 2 --> Left
// 3 --> Down
// 4 --> Right

bool Legal(int y,int Room)
{
	if(y==-1) return false;// Undecided

	if(y==1&&a[Room].Up_Left.first==1) return false;
	if(y==2&&a[Room].Up_Left.second==1) return false;
	if(y==3&&a[Room].Down_Right.first==30) return false;
	if(y==4&&a[Room].Down_Right.second==50) return false;

	// Out of map paths

	return true;
}

void Fill_board()
{
	memset(Layout,-1,sizeof Layout);
	int x,y,u,v;
	for(int i=0;i<Rooms_Count;++i)
	{
		x=a[i].Up_Left.first,y=a[i].Up_Left.second,u=a[i].Down_Right.first,v=a[i].Down_Right.second;
		for(int j=x;j<=u;++j){
			for(int k=y;k<=v;++k){
				Layout[j][k]=i+1;
			}
		}
	}
}

int D[Rows][Columns]; 
int dx[]={0,0,1,-1};
int dy[]={1,-1,0,0};
// Array used for path creating purposes

bool inside(int x,int y)
{
	if(x<1||y<1||x>30||y>50) return false;
	return true;
}
void Bfs(pair<int,int> End)
{
	memset(D,50,sizeof D);
	int oo=D[0][0];
	queue<pair<int,int>> q;
	q.push(End);
	D[End.first][End.second]=0;
	while(!q.empty())
	{
		pair<int,int> x=q.front();
		q.pop();
		for(int i=0;i<4;++i)
		{
			int X=x.first+dx[i],Y=x.second+dy[i];
			if(!inside(X,Y)) continue;
			//if(Layout[X][Y]>0) continue;
			if(D[X][Y]==oo) 
			{
				D[X][Y]=D[x.first][x.second]+1;
				q.push(make_pair(X,Y));
			}
		}
	}
}
void Match_Rooms(int x,int y)
{
	int Side_x=-1,Side_y=-1,Square_x=0,Square_y=0;
	while(!Legal(Side_x,x)){
		Side_x=rnd(1,4);
	}
	while(!Legal(Side_y,y)){
		Side_y=rnd(1,4);
	}
	pair<int,int> Start,End;

	if(Side_x&1) 
	{
		Square_x=rnd(a[x].Up_Left.second,a[x].Down_Right.second);
		if(Side_x==1){
			Start=make_pair(a[x].Up_Left.first,Square_x);
		}
		else {
			Start=make_pair(a[x].Down_Right.first,Square_x);
		}
	}
	else 
	{
		Square_x=rnd(a[x].Up_Left.first,a[x].Down_Right.first);
		if(Side_x==2){
			Start=make_pair(Square_x,a[x].Up_Left.second);
		}
		else{
			Start=make_pair(Square_x,a[x].Down_Right.second);
		}
	}

	if(Side_y&1)
	{
		Square_y=rnd(a[y].Up_Left.second,a[y].Down_Right.second);
		if(Side_y==1){
			End=make_pair(a[y].Up_Left.first,Square_y);
		}
		else {
			End=make_pair(a[y].Down_Right.first,Square_y);
		}
	}
	else 
	{
		Square_y=rnd(a[y].Up_Left.first,a[y].Down_Right.first);
		if(Side_y==2){
			End=make_pair(Square_y,a[y].Up_Left.second);
		}
		else{
			End=make_pair(Square_y,a[y].Down_Right.second);
		}
	}
	Bfs(End);
	// Create path
	pair<int,int> Current=Start;
	pair<int,int> Previous_Square=make_pair(-1,-1);
	while(End!=Current)
	{
		int Quickest_Path,Actual_Path;
		for(int i=0;i<4;++i)
		{
			int X=Current.first+dx[i],Y=Current.second+dy[i];
			if(!inside(X,Y)) continue;

			if(D[X][Y]==D[Current.first][Current.second]-1) 
			{
				Quickest_Path=i;
				break;
			}
		}
		vector<int> Other_Paths;
		for(int i=0;i<4;++i)
		{
			if(i==Quickest_Path) continue;
			int X=Current.first+dx[i],Y=Current.second+dy[i];
			if(!inside(X,Y)) continue;
			if(make_pair(X,Y)==Previous_Square) continue;
			Other_Paths.push_back(i);
		}
		int Random=rnd(1,100);
		if(Random>5||Other_Paths.empty()) Actual_Path=Quickest_Path;
		else Actual_Path=Other_Paths[rnd(0,(int)Other_Paths.size()-1)];
		int _X=Current.first+dx[Actual_Path],_Y=Current.second+dy[Actual_Path];
		Previous_Square=Current;
		Current=make_pair(_X,_Y);
		if(Layout[_X][_Y]==-1) Layout[_X][_Y]=0;
	}

	return;
}
void Print_Board()
{
	for(int i=1;i<=30;++i){
		for(int j=1;j<=50;++j){
			if(Layout[i][j]==-1) cout << ". ";
			else cout << Layout[i][j] << ' ';
		}
		cout << '\n';
	}
}
int main()
{
	freopen("Maze.map","w",stdout);

	for(int i=0;i<3;++i) Layer[i]=Rooms_Count/3;
	for(int i=1;i<=Rooms_Count%3;++i) Layer[rnd(0,2)]++;	

	for(int i=0;i<3;++i) Generate_Rooms(i);
	Fill_board();
	
	for(int i=1;i<Rooms_Count;++i) Match_Rooms(i-1,i);

	int Additional_Paths=rnd(0,3);
	while(Additional_Paths-->0)
	{
		int u=0,v=0;
		while(u==v)
		{
			u=rnd(0,Rooms_Count-1);
			v=rnd(0,Rooms_Count-1);
			Match_Rooms(u,v);
		}
	}
	

	Print_Board();

	return 0;
}