#ifndef __DEBUG_H__
#define __DEBUG_H__
/*Comment/uncomment the following line to disable/enable debugging,
 OR define(or NOT) it in Makefile.
*/
//#define NDEBUG

#undef hoostar_dbg					/* undef it, just in case */

#ifndef NDEBUG
	#ifdef __KERNEL__
			/* This one if debugging is on, and kernel space */
	#define hoostar_dbg(fmt, args...) printk( KERN_ERR fmt, ## args)
	#else
			 /* This one for user space */
	#define hoostar_dbg(fmt, args...) fprintf(stderr, fmt, ## args)
	#endif
#else
	 /* not debugging: nothing */
	#define hoostar_dbg(fmt, args...) 
#endif

#endif
