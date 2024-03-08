#include<bits/stdc++.h>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int rnd(int l, int r)
{
    return l + rng() % (r - l + 1);
}

class Map
{
public:
    int **Layout;
    Map()
    {
        Layout = new int*[35];
        for(int i = 0; i < 35; ++i) Layout[i] = new int[55];
        //memset(Layout, -1, sizeof Layout);
    	/*for(int i=0;i<35;++i) 
    	{
    		for(int j=0;j<55;++j) cout << Layout[i][j] << ' ';
    			cout << '\n';
    	}
    	exit(0);*/
    }
    ~Map()
	{
    	for(int i = 0; i < 35; ++i)
        	delete[] Layout[i];
   		delete[] Layout;
	}

    void Generate()
    {
        int Rooms_Count,Rooms_Created=0;
        int Layer[3];
        int D[35][55];
        
        //Layout = new int*[35];
        //for(int i = 0; i < 35; ++i) Layout[i] = new int[55];
        //int Layout[35][55];
        for(int i = 0; i < 35; ++i) for(int j = 0; j < 55; ++j) Layout[i][j] = -1;
        int dx[]= {0,0,1,-1};
        int dy[]= {1,-1,0,0};
        Rooms_Count = rnd(5, 9);
        pair<int,int> Up_Left[Rooms_Count],Down_Right[Rooms_Count];
        Layer[0] = Rooms_Count/3;
        Layer[1] = Rooms_Count/3;
        Layer[2] = Rooms_Count/3;
        for(int i = 1; i <= Rooms_Count % 3; ++i)
        {
            ++Layer[rnd(0,2)];
        }
        
        function<void(int x,int y)> Match_Rooms;
        function<void(pair<int,int> End)> Bfs;
        function<void()> Fill,Print_Board;
        function<void(int x)> Generate_Rooms;
        function<bool(int x,int y)> Legal, inside;

        Print_Board = [&](){
			for(int i=0;i<35;++i){
				for(int j=0;j<55;++j){
					if(Layout[i][j]==-1) cout << ". ";
					else cout << Layout[i][j] << ' ';
				}
			cout << '\n';
			}
		};

        inside = [&](int x,int y)
        {
            if(x<0 || y<0 || x>=35 || y>=55) return false;
            return true;
        };

        Legal = [&](int y,int Room)
        {
            if(y==-1) return false;// Undecided

            if(y==1&&Up_Left[Room].first==0) return false;
            if(y==2&&Up_Left[Room].second==0) return false;
            if(y==3&&Down_Right[Room].first==29) return false;
            if(y==4&&Down_Right[Room].second==49) return false;

            // Out of map paths

            return true;
        };

        Fill = [&]()
        {
            int x,y,u,v;
            for(int i=0; i<Rooms_Count; ++i)
            {
                x=Up_Left[i].first,y=Up_Left[i].second,u=Down_Right[i].first,v=Down_Right[i].second;
                cout << x << ' ' << y << ' ' << u << ' ' << v << '\n';
                
                for(int j=x; j<=u; ++j)
                {
                    for(int k=y; k<=v; ++k)
                    {
                        Layout[j][k]=i+1;
                    }
                }
            }
            Print_Board();
        };
        for(int i=0;i<3;++i) cout << Layer[i] << '\n';
        Generate_Rooms = [&](int _Layer)
        {
            int End;
            End = 0;
            for(int i = 0; i < Layer[_Layer]; ++i)
            {
                int Rooms_behind, Length, Width;
                Rooms_behind=Layer[_Layer]-i;
                Length=rnd(3,9),Width=rnd(3,9);
                Down_Right[Rooms_Created]=make_pair(rnd((1+_Layer)*10-(10-Width),(1+_Layer)*10),
                	rnd(End+1+Length,49-Rooms_behind*10-1));

                cout << i << ' ' << (1+_Layer)*10-(10-Width) << ' ' << (1+_Layer)*10 << '\n'; 
                cout << i << ' ' << End+1+Length << ' ' << 49-Rooms_behind*10-1 << '\n'; 
                Up_Left[Rooms_Created]=make_pair(Down_Right[Rooms_Created].first-Width+1,
                                                 Down_Right[Rooms_Created].second-Length+1);
                cout << Up_Left[Rooms_Created].first << ' ' << Up_Left[Rooms_Created].second << '\n';
                End=Down_Right[Rooms_Created].second;
                ++Rooms_Created;

            }

        };
        Bfs = [&](pair<int,int> End)
        {
            for(int i = 0; i < 35; ++i) 
            	for(int j = 0 ; j < 55; ++j) D[i][j] = 1000000000;
            int oo=D[0][0];
            queue<pair<int,int>> q;
            q.push(End);
            D[End.first][End.second]=0;
            while(!q.empty())
            {
                pair<int,int> x=q.front();
                q.pop();
                for(int i=0; i<4; ++i)
                {
                    int X=x.first+dx[i],Y=x.second+dy[i];
                    if(!inside(X,Y)) continue;

                    if(D[X][Y]==oo)
                    {
                        D[X][Y]=D[x.first][x.second]+1;
                        q.push(make_pair(X,Y));
                    }
                }
            }
        };

        Match_Rooms = [&](int x,int y)
        {
        		
            int Side_x=-1,Side_y=-1,Square_x=0,Square_y=0;
           	//while(!Legal(Side_x,x))
            //{
                Side_x=rnd(1,4);
            //}
            //while(!Legal(Side_y,y))
            //{
                Side_y=rnd(1,4);
            //}
            pair<int,int> Start,End;
            
            if(Side_x&1)
            {
                Square_x=rnd(Up_Left[x].second,Down_Right[x].second);
                if(Side_x==1)
                {
                    Start=make_pair(Up_Left[x].first,Square_x);
                }
                else
                {
                    Start=make_pair(Down_Right[x].first,Square_x);
                }
            }
            else
            {
                Square_x=rnd(Up_Left[x].first,Down_Right[x].first);
                if(Side_x==2)
                {
                    Start=make_pair(Square_x,Up_Left[x].second);
                }
                else
                {
                    Start=make_pair(Square_x,Down_Right[x].second);
                }
            }

            if(Side_y&1)
            {
                Square_y=rnd(Up_Left[y].second,Down_Right[y].second);
                if(Side_y==1)
                {
                    End=make_pair(Up_Left[y].first,Square_y);
                }
                else
                {
                    End=make_pair(Down_Right[y].first,Square_y);
                }
            }
            else
            {
                Square_y=rnd(Up_Left[y].first,Down_Right[y].first);
                if(Side_y==2)
                {
                    End=make_pair(Square_y,Up_Left[y].second);
                }
                else
                {
                    End=make_pair(Square_y,Down_Right[y].second);
                }
            }
            
            Bfs(End);
            // Create path
            
            pair<int,int> Current=Start;
            pair<int,int> Previous_Square=make_pair(-1,-1);
            while(End!=Current)
            {
                int Quickest_Path,Actual_Path;
                for(int i=0; i<4; ++i)
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
                for(int i=0; i<4; ++i)
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

        };
        
        for(int i=0;i<3;++i) Generate_Rooms(i);
        cout << Rooms_Created << ' ' << "GAY" << '\n';

		Fill();
		//cout << "I'm ok, I'm fine.";exit(0);
		for(int i=1;i<Rooms_Count;++i) Match_Rooms(i-1,i);
		int Additional_Paths=rnd(0,3);
		cout << Additional_Paths << '\n';
		Print_Board();

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
		/*for(int i = 0; i < 35; ++i)
        	delete[] Layout[i];
   		delete[] Layout;*/
		
    }

};
int main()
{
	freopen("Maze.map","w",stdout);

	Map The_Map;

	The_Map.Generate();

    return 0;
}
