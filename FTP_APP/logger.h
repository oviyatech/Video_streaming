
#define CONFIG_DEBUG

#ifdef CONFIG_DEBUG
#define PRINT_D(...) log_print(__FILE__, __LINE__, __VA_ARGS__ )
#endif



#ifdef CONFIG_FACTORY
#define PRINT_F(...) log_print(__FILE__, __LINE__, __VA_ARGS__ )
#endif

//#define CONFIG_FACTORY
