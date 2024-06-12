/*
 * resource set indices for gemrunnr
 *
 * created by ORCS 2.18
 */

/*
 * Number of Strings:        14
 * Number of Bitblks:        0
 * Number of Iconblks:       0
 * Number of Color Iconblks: 0
 * Number of Color Icons:    0
 * Number of Tedinfos:       0
 * Number of Free Strings:   0
 * Number of Free Images:    0
 * Number of Objects:        21
 * Number of Trees:          1
 * Number of Userblks:       0
 * Number of Images:         0
 * Total file size:          744
 */

#undef RSC_NAME
#ifndef __ALCYON__
#define RSC_NAME "gemrunnr"
#endif
#undef RSC_ID
#ifdef gemrunnr
#define RSC_ID gemrunnr
#else
#define RSC_ID 0
#endif

#ifndef RSC_STATIC_FILE
# define RSC_STATIC_FILE 0
#endif
#if !RSC_STATIC_FILE
#define NUM_STRINGS 14
#define NUM_FRSTR 0
#define NUM_UD 0
#define NUM_IMAGES 0
#define NUM_BB 0
#define NUM_FRIMG 0
#define NUM_IB 0
#define NUM_CIB 0
#define NUM_TI 0
#define NUM_OBS 21
#define NUM_TREE 1
#endif



#define MAIN_MENU                          0 /* menu */
#define PROGRAM_ABOUT                      8 /* STRING in tree MAIN_MENU */
#define FILE_LOAD_TEST                    17 /* STRING in tree MAIN_MENU */
#define FILE_QUIT                         18 /* STRING in tree MAIN_MENU */
#define TEST_EXECUTE                      20 /* STRING in tree MAIN_MENU */




#ifdef __STDC__
#ifndef _WORD
#  ifdef WORD
#    define _WORD WORD
#  else
#    define _WORD short
#  endif
#endif
extern _WORD gemrunnr_rsc_load(_WORD wchar, _WORD hchar);
extern _WORD gemrunnr_rsc_gaddr(_WORD type, _WORD idx, void *gaddr);
extern _WORD gemrunnr_rsc_free(void);
#endif
