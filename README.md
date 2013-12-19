libesp
======

ESP is the Extra Sensory Profiler, a high-performance instrumented profiler for
game development.

To use ESP, you add function calls to your existing codebase. These functions
are called "instruments". ESP offers two kinds of instruments, Zones and Probes.
Zones profile durations of time, and are signified with esp_zone() and
esp_end(). Probes allow your application to export timestamped data into the
event stream, enabling you to capture any metrics you can imagine in your code.

libesp does not include a data visualizer. It outputs a standard sqlite database
of collated events. A great deal of timing data can be recovered directly from
SQL queries to this database. Visualization is a separate project.


Requirements
============

This code is designed and tested to run on multicore 64-bit x86 processors.
I'm not saying it *can't* work well elsewhere, but it probably *doesn't* work
anywhere else at the time of this writing. Why multicore? Because the timestamp
thread is greedy. This can certainly be improved, but presently the timestamp
thread will happily spin 100% of a core.

Also, you might need fast storage. I'm on SSD, and everything is near-instantaneous. I
don't know what it's like on spinning platters. If it's a problem, you can
probably make up for this with a bunch of RAM, since the OS can buffer the sort
of bulk sequential disk writes that are done during the application lifetime.


Limitations
===========

You can overload ESP with sheer event volume. Try to keep it to a couple
thousand per frame, and try to keep the framerate below 120Hz.

libesp is designed primarily for games, with low overhead on the profiled path.
It often sacrifices data safety for speed.

It uses fairly large buffers, but the sizes of these can be tuned, and they are
all allocated during program startup.


Building ESP on Linux
===============================

The in-process profiler component of ESP is built as a single staticly linked
library. It does *require* some standard dynamic libraries (stdc++, pthread, dl,
rt), but is itself a position-independent static library (meaning you can easily
create a dynamic library from it if desired).

You'll need gcc 4.8 or higher, and python installed.

Clone from github (where you're likely reading this). Type "make", and watch as
it compiles the library (libesp.a), the uplift tool (esp_uplift), sqlite CLI
tool (sqlite), and the tests (./tests/test*).

You can now include the header file "profile.h", and link with the "libesp.a"
library. Make sure to link your application with the libraries mentioned above.

The Makefile is a convenience for calling scons_local, which is a copy of the
SCons build system. This is a python script for building everything. Have a look
in there and you can see how to directly invoke SCons.

Additionally, you can just copy the source and header files from the github
clone into your own build system. You might want to *not* add the "./src/uplift"
directory if that's the case, though, since this is a command line utility and
does not need to be embedded within your program.


Building ESP on Windows
=======================

Coming soon!


Building ESP on OSX
===================

Coming soon!


Using ESP
=========

First, a note on strings: strings given to ESP should be in stable storage. The
pointers to the strings themselves are used to establish identity. All strings
given to ESP should not move or change contents between the time you call
esp_init() and esp_shutdown().

Next, let's define a concept: zones. A zone is just a named section or interval
of code. Each zone should have a meaningful name, and zones with the same name
will be treated as equivalent within ESP. In general, it's easiest to give every
zone a unique name--but, you can use the equivalence to your advantage in
certain situations.

Now the part you've been waiting for. It's really easy.

To profile a section, surround it with esp_zone() and esp_end(). This will
generate an ESP event timestamped at the moment esp_zone() was called, and with
a value matching the duration of the zone in microseconds.

    void Rat::DoMovement()
    {
        esp_zone("rat_movement");
        while (this->NotThere()) { this->Scurry(); }
        esp_end();
    }
    
This will output one zone profile event for each call to Rat::DoMovement().

You can nest zones. Internally, the zones are arranged in a stack during
profiling.

    Cheese* Rat::NearestCheese()
    {
        esp_zone("nearest_cheese");
        //find some cheese
        esp_end();
        return cheese;
    }
    
    void Rat::FindCheese()
    {
        esp_zone("find_cheese");
        for (int i = 0; i < 5; i++){
            targetCheese[i] = NearestCheese();
        }
        esp_end();
    }

This will generate an event for "find_cheese", with five children events--one
for each "nearest_cheese".

That's pretty much all you need to know to instrument your code. But, as always,
there's a little bit of boilerplate.

At the start of your program, before you call any esp_zone() or esp_sample_*()
functions, you need to call esp_init(). This sets up the global profiling context.

For each thread you spawn in your program (and for which you would like
profiling information), you must call esp_thread_init(). This sets up the
thread-local profiling context. This function is called automatically for you in
the main thread by the esp_init() function, so you need only invoke it in
additional threads.

During the course of your program, in the main loop, you need to call
esp_frame_end() at the end of each iteration. 

Finally, before you exit the program, you should call esp_shutdown(). This will
flush all pending events to disk and block until IO is complete.








