#ifndef APP_H
#define APP_H
#include "../Terminal/terminal.h"
#include "../Card/card.h"
#include"../Server/server.h"


typedef enum {
	QUIT,
	PROCEED
}En_userOptions_t;

void appStart(void);

#endif // !APP_H

