// CLaY - TUI-based ssh client
// built from 2 diffrent OSs
// dLL44 on git, ya herd?

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ncurses.h>
#include "bombastic.h"
#include "libssh/libssh.h"

void recovermainwin(WINDOW* mainwin, int row, int col)
{
    box(mainwin, 0, 0);
    mvwhline(mainwin, row-5, 1, ACS_HLINE, col - 4 );
}

int main() {
    int row, col;
    bool running = true;
    
    // Initialize curses
    initscr();
    cbreak();
    getmaxyx(stdscr, row, col);
    printw(" CLaY - TUI-Based SSH Client :: DEBUG x=%d, y=%d ::", col, row);
    refresh();
    // newwin(numberOfRows, numberOfCols, startY, startX);
    WINDOW* mainwin = newwin(row - 2, col - 2, 1, 1);
    WINDOW* alertwin = newwin((row/2)-1, col/2, row/2, col/2);
    WINDOW* terminal = newwin(row/2, (col/2)-5,  row/2, col-col+1);
    
    if (mainwin == NULL) {
        printw("Error creating window!");
        refresh();
        getch();
        endwin();
        return 1;
    }

    box(mainwin, 0, 0);
    wrefresh(mainwin);
    
    mvwprintw(mainwin, 1,2, "Prev. Sessions");
    mvwvline(mainwin, 1, ((col/2)/2)-5, ACS_VLINE, row -5);
    mvwhline(mainwin, 2, 1, ACS_HLINE, col -4);
    mvwhline(mainwin, row-5, 1, ACS_HLINE, col - 4 );
    
    /**
     *  BINDINGS
     * ----------
     * ctrl - n = new session
     * ctrl - p = profile selection
     * ctrl - l = new profile
     * ctrl - m = del profile
     * ctrl - d = del prev session
     * ctrl - e = edit profile
     * ctrl - q = mv cursor to prev sessions
     * ctrl - t = mv cursor to ssh term
     * ctrl - h = show the list of bindings
    */
    wrefresh(mainwin);
    /**
     * todo
     *& add functionality
    */
    ssh_session session;
    int rc;
    
    while ( running )
    {
        int ch = getch();   
        switch (ch)
        {
            case CTRL('n'):
                char ip[256], user[256], passwd[256], port_str[6];
                int port;
                session = ssh_new();
                if ( session == NULL )
                {
                    mvwprintw(alertwin, 1, 1, "ERROR creating a new ssh sesh.");
                    wrefresh(alertwin);
                    getch();
                    endwin();
                    return 1;
                }
                box(alertwin, 0,0);
                mvwprintw(alertwin, 1, 1, "Enter Hostname/IP:");
                box(alertwin, 0,0);
                wgetstr(alertwin, ip);
                mvwprintw(alertwin, 2, 1, "Enter Port:");
                wgetnstr(alertwin, port_str, sizeof(port_str));
                port = atoi(port_str);
                box(alertwin, 0,0);
                mvwprintw(alertwin, 3, 1, "Enter Username:");
                wgetstr(alertwin, user);
                noecho();
                box(alertwin, 0,0);
                mvwprintw(alertwin,4,1, "Enter Password:");
                wgetstr(alertwin, passwd);
                echo();
                
                ssh_options_set(session, SSH_OPTIONS_HOST, ip); 
                ssh_options_set(session, SSH_OPTIONS_PORT, &port);               
                rc = ssh_connect(session);
                if ( rc != SSH_OK)
                {
                    mvwprintw(alertwin, 4, 1, "ERR coonecting to %s: %s", ip, ssh_get_error(session));
                    wrefresh(alertwin);
                    ssh_free(session);
                    continue;
                }
                rc = ssh_userauth_password(session, user, passwd);
                if ( rc != SSH_AUTH_SUCCESS)
                {
                    mvwprintw(alertwin, 4, 1, "ERR connecting with paswd: %s", ssh_get_error(session));
                    wrefresh(alertwin);
                    ssh_disconnect(session);
                    ssh_free(session);
                    continue;
                }
                
                mvwprintw(alertwin, 4, 1, "Complete authed!");
                wrefresh(alertwin);
                getch();
                wclear(alertwin);
                wrefresh(alertwin);
                recovermainwin(mainwin, row, col);
                wrefresh(mainwin);
                
                break;
            case CTRL('p'):
                // profile selection logic
                break;
            case CTRL('l'):
                break;
            case CTRL('m'):
                break;
            case CTRL('d'):
                break;
            case CTRL('e'):
                break;
            case CTRL('q'):
                break;
            case CTRL('t'):
                break;
            case CTRL('h'):
                box(alertwin, 0,0);
                mvwprintw(alertwin, 1, 1, "CLaY Bindings:");
                mvwprintw(alertwin, 2, 1, "<C-n> - new session");
                mvwprintw(alertwin, 3, 1, "<C-p> - profile selection");
                wrefresh(alertwin);
                getch();
                wclear(alertwin);
                wrefresh(alertwin);
                recovermainwin(mainwin, row, col);
                wrefresh(mainwin);
                break;
            case CTRL('c'):
                running = false; 
                break;
            case CTRL('g'):
                box(terminal, 0,0);
                wrefresh(terminal);
                getch();
                wclear(terminal);
                wrefresh(terminal);
            default: break;
        }
    }
    endwin();

    return 0;
}
