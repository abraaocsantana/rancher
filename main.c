#include <curses.h>
#include <dirent.h>

int main(){
	initscr();
	raw();

	int row,col;
	getmaxyx(stdscr,row,col);

	DIR           *d;
  	struct dirent *dir;
  	d = opendir("..");
	int count = 0;
  	if (d)
  	{
    		while ((dir = readdir(d)) != NULL)
    		{
      			mvprintw(count,0,"%s\n", dir->d_name);
			++count;
    		}

    		closedir(d);
  	}

	d = opendir(".");
	count=0;
  	if (d)
  	{
    		while ((dir = readdir(d)) != NULL)
    		{
      			mvprintw(count,col/2,"%s\n", dir->d_name);
			++count;
    		}

    		closedir(d);
  	}

	while(getch() != 27){
   	}
	endwin();
	return (0);
}
