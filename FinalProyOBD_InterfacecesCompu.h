/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: panelCB */
#define  PANEL_QUITBUTTON                 2       /* control type: command, callback function: QuitCallback */
#define  PANEL_GUI_IAT                    3       /* control type: numeric, callback function: (none) */
#define  PANEL_GUI_MAF                    4       /* control type: numeric, callback function: (none) */
#define  PANEL_GUI_FUELPRE                5       /* control type: scale, callback function: (none) */
#define  PANEL_GUI_MANIFOLD               6       /* control type: numeric, callback function: (none) */
#define  PANEL_GUI_THROTTLE               7       /* control type: numeric, callback function: (none) */
#define  PANEL_GUI_RPM                    8       /* control type: scale, callback function: (none) */
#define  PANEL_GUI_SPEED                  9       /* control type: scale, callback function: (none) */
#define  PANEL_TIMER                      10      /* control type: timer, callback function: TimerCallback */
#define  PANEL_GUI_LOAD                   11      /* control type: scale, callback function: (none) */
#define  PANEL_GUI_COOLANT                12      /* control type: scale, callback function: (none) */
#define  PANEL_MANIFOLD_ERROR             13      /* control type: textMsg, callback function: (none) */
#define  PANEL_FUELPRE_ERROR              14      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TimerCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
