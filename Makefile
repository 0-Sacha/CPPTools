# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug_x64
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug_x64)
  CPPTools_config = debug_x64
  Sandbox_config = debug_x64

else ifeq ($(config),debug_win32)
  CPPTools_config = debug_win32
  Sandbox_config = debug_win32

else ifeq ($(config),release_x64)
  CPPTools_config = release_x64
  Sandbox_config = release_x64

else ifeq ($(config),release_win32)
  CPPTools_config = release_win32
  Sandbox_config = release_win32

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := CPPTools Sandbox

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

CPPTools:
ifneq (,$(CPPTools_config))
	@echo "==== Building CPPTools ($(CPPTools_config)) ===="
	@${MAKE} --no-print-directory -C CPPTools -f Makefile config=$(CPPTools_config)
endif

Sandbox: CPPTools
ifneq (,$(Sandbox_config))
	@echo "==== Building Sandbox ($(Sandbox_config)) ===="
	@${MAKE} --no-print-directory -C Sandbox -f Makefile config=$(Sandbox_config)
endif

clean:
	@${MAKE} --no-print-directory -C CPPTools -f Makefile clean
	@${MAKE} --no-print-directory -C Sandbox -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug_x64"
	@echo "  debug_win32"
	@echo "  release_x64"
	@echo "  release_win32"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   CPPTools"
	@echo "   Sandbox"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"