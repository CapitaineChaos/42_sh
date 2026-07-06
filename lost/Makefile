# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/19 15:51:31 by cmassol           #+#    #+#              #
#    Updated: 2025/05/30 03:58:24 by smaitre          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS		+= --silent --no-print-directory --warn-undefined-variables

DLVL 			?= 0
NAME 			:= minishell
BONUS			:= bonus

CC				:= cc
CFLAGS			:= -Wall -Wextra -Werror -MMD -MP

RM				:= rm -f

SRC_BASE		:= src
OBJ_DIR			:= obj
OBJ_BASE		:= $(OBJ_DIR)/lv
INC_DIR			:= include

HEADERS			:= include/minishell.h 

# Sources auto-decouvertes : ajouter un .c n_importe ou sous src/ suffit.
ALL_FILES := $(shell find $(SRC_BASE) -name '*.c')


OBJ_FILES	:= $(patsubst $(SRC_BASE)/%, $(OBJ_BASE)/%, $(ALL_FILES:.c=.o))

LIBS		:= -lreadline


BD = \033[1m
DM = \033[2m
IT = \033[3m
UN = \033[4m
BN = \033[5m
RV = \033[7m
HD = \033[8m

RT = \033[0;39m
BK = \033[0;90m
RD = \033[0;91m
GN = \033[0;92m
YL = \033[0;93m
BL = \033[0;94m
MG = \033[0;95m
CY = \033[0;96m
WT = \033[0;97m



export OBJ_COUNT := $(words $(OBJ_FILES))
MAX := $(shell v=$$(expr $(shell stty size | awk '{print $$2}') - 12); [ $$v -gt 100 ] && echo 100 || echo $$v)
MIN := $(shell expr $(MAX) - 30)
COUNT ?= 0


all:
	@if make -q $(NAME) 2>/dev/null; then \
		echo "$(GN)          () () $(RT)"; \
		echo "$(GN)          (>.<) $(RT)"; \
		echo "$(GN)          ( . ) $(RT)"; \
		echo "$(GN)           n n  $(RT)"; \
		echo "$(MG)>>>>> Nothing to be done in $(NAME).\n$(RT)"; \
	else \
		$(MAKE) withcat; \
	fi;

$(BONUS): $(NAME)
	@cp ./$(NAME) ./$(NAME)_$@

$(NAME): $(OBJ_FILES)
	@$(CC) $(CFLAGS) $(OBJ_FILES) $(LIBS) -o $(NAME)
	@if [ $$? -eq 0 ]; then \
		printf "\033[2;0H\033[K"; \
		printf "\033[5;25Ho\033[5;27Ho"; \
		printf "\033[5;25Ho\033[5;27Ho"; \
		printf "\033[11;2HMinishell : 🥳\n\n"; \
		printf "\033[12$(GN)          () () $(RT)\n"; \
		printf "\033[13$(GN)          (o.o) $(RT)\n"; \
		printf "\033[14$(GN)          ( . ) $(RT)\n"; \
		printf "\033[15$(GN)           n n  $(RT)\n"; \
		printf "\033[16$(MG)>>>>> $(NAME) linked successfully!$(RT)\n\n"; \
		tput cnorm; \
	else \
		printf ""; \
		printf "\033[12$(BK)         ()_() $(RT)"; \
		printf "\033[13$(BK)         (   ) $(RT)"; \
		printf "\033[14$(BK)         ( $(RD)x$(RT) $(BK)) $(RT)"; \
		printf "\033[15$(BK)          n n  $(RT)"; \
		printf "\033[16$(RD)>>>>> Linking failed for $(NAME)$(RT)\n\n"; \
		tput cnorm; \
		exit 1; \
	fi

withcat:
	@export COUNT=0
	@clear
	@tput civis
	@printf "\033[1;0H$(YL) **** >>>>> linking: $(NAME) <<<<< **** $(RT)"
	@printf "\033[2;0H\033[K"
	@printf "\033[3;0H      _._     _,-'\"\"\`-._\033[K"
	@printf "\033[4;0H     (,-.\`._,'(       |\\\`-/|\033[K"
	@printf "\033[5;0H         \`-.-' \\ )-\`( , ~ ~)\033[K"
	@printf "\033[6;0H               \`-    \\\`_\`\"'-\033[K"
	@if make -q $(NAME) 2>/dev/null; then \
		printf ">>>>>  Nothing to be done in Minishell;\n" ; \
		printf "The cat is sleeping, so please do not wake him up.\n\n"; \
		tput cnorm; \
	else \
		printf "\n\n"; \
		$(MAKE) $(NAME); \
		tput cnorm; \
	fi
	


$(OBJ_BASE)/%.o: $(SRC_BASE)/%.c
	@mkdir -p $(@D)
	$(eval COUNT := $(shell expr $(COUNT) + 1))
	@COL=$$(expr $(MAX) - \( $(MIN) \* $(COUNT) / $(OBJ_COUNT) \)); \
	CNT=$$(expr $(COUNT)); \
	THRESH=$$(expr $(MAX) - 30); \
	CAP=5; \
	if [ "$$THRESH" -gt "$$CAP" ]; then \
		printf "\033[6;%sH   _\033[K\n" "$$COL"; \
		printf "\033[7;%sH (( )_ (\033[K\n" "$$COL"; \
		printf "\033[8;%sH <;___)_) \033[K\n" "$$COL"; \
		if [ $$(expr $$COL % 2) -eq 0 ]; then \
			printf "\033[9;%sH   \" \"  \033[K\n" "$$COL"; \
		else \
			printf "\033[9;%sH  \"   \"  \033[K\n" "$$COL"; \
		fi; \
		printf "\033[10;2HMinishell : %2d/%2d" "$$CNT" "$$OBJ_COUNT"; \
	fi
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@ > /dev/null || { \
		echo "" ; \
		echo "" ; \
		echo "$(GN)          () () $(RT)"; \
		echo "$(GN)          ($(RD)$(BD)x$(RT).$(RD)$(BD)x$(RT)$(GN)) $(RT)"; \
		echo "$(GN)     $(BK)$(BD)>>---$(RT)$(GN)($(BK)$(BD)-$(RT)$(GN). )$(BK)$(BD)-->$(RT)" ; \
		echo "$(GN)           n n  $(RT)"; \
		echo "$(MG)>>>>> Erreur de compilation détectée dans $(RD)$(UN)$(BD)$<$(RT)"; \
		echo "" ; \
		cput cnorm; \
		exit 1; \
	}

-include $(OBJ_FILES:.o=.d)

clean:
	@echo "$(MG)CLEAN OBJECTS FOR : $(NAME) ! $(RT)"
	@rm -rf $(OBJ_DIR)
	
fclean: clean
	@echo "$(MG)FCLEAN NAME FOR : $(NAME) ! $(RT)"
	@rm -f $(NAME)
	
re: fclean all

.PHONY: all clean fclean re

