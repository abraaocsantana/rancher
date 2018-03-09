#include <curses.h>
#include <dirent.h>
#include <stdlib.h>
#include <limits.h>
#include <libgen.h>
#include <string.h>

//Test Tool

#define MAX_PATHNODES_ON_MEM_PER_AREA 100

#define FIRST_COL(maxCol) 0
#define SECOND_COL(maxCol) maxCol/8
#define THIRD_COL(maxCol) maxCol/2

#define FIRST_SECOND_COL_SEPARATOR(maxCol) (maxCol/8) - 1
#define SECOND_THIRD_COL_SEPARATOR(maxCol) (maxCol/2) - 1

int maxRow,maxCol;
int midCursor, leftCursor;

struct PathNode{
  char pathName[PATH_MAX];
  enum NodeType{
    Dir,
    File,
    Unknown
  } type;
};

struct PathNodesPerArea{
  int pathNodeCount;
  struct PathNode pathNodes[MAX_PATHNODES_ON_MEM_PER_AREA];
};

struct PathNodesPerArea midPathNodes;
struct PathNodesPerArea leftPathNodes;
struct PathNodesPerArea rightPathNodes;

char* lastPathName(struct PathNodesPerArea *nodeContainer){
  return nodeContainer->pathNodes[nodeContainer->pathNodeCount].pathName;
}

enum NodeType* lastPathType(struct PathNodesPerArea *nodeContainer){
  return &nodeContainer->pathNodes[nodeContainer->pathNodeCount].type;
}

void readPath(char pathName[], struct PathNodesPerArea *nodeContainer){
  nodeContainer->pathNodeCount=0;
  DIR		*d;
	struct dirent	*dir;
	d = opendir(pathName);
  if (d)
	{
    seekdir(d,2);
    while ((dir = readdir(d)) != NULL)
		{
      strcpy(lastPathName(nodeContainer),dir->d_name);
      switch (dir->d_type) {
        case DT_DIR:
          (*lastPathType(nodeContainer)) = Dir;
        break;
        case DT_REG:
          (*lastPathType(nodeContainer)) = File;
        break;
        default:
          (*lastPathType(nodeContainer)) = Unknown;
        break;
      }
      //printw("%i\n",nodeContainer->pathNodeCount);
      //printw("%i\n",(*lastPathType(nodeContainer)));
      //printw("%s",lastPathName(nodeContainer));
      ++nodeContainer->pathNodeCount;
    }
  }
}

void drawSeparator(int col){
	//init_pair(2,COLOR_RED,COLOR_BLACK);
	//attron(A_STANDOUT | COLOR_PAIR(2)/*| A_UNDERLINE*/);
	for(int i=0;i<maxRow;++i){
		mvaddch(i,col,' ');
	}
	//attroff(A_STANDOUT | COLOR_PAIR(2)/*| A_UNDERLINE*/);
}

void printDir(struct PathNodesPerArea* nodeContainer, int colStart, int cursor){
  init_pair(1,COLOR_BLUE,COLOR_BLACK);
  for (int i = 0; i < nodeContainer->pathNodeCount; ++i) {
    /*switch (nodeContainer.pathNodes[i]) {
      case Dir:
      attron(A_STANDOUT | COLOR_PAIR(1));
			mvprintw(i,colStart, dir->d_name);
			attroff(A_STANDOUT | COLOR_PAIR(1));
      break;
      case File:
      attron(A_STANDOUT | COLOR_PAIR(1));
			mvprintw(i,colStart, dir->d_name);
			attroff(A_STANDOUT | COLOR_PAIR(1));
      break;
    }*/
    if(i == cursor){
			attron(A_STANDOUT | COLOR_PAIR(1)/*| A_UNDERLINE*/);
			mvprintw(i,colStart,/*"%s",*/ nodeContainer->pathNodes[i].pathName);
			attroff(A_STANDOUT | COLOR_PAIR(1)/*| A_UNDERLINE*/);
		}else{
      mvprintw(i,colStart,/*"%s",*/ nodeContainer->pathNodes[i].pathName);
    }
	}
}

void draw(char *pathName){

  char leftPathName[PATH_MAX];
  strcpy(leftPathName,pathName);
  strcat(leftPathName,"/..");

  char righPathName[PATH_MAX];
  strcpy(righPathName,pathName);
  strcat(righPathName,"/");
  strcat(righPathName,midPathNodes.pathNodes[midCursor].pathName);

  readPath(leftPathName, &leftPathNodes);
  readPath(pathName, &midPathNodes);

  if(midPathNodes.pathNodes[midCursor].type == Dir){
    readPath(righPathName, &rightPathNodes);
  }else{
    rightPathNodes.pathNodeCount=0;
  }

  printDir(&leftPathNodes, FIRST_COL(maxCol),leftCursor);

	drawSeparator(FIRST_SECOND_COL_SEPARATOR(maxCol));

	printDir(&midPathNodes, SECOND_COL(maxCol) ,midCursor);

	drawSeparator(SECOND_THIRD_COL_SEPARATOR(maxCol));

	printDir(&rightPathNodes, THIRD_COL(maxCol) ,-1);
}

int main(){
  initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();

	getmaxyx(stdscr,maxRow,maxCol);

  init_pair(1,COLOR_BLUE,COLOR_BLACK);
  init_pair(2,COLOR_GREEN,COLOR_BLACK);

	start_color();

  midCursor=0;
  leftCursor=0;

  char currentInput;

  do{
    if(currentInput==3 && midCursor>0)
      --midCursor;
    else if(currentInput==2 && midCursor<midPathNodes.pathNodeCount-1)
      ++midCursor;
		else if(currentInput==4){

		}
		else if(currentInput==5){
			
		}
    draw(".");
		refresh();
    mvprintw(maxRow-1,0,"%i",midCursor);
    mvprintw(maxRow-2,0,"%i",currentInput);
    currentInput = getch();
	}while(currentInput != 27);

	endwin();

	return 0;
}
