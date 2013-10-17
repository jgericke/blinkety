/* blinkety.c
   JB Gericke

   gcc blinkety.c -o blinkety
*/
#include <stdio.h>
#include <stdlib.h> 
#include <sys/ioctl.h> /* TIOCGWINSZ */
#include <unistd.h>    /* usleep */
#include <signal.h>    /* signal */
#include <time.h>      /* srand seed */

void normalize_console(int sig) {

     /* catch interrupt */
     signal(sig, SIG_IGN);
     /* \x1B[0m - normal colour
        \033[2J - clear console 
        \e[?25h - return cursor */
     printf("%s","\x1B[0m\033[2J\e[?25h");
     exit(0);
}

int return_rand(int limit) {
    /* return random integer between limit */
    return(rand() % limit);
}

int main(void) {

    static const char *colour_table[] = {	 
					"\x1B[31m", /* red */
					"\x1B[32m", /* green */
					"\x1B[33m", /* yellow */
					"\x1B[34m", /* blue */
					"\x1B[35m", /* magenta */
					"\x1B[36m", /* cyan */
					"\x1B[37m"  /* white */
				  };

    static const char *blinkety_table[] = {
					"☆",
					"☾",
					"★",
					"¸",
					".",
					"*"
				  };

    
    /* grab console rows/columns */
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    /* elements in our colour and blinkety tables */
    int ctbl_size = (sizeof(colour_table) / sizeof(colour_table[0])) - 1;
    int btbl_size = (sizeof(blinkety_table) / sizeof(blinkety_table[0])) - 1;
    /* seed srand */
    srand(time(0));
    /* clear console */
    printf("\033[2J");
    /* catch interrupt */
    signal(SIGINT, normalize_console);
    while (1) {
       /* \e[?25l - hide cursor 
          %d,%dH  - move cursor randomly on x/y
          %s%s    - set to random colour and blinkety char */
       printf("\e[?25l\033[%d;%dH%s%s\n", return_rand(w.ws_row), return_rand(w.ws_col), colour_table[return_rand(ctbl_size)], blinkety_table[return_rand(btbl_size)]); 
       usleep(10); /* No, YOU sleep! */
    }
    return(0);
}
