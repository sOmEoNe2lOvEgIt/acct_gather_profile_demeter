##
## ATOS PROJECT 2022
## MAKEFILE
## File description:
## Wow, such make, much file!
##

SLURM_ROOT_DIR = /usr
SLURM_INC_DIR = /root/SLURM/slurm.build
SLURM_LIB_DIR = /usr/lib64/slurm
SLURM_BUILD = 21.08.8-2
SLURM_BUILD_DIR = /root/rpmbuild/BUILD/slurm-$(SLURM_BUILD)

PLUGIN_TYPE = prep
PLUGIN_NAME = demeter
PLUGIN_FILE = $(PLUGIN_TYPE)_$(PLUGIN_NAME).so
DEMETER_LIB_DIR = /home/atos/lib_demeter
DEMETER_RUN_PATH = /shared/

SRC_FILES = src/demeter.c

CC      = gcc
CFLAGS  ?= -Wall -fPIC -g3 -gstrict-dwarf -I$(SLURM_INC_DIR) -I$(SLURM_BUILD_DIR) -Iinclude -I$(DEMETER_LIB_DIR)/include -Wl,-rpath=$(DEMETER_RUN_PATH)
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
