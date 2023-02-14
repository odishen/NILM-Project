#ifndef LWM2M_H_
#define LWM2M_H_
#include "sys.h"

extern u8 learn_flag;
extern char learn_num; 
extern char learn_text;
extern u8 tran_flag;
extern u8 init_flag;
void BC35_Init(void);

void Cmd_MIPLNOTIFY_float( int resld, float val ); 
void Cmd_MIPLNOTIFY_integer( int resld, u8 val );
void Cmd_MIPLEXECUTERSP(void);
void Cmd_MIPLNOTIFY_OK( char learn_text, char learn_number);
void Cmd_MIPLNOTIFY_strInit( void ) ;
#endif
