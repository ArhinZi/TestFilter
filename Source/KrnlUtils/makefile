# Makefile for the Kernel Utilities Functions


# ----------------------------------------------------------------------

# Includes
# --------

# *** Win32.mak inclusion ***

!include <Win32.mak>

!include "..\CommonFiles\CmnMacros.mak

# ----------------------------------------------------------------------

# Project Specific Macros
# -----------------------

PROJ = KrnlUtils

LIB_OBJS = $(TARG_PATH)\$(PROJ).obj $(TARG_PATH)\WxpX86.obj $(TARG_PATH)\WlhX64.obj \
	$(TARG_PATH)\WlhX86.obj $(TARG_PATH)\Win7X64.obj $(TARG_PATH)\Win7X86.obj
GLOBAL_DEP = KrnlUtils.h KUPrivate.h


all: $(TARG_PATH) $(TARG_PATH)\$(PROJ).lib

#----- Build rule for $(PROJ).obj
$(TARG_PATH)\$(PROJ).obj: $(@B).cpp $(GLOBAL_DEP) WxpX86.h WlhX64.h
	$(cc) $(CUST_COMPIL_OPTS)  $(PROJ).cpp

#----- Build rule for WxpX86.obj
$(TARG_PATH)\WxpX86.obj: $$(@B).cpp $(GLOBAL_DEP) KrnlDefs.h WxpX86.h PlatfDepCompile.cpp
	$(cc) $(CUST_COMPIL_OPTS)  $(@B).cpp

#----- Build rule for WlhX86.obj
$(TARG_PATH)\WlhX86.obj: $$(@B).cpp $(GLOBAL_DEP) KrnlDefs.h WlhX86.h PlatfDepCompile.cpp
	$(cc) $(CUST_COMPIL_OPTS) $(@B).cpp

#----- Build rule for WlhX64.obj
$(TARG_PATH)\WlhX64.obj: $$(@B).cpp $(GLOBAL_DEP) KrnlDefs.h WlhX64.h PlatfDepCompile.cpp
	$(cc) $(CUST_COMPIL_OPTS)  $(@B).cpp

#----- Build rule for Win7X64.obj
$(TARG_PATH)\Win7X64.obj: $$(@B).cpp $(GLOBAL_DEP) KrnlDefs.h $$(@B).h PlatfDepCompile.cpp
	$(cc) $(CUST_COMPIL_OPTS)  $(@B).cpp

#----- Build rule for Win7X86.obj
$(TARG_PATH)\Win7X86.obj: $$(@B).cpp $(GLOBAL_DEP) KrnlDefs.h $$(@B).h PlatfDepCompile.cpp
	$(cc) $(CUST_COMPIL_OPTS)  $(@B).cpp



#----- Build rule for LIB
$(TARG_PATH)\$(PROJ).lib: $(LIB_OBJS)
	lib /OUT:$@ $**


!include "..\CommonFiles\CmnTargets.mak"