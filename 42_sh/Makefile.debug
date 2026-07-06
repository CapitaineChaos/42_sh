# Debug via le debugview web (../tools/debugview). Plus de debug terminal.
# `make debug` compile un binaire debug (-DDLVL -DDBG_VIEW) puis lance
# debugserve.py : pseudo-terminal xterm.js + panneaux (tokens/ast/exec/footer)
# alimentés par les fichiers /dev/shm/sh42_dbg/*.

DBG_LVL			?= 11
DBG_BUILD		:= /dev/shm/sh42_build
DBG_SERVE		:= ../tools/debugview/debugserve.py

DBG_OBJ_BASE	:= $(DBG_BUILD)/lv$(DBG_LVL)
DBG_OBJ_FILES	:= $(patsubst $(SRC_BASE)/%, $(DBG_OBJ_BASE)/%, $(ALL_FILES:.c=.o))
DBGFLAGS		:= $(CFLAGS) -g3 -O0 -DDLVL=$(DBG_LVL) -DDBG_VIEW

$(DBG_OBJ_BASE)/%.o: $(SRC_BASE)/%.c
	@mkdir -p $(@D)
	@$(CC) $(DBGFLAGS) -I$(INC_DIR) -c $< -o $@ \
		|| { echo "$(RD)>>>>> Erreur de compilation : $(BD)$<$(RT)"; exit 1; }

debug_build: $(DBG_OBJ_FILES)
	@$(CC) $(DBGFLAGS) $(DBG_OBJ_FILES) $(LIBS) -o $(NAME) \
		&& echo "$(GN)>>>>> $(NAME) (debug DLVL=$(DBG_LVL)) linked$(RT)" \
		|| { echo "$(RD)>>>>> Linking failed for $(NAME)$(RT)"; exit 1; }

debug: debug_build
	@echo "$(MG)>>>>> debugview : http://127.0.0.1:8765$(RT)"
	@python3 $(DBG_SERVE)

-include $(DBG_OBJ_FILES:.o=.d)

testv:
	@$(MAKE) all
	valgrind --track-fds=yes --trace-children=yes --track-origins=yes \
		--leak-check=full -s --show-leak-kinds=all \
		--suppressions=.vignore.supp ./$(NAME)

gen:
	valgrind --leak-check=full --show-leak-kinds=all \
		--errors-for-leak-kinds=reachable --gen-suppressions=all ./$(NAME)

.PHONY: debug debug_build testv gen
