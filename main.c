#include <curses.h>
#include <dirent.h>
#include <stdlib.h>
#include <limits.h>
#include <libgen.h>

int maxRow,maxCol;

void drawSeparator(int col){
	//init_pair(2,COLOR_RED,COLOR_BLACK);
	//attron(A_STANDOUT | COLOR_PAIR(2)/*| A_UNDERLINE*/);
	for(int i=0;i<maxRow;++i){
		mvaddch(i,col,' ');
	}
	//attroff(A_STANDOUT | COLOR_PAIR(2)/*| A_UNDERLINE*/);
}

void printDir(char path[], int colStart, int cursor){
	DIR		*d;
	struct dirent	*dir;
	d = opendir(path);
	int count = 0;
	if (d)
	{
		init_pair(1,COLOR_BLUE,COLOR_BLACK);
		seekdir(d,2);
		while ((dir = readdir(d)) != NULL)
		{
			if(count == cursor){
				attron(A_STANDOUT | COLOR_PAIR(1)/*| A_UNDERLINE*/);
				mvprintw(count,colStart,"%s", dir->d_name);
				attroff(A_STANDOUT | COLOR_PAIR(1)/*| A_UNDERLINE*/);
			}else
				mvprintw(count,colStart,"%s", dir->d_name);
			++count;
		}

		closedir(d);
	}
}

int main(){
	initscr();
	start_color();
	raw();

	getmaxyx(stdscr,maxRow,maxCol);

	int cursor=0;
	int leftCursor=0;

	printDir("..", 0,leftCursor);

	drawSeparator(maxCol/8);

	printDir(".", maxCol/8 + 1 ,cursor);

	drawSeparator(maxCol/2);

	printDir(".", maxCol/2 + 1 ,cursor);

	char buf[PATH_MAX];
	mvprintw(maxRow-1,10,"%s", realpath(".", buf));

	while(getch() != 27){
	}
	endwin();
	return (0);
}
