
add_library(PD STATIC
src/g_canvas.c
src/g_graph.c
src/g_text.c
src/g_rtext.c
src/g_array.c
src/g_template.c
src/g_io.c
src/g_scalar.c
src/g_guiconnect.c
src/g_readwrite.c
src/g_clone.c
src/m_pd.c
src/m_class.c
src/m_obj.c
src/m_atom.c
src/m_memory.c
src/m_binbuf.c
src/s_print.c
src/s_utf8.c
src/d_ugen.c
src/d_resample.c
#src/d_soundfile.c
#src/d_soundfile_wave.c
#src/d_soundfile_aiff.c
#src/d_soundfile_caf.c
#src/d_soundfile_next.c
src/d_global.c
src/d_osc.c
src/d_ctl.c
src/d_arithmetic.c
src/d_array.c
src/d_dac.c
src/d_delay.c
src/d_filter.c
src/d_math.c
src/d_misc.c
src/x_arithmetic.c
src/x_connective.c
src/x_interface.c
src/x_misc.c
src/x_time.c
src/x_acoustics.c
src/x_gui.c
src/x_array.c
src/x_scalar.c
src/x_text.c
src/x_list.c
src/x_vexp.c
src/x_vexp_fun.c
src/x_vexp_if.c
src/s_inter_gui.c

)

target_compile_definitions(PD PUBLIC
    PD_INTERNAL=1
    PD_HEADLESS=1
)

target_include_directories(PD PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
)

target_link_libraries(PD pico_stdlib pico_util_buffer pico_multicore hardware_pio hardware_dma)

