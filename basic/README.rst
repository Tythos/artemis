Basic cross-platforom build examples and scripts for GCC and MSVC toolchains.
Should be a solid pathfinder for future Artemis rewrites, and serves as a good
refresher.

Batch files are for MSVC. This assumes SDL2 has been extracted to %SDL_ROOT%,
that GLEW has been extracted to %GLEW_ROOT%, and that both have been build in
the x64/Debug configuration. MSVC programs *cl.exe* and *link.exe* should be
exposed within the current command line scope when invoked; try running the
appropriate *vcvarsall.bat* for your installation to ensure all system include
and library paths are exposed, too.

Shell scripts are for GCC--at least, once they're written.
