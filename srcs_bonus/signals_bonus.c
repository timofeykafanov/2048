
#include "../includes/wong_kar_wai.h"

int received_signal = 0;

void resize_handler(int sig)
{
	if (sig == SIGWINCH)
		received_signal = SIGWINCH;
}
