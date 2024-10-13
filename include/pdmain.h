#ifndef PDMAIN_H
#define PDMAIN_H

#include "m_pd.h"
#include "s_stuff.h"
#include "m_imp.h"
#include "g_canvas.h"
#include "g_undo.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#define IOCHANS 1

#ifdef __cplusplus
extern "C"
{
#endif


/* Function prototypes */
void pd_init(void);
void glob_open(t_pd *ignore, t_symbol *name, t_symbol *dir, t_floatarg f);
void pdmain_print(const char *s);
void espd_printtimediff(void);
void trymem(int foo);
void pd_sendmsg(char *buf, int bufsize);
void canvas_start_dsp(void);
void pdmain_init(void);
void pdmain_tick(void);

/* Global externs */
extern float soundin[], soundout[];
extern t_class *glob_pdobject;

/* Utility functions for Pd */
void glob_init(void);
void g_array_setup(void);
void g_canvas_setup(void);
void g_guiconnect_setup(void);
void g_bang_setup(void);
void g_hradio_setup(void);
void g_hslider_setup(void);
void g_mycanvas_setup(void);
void g_numbox_setup(void);
void g_toggle_setup(void);
void g_vradio_setup(void);
void g_vslider_setup(void);
void g_vumeter_setup(void);
void g_io_setup(void);
void g_scalar_setup(void);
void g_template_setup(void);
void g_text_setup(void);
void g_traversal_setup(void);
void clone_setup(void);
void m_pd_setup(void);
void x_acoustics_setup(void);
void x_interface_setup(void);
void x_connective_setup(void);
void x_time_setup(void);
void x_arithmetic_setup(void);
void x_array_setup(void);
void x_midi_setup(void);
void x_misc_setup(void);
void x_net_setup(void);
void x_qlist_setup(void);
void x_gui_setup(void);
void x_list_setup(void);
void x_scalar_setup(void);
void expr_setup(void);
void d_arithmetic_setup(void);
void d_array_setup(void);
void d_ctl_setup(void);
void d_dac_setup(void);
void d_delay_setup(void);
void d_fft_setup(void);
void d_filter_setup(void);
void d_global_setup(void);
void d_math_setup(void);
void d_misc_setup(void);
void d_osc_setup(void);
void d_soundfile_setup(void);
void d_ugen_setup(void);

/* Compatibility level and settings */
//extern int pd_compatibilitylevel;
//extern int sys_verbose;
//extern int sys_noloadbang;

#ifdef __cplusplus
}
#endif


#endif /* PDMAIN_H */
