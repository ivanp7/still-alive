#!/usr/bin/env python

import archi
import ctypes as c
import os.path as p
import sys

#########################
# Customization options #
#########################

FONT_FILE_PATHNAME = p.join(p.abspath(p.dirname(sys.argv[0])), "font.psf") # pathname of a font file in PSF2 format

NUM_THREADS = 4 # number of rendering threads

WINDOW_WIDTH_SCALE  = 2 # number of window pixels per texture pixels, can be fractional
WINDOW_HEIGHT_SCALE = WINDOW_WIDTH_SCALE

WINDOW_TITLE = "Still Alive"

#############################
# Construct the application #
#############################

app = archi.Application()

# Add signals to the watch set
app.watch_signal('SIGINT')  # the app handles Ctrl+C
app.watch_signal('SIGQUIT') # the app handles Ctrl+\
app.watch_signal('SIGTERM') # the app handles termination
app.watch_signal('SIGCONT') # the app handles Ctrl+Z

# Load plugin libraries
app.load_library('libsdl', 'libarchi-sdl.so')     # load the plugin for SDL support
app.load_library('still-alive', 'still-alive.so') # load the song plugin

# Register context interfaces
app.register_interface('sdl_library', 'plugin_sdl_library_interface', 'libsdl')       # register the SDL library interface
app.register_interface('sdl_window', 'plugin_sdl_window_context_interface', 'libsdl') # register the SDL window context interface
app.register_interface('font_psf2', 'plugin_font_psf2_context_interface', 'libsdl')   # register the PSF2 font interface
app.register_interface('glados', 'glados_interface', 'still-alive')                   # register the song interface

# Load the font
font_file = app.interface(archi.FILE_INTERFACE).new_context(
        'font_file', # a file containing the font
        pathname=archi.Value(c.create_string_buffer(FONT_FILE_PATHNAME.encode()), type=archi.VALUE_STRING),
        readable=archi.Value(type=archi.VALUE_TRUE))

font_file.map(readable=archi.Value(type=archi.VALUE_TRUE)) # map the file into memory

font = app.value_list_interface().new_context(
        'config') # font configuration
font.bytes = font_file.mapped

font = app.interface('font_psf2').new_context('font', font) # font of the text

# Create rendering threads
thread_group = app.interface(archi.THREAD_GROUP_INTERFACE).new_context(
        'thread_group', # a group of threads for multicore rendering
        num_threads=archi.Value(c.c_size_t(NUM_THREADS), type=archi.VALUE_UINT)) # number of threads in the group

# Initialize SDL2
sdl_library = app.interface('sdl_library').new_context(
        'sdl_library', # an auxiliary context helping with initializing/finalizing SDL library
        audio=archi.Value(type=archi.VALUE_TRUE), # initialize the 'audio' subsystem
        video=archi.Value(type=archi.VALUE_TRUE)) # initialize the 'video' subsystem

# NOTICE: glados and sdl_window must be created after sdl_library

# Create the window and configure the GLaDOS context
glados = app.interface('glados').new_context('glados') # the GLaDOS context (song context)

glados.font = font # set text font
glados.thread_group = thread_group # set rendering thread group

sdl_window = app.value_list_interface().new_context(
        'config', # configuration for a drawable window
        window_width=archi.Value(c.c_float(WINDOW_WIDTH_SCALE), type=archi.VALUE_FLOAT),
        window_height=archi.Value(c.c_float(WINDOW_HEIGHT_SCALE), type=archi.VALUE_FLOAT),
        window_title=archi.Value(c.create_string_buffer(WINDOW_TITLE.encode()), type=archi.VALUE_STRING))

sdl_window.texture_width  = glados.texture_width
sdl_window.texture_height = glados.texture_height

sdl_window = app.interface('sdl_window').new_context('sdl_window', sdl_window) # a drawable window

glados.sdl_window = sdl_window # set the window

glados.init_sound() # prepare the music for playing

# Configure the built-in contexts
app_signal = app.context(archi.APP_SIGNAL_CONTEXT) # obtain the application signal management context
app_signal.signal_handler = glados.signal_handler   # install the signal handler from GLaDOS
glados.signal_flags       = app_signal.signal_flags # provide pointer to the signal flags to GLaDOS

app_fsm = app.context(archi.APP_FSM_CONTEXT) # obtain the application finite state machine
app_fsm.entry_state_function = glados.entry_state_function # set entry state function of the application FSM
app_fsm.entry_state_data     = glados                      # set data of the entry state to GLaDOS itself

#################################
# Create the configuration file #
#################################

archi.fossilize(app, sys.argv[1])

