# Targets common to several makefiles
# -----------------------------------


#----- If TARG_PATH does not exist, create the directory
$(TARG_PATH) :
    if not exist "$(TARG_PATH)\" mkdir $(TARG_PATH)


#----------------------------------------------------------------------
# Clean-up, rebuild, etc.
# -----------------------

# Target for cleaning things up.
#
clean:
	if exist $(TARG_PATH) rd /s /q $(TARG_PATH)

# Delete only files created by the linker
#
cleanlinkout:
	if exist $(TARG_PATH)\*.exe del $(TARG_PATH)\*.exe
	if exist $(TARG_PATH)\*.dll del $(TARG_PATH)\*.dll

# Full rebuild
#	
rebuild: clean all

# Relink only
#
relink: cleanlinkout all
