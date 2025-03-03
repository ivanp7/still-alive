#!/usr/bin/env python

import archi
import ctypes as c
import sys

##############################
# Values that can be changed #
##############################

NUM_THREADS = 4 # number of rendering threads

WINDOW_WIDTH_SCALE  = 2 # number of window pixels per texture pixels, can be fractional
WINDOW_HEIGHT_SCALE = WINDOW_WIDTH_SCALE

WINDOW_TITLE = "Still Alive"

#############################
# Construct the application #
#############################

app = archi.Application()

# Add signals to the watch set
app.watch_signal('SIGINT')  # the song handles Ctrl+C
app.watch_signal('SIGQUIT') # the song handles Ctrl+\
app.watch_signal('SIGTERM') # the song handles termination
app.watch_signal('SIGCONT') # the song handles Ctrl+Z

# Load plugin libraries
app.load_library('libsdl', 'libarchi-sdl.so')     # load the plugin for SDL support
app.load_library('still-alive', 'still-alive.so') # load the song plugin

# Register context interfaces
app.register_interface('sdl_library', 'plugin_sdl_library_interface', 'libsdl')       # register the SDL library interface
app.register_interface('sdl_window', 'plugin_sdl_window_context_interface', 'libsdl') # register the SDL window context interface
app.register_interface('glados', 'glados_interface', 'still-alive')                   # register the song interface

# Obtain context interfaces
value_list_interface = app.value_list_interface()

thread_group_interface = app.interface(archi.THREAD_GROUP_INTERFACE)
sdl_library_interface = app.interface('sdl_library')
sdl_window_interface = app.interface('sdl_window')
glados_interface = app.interface('glados')

# Obtain built-in contexts
app_signal = app.context(archi.APP_SIGNAL_CONTEXT) # obtain the application signal management context
app_fsm = app.context(archi.APP_FSM_CONTEXT) # obtain the application finite state machine

# Create contexts
thread_group = thread_group_interface(
        'thread_group', # a group of threads for multicore rendering
        num_threads=archi.Value(c.c_size_t(NUM_THREADS), type=archi.VALUE_UINT)) # number of threads in the group

sdl_library = sdl_library_interface(
        'sdl_library', # an auxiliary context helping with initializing/finalizing SDL library
        audio=archi.Value(None, type=archi.VALUE_TRUE), # initialize the 'audio' subsystem
        video=archi.Value(None, type=archi.VALUE_TRUE)) # initialize the 'video' subsystem

# NOTICE: glados and sdl_window must be created after sdl_library

glados = glados_interface('glados') # the GLaDOS context (song context)

sdl_window = value_list_interface(
        'config', # configuration for a drawable window
        window_width=archi.Value(c.c_float(WINDOW_WIDTH_SCALE), type=archi.VALUE_FLOAT),   # window frame width scale
        window_height=archi.Value(c.c_float(WINDOW_HEIGHT_SCALE), type=archi.VALUE_FLOAT), # window frame height scale
        window_title=archi.Value(c.create_string_buffer(WINDOW_TITLE.encode()), type=archi.VALUE_STRING)) # window title

sdl_window.texture_width  = glados.texture_width
sdl_window.texture_height = glados.texture_height

sdl_window = sdl_window_interface('sdl_window', sdl_window) # a drawable window

# Configure the GLaDOS context
glados.signal_flags = app_signal.signal_flags # provide pointer to the signal flags to GLaDOS
glados.thread_group = thread_group
glados.sdl_window = sdl_window

glados.init() # invoke internal GLaDOS initialization that depends on the external pointers

# Configure the built-in contexts
app_signal.signal_handler = glados.signal_handler # install the signal handler from GLaDOS

app_fsm.entry_state_function = glados.entry_state_function # set entry state function of the application FSM
app_fsm.entry_state_data     = glados                      # set data of the entry state to GLaDOS itself

#################################
# Create the configuration file #
#################################

archi.fossilize(app, sys.argv[1])

