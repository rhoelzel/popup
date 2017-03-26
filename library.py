import sys, inspect, ctypes, os

sys.dont_write_bytecode = True

try:
    if sys.version_info.major == 2:
        # We are using Python 2.x
        import Tkinter as tk
    elif sys.version_info.major == 3:
        # We are using Python 3.x
        import tkinter as tk
except:
        print("tkinter not found / not using tk")

""" Load shared library:"""
if sys.platform=='linux2':
    sep='/'
    libfile="libpopup.so"
elif sys.platform=='darwin':
    sep='/'
    libfile="libpopup.dylib"
elif sys.platform=='win32':
    sep='\\'
    libfile="libpopup.dll"
else:
    quit()

libpath=os.getcwd()
lib=ctypes.CDLL(libpath+sep+"build"+sep+libfile)
lib.up.argtypes=[ctypes.c_double]

if os.name== 'posix':
    import select
    def stdin_ready():
        """ 
        A function to determine whether the Python
        interpreter is ready to receive input on
        Unix systems.
        """ 
        infds, outfds, erfds = select.select([sys.stdin],[],[],0)
        if infds:
            return True
        else:
            return False
elif sys.platform=='win32':
    import msvcrt
    def stdin_ready():
        """ 
        A function to determine whether the Python
        interpreter is ready to receive input on
        Windows.
        """
        return msvcrt.kbhit()

if tk:
    tkroot=tk.Tk()
    tkroot.withdraw()

KEY_INTERVAL=0.05 # wait for events for KEY_INTERVAL seconds,
# between checks whether a key has been hit at the Python interface


def hook():       
    """ 
    If the Python interpreter is idle, the scigma.gui event 
    loop is called. The argument KEY_INTERVAL given to
    scigma_gui_loop() specifies the time to wait for gui
    events in seconds, before returning control to the 
    interpreter. Note that this is not a busy wait - a large
    KEY_INTERVAL therefore will reduce system load, but make
    the python console less responsive
    """

    while not stdin_ready():
        lib.up(KEY_INTERVAL)
        if tk:
            tkroot.update_idletasks()
            tkroot.update()
    return 0

"""
This installs hook() as PyOS_InputHook, calling
the scigma event loop whenever the interpreter
is idle.
"""

c_hook=ctypes.PYFUNCTYPE(ctypes.c_int)(hook)

def up():
    ctypes.c_void_p.in_dll(ctypes.pythonapi,"PyOS_InputHook").value=ctypes.cast(c_hook,ctypes.c_void_p).value
def down():
    ctypes.c_void_p.in_dll(ctypes.pythonapi,"PyOS_InputHook").value=None
    lib.down()

    
def hello():
    print ("hello!")

def goodbye():
    print ("good bye!")
    
# create a popup menu
def popup(a,b):
    x=tkroot.winfo_pointerx()#-gui.tkroot.winfo_rootx()
    y=tkroot.winfo_pointery()#-gui.tkroot.winfo_rooty()
    menu=tk.Menu(tkroot, tearoff=0)
    menu.add_command(label='hello', command=hello)
    menu.add_command(label='good bye', command=goodbye)
    menu.tk_popup(x,y)


C_CallbackType=ctypes.CFUNCTYPE(None,ctypes.c_int, ctypes.c_int)
callback=C_CallbackType(popup)
lib.set_python_callback.argtypes=[C_CallbackType]
lib.set_python_callback(callback)
