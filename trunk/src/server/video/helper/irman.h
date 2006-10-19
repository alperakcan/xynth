
#if defined(VIDEO_HELPER_IRMAN)

int ir_init (char *filename);
int ir_finish (void);
unsigned char * ir_read_code (unsigned long timeout);
unsigned char * ir_get_code (void);
unsigned char * ir_get_code_time (unsigned long timeout);
unsigned char * ir_poll_code (void);
int ir_valid_code (char *text);
char * ir_code_to_text (unsigned char *code);
int ir_hex_to_int (unsigned char hex);
unsigned char * ir_text_to_code (char *text);
void ir_set_enabled (int val);
int ir_open_port (char *filename);
int ir_get_portfd (void);
int ir_close_port (void);
int ir_write_char (unsigned char data);
int ir_read_char (long timeout);
void ir_clear_buffer (void);
void ir_usleep (unsigned long usec);
int irman_init (char *port);
char * irman_getcode (void);
int irman_uninit (void);

#endif
