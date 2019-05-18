#include "myReadKey.h"



int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{
	if (regime < 0 || regime > 1 || echo < 0 || echo > 1 || sigint < 0 || sigint > 1)
	return -1;

	rk_mytermsave();
	tcgetattr(0, &termStateDefault);
	termState = termStateDefault;

	if (regime == 1)
		termState.c_lflag |= ICANON;
	else
		termState.c_lflag &= ~ICANON;

	if (echo == 1)
		termState.c_lflag |= ECHO;
	else
		termState.c_lflag &= ~ECHO;

	if (sigint == 1)
		termState.c_lflag |= ISIG;
	else
		termState.c_lflag &= ~ISIG;

	termState.c_cc [VMIN] = vmin; //znaki
	termState.c_cc [VTIME] = vtime; //vremya

	tcsetattr(0, TCSANOW, &termState);

	return 0;
}

int rk_readkey(enum keys *key)
{
	char buffer;
	rk_mytermregime(0, 0, 1, 0, 0);
	read(1, &buffer, 1);
	switch(buffer)
	{
		case 'c':
			*key = IgnrCancel;
			break;
		case 'p':
			*key = Reset;
			break;
		case 'i':
			*key = Input;
			break;
		case 'q':
			*key = Quit;
			break;
		case 'l':
			*key = Load;
			break;
		case 's':
			*key = Save;
			break;
		case 'r':
			*key = Run;
			break;
		case 't':
			*key = sTep;
			break;
			case '\E':
				read(1, &buffer, 1);
				read(1, &buffer, 1);
				switch(buffer)
				{
					case 65:
						*key = up;
						break;
					case 66:
						*key = down;
						break;
					case 67:
						*key = right;
						break;
					case 68:
						*key = left;
						break;
					case '1':
						read(1, &buffer, 1);
						if(buffer == '5')
						{
							read(1, &buffer, 1);
							if (buffer == '~')
								*key = f5;
						}
						if(buffer == '7')
						{
							read(1, &buffer, 1);
							if(buffer == '~')
								*key = f6;
						}
						break;
					default:
						*key = incorrectkey;
				}
				break;
			default:
				*key = incorrectkey;
				break;
			}

	tcsetattr(0, TCSANOW, &termStateDefault); // vozvr k pervonachalnomu vidu
	return 0;
}

void rk_mytermsave()
{
	tcgetattr(0, &termStateSave);
}

void rk_mytermrestore()
{
	tcsetattr(0, TCSANOW, &termStateSave);
}
