/********************************************
數字代表費洛蒙濃度
*******************************************/
#include <iostream>
#include <iomanip>          //setw
#include <windows.h>        //gotoxy, hidecursor, SetConsoleTextAttribute, SetWindowLong
#include <ctime>            //time
#include <conio.h>          //getch, kbhit
#include <cmath>            //abs
#include <fcntl.h>
#include <io.h>
#define SIZE 40
#define ANT 0               //2是三角形 1是箭頭 0是單一符號
#define YOUNG_COLOR 10
#define MID_COLOR 6
#define OLD_COLOR 12
#define FIND_COLOR 3
#define TEN_FOOD_COLOR 16
#define FIV_FOOD_COLOR 176
#define DEAD_COLOR 128
#define NONE_COLOR 240
#define BORN_COROR 51
#define HOME_COLOR 0
using namespace std;

void gotoxy(int x, int y);  //將游標移至指定座標
void hidecursor();          //隱藏游標
void output(int i,int j,int color,int ant,int life ,int pher);

int main(){
    int a, position_cycle=0, ant_cycle=0, food_cycle=0, x, y, d[4], direction, MAX=0, sidestep, skip, foodnum=0;
    char console_size[20], ant1[SIZE][SIZE], ant2[SIZE][SIZE], produce[SIZE][SIZE];
    int pheromone[SIZE][SIZE]={0}, color1[SIZE][SIZE]={0}, color2[SIZE][SIZE]={0},timeinterval1[SIZE][SIZE]={0},timeinterval2[SIZE][SIZE]={0}, search1[SIZE][SIZE]={0}, search2[SIZE][SIZE]={0};

    SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);  //prevent users from resizing and maximizing console window
    sprintf(console_size, "MODE %d,%d", 2*SIZE, SIZE);
    system(console_size);

    srand(time(NULL));

    if(ANT==2){
        _setmode(_fileno(stdout), _O_WTEXT);
    }

    for(int i=0; i<SIZE; i++){          //設定並輸出初始狀態
		for(int j=0; j<SIZE; j++){
            ant1[i][j]=' ';
            if(i>=SIZE/2-1 && i<=SIZE/2+1 && j>=SIZE/2-1 && j<=SIZE/2+1){
                produce[i][j]='x';
            }
            a=rand()%200;
            if((a==0 || a==1) && produce[i][j]!='x'){
                color1[i][j]=10;
            }else if((a==2|| a==3 || a==4) && produce[i][j]!='x' && i!=SIZE/2 && j!=SIZE/2){
                switch (position_cycle){
                    case 0:
                        ant1[i][j]='u';
                        break;
                    case 1:
                        ant1[i][j]='l';
                        break;
                    case 2:
                        ant1[i][j]='d';
                        break;
                    case 3:
                        ant1[i][j]='r';
                        break;
                }
                switch(rand()%3+1){
                    case 1:
                        search1[i][j]=1;
                        break;
                    case 2:
                        search1[i][j]=2;
                        break;
                    case 3:
                        search1[i][j]=3;
                        break;
                }
                if(position_cycle==3){
                    position_cycle=0;
                }else{
                    position_cycle++;
                }
            }
            output(i,j,color1[i][j],ant1[i][j],0, 0);
			color2[i][j]=color1[i][j], ant2[i][j]=ant1[i][j], search2[i][j]=search1[i][j];
		}
	}

	Countine:

    while(kbhit()==1){      //清除緩衝區
        getch();
    }

    system(console_size);

    while(kbhit()==0){      //按任意鍵暫停

		hidecursor();       //隱藏游標
		SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);  //prevent users from resizing and maximizing console window

		for(int i=0; i<SIZE; i++){
			for(int j=0; j<SIZE; j++){
				if(timeinterval2[i][j]>250){		//死亡螞蟻
					ant1[i][j]=' ';
					ant2[i][j]=' ';
					timeinterval2[i][j]=0;
					search2[i][j]=0;
					if(produce[i][j]!='x'){
                        color1[i][j]=-1;
                        color2[i][j]=-1;
					}
				}
                sidestep=1;
                if(((i==SIZE/2-1&&j==SIZE/2-1)||(i==SIZE/2-1&&j==SIZE/2+1)||(i==SIZE/2+1&&j==SIZE/2-1)||(i==SIZE/2+1&&j==SIZE/2+1)) && (ant1[i][j]=='L' || ant1[i][j]=='U'|| ant1[i][j]=='R'|| ant1[i][j]=='D')){   //找到食物的螞蟻回到巢
                    a=search1[i][j];   //新螞蟻採用跟回巢螞蟻一樣的移動策略
                    search2[i][j]=0;
                    ant2[i][j]=' ';
                    timeinterval2[i][j]=0;
                    skip=0;
                    do{     //產生新螞蟻
                        if(position_cycle==3){
                            position_cycle=0;
                        }else{
                            position_cycle++;
                        }
                        if(position_cycle==0 && ant2[SIZE/2-1][SIZE/2]==' '){
                            ant2[SIZE/2-1][SIZE/2]='u';
                            search2[SIZE/2-1][SIZE/2]=a;
                            break;
                        }else if(position_cycle==1 && ant2[SIZE/2][SIZE/2-1]==' '){
                            ant2[SIZE/2][SIZE/2-1]='l';
                            search2[SIZE/2][SIZE/2-1]=a;
                            break;
                        }else if(position_cycle==2 && ant2[SIZE/2+1][SIZE/2]==' '){
                            ant2[SIZE/2+1][SIZE/2]='d';
                            search2[SIZE/2+1][SIZE/2]=a;
                            break;
                        }else if(position_cycle==3 && ant2[SIZE/2][SIZE/2+1]==' '){
                            ant2[SIZE/2][SIZE/2+1]='r';
                            search2[SIZE/2][SIZE/2+1]=a;
                            break;
                        }
                        skip++;
                    }while(skip<3);
                    if(foodnum<=10){    //產生新食物
                    	if(food_cycle==9){
                        	food_cycle=0;
                        	do{
                            	x=rand()%SIZE;
                            	y=rand()%SIZE;
                        	}while(color2[x][y]!=0 || produce[x][y]=='x');
                        	color2[x][y]=10;
                    	}else{
                        	food_cycle++;
                    	}
                	}
                }else if((ant1[i][j]=='u' || ant1[i][j]=='d' || ant1[i][j]=='l' || ant1[i][j]=='r') && color1[i][j]!=0){       //螞蟻找到食物
                    if(i>SIZE/2){
                    	ant2[i][j]='U';
					}else if(i<SIZE/2){
						ant2[i][j]='D';
					}else if(i==SIZE/2 && j>SIZE/2){
                        ant2[i][j]='L';
					}else if(i==SIZE/2 && j<SIZE/2){
                        ant2[i][j]='R';
					}
					if(color1[i][j]>1){
                        pheromone[i][j]=(abs(i-SIZE/2)+abs(j-SIZE/2))*2;
					}
					if(color1[i][j]>0){
                        color2[i][j]=color1[i][j]-1;
					}else if(color1[i][j]<0){
                        color2[i][j]=color1[i][j]+1;
					}else{
                        color2[i][j]=color1[i][j];
					}
					timeinterval2[i][j]=timeinterval1[i][j]-50;
					if(timeinterval2[i][j]<0){
                        timeinterval2[i][j]=0;
					}
					search2[i][j]=search1[i][j];
                }else if((ant1[i][j]=='u' || ant1[i][j]=='d' || ant1[i][j]=='l' || ant1[i][j]=='r')){       //螞蟻沒有找到食物
                    if(timeinterval1[i][j]==80||timeinterval1[i][j]==180){   //換一種新的移動策略
                        do{
                            a=rand()%3+1;
                        }while(a==search1[i][j]);
                        search1[i][j]=a;
                    }
                    d[0]=0, d[1]=0, d[2]=0, d[3]=0, direction=-1;
                    for(int step=1;step<=4;step++){         //以周圍食物作為判斷依據
                    	for(int k=-step; k<=step; k++){
                    		if(k==4||k==-4){
                    			continue;
							}
                        	for(int l=-step; l<=step; l++){
                        		if(l==4||l==-4){
                    				continue;
								}
                    	        if(k+l==step || k+l==-step || k-l==step || k-l==-step){
                                	if(color1[i+k][j+l]!=0 && i+k>=0 && j+l>=0 && i+k<=SIZE-1 && j+l<=SIZE-1){
                                	    if(k<0&&k+l<0&&k-l<=0){
											d[0]++;
										}else if(k>0&&k+l>0&&k-l>=0){
											d[1]++;
										}else if(l<0&&k+l<=0&&k-l>0){
											d[2]++;
										}else if(l>0&&k+l>=0&&k-l<0){
											d[3]++;
										}
                                	}
                            	}
                        	}
                    	}
                    	if(d[0]!=0 || d[1]!=0 || d[2]!=0 || d[3]!=0){
                            MAX=0;
                            for(int k=0; k<4; k++){             //判斷螞蟻前進方向
                                if(d[k]>MAX){
                                    MAX=d[k];
                                }
                            }
                            do{
                                a=rand()%4;
                                if(d[a]==MAX){
                                    direction=a;
                                }
                            }while(d[a]!=MAX);
                            switch(direction){
                                case 0:
                                    if(ant2[i-1][j]==' ' && produce[i-1][j]!='x'){
                                        ant2[i-1][j]='u';
                                        sidestep=0;
                                        timeinterval2[i-1][j]=timeinterval1[i][j]+1;
                                        search2[i-1][j]=search1[i][j];
                                    }
                                    break;
                                case 1:
                                    if(ant2[i+1][j]==' ' && produce[i+1][j]!='x'){
                                        ant2[i+1][j]='d';
                                        sidestep=0;
                                        timeinterval2[i+1][j]=timeinterval1[i][j]+1;
                                        search2[i+1][j]=search1[i][j];
                                    }
                                    break;
                                case 2:
                                    if(ant2[i][j-1]==' ' && produce[i][j-1]!='x'){
                                        ant2[i][j-1]='l';
                                        sidestep=0;
                                        timeinterval2[i][j-1]=timeinterval1[i][j]+1;
                                        search2[i][j-1]=search1[i][j];
                                    }
                                    break;
                                case 3:
                                    if(ant2[i][j+1]==' ' && produce[i][j+1]!='x'){
                                        ant2[i][j+1]='r';
                                        sidestep=0;
                                        timeinterval2[i][j+1]=timeinterval1[i][j]+1;
                                        search2[i][j+1]=search1[i][j];
                                    }
                                    break;
                                default:
                                    break;
                            }
                            if(sidestep==0){
                                ant2[i][j]=' ';
                                timeinterval2[i][j]=0;
                                search2[i][j]=0;
                                goto Leave;
                            }
						}
                	}
                    d[0]=0, d[1]=0, d[2]=0, d[3]=0, direction=-1;
                    //以費洛蒙作為判斷依據
                    for(int k=-3; k<=3; k++){		//判斷周圍
                        for(int l=-3; l<=3; l++){
                            if((k+l<-4)||(k+l>4)||(k-l<-4)||(k-l>4)||(k==0&&l==0)){
                                continue;		//中心和某四角落跳過
                            }else if(pheromone[i+k][j+l]!=0 && i+k>=0 && j+l>=0 && i+k<=SIZE-1 && j+l<=SIZE-1){
                                if(k<0&&((l>-2&&k<-1)||l==0||l==-1)){
                                    d[0]+=pheromone[i+k][j+l];
                                }else if(k>0&&((l<2&&k>1)||l==0||l==1)){
                                    d[1]+=pheromone[i+k][j+l];
                                }else if(l<0&&((k<2&&l<-1)||k==0||k==1)){
                                    d[2]+=pheromone[i+k][j+l];
                                }else if(l>0&&((k>-2&&l>1)||k==0||k==-1)){
                                    d[3]+=pheromone[i+k][j+l];
                                }
                            }
                        }
                    }
                    if(d[0]!=0 || d[1]!=0 || d[2]!=0 || d[3]!=0){
                        MAX=0;
                        for(int k=0; k<4; k++){             //判斷螞蟻前進方向
                            if(d[k]>MAX){
                                MAX=d[k];
                            }
                        }
                        do{
                            a=rand()%4;
                            if(d[a]==MAX){
                                direction=a;
                            }
                        }while(d[a]!=MAX);
                        switch(direction){
                            case 0:
                                if(ant2[i-1][j]==' ' && produce[i-1][j]!='x'){
                                    ant2[i-1][j]='u';
                                    sidestep=0;
                                    timeinterval2[i-1][j]=timeinterval1[i][j]+1;
                                    search2[i-1][j]=search1[i][j];
                                }
                                break;
                            case 1:
                                if(ant2[i+1][j]==' ' && produce[i+1][j]!='x'){
                                    ant2[i+1][j]='d';
                                    sidestep=0;
                                    timeinterval2[i+1][j]=timeinterval1[i][j]+1;
                                    search2[i+1][j]=search1[i][j];
                                }
                                break;
                            case 2:
                                if(ant2[i][j-1]==' ' && produce[i][j-1]!='x'){
                                    ant2[i][j-1]='l';
                                    sidestep=0;
                                    timeinterval2[i][j-1]=timeinterval1[i][j]+1;
                                    search2[i][j-1]=search1[i][j];
                                }
                                break;
                            case 3:
                            	if(ant2[i][j+1]==' ' && produce[i][j+1]!='x'){
                                    ant2[i][j+1]='r';
                                    sidestep=0;
                                    timeinterval2[i][j+1]=timeinterval1[i][j]+1;
                                    search2[i][j+1]=search1[i][j];
                                }
                                break;
                            default:
                                break;
                        }
                        if(sidestep==0){
                        	ant2[i][j]=' ';
                        	timeinterval2[i][j]=0;
                        	search2[i][j]=0;
                            goto Leave;
                        }
					}
					d[0]=0, d[1]=0, d[2]=0, d[3]=0, direction=-1;
                    Leave:
                    if(d[0]==0 && d[1]==0 && d[2]==0 && d[3]==0){       //周圍沒有食物也沒有費洛蒙
                        if((ant2[i-1][j]==' ' && i!=0 && produce[i-1][j]!='x') || (ant2[i+1][j]==' ' && i!=SIZE-1 && produce[i+1][j]!='x') || (ant2[i][j-1]==' ' && j!=0 && produce[i][j-1]!='x') || (ant2[i][j+1]==' ' && j!=SIZE-1 && produce[i][j+1]!='x')){
                            if(search1[i][j]==3){           //每十五步就左轉或右轉
                                if(timeinterval1[i][j]%15==14){
                                    a=rand()%2+6;
                                }else{
                                    a=0;
                                }
                            }else if(search1[i][j]==2){      //碰到障礙物才轉彎
                                a=0;
                            }else if(search1[i][j]==1){      //隨機
                                a=rand()%10;
                            }
                            do{
                                switch (a){
                                    case 0:     //直走
                                    case 1:
                                    case 2:
                                    case 3:
                                    case 4:
                                        if(ant1[i][j]=='u' && ant2[i-1][j]==' ' && i!=0 && produce[i-1][j]!='x'){
                                            ant2[i-1][j]='u';
                                            sidestep=0;
                                            timeinterval2[i-1][j]=timeinterval1[i][j]+1;
                                            search2[i-1][j]=search1[i][j];
                                        }else if(ant1[i][j]=='d' && ant2[i+1][j]==' ' && i!=SIZE-1 && produce[i+1][j]!='x'){
                                            ant2[i+1][j]='d';
                                            sidestep=0;
                                            timeinterval2[i+1][j]=timeinterval1[i][j]+1;
                                            search2[i+1][j]=search1[i][j];
                                        }else if(ant1[i][j]=='l' && ant2[i][j-1]==' ' && j!=0 && produce[i][j-1]!='x'){
                                            ant2[i][j-1]='l';
                                            sidestep=0;
                                            timeinterval2[i][j-1]=timeinterval1[i][j]+1;
                                            search2[i][j-1]=search1[i][j];
                                        }else if(ant1[i][j]=='r' && ant2[i][j+1]==' ' && j!=SIZE-1 && produce[i][j+1]!='x'){
                                            ant2[i][j+1]='r';
                                            sidestep=0;
                                            timeinterval2[i][j+1]=timeinterval1[i][j]+1;
                                            search2[i][j+1]=search1[i][j];
                                        }else{
                                            a+=5;
                                        }
                                        break;
                                    case 5:     //右轉
                                    case 6:
                                        if(ant1[i][j]=='u' && ant2[i][j+1]==' ' && j!=SIZE-1 && produce[i][j+1]!='x'){
                                            ant2[i][j+1]='r';
                                            sidestep=0;
                                            timeinterval2[i][j+1]=timeinterval1[i][j]+1;
                                            search2[i][j+1]=search1[i][j];
                                        }else if(ant1[i][j]=='d' && ant2[i][j-1]==' ' && j!=0 && produce[i][j-1]!='x'){
                                            ant2[i][j-1]='l';
                                            sidestep=0;
                                            timeinterval2[i][j-1]=timeinterval1[i][j]+1;
                                            search2[i][j-1]=search1[i][j];
                                        }else if(ant1[i][j]=='l' && ant2[i-1][j]==' ' && i!=0 && produce[i-1][j]!='x'){
                                            ant2[i-1][j]='u';
                                            sidestep=0;
                                            timeinterval2[i-1][j]=timeinterval1[i][j]+1;
                                            search2[i-1][j]=search1[i][j];
                                        }else if(ant1[i][j]=='r' && ant2[i+1][j]==' ' && i!=SIZE-1 && produce[i+1][j]!='x'){
                                            ant2[i+1][j]='d';
                                            sidestep=0;
                                            timeinterval2[i+1][j]=timeinterval1[i][j]+1;
                                            search2[i+1][j]=search1[i][j];
                                        }
                                        break;
                                    case 7:     //左轉
                                    case 8:
                                        if(ant1[i][j]=='u' && ant2[i][j-1]==' ' && j!=0 && produce[i][j-1]!='x'){
                                            ant2[i][j-1]='l';
                                            sidestep=0;
                                            timeinterval2[i][j-1]=timeinterval1[i][j]+1;
                                            search2[i][j-1]=search1[i][j];
                                        }else if(ant1[i][j]=='d' && ant2[i][j+1]==' ' && j!=SIZE-1 && produce[i][j+1]!='x'){
                                            ant2[i][j+1]='r';
                                            sidestep=0;
                                            timeinterval2[i][j+1]=timeinterval1[i][j]+1;
                                            search2[i][j+1]=search1[i][j];
                                        }else if(ant1[i][j]=='l' && ant2[i+1][j]==' ' && i!=SIZE-1 && produce[i+1][j]!='x'){
                                            ant2[i+1][j]='d';
                                            sidestep=0;
                                            timeinterval2[i+1][j]=timeinterval1[i][j]+1;
                                            search2[i+1][j]=search1[i][j];
                                        }else if(ant1[i][j]=='r' && ant2[i-1][j]==' ' && i!=0 && produce[i-1][j]!='x'){
                                            ant2[i-1][j]='u';
                                            sidestep=0;
                                            timeinterval2[i-1][j]=timeinterval1[i][j]+1;
                                            search2[i-1][j]=search1[i][j];
                                        }
                                        break;
                                    case 9:     //後退
                                        if(ant1[i][j]=='u' && ant2[i+1][j]==' ' && i!=SIZE-1 && produce[i+1][j]!='x'){
                                            ant2[i+1][j]='d';
                                            sidestep=0;
                                            timeinterval2[i+1][j]=timeinterval1[i][j]+1;
                                            search2[i+1][j]=search1[i][j];
                                        }else if(ant1[i][j]=='d' && ant2[i-1][j]==' ' && i!=0 && produce[i-1][j]!='x'){
                                            ant2[i-1][j]='u';
                                            sidestep=0;
                                            timeinterval2[i-1][j]=timeinterval1[i][j]+1;
                                            search2[i-1][j]=search1[i][j];
                                        }else if(ant1[i][j]=='l' && ant2[i][j+1]==' ' && j!=SIZE-1 && produce[i][j+1]!='x'){
                                            ant2[i][j+1]='r';
                                            sidestep=0;
                                            timeinterval2[i][j+1]=timeinterval1[i][j]+1;
                                            search2[i][j+1]=search1[i][j];
                                        }else if(ant1[i][j]=='r' && ant2[i][j-1]==' ' && j!=0 && produce[i][j-1]!='x'){
                                            ant2[i][j-1]='l';
                                            sidestep=0;
                                            timeinterval2[i][j-1]=timeinterval1[i][j]+1;
                                            search2[i][j-1]=search1[i][j];
                                        }
                                        break;
                                }
                                a=rand()%10;        //如果原本的移動策略受到阻礙就隨機走
                            }while(sidestep==1);
                            ant2[i][j]=' ';
                            timeinterval2[i][j]=0;
                            search2[i][j]=0;
                        }else{
                            timeinterval2[i][j]=timeinterval1[i][j]+1;
                            search2[i][j]=search1[i][j];
                        }
                    }
                }else if(ant1[i][j]=='U'){      //正在回巢的螞蟻(向上走)
                    if((j==SIZE/2)||(i>=SIZE/2+1 && j>SIZE/2+1)){
                        if(ant2[i][j-1]==' '){
                            ant2[i][j-1]='L';
                            sidestep=0;
                            timeinterval2[i][j-1]=timeinterval1[i][j]+1;
                            search2[i][j-1]=search1[i][j];
                            if(pheromone[i][j]>0){
                                pheromone[i][j-1]=pheromone[i][j]-1;
                            }
                        }
                    }else if(i>=SIZE/2+1 && j<SIZE/2-1 ||(i==SIZE/2-1)){
                        if(ant2[i][j+1]==' '){
                            ant2[i][j+1]='R';
                            sidestep=0;
                            timeinterval2[i][j+1]=timeinterval1[i][j]+1;
                            search2[i][j+1]=search1[i][j];
                            if(pheromone[i][j]>0){
                                pheromone[i][j+1]=pheromone[i][j]-1;
                            }
                        }
                    }else if(ant2[i-1][j]==' '){
                        ant2[i-1][j]='U';
                        sidestep=0;
                        timeinterval2[i-1][j]=timeinterval1[i][j]+1;
                        search2[i-1][j]=search1[i][j];
                        if(pheromone[i][j]>0){
                            pheromone[i-1][j]=pheromone[i][j]-1;
                        }
                    }
                    if(sidestep==0){
                        ant2[i][j]=' ';
                        timeinterval2[i][j]=0;
                        search2[i][j]=0;
                        pheromone[i][j]++;
                    }else{
                        timeinterval2[i][j]=timeinterval1[i][j]+1;
                        search2[i][j]=search1[i][j];
					}
                }else if(ant1[i][j]=='D'){      //正在回巢的螞蟻(向下走)
                    if(i<=SIZE/2-1 && j>SIZE/2+1 ||(i==SIZE/2+1)){
                        if(ant2[i][j-1]==' '){
                            ant2[i][j-1]='L';
                            sidestep=0;
                            timeinterval2[i][j-1]=timeinterval1[i][j]+1;
                            search2[i][j-1]=search1[i][j];
                            if(pheromone[i][j]>0){
                                pheromone[i][j-1]=pheromone[i][j]-1;
                            }
                        }
                    }else if((j==SIZE/2)||(i<=SIZE/2-1 && j<SIZE/2-1)){
                        if(ant2[i][j+1]==' '){
                            ant2[i][j+1]='R';
                            sidestep=0;
                            timeinterval2[i][j+1]=timeinterval1[i][j]+1;
                            search2[i][j+1]=search1[i][j];
                            if(pheromone[i][j]>0){
                                pheromone[i][j+1]=pheromone[i][j]-1;
                            }
                        }
                    }else if(ant2[i+1][j]==' '){
                        ant2[i+1][j]='D';
                        sidestep=0;
                        timeinterval2[i+1][j]=timeinterval1[i][j]+1;
                        search2[i+1][j]=search1[i][j];
                        if(pheromone[i][j]>0){
                            pheromone[i+1][j]=pheromone[i][j]-1;
                        }
                    }
                    if(sidestep==0){
                        ant2[i][j]=' ';
                        timeinterval2[i][j]=0;
                        search2[i][j]=0;
                        pheromone[i][j]++;
                    }else{
                        timeinterval2[i][j]=timeinterval1[i][j]+1;
                        search2[i][j]=search1[i][j];
					}
                }else if(ant1[i][j]=='L'){      //正在回巢的螞蟻(向左走)
                	if(j>=SIZE/2+1 && i>SIZE/2+1 || (j==SIZE/2-1)){
                        if(ant2[i-1][j]==' '){
                            ant2[i-1][j]='U';
                            sidestep=0;
                            timeinterval2[i-1][j]=timeinterval1[i][j]+1;
                            search2[i-1][j]=search1[i][j];
                            if(pheromone[i][j]>0){
                                pheromone[i-1][j]=pheromone[i][j]-1;
                            }
                        }
                    }else if((i==SIZE/2)||(j>=SIZE/2+1 && i<SIZE/2-1)){
                        if(ant2[i+1][j]==' '){
                            ant2[i+1][j]='D';
                            sidestep=0;
                            timeinterval2[i+1][j]=timeinterval1[i][j]+1;
                            search2[i+1][j]=search1[i][j];
                            if(pheromone[i][j]>0){
                                pheromone[i+1][j]=pheromone[i][j]-1;
                            }
                        }
                    }else if(ant2[i][j-1]==' '){
                        ant2[i][j-1]='L';
                        sidestep=0;
                        timeinterval2[i][j-1]=timeinterval1[i][j]+1;
                        search2[i][j-1]=search1[i][j];
                        if(pheromone[i][j]>0){
                            pheromone[i][j-1]=pheromone[i][j]-1;
                        }
                    }
                    if(sidestep==0){
                        ant2[i][j]=' ';
                        timeinterval2[i][j]=0;
                        search2[i][j]=0;
                        pheromone[i][j]++;
                    }else{
                        timeinterval2[i][j]=timeinterval1[i][j]+1;
                        search2[i][j]=search1[i][j];
					}
                }else if(ant1[i][j]=='R'){      //正在回巢的螞蟻(向右走)
                	if((i==SIZE/2)||(j<=SIZE/2-1 && i>SIZE/2+1)){
                        if(ant2[i-1][j]==' '){
                            ant2[i-1][j]='U';
                            sidestep=0;
                            timeinterval2[i-1][j]=timeinterval1[i][j]+1;
                            search2[i-1][j]=search1[i][j];
                            if(pheromone[i][j]>0){
                                pheromone[i-1][j]=pheromone[i][j]-1;
                            }
                        }
                    }else if(j<=SIZE/2-1 && i<SIZE/2-1 || (j==SIZE/2+1)){
                        if(ant2[i+1][j]==' '){
                            ant2[i+1][j]='D';
                            sidestep=0;
                            timeinterval2[i+1][j]=timeinterval1[i][j]+1;
                            search2[i+1][j]=search1[i][j];
                            if(pheromone[i][j]>0){
                                pheromone[i+1][j]=pheromone[i][j]-1;
                            }
                        }
                    }else if(ant2[i][j+1]==' '){
                        ant2[i][j+1]='R';
                        sidestep=0;
                        timeinterval2[i][j+1]=timeinterval1[i][j]+1;
                        search2[i][j+1]=search1[i][j];
                        if(pheromone[i][j]>0){
                            pheromone[i][j+1]=pheromone[i][j]-1;
                        }
                    }
                    if(sidestep==0){
                        ant2[i][j]=' ';
                        timeinterval2[i][j]=0;
                        search2[i][j]=0;
                        pheromone[i][j]++;
                    }else{
                        timeinterval2[i][j]=timeinterval1[i][j]+1;
                        search2[i][j]=search1[i][j];
					}
                }
			}
		}
        foodnum=0;
        gotoxy(0, 0);

		for(int i=0; i<SIZE; i++){
            for(int j=0; j<SIZE; j++){
                if(pheromone[i][j]!=0){
                	pheromone[i][j]--;
				}
				if(pheromone[i][j]<=0 || produce[i][j]=='x'){
                    pheromone[i][j]=0;
				}
				if(color2[i][j]>0){
        			foodnum++;
				}
//				if(i==SIZE/2 && j==SIZE/2){
//                    cout << setw(2) << "#";
//                }else if(color2[i][j]!=0){        //輸出新狀態
//                    cout << setw(2) << "*";
//                }else if(ant2[i][j]!=' '){
//                    cout << setw(2) << "@";
//                }else if(pheromone[i][j]!=0){
//                    cout << setw(2) << pheromone[i][j];
//            	}else{
//                    cout << "  ";
//            	}
				output(i,j,color2[i][j],ant2[i][j],timeinterval2[i][j], pheromone[i][j]);    //輸出新狀態
				ant1[i][j]=ant2[i][j], color1[i][j]=color2[i][j], timeinterval1[i][j]=timeinterval2[i][j], search1[i][j]=search2[i][j];    //新狀態覆蓋舊狀態
            }
        }
	    //for(int i=0; i<100000000; i++);
    }

    getch();  //清除緩衝區

    while(1){
        if(kbhit()==1){     //按任意鍵繼續
            goto Countine;
        }
    }
    return 0;
}

void output(int i,int j,int color,int ant,int life, int pher){			//輸出畫面
    if(i==SIZE/2&&j==SIZE/2){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), HOME_COLOR);
	}else if(color>5){
	    if(ant=='L'||ant=='U'||ant=='D'||ant=='R'){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEN_FOOD_COLOR+FIND_COLOR);
	    }else if(life<81){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEN_FOOD_COLOR+YOUNG_COLOR);
		}else if(life<161){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEN_FOOD_COLOR+MID_COLOR);
		}else{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEN_FOOD_COLOR+OLD_COLOR);
		}
	}else if(color>0){
	    if(ant=='L'||ant=='U'||ant=='D'||ant=='R'){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FIV_FOOD_COLOR+FIND_COLOR);
	    }else if(life<81){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FIV_FOOD_COLOR+YOUNG_COLOR);
		}else if(life<161){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FIV_FOOD_COLOR+MID_COLOR);
		}else{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FIV_FOOD_COLOR+OLD_COLOR);
		}
	}else if(color<0){
	    if(ant=='L'||ant=='U'||ant=='D'||ant=='R'){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEAD_COLOR+FIND_COLOR);
	    }else if(life<81){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEAD_COLOR+YOUNG_COLOR);
		}else if(life<161){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEAD_COLOR+MID_COLOR);
		}else{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEAD_COLOR+OLD_COLOR);
		}
	}else if(color==0){
	    if(ant=='L'||ant=='U'||ant=='D'||ant=='R'){
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NONE_COLOR+FIND_COLOR);
	    }else if(life<81){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NONE_COLOR+YOUNG_COLOR);
		}else if(life<161){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NONE_COLOR+MID_COLOR);
		}else{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NONE_COLOR+OLD_COLOR);
		}
	}

	switch (ANT){
        case 0:
            switch (ant){
            	case ' ':
                	if(pher!=0 && color==0){
                    	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NONE_COLOR);
                    	cout << setw(2) << pher;
                	}else{
                	    cout << setw(2) << " ";
                	}
                	break;
            	default:
                	cout << setw(2) << "●";
                	break;
            }
            break;
        case 1:
            switch (ant){
                case 'u':
                case 'U':
                    cout << setw(2) << "↑";
                    break;
                case 'D':
                case 'd':
                    cout << setw(2) << "↓";
                    break;
                case 'L':
                case 'l':
                    cout << setw(2) << "←";
                    break;
                case 'R':
                case 'r':
                    cout << setw(2) << "→";
                    break;
                default:
                    if(pher!=0 && color==0){
                    	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NONE_COLOR);
                    	cout << setw(2) << pher;
                	}else{
                	    cout << setw(2) << " ";
                	}
                	break;
            }
            break;
        case 2:
            switch (ant){
                case 'u':
                case 'U':
                    wcout << L"\U000025B2";
                    break;
                case 'D':
                case 'd':
                    wcout << L"\U000025BC";
                    break;
                case 'L':
                case 'l':
                    wcout << L"\U000025C0";
                    break;
                case 'R':
                case 'r':
                    wcout << L"\U000025B6";
                    break;
                default:
                    wcout << setw(2) << " ";
                    break;
            }
            break;
	}
}

void gotoxy(int x, int y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hConsole, pos);
} //http://www.cplusplus.com/forum/beginner/28859/

void hidecursor() {
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 1;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
} //https://stackoverflow.com/questions/30126490/how-to-hide-console-cursor-in-c
