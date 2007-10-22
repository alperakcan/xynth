
typedef struct xd_widget_s {
	/** widget name */
	char *name;
	/** number of required args for widget */
	unsigned int nargs;
	/** widget function */
	int (*func) (int argc, char *argv[]);
	/** widget help */
	char *help;
} xd_widget_t;
