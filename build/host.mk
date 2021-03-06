HOST_EXEEXT = $(findstring .exe,$(MAKE))
HOSTCC = gcc$(HOST_EXEEXT)
HOSTCXX = g++$(HOST_EXEEXT)
HOSTCPP = cpp$(HOST_EXEEXT)

HOST_CPPFLAGS = $(INCLUDES) $(CPPFLAGS) $(TARGET_CPPFLAGS)
HOST_CXXFLAGS = $(OPTIMIZE) $(CXX_FEATURES) $(CXXFLAGS) $(TARGET_CPPFLAGS)
HOST_CFLAGS = $(OPTIMIZE) $(C_FEATURES) $(CFLAGS)

host-cc-flags = $(DEPFLAGS) $(HOST_CFLAGS) $(HOST_CPPFLAGS)
host-cxx-flags = $(DEPFLAGS) $(HOST_CXXFLAGS) $(HOST_CPPFLAGS)
host-ld-flags = -lm

$(HOST_OUTPUT_DIR)/%.o: %.c | $(HOST_OUTPUT_DIR)/%/../dirstamp
	@$(NQ)echo "  HOSTCC  $@"
	$(Q)$(HOSTCC) -c $(host-cc-flags) -o $@ $^

$(HOST_OUTPUT_DIR)/%.o: %.cpp | $(HOST_OUTPUT_DIR)/%/../dirstamp
	@$(NQ)echo "  HOSTCXX $@"
	$(Q)$(HOSTCXX) -c $(host-cxx-flags) -o $@ $^

$(HOST_OUTPUT_DIR)/%$(HOST_EXEEXT): $(HOST_OUTPUT_DIR)/%.o
	@$(NQ)echo "  HOSTLD  $@"
	$(Q)$(HOSTCC) $(host-ld-flags) -o $@ $^
