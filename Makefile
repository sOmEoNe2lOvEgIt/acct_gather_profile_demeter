##
## ATOS PROJECT 2022
## MAKEFILE
## File description:
## Wow, such make, much file!
##

include ./Makefile.inc

PLUGIN_TYPE = prep
PLUGIN_NAME = demeter
PLUGIN_FILE = $(PLUGIN_TYPE)_$(PLUGIN_NAME).so

SRC_FILES = src/demeter.c

CC      = gcc
CFLAGS  ?= -Wall -fPIC -g3 -gstrict-dwarf -I$(SLURM_INC_DIR) -Iinclude -I$(DEMETER_LIB_DIR)/include -Wl,-rpath=$(DEMETER_RUN_PATH)
LDFLAGS ?= -shared -L. -L$(DEMETER_LIB_DIR) -ldemeter

all: $(PLUGIN_FILE)

default: $(PLUGIN_FILE)

$(PLUGIN_FILE): $(SRC_FILES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

send: all
	scp $(PLUGIN_FILE) my_vm:/home/compose_fake_taranis/plugin/

clean:
	rm -f $(PLUGIN_FILE)
	rm -f $(LIB_FILE)

re: clean all
