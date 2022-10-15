// https://pubs.opengroup.org/onlinepubs/7908799/xcurses/curses.h.html

#include <curses.h>
#include <stdlib.h>
#include <time.h>

// 8 colors for standard Curses
#define PALLETE_SIZE (COLOR_WHITE - COLOR_BLACK + 1)

// Estrutura com estado interno da aplica��o
typedef struct gameData
{
    int ultimaTecla;
    int treeSize;

} gameData;

//////////////////////////////////////////////////////////////////////
// Fun��es auxiliares para a biblioteca Curses
//////////////////////////////////////////////////////////////////////
void initScreen()
{
#ifdef XCURSES
    Xinitscr(argc, argv);
#else
    initscr();
#endif

    if (has_colors())
    {
        int bg = 0, fg = 0;

        start_color();

        for(bg = COLOR_BLACK; bg <= COLOR_WHITE; bg++)
            for(fg = COLOR_BLACK; fg <= COLOR_WHITE; fg++)
                init_pair(bg*PALLETE_SIZE + fg + 1, fg, bg); // color 0 is system default (reserved)
    }

    // Trata a tecla Enter como \n
    //nl();
    // Teclas digitadas pelo usu�rio n�o aparecem na tela
    noecho();
    // 0 para cursor invis�vel
    curs_set(0);
    // Define getch como non-blocking de acordo com o timeout abaixo
    nodelay(stdscr, TRUE);
    // Timeout em 0 determina getch como non-blocking, n�o espera entrada do usu�rio
    timeout(0);
    // Habilita teclas de function (F1, F2, ...), flechas, etc
    keypad(stdscr, TRUE);
}

void setColor(short int fg, short int bg, chtype attr)
{
    chtype color = COLOR_PAIR(bg*PALLETE_SIZE + fg + 1);
    color |= attr;
    attrset(color);
}


//////////////////////////////////////////////////////////////////////
// Fun��es da aplica��o
//////////////////////////////////////////////////////////////////////
void initGame(gameData * game)
{
    game->ultimaTecla = -1;
}

// Gerencia entradas do usu�rio e controla o estado interno em game
// H� diversas op��es de teclas na Curses, como KEY_UP, KEY_DOWN, etc.
void handleInputs(gameData * game)
{
    int entrada = getch();

    if(entrada != -1)
        game->ultimaTecla = entrada;

    switch(game->ultimaTecla)
    {
        case 'q':
        case 'Q':
            curs_set(1);
            endwin();
            exit(EXIT_SUCCESS);
            break;
        case KEY_UP:
            if (game->treeSize < 50)
            {
               game->treeSize++;
            }
            game->ultimaTecla = -1;
            break;
        case KEY_DOWN:
            if(game->treeSize > 1)
            {
                game->treeSize--;
            }
            game->ultimaTecla = -1;
            break;
        case KEY_RESIZE:
            // Finaliza a tela atual e cria uma nova
            endwin();
            initScreen();

            clear();
            refresh();

            break;
    }
}

void doUpdate(gameData * game)
{

}

void drawScreen(gameData * game)
{
   
    int i = 0, j = 0;
    clear();


    for(i = getmaxy(stdscr) - game->treeSize; i < getmaxy(stdscr); i++ )
    {
        for(j = getmaxx(stdscr)/2 - game->treeSize - i + 62 ; j < getmaxx(stdscr)/2 + game->treeSize + i - 61; j++)
        {
            setColor(COLOR_YELLOW, COLOR_GREEN, 0);
            mvaddch(i, j, '*');
            if (game->treeSize > 10)
            {
                setColor(COLOR_RED, COLOR_BLUE, 0);
                mvaddch(i, j, '*');
            }

            if (game->treeSize > 20)
            {
                setColor(COLOR_BLUE, COLOR_YELLOW, 0);
                mvaddch(i, j, '*');
            }

            if (game->treeSize > 30)
            {
                setColor(COLOR_WHITE, COLOR_RED, 0);
                mvaddch(i, j, '*');
            }
            if (game->treeSize > 40)
            {
                setColor(COLOR_CYAN, COLOR_WHITE, 0);
                mvaddch(i, j, '*');
            }
            if (game->treeSize == 50)
            {
                setColor(COLOR_YELLOW, COLOR_GREEN, 0);
                mvaddch(i, j, '*');
            }
        }
    }
    for(i = getmaxy(stdscr)-game->treeSize; i <= getmaxy(stdscr); i++)
    {
        setColor(COLOR_RED, COLOR_RED, 0);
        mvaddch(i, getmaxx(stdscr)/2,'|');
    }
    setColor(COLOR_WHITE,COLOR_YELLOW, 0);
    mvprintw((i-2)-game->treeSize, (j-1)-game->treeSize, "*");
    mvprintw((i-3)-game->treeSize, (j)-game->treeSize, "*");
    mvprintw((i-3)-game->treeSize, (j-2)-game->treeSize, "*");
    mvprintw((i-4)-game->treeSize, (j-1)-game->treeSize, "*");
    setColor(COLOR_WHITE,COLOR_CYAN, 0);
    mvprintw((i-6)-game->treeSize, (j-10)-game->treeSize, "XMAS tree in october!");
    if (game->treeSize > 10)
    {
	setColor(COLOR_RED, COLOR_WHITE,0);
	mvprintw((i-7)-game->treeSize, (j-21)-game->treeSize, "Jingle Bells");
    }
    if (game->treeSize > 20)
    {
	setColor(COLOR_WHITE, COLOR_RED, 0);
    	mvprintw((i-7)-game->treeSize,(j-21)-game->treeSize,"it's the xmas song");
	clear();
    }
    if (game->treeSize > 30)
    {
	setColor(COLOR_BLUE, COLOR_YELLOW, 0);
	mvprintw((i-7)-game->treeSize,(j-21)-game->treeSize, "I forgot the rest");
    }
    refresh();
}

int main(int argc, char *argv[])
{
    // estrutura com dados internos da aplica��o
    gameData game;

    // inicializa a tela pelo Curses e o estado inicial da aplica��o
    initScreen();
    initGame(&game);

    // La�o principal sem retorno, pode ser removido para exibi��o direta de informa��o na tela
	while(1)
    {
        // Gerencia entradas do usu�rio pelo teclado
        handleInputs(&game);

        // Gerencia l�gica da aplica��o
        doUpdate(&game);

        // Atualiza a tela
        drawScreen(&game);

        // Controla o FPS da aplica��o
        napms(10);
    }

    // Encerra a tela inicializada em initScreen
    endwin();

	return 0;
}
