# Macros common to several makefiles
# ----------------------------------

# Target path for the build
#
TARG_PATH = Release\$(APPVER)\$(PROCESSOR_ARCHITECTURE)
!IF ("$(NODEBUG)" == "")
TARG_PATH = Debug\$(APPVER)\$(PROCESSOR_ARCHITECTURE)
!ENDIF

# Compiler options
#
MY_COMPIL_OPTS = /FAs /Fa"$(TARG_PATH)\\" /EHac- /Fp"$(TARG_PATH)\\"  \
	/Fo"$(TARG_PATH)\\" /Fd"$(TARG_PATH)\\" /D_UNICODE /DUNICODE 
CUST_COMPIL_OPTS = $(cdebug) $(cflags) $(cvarsmt) $(MY_COMPIL_OPTS)


# Linker options
#
MY_LINK_OPTS_EXE = /MAP:$*.map /MAPINFO:EXPORTS -out:$@ /LIBPATH:..\KrnlUtils\$(TARG_PATH)
!IF ("$(NODEBUG)" != "")

# In release builds, win32.mak does not specify the linker opts to
# generate symbols, so we add them here, to have symbols for relase builds
# as well as for debug ones.
#
MY_LINK_OPTS_EXE = $(MY_LINK_OPTS_EXE) /DEBUG /DEBUGTYPE:cv
!ENDIF

LINK_OPTS_EXE = $(ldebug) $(conlflags) $(conlibsdll)  $(MY_LINK_OPTS_EXE)

