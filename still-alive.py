#!/usr/bin/env python

import archi
import ctypes as c
import sys


##########################################
# Hardcoded constants, don't change them #
##########################################

FONT_WIDTH = 6
FONT_HEIGHT = 12

SCREEN_SIZE_X = 1 + (35 + 1) + 1 + 41 + 1
SCREEN_SIZE_Y = 1 + (23 + 5) + 1

TEXTURE_WIDTH = FONT_WIDTH * SCREEN_SIZE_X
TEXTURE_HEIGHT = FONT_HEIGHT * SCREEN_SIZE_Y

##############################
# Values that can be changed #
##############################

NUM_THREADS = 4 # number of rendering threads

WINDOW_WIDTH = TEXTURE_WIDTH * 2
WINDOW_HEIGHT = TEXTURE_HEIGHT * 2
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

# Register interfaces
app.register_interface('sdl_library', 'plugin_sdl_library_interface', 'libsdl')       # register the SDL library interface
app.register_interface('sdl_window', 'plugin_sdl_window_context_interface', 'libsdl') # register the SDL window context interface
app.register_interface('glados', 'glados_interface', 'still-alive')                   # register the song interface

# Obtain built-in contexts
app_signal = app.builtin_context(archi.APP_SIGNAL_CONTEXT_KEY) # obtain the application signal management context
app_fsm = app.builtin_context(archi.APP_FSM_CONTEXT_KEY) # obtain the application finite state machine

# Create contexts
thread_group = app.new_context(
        'thread_group', 'thread_group', # a group of threads for multicore rendering
        num_threads=archi.Value(c.c_size_t(NUM_THREADS), type=archi.VALUE_UINT)) # number of threads in the group

sdl_library = app.new_context(
        'sdl_library', 'sdl_library', # an auxiliary context helping with initializing/finalizing SDL library
        audio=archi.Value(None), # initialize the 'audio' subsystem
        video=archi.Value(None)) # initialize the 'video' subsystem

# NOTICE: sdl_library must be created once before any sdl_window to initialize SDL properly

sdl_window = app.new_context(
        'sdl_window', 'sdl_window', # a drawable window
        texture_width=archi.Value(c.c_int(TEXTURE_WIDTH), type=archi.VALUE_SINT),   # window canvas width
        texture_height=archi.Value(c.c_int(TEXTURE_HEIGHT), type=archi.VALUE_SINT), # window canvas height
        window_width=archi.Value(c.c_int(WINDOW_WIDTH), type=archi.VALUE_SINT),     # window frame width
        window_height=archi.Value(c.c_int(WINDOW_HEIGHT), type=archi.VALUE_SINT),   # window frame height
        window_title=archi.Value(c.create_string_buffer(WINDOW_TITLE.encode()), type=archi.VALUE_STRING)) # window title

glados = app.new_context('glados', 'glados') # the GLaDOS context (song context)

# Configure the GLaDOS context
glados.signal_flags = app_signal.signal_flags     # copy pointer to the signal flags to GLaDOS
glados.thread_group = thread_group # copy pointer to the thread group to GLaDOS
glados.sdl_window = sdl_window     # copy pointer to the window to GLaDOS

glados.init() # invoke internal GLaDOS initialization that depends on the external pointers

# Configure the built-in contexts
app_signal.signal_handler = glados.signal_handler # install the signal handler from GLaDOS

app_fsm.entry_state_function = glados.entry_state_function # set function of the entry state of the application FSM
app_fsm.entry_state_data = glados                          # set data of the entry state to GLaDOS itself

#################################
# Create the configuration file #
#################################

archi.fossilize(app, sys.argv[1])

