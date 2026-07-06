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
NAME 			:= 42_sh
BONUS			:= bonus

CC				:= cc
CFLAGS			:= -Wall -Wextra -Werror -MMD -MP
# >>>HIDE_START
CFLAGS			:= -Wall -Wextra -Werror -MMD -MP -g3
# <<<HIDE_END

RM				:= rm -f

SRC_BASE		:= src
OBJ_DIR			:= obj
OBJ_BASE		:= $(OBJ_DIR)/lv
INC_DIR			:= include

HEADERS			:= include/minishell.h 

SRCS_ROOT = \
				launcher

# >>>HIDE_START
SRCS_DEBUG = \
				convert_ast_types \
				convert_dbg_level \
				convert_tok_type \
				convert_tok_tclass \
				print_arg \
				print_tokens \
				print_ast_node \
				print_ast_tree \
				print_pendings \
				print_title \
				trace_1 \
				trace_2 \
				trace_3 \
				trace_debug_lvl \
				write_file \
				dbg_view
# <<<HIDE_END

SRCS_READING = \
				collect_heredoc \
				prompt \
				read_helpers \
				read_heredoc \
				read_main \
				read_sig \
				read

SRCS_CHECKER = \
				ctx/ctx_convert \
				ctx/ctx_helpers \
				ctx/ctx_lifecycle \
				ctx/ctx_ops \
				checker_aux \
				checker_core \
				checker_lifecycle \
				checker_shortcuts \
				checker

SRCS_LEXER = \
				helpers_1 \
				helpers_2 \
				lex_dquoted \
				lex_squoted \
				lex_unquoted \
				lx_run \
				lx_tokenize_struct \
				lx_tokenize_op_rd \
				lexer_lifecycle

SRCS_PARSER = \
				convert \
				parser_lifecycle \
				prs_run \
				rd_parser

SRCS_EXPANDER = \
				dollar/exp_heredoc \
				dollar/exp_var \
				dollar_special/args \
				tilde/exp_tilde \
				wildcard/exp_wild_run \
				wildcard/exp_wildcard \
				wildcard/sort \
				split/splitt_aux \
				split/splitt_parts \
				split/splitt_token \
				split/splitt \
				aggregate \
				aux \
				process_exp \
				tag_dollar \
				tag_glob \
				tag_tilde

SRCS_ASSEMBLER = \
				asm_build \
				asm_run

SRCS_EXECUTOR = \
				builtin/bt_cd \
				builtin/bt_dot \
				builtin/bt_echo \
				builtin/bt_env \
				builtin/bt_exit \
				builtin/bt_export \
				builtin/bt_pwd \
				builtin/bt_unset \
				aux \
				exec_and_or \
				exec_cmd_builtin \
				exec_cmd_empty \
				exec_cmd_external \
				exec_cmd \
				exec_codes \
				exec_msgs \
				exec_pipe \
				exec_pipe_aux \
				exec_run \
				exec_sequence \
				exec_subshell \
				execute_builtin \
				redirs_apply

SRCS_MINISHELL = \
				minishell_lifecycle \
				minishell

SRCS_ENV = \
				env_lifecycle \
				env_ops \
				env \
				ft_getenv \
				minishell_env \
				sort
SRCS_AST = \
				ast_create \
				ast_helpers \
				ast_lifecycle \
				push_swap

SRCS_NORMALIZE = \
				assign \
				normalize_token \
				normalize_part

SRCS_TOKEN = \
				tk_ctrl_emit \
				tk_glob_emit \
				tk_op_redir_emit \
				tk_parts \
				tk_word_emit \
				token_append \
				token_content \
				token_lifecycle

SRCS_SIGNAL = \
				handles \
				init
SRCS_INPUT = \
				lexer_sub_input \
				inp_adv \
				inp_cmp

SRCS_HELPERS = \
				printer \
				proc \
				shell_level \
				valid_id

SRCS_UTILS = \
				deque/lifecycle \
				deque/ops \
				gnl/nano_gnl \
				printf_fd/ft_printf_fd_process_hex \
				printf_fd/ft_printf_fd_process_nbr \
				printf_fd/ft_printf_fd_process_str \
				printf_fd/ft_printf_fd_setparams \
				printf_fd/ft_printf_fd \
				std/ft_atoi \
				std/ft_atoll \
				std/ft_cput_fd \
				std/ft_char_is \
				std/ft_dup \
				std/ft_free \
				std/ft_len \
				std/ft_mem \
				std/ft_nb_cmp \
				std/ft_split \
				std/ft_str_cmp \
				std/ft_striteri \
				std/ft_strjoin \
				std/ft_strlcpy \
				std/ft_substr \
				std/ft_toa \
				std/ft_tolower \
				std/ft_toupper \
				sstr/sstr_aux \
				sstr/sstr \
				strapi/ft_strapi_add_remove \
				strapi/ft_strapi_append \
				strapi/ft_strapi_get \
				strapi/ft_strapi_lifecycle \
				strapi/ft_strapi_utils \
				wput/ft_wputchar_fd \
				wput/ft_wputstr_fd \
				get_term_size \
				time \
				logger

# >>>HIDE_START
SRCS_DEBUG_FULL 	:= $(addprefix $(SRC_BASE)/m_debug/, $(addsuffix .c, $(SRCS_DEBUG)))
# <<<HIDE_END
SRCS_ROOT_FULL 		:= $(addprefix $(SRC_BASE)/, $(addsuffix .c, $(SRCS_ROOT)))
SRCS_READING_FULL 	:= $(addprefix $(SRC_BASE)/step_1_reading/, $(addsuffix .c, $(SRCS_READING)))
SRCS_CHECKER_FULL 	:= $(addprefix $(SRC_BASE)/step_2_checking/, $(addsuffix .c, $(SRCS_CHECKER)))
SRCS_LEXER_FULL 	:= $(addprefix $(SRC_BASE)/step_3_lexing/, $(addsuffix .c, $(SRCS_LEXER)))
SRCS_PARSER_FULL 	:= $(addprefix $(SRC_BASE)/step_4_parsing/, $(addsuffix .c, $(SRCS_PARSER)))
SRCS_EXPANDER_FULL 	:= $(addprefix $(SRC_BASE)/step_5_expanding/, $(addsuffix .c, $(SRCS_EXPANDER)))
SRCS_ASSEMBLER_FULL := $(addprefix $(SRC_BASE)/step_6_assembling/, $(addsuffix .c, $(SRCS_ASSEMBLER)))
SRCS_EXECUTOR_FULL 	:= $(addprefix $(SRC_BASE)/step_7_execution/, $(addsuffix .c, $(SRCS_EXECUTOR)))
SRCS_ENV_FULL 		:= $(addprefix $(SRC_BASE)/env/, $(addsuffix .c, $(SRCS_ENV)))
SRCS_UTILS_FULL 	:= $(addprefix $(SRC_BASE)/utils/, $(addsuffix .c, $(SRCS_UTILS)))
SRCS_HELPERS_FULL 	:= $(addprefix $(SRC_BASE)/helpers/, $(addsuffix .c, $(SRCS_HELPERS)))
SRCS_SIGNAL_FULL 	:= $(addprefix $(SRC_BASE)/m_signal/, $(addsuffix .c, $(SRCS_SIGNAL)))
SRCS_INPUT_FULL 	:= $(addprefix $(SRC_BASE)/m_input/, $(addsuffix .c, $(SRCS_INPUT)))
SRCS_NORMALIZE_FULL := $(addprefix $(SRC_BASE)/m_normalize/, $(addsuffix .c, $(SRCS_NORMALIZE)))
SRCS_TOKEN_FULL 	:= $(addprefix $(SRC_BASE)/m_token/, $(addsuffix .c, $(SRCS_TOKEN)))
SRCS_AST_FULL 		:= $(addprefix $(SRC_BASE)/m_ast/, $(addsuffix .c, $(SRCS_AST)))
SRCS_MINISHELL_FULL := $(addprefix $(SRC_BASE)/minishell/, $(addsuffix .c, $(SRCS_MINISHELL)))

ALL_FILES 	:= \
		$(SRCS_ROOT_FULL) \
		$(SRCS_READING_FULL) \
		$(SRCS_CHECKER_FULL) \
		$(SRCS_ENV_FULL) \
		$(SRCS_UTILS_FULL) \
		$(SRCS_HELPERS_FULL) \
		$(SRCS_LEXER_FULL) \
		$(SRCS_PARSER_FULL) \
		$(SRCS_EXPANDER_FULL) \
		$(SRCS_ASSEMBLER_FULL) \
		$(SRCS_SIGNAL_FULL) \
		$(SRCS_MINISHELL_FULL) \
		$(SRCS_NORMALIZE_FULL) \
		$(SRCS_INPUT_FULL) \
		$(SRCS_TOKEN_FULL) \
		$(SRCS_AST_FULL) \
		$(SRCS_EXECUTOR_FULL)

# >>>HIDE_START
		ALL_FILES := $(ALL_FILES) $(SRCS_DEBUG_FULL)
# <<<HIDE_END

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

# >>>HIDE_START
include Makefile.debug
# <<<HIDE_END
