# Makefile for the Test Client

# ----------------------------------------------------------------------

# Includes
# --------

# *** Win32.mak inclusion ***

!include <Win32.mak>

!include "..\CommonFiles\CmnMacros.mak"

# ----------------------------------------------------------------------

# Project Specific Macros
# -----------------------

PROJ = TestClt

PROJ_OBJS  = $(TARG_PATH)\$(PROJ).obj 
EXTRA_LIBS = KrnlUtils.lib 


all: $(TARG_PATH) $(TARG_PATH)\$(PROJ).exe 


#----- Build rule for $(PROJ).obj
$(TARG_PATH)\$(PROJ).obj: $(PROJ).c $(GLOBAL_DEP) ..\Driver\DrvR3.h
	$(cc) $(CUST_COMPIL_OPTS) /I ..\Driver /I ..\KrnlUtils $(PROJ).c


# Build rule for EXE
#----- Build rule for EXE
$(TARG_PATH)\$(PROJ).EXE: $(PROJ_OBJS) 
	$(link) $(LINK_OPTS_EXE) $(PROJ_OBJS) $(EXTRA_LIBS)


!include "..\CommonFiles\CmnTargets.mak"
