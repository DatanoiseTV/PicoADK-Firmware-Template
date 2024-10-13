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


#ifdef __cplusplus
extern "C"
{
#endif


#define IOCHANS 1



void pd_init(void);
void glob_open(t_pd *ignore, t_symbol *name, t_symbol *dir, t_floatarg f);

void pdmain_print( const char *s);
void espd_printtimediff( void);
/*
sed command to prepare patch:
    sed 's/;$/;\\/' foo.pd | sed 's/#N //'
*/

#if 0
static const char patchfile[] = "\
canvas 274 279 752 643 12;\n\
#X obj 123 146 loadbang;\n\
#X obj 123 171 metro 1000;\n\
#X obj 123 196 print poodle;\n\
#X connect 0 0 1 0;\n\
#X connect 1 0 2 0;\n\
";
#endif
#if 0
static const char patchfile[] = "\
canvas 0 50 450 300 12;\n\
#X obj 190 104 loadbang;\n\
#X msg 190 129 \; pd dsp 1;\n\
#X obj 118 123 dac~ 1;\n\
#X obj 118 98 osc~ 440;\n\
#X connect 0 0 1 0;\n\
#X connect 3 0 2 0;\n\
";
#endif
#ifdef PD_INCLUDEPATCH
#include "testpatch.c"
#endif

void trymem(int foo)
{
#if 1
    int i;
    char msg[80];
    for (i = 1; i < 500; i++)
    {
        char *foo = malloc(i*1024);
        if (foo)
            free(foo);
        else break;
    }
    sprintf(msg, "%d max mem %dk\n", foo, i-1);
  //  pdmain_print(msg);
#endif
}

void pd_sendmsg(char *buf, int bufsize)
{
    static t_binbuf *b;
    if (!b)
        b = binbuf_new();
    binbuf_text(b, buf, bufsize);
    binbuf_eval(b, 0, 0, 0);
}

extern float soundin[], soundout[];
void  canvas_start_dsp( void);
void pdmain_init( void)
{
    /* sys_printhook = pdmain_print; */
    pd_init();
    STUFF->st_dacsr = sys_getsr();
    STUFF->st_soundout = soundout;
    STUFF->st_soundin = soundin;

#ifdef PD_INCLUDEPATCH
    {
        t_binbuf *b = binbuf_new();
        glob_setfilename(0, gensym("main-patch"), gensym("."));
        binbuf_text(b, patchfile, strlen(patchfile));
        binbuf_eval(b, &pd_canvasmaker, 0, 0);
        canvas_loadbang((t_canvas *)s__X.s_thing);
        vmess(s__X.s_thing, gensym("pop"), "i", 0);
        glob_setfilename(0, &s_, &s_);
        binbuf_free(b);
    }
#endif
}


void pdmain_tick( void)
{
    memset(soundout, 0, 64*sizeof(float));
    sched_tick();
}

/* ----------------- stuff to keep Pd happy -------------------- */

t_class *glob_pdobject;

void glob_foo(void *dummy, t_floatarg f);

static void glob_beginnew(void *dummy, t_symbol *pname, t_symbol *pdir)
{
    glob_setfilename(0, pname, pdir);
    pd_bind(&pd_canvasmaker, &s__N);
}

static void glob_endnew(void *dummy)
{
    pd_unbind(&pd_canvasmaker, &s__N);
    if ((t_canvas *)s__X.s_thing)
    {
        canvas_loadbang((t_canvas *)s__X.s_thing);
        vmess(s__X.s_thing, gensym("pop"), "i", 0);
    }
    glob_setfilename(0, &s_, &s_);
}

static void glob_close(void *dummy, t_symbol *pname)
{
    t_pd *c = pd_findbyclass(pname, canvas_class);
    if (c)
        pd_free(c);
 }


void glob_dsp(void *dummy, t_symbol *s, int argc, t_atom *argv);

void glob_init( void)
{
    glob_pdobject = class_new(gensym("pd"), 0, 0, sizeof(t_pd),
        CLASS_DEFAULT, A_NULL);
    class_addmethod(glob_pdobject, (t_method)glob_dsp, gensym("dsp"),
        A_GIMME, 0);
  //  class_addmethod(glob_pdobject, (t_method)glob_foo, gensym("foo"),
 //       A_DEFFLOAT, 0);
    class_addmethod(glob_pdobject, (t_method)glob_beginnew, gensym("begin-new"),
        A_SYMBOL, A_SYMBOL, 0);
    class_addmethod(glob_pdobject, (t_method)glob_close, gensym("close"),
        A_SYMBOL, 0);
    class_addmethod(glob_pdobject, (t_method)glob_endnew, gensym("end-new"),
        0);
    pd_bind(&glob_pdobject, gensym("pd"));
}

void g_array_setup(void);
void g_canvas_setup(void);
void g_guiconnect_setup(void);
/* iemlib */
void g_bang_setup(void);
void g_hradio_setup(void);
void g_hslider_setup(void);
void g_mycanvas_setup(void);
void g_numbox_setup(void);
void g_toggle_setup(void);
void g_vradio_setup(void);
void g_vslider_setup(void);
void g_vumeter_setup(void);
/* iemlib */
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

void conf_init(void)
{
    trymem(10);
    g_array_setup();
    g_canvas_setup();
    g_text_setup();
    x_time_setup();
    x_interface_setup();
    x_misc_setup();
    g_guiconnect_setup();
    g_scalar_setup();
    g_template_setup();
    m_pd_setup();
    x_acoustics_setup();
    x_interface_setup();
    x_connective_setup();
    x_time_setup();
    x_arithmetic_setup();
    x_array_setup();
    x_misc_setup();
    x_qlist_setup();
    x_gui_setup();
    x_list_setup();
    x_scalar_setup();
    d_global_setup();
//    d_soundfile_setup();
    d_ugen_setup();
    d_dac_setup();
    d_ctl_setup();
    d_osc_setup();
    d_arithmetic_setup();
    d_array_setup();
    clone_setup();
    d_delay_setup();
    d_filter_setup();
    d_math_setup();
    d_misc_setup();
    expr_setup();
    trymem(11);
}

/*
    g_traversal_setup();
*/

/* ------- STUBS that do nothing ------------- */
int sys_get_outchannels(void) {return(IOCHANS); }
int sys_get_inchannels(void) {return(IOCHANS); }
float sys_getsr( void) {return (48000);}
int sys_getblksize(void) { return (DEFDACBLKSIZE); }

int pd_compatibilitylevel = 100;
int sys_verbose = 0;
int sys_noloadbang = 0;

int audio_shouldkeepopen(void) { return (0);}
int audio_isopen( void) { return (1); }
void sys_reopen_audio ( void) { }
void sys_close_audio ( void) { }

t_symbol *sys_libdir = &s_;

void sys_vgui(const char *format, ...) {}
void sys_gui(const char *s) { }

int sys_havegui(void) {return (0);}
int sys_havetkproc(void) {return (0);}
int sys_pollgui( void) { return (0); }

void sys_lock(void) {}
void sys_unlock(void) {}
void pd_globallock(void) {}
void pd_globalunlock(void) {}
int sys_defaultfont = 1;
int sys_nearestfontsize(int fontsize) {return (1);}
int sys_noautopatch = 1;

void canvas_undo_cleardirty(t_canvas *x) {}

t_undo_action *canvas_undo_init(t_canvas *x) {return (0);}
t_undo_action *canvas_undo_add(t_canvas *x,
 t_undo_type type, const char *name, void *data) {return (0);}
void canvas_undo_undo(t_canvas *x) {}
void canvas_undo_redo(t_canvas *x) {}
void canvas_undo_rebranch(t_canvas *x) {}
void canvas_undo_check_canvas_pointers(t_canvas *x) {}
void canvas_undo_purge_abstraction_actions(t_canvas *x) {}
void canvas_undo_free(t_canvas *x) {}
void *canvas_undo_set_create(t_canvas *x) { return (0); }
void *canvas_undo_set_recreate(t_canvas *x,
    t_gobj *y, int old_pos) { return (0); }
void canvas_noundo(t_canvas *x) {}
void canvas_properties(t_canvas *x) {}
void pd_undo_set_objectstate(t_canvas *canvas, t_pd *x, t_symbol *s,
                                    int undo_argc, t_atom *undo_argv,
                                    int redo_argc, t_atom *redo_argv) {}

void glist_select(t_glist *x, t_gobj *y) {}
void glist_deselect(t_glist *x, t_gobj *y) {}
void glist_noselect(t_glist *x) {}
int glist_isselected(t_glist *x, t_gobj *g) {return (0);}
void glist_getnextxy(t_glist *gl, int *xpix, int *ypix) {*xpix = *ypix = 40;}
int glist_getindex(t_glist *x, t_gobj *y) {return (0);}

void canvas_create_editor(t_canvas *x) {}
void canvas_destroy_editor(t_canvas *x) {}
void canvas_editor_for_class(t_class *c) {}
void g_editor_setup( void) {}
void canvas_finderror(const void *error_object) {}
void canvas_setcursor(t_canvas *x, unsigned int cursornum) {}
void canvas_setgraph(t_glist *x, int flag, int nogoprect) {}
int canvas_hitbox(t_canvas *x, t_gobj *y, int xpos, int ypos,
    int *x1p, int *y1p, int *x2p, int *y2p) {return (0);}
void canvas_restoreconnections(t_canvas *x) {}
void canvas_reload(t_symbol *name, t_symbol *dir, t_glist *except) {}

void sys_queuegui(void *client, t_glist *glist, t_guicallbackfn f) {}
void sys_unqueuegui(void *client) {}
char sys_fontweight[10] = "no";
char sys_font[10] = "no";
int sys_hostfontsize(int fontsize, int zoom) { return (1);}
int sys_zoom_open = 1;
int sys_zoomfontwidth(int fontsize, int zoom, int worstcase) { return (1);}
int sys_zoomfontheight(int fontsize, int zoom, int worstcase) { return (1);}
int sys_load_lib(t_canvas *canvas, const char *classname) { return (0);}

void s_inter_newpdinstance( void) {}
void x_midi_newpdinstance( void) {}

t_symbol *iemgui_raute2dollar(t_symbol *s) {return(s);}
t_symbol *iemgui_dollar2raute(t_symbol *s) {return(s);}

/* --------------- m_sched.c -------------------- */
#define TIMEUNITPERMSEC (32. * 441.)
#define TIMEUNITPERSECOND (TIMEUNITPERMSEC * 1000.)
void dsp_tick(void);
static int sched_diddsp;
int sys_quit = 0;
void sched_init(void) {}


typedef void (*t_clockmethod)(void *client);

struct _clock
{
    double c_settime;       /* in TIMEUNITS; <0 if unset */
    void *c_owner;
    t_clockmethod c_fn;
    struct _clock *c_next;
    t_float c_unit;         /* >0 if in TIMEUNITS; <0 if in samples */
};

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

t_clock *clock_new(void *owner, t_method fn)
{
    t_clock *x = (t_clock *)getbytes(sizeof *x);
    x->c_settime = -1;
    x->c_owner = owner;
    x->c_fn = (t_clockmethod)fn;
    x->c_next = 0;
    x->c_unit = TIMEUNITPERMSEC;
    return (x);
}

void clock_unset(t_clock *x)
{
    if (x->c_settime >= 0)
    {
        if (x == pd_this->pd_clock_setlist)
            pd_this->pd_clock_setlist = x->c_next;
        else
        {
            t_clock *x2 = pd_this->pd_clock_setlist;
            while (x2->c_next != x) x2 = x2->c_next;
            x2->c_next = x->c_next;
        }
        x->c_settime = -1;
    }
}

    /* set the clock to call back at an absolute system time */
void clock_set(t_clock *x, double setticks)
{
    if (setticks < pd_this->pd_systime) setticks = pd_this->pd_systime;
    clock_unset(x);
    x->c_settime = setticks;
    if (pd_this->pd_clock_setlist &&
        pd_this->pd_clock_setlist->c_settime <= setticks)
    {
        t_clock *cbefore, *cafter;
        for (cbefore = pd_this->pd_clock_setlist,
            cafter = pd_this->pd_clock_setlist->c_next;
                cbefore; cbefore = cafter, cafter = cbefore->c_next)
        {
            if (!cafter || cafter->c_settime > setticks)
            {
                cbefore->c_next = x;
                x->c_next = cafter;
                return;
            }
        }
    }
    else x->c_next = pd_this->pd_clock_setlist, pd_this->pd_clock_setlist = x;
}

    /* set the clock to call back after a delay in msec */
void clock_delay(t_clock *x, double delaytime)
{
    clock_set(x, (x->c_unit > 0 ?
        pd_this->pd_systime + x->c_unit * delaytime :
            pd_this->pd_systime -
                (x->c_unit*(TIMEUNITPERSECOND/STUFF->st_dacsr)) * delaytime));
}

    /* set the time unit in msec or (if 'samps' is set) in samples.  This
    is flagged by setting c_unit negative.  If the clock is currently set,
    recalculate the delay based on the new unit and reschedule */
void clock_setunit(t_clock *x, double timeunit, int sampflag)
{
    double timeleft;
    if (timeunit <= 0)
        timeunit = 1;
    /* if no change, return to avoid truncation errors recalculating delay */
    if ((sampflag && (timeunit == -x->c_unit)) ||
        (!sampflag && (timeunit == x->c_unit * TIMEUNITPERMSEC)))
            return;

        /* figure out time left in the units we were in */
    timeleft = (x->c_settime < 0 ? -1 :
        (x->c_settime - pd_this->pd_systime)/((x->c_unit > 0)? x->c_unit :
            (x->c_unit*(TIMEUNITPERSECOND/STUFF->st_dacsr))));
    if (sampflag)
        x->c_unit = -timeunit;  /* negate to flag sample-based */
    else x->c_unit = timeunit * TIMEUNITPERMSEC;
    if (timeleft >= 0)  /* reschedule if already set */
        clock_delay(x, timeleft);
}

    /* get current logical time.  We don't specify what units this is in;
    use clock_gettimesince() to measure intervals from time of this call. */
double clock_getlogicaltime(void)
{
    return (pd_this->pd_systime);
}

    /* OBSOLETE (misleading) function name kept for compatibility */
double clock_getsystime(void) { return (pd_this->pd_systime); }

    /* elapsed time in milliseconds since the given system time */
double clock_gettimesince(double prevsystime)
{
    return ((pd_this->pd_systime - prevsystime)/TIMEUNITPERMSEC);
}

    /* elapsed time in units, ala clock_setunit(), since given system time */
double clock_gettimesincewithunits(double prevsystime,
    double units, int sampflag)
{
            /* If in samples, divide TIMEUNITPERSECOND/sys_dacsr first (at
            cost of an extra division) since it's probably an integer and if
            units == 1 and (sys_time - prevsystime) is an integer number of
            DSP ticks, the result will be exact. */
    if (sampflag)
        return ((pd_this->pd_systime - prevsystime)/
            ((TIMEUNITPERSECOND/STUFF->st_dacsr)*units));
    else return ((pd_this->pd_systime - prevsystime)/(TIMEUNITPERMSEC*units));
}

    /* what value the system clock will have after a delay */
double clock_getsystimeafter(double delaytime)
{
    return (pd_this->pd_systime + TIMEUNITPERMSEC * delaytime);
}

void clock_free(t_clock *x)
{
    clock_unset(x);
    freebytes(x, sizeof *x);
}
    /* take the scheduler forward one DSP tick, also handling clock timeouts */
void sched_tick(void)
{
    double next_sys_time = pd_this->pd_systime +
        (STUFF->st_schedblocksize/STUFF->st_dacsr) * TIMEUNITPERSECOND;
    int countdown = 5000;
    while (pd_this->pd_clock_setlist &&
        pd_this->pd_clock_setlist->c_settime < next_sys_time)
    {
        t_clock *c = pd_this->pd_clock_setlist;
        pd_this->pd_systime = c->c_settime;
        clock_unset(pd_this->pd_clock_setlist);
        outlet_setstacklim();
        (*c->c_fn)(c->c_owner);
        if (!countdown--)
        {
            countdown = 5000;
            sys_pollgui();
        }
        if (sys_quit)
            return;
    }
    pd_this->pd_systime = next_sys_time;
    dsp_tick();
    sched_diddsp++;
}

/* ------------------------ g_editor.c ---------------------- */

struct _instanceeditor
{
    t_binbuf *copy_binbuf;
    char *canvas_textcopybuf;
    int canvas_textcopybufsize;
    t_undofn canvas_undo_fn;         /* current undo function if any */
    int canvas_undo_whatnext;        /* whether we can now UNDO or REDO */
    void *canvas_undo_buf;           /* data private to the undo function */
    t_canvas *canvas_undo_canvas;    /* which canvas we can undo on */
    const char *canvas_undo_name;
    int canvas_undo_already_set_move;
    double canvas_upclicktime;
    int canvas_upx, canvas_upy;
    int canvas_find_index, canvas_find_wholeword;
    t_binbuf *canvas_findbuf;
    int paste_onset;
    t_canvas *paste_canvas;
    t_glist *canvas_last_glist;
    int canvas_last_glist_x, canvas_last_glist_y;
    t_canvas *canvas_cursorcanvaswas;
    unsigned int canvas_cursorwas;
};

extern t_class *text_class;

void canvas_startmotion(t_canvas *x)
{
    int xval, yval;
    if (!x->gl_editor) return;
    glist_getnextxy(x, &xval, &yval);
    if (xval == 0 && yval == 0) return;
    x->gl_editor->e_onmotion = MA_MOVE;
    x->gl_editor->e_xwas = xval;
    x->gl_editor->e_ywas = yval;
}

void g_editor_newpdinstance(void)
{
    EDITOR = getbytes(sizeof(*EDITOR));
        /* other stuff is null-checked but this needs to exist: */
    EDITOR->copy_binbuf = binbuf_new();
}

void g_editor_freepdinstance(void)
{
    if (EDITOR->copy_binbuf)
        binbuf_free(EDITOR->copy_binbuf);
    if (EDITOR->canvas_undo_buf)
    {
        if (!EDITOR->canvas_undo_fn)
            bug("g_editor_freepdinstance");
        else (*EDITOR->canvas_undo_fn)
            (EDITOR->canvas_undo_canvas, EDITOR->canvas_undo_buf, UNDO_FREE);
    }
    if (EDITOR->canvas_findbuf)
        binbuf_free(EDITOR->canvas_findbuf);
    freebytes(EDITOR, sizeof(*EDITOR));
}

void canvas_connect(t_canvas *x, t_floatarg fwhoout, t_floatarg foutno,
    t_floatarg fwhoin, t_floatarg finno)
{
    int whoout = fwhoout, outno = foutno, whoin = fwhoin, inno = finno;
    t_gobj *src = 0, *sink = 0;
    t_object *objsrc, *objsink;
    t_outconnect *oc;
    int nin = whoin, nout = whoout;
    if (EDITOR->paste_canvas == x) whoout += EDITOR->paste_onset,
        whoin += EDITOR->paste_onset;
    for (src = x->gl_list; whoout; src = src->g_next, whoout--)
        if (!src->g_next) {
            src = NULL;
            logpost(sink, PD_DEBUG, "cannot connect non-existing object");
            goto bad; /* bug fix thanks to Hannes */
        }
    for (sink = x->gl_list; whoin; sink = sink->g_next, whoin--)
        if (!sink->g_next) {
            sink = NULL;
            logpost(src, PD_DEBUG, "cannot connect to non-existing object");
            goto bad;
        }

        /* check they're both patchable objects */
    if (!(objsrc = pd_checkobject(&src->g_pd)) ||
        !(objsink = pd_checkobject(&sink->g_pd))) {
        logpost(src?src:sink, PD_DEBUG, "cannot connect unpatchable object");
        goto bad;
    }

        /* if object creation failed, make dummy inlets or outlets
           as needed */
    if (pd_class(&src->g_pd) == text_class && objsrc->te_type == T_OBJECT)
        while (outno >= obj_noutlets(objsrc))
            outlet_new(objsrc, 0);
    if (pd_class(&sink->g_pd) == text_class && objsink->te_type == T_OBJECT)
        while (inno >= obj_ninlets(objsink))
            inlet_new(objsink, &objsink->ob_pd, 0, 0);

    if (!(oc = obj_connect(objsrc, outno, objsink, inno))) goto bad;
    if (glist_isvisible(x) && x->gl_havewindow)
    {
        char tag[128];
        char*tags[] = {tag, "cord"};
        sprintf(tag, "l%p", oc);
        pdgui_vmess(0, "crr iiii ri rS",
            glist_getcanvas(x), "create", "line",
            0, 0, 0, 0,
            "-width", (obj_issignaloutlet(objsrc, outno) ? 2 : 1) * x->gl_zoom,
            "-tags", 2, tags);
        canvas_fixlinesfor(x, objsrc);
    }
    return;

bad:
    post("%s %d %d %d %d (%s->%s) connection failed",
        x->gl_name->s_name, nout, outno, nin, inno,
            (src? class_getname(pd_class(&src->g_pd)) : "???"),
            (sink? class_getname(pd_class(&sink->g_pd)) : "???"));
}


void canvas_vis(t_canvas *x, t_floatarg f) {}

/* ------------------- s_file.c ------------------------ */
#define DEBUG(x)

/* add a single item to a namelist.  If "allowdup" is true, duplicates
may be added; otherwise they're dropped.  */

t_namelist *namelist_append(t_namelist *listwas, const char *s, int allowdup)
{
    t_namelist *nl, *nl2;
    nl2 = (t_namelist *)(getbytes(sizeof(*nl)));
    nl2->nl_next = 0;
    nl2->nl_string = (char *)getbytes(strlen(s) + 1);
    strcpy(nl2->nl_string, s);
    sys_unbashfilename(nl2->nl_string, nl2->nl_string);
    if (!listwas)
        return (nl2);
    else
    {
        for (nl = listwas; ;)
        {
            if (!allowdup && !strcmp(nl->nl_string, s))
            {
                freebytes(nl2->nl_string, strlen(nl2->nl_string) + 1);
                return (listwas);
            }
            if (!nl->nl_next)
                break;
            nl = nl->nl_next;
        }
        nl->nl_next = nl2;
    }
    return (listwas);
}

void namelist_free(t_namelist *listwas)
{
    t_namelist *nl, *nl2;
    for (nl = listwas; nl; nl = nl2)
    {
        nl2 = nl->nl_next;
        t_freebytes(nl->nl_string, strlen(nl->nl_string) + 1);
        t_freebytes(nl, sizeof(*nl));
    }
}

    /* change '/' characters to the system's native file separator */
void sys_bashfilename(const char *from, char *to)
{
    char c;
    while ((c = *from++))
    {
#ifdef _WIN32
        if (c == '/') c = '\\';
#endif
        *to++ = c;
    }
    *to = 0;
}

    /* change the system's native file separator to '/' characters  */
void sys_unbashfilename(const char *from, char *to)
{
    char c;
    while ((c = *from++))
    {
#ifdef _WIN32
        if (c == '\\') c = '/';
#endif
        *to++ = c;
    }
    *to = 0;
}

/* test if path is absolute or relative, based on leading /, env vars, ~, etc */
int sys_isabsolutepath(const char *dir)
{
    if (dir[0] == '/' || dir[0] == '~'
#ifdef _WIN32
        || dir[0] == '%' || (dir[1] == ':' && dir[2] == '/')
#endif
        )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* expand env vars and ~ at the beginning of a path and make a copy to return */
void sys_expandpath(const char *from, char *to, int bufsize)
{
    if ((strlen(from) == 1 && from[0] == '~') || (strncmp(from,"~/", 2) == 0))
    {
#ifdef _WIN32
        const char *home = getenv("USERPROFILE");
#else
        const char *home = getenv("HOME");
#endif
        if (home)
        {
            strncpy(to, home, bufsize);
            to[bufsize-1] = 0;
            strncpy(to + strlen(to), from + 1, bufsize - strlen(to));
            to[bufsize-1] = 0;
        }
        else *to = 0;
    }
    else
    {
        strncpy(to, from, bufsize);
        to[bufsize-1] = 0;
    }
#ifdef _WIN32
    {
        char *buf = alloca(bufsize);
        ExpandEnvironmentStrings(to, buf, bufsize-1);
        buf[bufsize-1] = 0;
        strncpy(to, buf, bufsize);
        to[bufsize-1] = 0;
    }
#endif
}


int sys_open(const char *path, int oflag, ...)
{
    int fd;
    char pathbuf[MAXPDSTRING];
    sys_bashfilename(path, pathbuf);
    if (oflag & O_CREAT)
    {
        mode_t mode;
        int imode;
        va_list ap;
        va_start(ap, oflag);

        /* Mac compiler complains if we just set mode = va_arg ... so, even
        though we all know it's just an int, we explicitly va_arg to an int
        and then convert.
           -> http://www.mail-archive.com/bug-gnulib@gnu.org/msg14212.html
           -> http://bugs.debian.org/647345
        */

        imode = va_arg (ap, int);
        mode = (mode_t)imode;
        va_end(ap);
        fd = open(pathbuf, oflag, mode);
    }
    else
        fd = open(pathbuf, oflag);
    return fd;
}

FILE *sys_fopen(const char *filename, const char *mode)
{
  char namebuf[MAXPDSTRING];
  sys_bashfilename(filename, namebuf);
  return fopen(namebuf, mode);
}

   /* close a previously opened file
   this is needed on platforms where you cannot open/close resources
   across dll-boundaries, but we provide it for other platforms as well */
int sys_close(int fd)
{
    return close(fd);
}

int sys_fclose(FILE *stream)
{
    return fclose(stream);
}

int sys_usestdpath = 0;



    /* try to open a file in the directory "dir", named "name""ext",
    for reading.  "Name" may have slashes.  The directory is copied to
    "dirresult" which must be at least "size" bytes.  "nameresult" is set
    to point to the filename (copied elsewhere into the same buffer).
    The "bin" flag requests opening for binary (which only makes a difference
    on Windows). */

int sys_trytoopenit(const char *dir, const char *name, const char* ext,
    char *dirresult, char **nameresult, unsigned int size, int bin,
    int okgui)
{
    int fd;
    char buf[MAXPDSTRING];
    if (strlen(dir) + strlen(name) + strlen(ext) + 4 > size)
        return (-1);
    sys_expandpath(dir, buf, MAXPDSTRING);
    strcpy(dirresult, buf);
    if (*dirresult && dirresult[strlen(dirresult)-1] != '/')
        strcat(dirresult, "/");
    strcat(dirresult, name);
    strcat(dirresult, ext);

    DEBUG(post("looking for %s",dirresult));
        /* see if we can open the file for reading */
    if ((fd=sys_open(dirresult, O_RDONLY)) >= 0)
    {
            /* in unix, further check that it's not a directory */
#ifdef HAVE_UNISTD_H
        struct stat statbuf;
        int ok =  ((fstat(fd, &statbuf) >= 0) &&
            !S_ISDIR(statbuf.st_mode));
        if (!ok)
        {
            if (okgui)
                logpost(NULL, PD_VERBOSE, "tried %s; stat failed or directory",
                    dirresult);
            close (fd);
            fd = -1;
        }
        else
#endif
        {
            char *slash;
            if (okgui)
                logpost(NULL, PD_VERBOSE, "tried %s and succeeded", dirresult);
            sys_unbashfilename(dirresult, dirresult);
            slash = strrchr(dirresult, '/');
            if (slash)
            {
                *slash = 0;
                *nameresult = slash + 1;
            }
            else *nameresult = dirresult;

            return (fd);
        }
    }
    else
    {
        if (okgui)
            logpost(NULL, PD_VERBOSE, "tried %s and failed", dirresult);
    }
    return (-1);
}

    /* check if we were given an absolute pathname, if so try to open it
    and return 1 to signal the caller to cancel any path searches */
int sys_open_absolute(const char *name, const char* ext,
    char *dirresult, char **nameresult, unsigned int size, int bin, int *fdp,
    int okgui)
{
    if (sys_isabsolutepath(name))
    {
        char dirbuf[MAXPDSTRING], *z = strrchr(name, '/');
        int dirlen;
        if (!z)
            return (0);
        dirlen = (int)(z - name);
        if (dirlen > MAXPDSTRING-1)
            dirlen = MAXPDSTRING-1;
        strncpy(dirbuf, name, dirlen);
        dirbuf[dirlen] = 0;
        *fdp = sys_trytoopenit(dirbuf, name+(dirlen+1), ext,
            dirresult, nameresult, size, bin, 1);
        return (1);
    }
    else return (0);
}

int do_open_via_path(const char *dir, const char *name,
    const char *ext, char *dirresult, char **nameresult, unsigned int size,
    int bin, t_namelist *searchpath, int okgui)
{
    t_namelist *nl;
    int fd = -1;

        /* first check if "name" is absolute (and if so, try to open) */
    if (sys_open_absolute(name, ext, dirresult, nameresult, size, bin, &fd, 1))
        return (fd);

        /* otherwise "name" is relative; try the directory "dir" first. */
    if ((fd = sys_trytoopenit(dir, name, ext,
        dirresult, nameresult, size, bin, 1)) >= 0)
            return (fd);

        /* next go through the temp paths from the commandline */
    for (nl = STUFF->st_temppath; nl; nl = nl->nl_next)
        if ((fd = sys_trytoopenit(nl->nl_string, name, ext,
            dirresult, nameresult, size, bin, 1)) >= 0)
                return (fd);
        /* next look in built-in paths like "extra" */
    for (nl = searchpath; nl; nl = nl->nl_next)
        if ((fd = sys_trytoopenit(nl->nl_string, name, ext,
            dirresult, nameresult, size, bin, 1)) >= 0)
                return (fd);
        /* next look in built-in paths like "extra" */
    if (sys_usestdpath)
        for (nl = STUFF->st_staticpath; nl; nl = nl->nl_next)
            if ((fd = sys_trytoopenit(nl->nl_string, name, ext,
                dirresult, nameresult, size, bin, 1)) >= 0)
                    return (fd);

    *dirresult = 0;
    *nameresult = dirresult;
    return (-1);
}

    /* open via path, using the global search path. */
int open_via_path(const char *dir, const char *name, const char *ext,
    char *dirresult, char **nameresult, unsigned int size, int bin)
{
    return (do_open_via_path(dir, name, ext, dirresult, nameresult,
        size, bin, STUFF->st_searchpath, 1));
}

void open_via_helppath(const char *name, const char *dir) {}

/* --------------------- s_inter.c --------------- */

    /* get "real time" in seconds; take the
    first time we get called as a reference time of zero. */
double sys_getrealtime(void)
{
    static struct timeval then;
    struct timeval now;
    gettimeofday(&now, 0);
    if (then.tv_sec == 0 && then.tv_usec == 0) then = now;
    return ((now.tv_sec - then.tv_sec) +
        (1./1000000.) * (now.tv_usec - then.tv_usec));
}

#ifdef __cplusplus
}
#endif
