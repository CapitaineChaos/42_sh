#!/bin/bash
# MiniShell Tester Script
rm -f ./minishell

make -C ../

if [ $? -eq 0 ]; then
	echo "Compilation réussie !"
else
	echo "Erreur de compilation."
	exit 1
fi

source ./test_prog_err.sh
cp ../minishell minishell

MINISHELL_PATH="./minishell"

readarray -t lst_common  < lst_common
readarray -t lst_wildcat < lst_wildcat
readarray -t lst_lapinou < lst_lapinou
readarray -t lst_bonus   < lst_bonus

all=("${lst_common[@]}" "${lst_wildcat[@]}" "${lst_lapinou[@]}" "${lst_bonus[@]}")


# Lecture des arguments du tester

QUIET=0
VALGRIND=0
ONLY_ICONS=0
USE_ENV_I=0
declare -a POSITIONAL_ARGS=()

# Parser les arguments
while [[ $# -gt 0 ]]; do
	case "$1" in
		-q|--quiet-output)   QUIET=1;           shift ;;
		-v|--valgrind)       VALGRIND=1;        shift ;;
		-i|--env-i)          USE_ENV_I=1;       shift ;;
		-o|--only-icons)     ONLY_ICONS=1;      shift ;;
		*)  POSITIONAL_ARGS+=("$1"); shift ;;
	esac
done


#
#
# echo -e "unset -HELLO" | LANGUAGE=en LANG=en_US.UTF-8 LC_ALL=en_US.UTF-8 bash --posix 
#
# printf "%s\n" bonus_* > lst_bonus
# printf "%s\n" wildcat_* > lst_wildcat
# printf "%s\n" common_* > lst_common
# printf "%s\n" lapinou_* > lst_lapinou
#
# Run a specific test if the file name was specified

shopt -s nullglob
if (( ${#POSITIONAL_ARGS[@]} )); then
	for arg in "${POSITIONAL_ARGS[@]}"; do
		if [[ -f $arg ]]; then
			# argument est un fichier .tests existant
			test_lists+=("$arg")
		else
			case "$arg" in
				wildcat|wildcat_*)
					for f in wildcat_*.tests;   do test_lists+=("$f"); done
					;;
				lapinou|lapinou_*)
					for f in lapinou_*.tests;   do test_lists+=("$f"); done
					;;
				common|common_*)
					for f in common_*.tests;    do test_lists+=("$f"); done
					;;
				bonus|bonus_*)
					for f in bonus_*.tests;     do test_lists+=("$f"); done
					;;
				*)
					echo "Aucun test trouvé pour : $arg" >&2
					exit 1
					;;
			esac
		fi
	done
else
	# aucun argument, on prend toutes les catégories dans l'ordre demandé
	for f in common_*.tests;  do test_lists+=("$f"); done
	for f in lapinou_*.tests; do test_lists+=("$f"); done
	for f in bonus_*.tests;   do test_lists+=("$f"); done
	for f in wildcat_*.tests; do test_lists+=("$f"); done
fi
shopt -u nullglob

RESET="\033[0m"
BOLD="\e[1m"
YELLOW="\033[0;33m"
BYELLOW="\033[1;33m"
GREY="\033[38;5;244m"
GREEN="\033[0;32m"
BGREEN="\033[1;32m"
PURPLE="\033[0;35m"
BPURPLE="\033[1;35m"
BLUE="\033[0;34m"
BBLUE="\033[1;34m"
CYAN="\033[0;36m"
BCYAN="\033[1;36m"
RED="\033[0;31m"
BRED="\033[1;31m"
END="\033[0m"

chmod 000 ./test_files/invalid_permission
mkdir ./outfiles
mkdir ./mini_outfiles
mkdir ./bash_outfiles

printf $GREEN
echo "🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴";
echo "🌴🌴🌴Lapinou 🐰🐯 WiLDCaT🌴🌴MiniShELL🌴🌴🌴"
echo "🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴🌴";
echo ""
printf $RESET

# Récupération du prompt pour soustraction future
# PROMPT=$(echo -e "\nexit\n" | $MINISHELL_PATH | head -n 1 | sed "s/\x1B\[[0-9;]\{1,\}[A-Za-z]//g" )

# Helper commands:
REMOVE_COLORS="sed s/\x1B\[[0-9;]\{1,\}[A-Za-z]//g"
REMOVE_EXIT="grep -v ^exit$"

# Définition des fichiers temporaires en mémoire partagée
TMP_MINI_OUT=$(mktemp /dev/shm/minishell_test_out.XXXXX)
TMP_MINI_ERR=$(mktemp /dev/shm/minishell_test_err.XXXXX)
TMP_BASH_OUT=$(mktemp /dev/shm/bash_test_out.XXXXX)
TMP_BASH_ERR=$(mktemp /dev/shm/bash_test_err.XXXXX)
TMP_MINI_VAL=$(mktemp /dev/shm/mini_valgrind.XXXXXX)

# Nettoyage sur exit pour pas se casser la tête
trap	'rm -f "$TMP_MINI_OUT" "$TMP_MINI_ERR" "$TMP_BASH_OUT" "$TMP_BASH_ERR"; \
		chmod 666 ./test_files/invalid_permission; \
		rm -rf ./outfiles ./mini_outfiles ./bash_outfiles' EXIT


# Boucle principale
for testfile in ${test_lists[*]}; do

	printf $RED
	echo ———————————— $testfile
	printf "${BBLUE}"
	printf "      M I N I S H E L L :  ${BCYAN}O${BBLUE}utput ${BYELLOW}|${BBLUE} exit ${BCYAN}C${BBLUE}ode ${BYELLOW}|${BBLUE} ${BCYAN}D${BBLUE}iff outfiles ${BYELLOW}|${BBLUE} ${BCYAN}E${BBLUE}rror msg\n"
	printf "${BCYAN}"
	printf "      O C D E\n"

	printf $GREY
	while IFS= read -r teste || [[ -n $teste ]]; do
		((i++))

		rm -rf ./outfiles/* ./mini_outfiles/* ./bash_outfiles/* 2>/dev/null

		: > "$TMP_MINI_VAL"  # Vide le fichier
 
		if [[ "$VALGRIND" -eq 1 ]]; then
			(printf "%s\n" "$teste" | valgrind --leak-check=full --error-exitcode=99 --log-file="$TMP_MINI_VAL" env LANGUAGE=en LANG=en_US.UTF-8 LC_ALL=en_US.UTF-8 "$MINISHELL_PATH" >"$TMP_MINI_OUT" 2>"$TMP_MINI_ERR") 2> /dev/null
		else
			(printf "%s\n" "$teste" | LANGUAGE=en LANG=en_US.UTF-8 LC_ALL=en_US.UTF-8 "$MINISHELL_PATH" >"$TMP_MINI_OUT" 2>"$TMP_MINI_ERR") 2> /dev/null
		fi
		MINI_EXIT_CODE=$?
		MINI_VALGRIND_LOG=$(cat "$TMP_MINI_VAL")
		cp ./outfiles/* ./mini_outfiles &>/dev/null || true
		MINI_OUTPUT=$(cat "$TMP_MINI_OUT")
		MINI_ERROR_MSG=$(cat "$TMP_MINI_ERR" |  sed 's/.*: *//')

		rm -rf ./outfiles/* 2> /dev/null
		rm -rf ./bash_outfiles/* 2> /dev/null

		(printf "%s\n" "$teste" | LANGUAGE=en LANG=en_US.UTF-8 LC_ALL=en_US.UTF-8 bash --posix --noprofile --norc >"$TMP_BASH_OUT" 2>"$TMP_BASH_ERR") 2> /dev/null
		BASH_EXIT_CODE=$?
		cp ./outfiles/* ./bash_outfiles &>/dev/null || true
		BASH_OUTPUT=$(cat "$TMP_BASH_OUT")
		BASH_ERROR_MSG=$(cat "$TMP_BASH_ERR" |  sed 's/.*: *//')

		OUTFILES_DIFF=$(diff --brief ./mini_outfiles ./bash_outfiles 2>/dev/null)

		err_t=""
		if [ "$MINI_EXIT_CODE" != "$BASH_EXIT_CODE" ]; then
			test_prog_err "$MINI_EXIT_CODE" "$MINI_VALGRIND_LOG"
		fi

		if [[ "$ONLY_ICONS" -eq 0 ]]; then
			printf $YELLOW
			printf "Test %3s: " $i
			if [[ "$MINI_OUTPUT" == "$BASH_OUTPUT" && "$MINI_EXIT_CODE" == "$BASH_EXIT_CODE" && -z "$OUTFILES_DIFF" ]]; then
				printf ✅
				((ok++))
				if [ "$MINI_ERROR_MSG" != "$BASH_ERROR_MSG" ]; then
					printf "⚠️ "
				fi
			else
				printf ❌
			fi
		else
			printf $YELLOW
			printf "%4s: " $i
			if [[ "$MINI_OUTPUT" == "$BASH_OUTPUT" ]]; then
				printf "✅"
				((ok_output++))
			else
				printf "❌"
			fi
			if [[ "$MINI_EXIT_CODE" == "$BASH_EXIT_CODE" ]]; then
				printf "✅"
				((ok_code++))
			else
				printf "❌"
			fi
			if [[ -z "$OUTFILES_DIFF" ]]; then
				printf "✅"
				((ok_diff++))
			else
				printf "❌"
			fi
			if [[ "$MINI_ERROR_MSG" == "$BASH_ERROR_MSG" ]]; then
				printf "✅"
				((ok_error++))
			else
				printf "❌"
			fi
			if [[ "$MINI_EXIT_CODE" -eq 99 ]]; then
				printf "🥶 "
			fi
			if [[ -n "$err_t" ]]; then
				printf "💥 %b" "$err_t"
				((serious_errors++))
			fi
		fi


		 printf "%b%s%b\n" "$GREY" " $teste" "$END"
		if [ "$OUTFILES_DIFF" ] && [ "$ONLY_ICONS" -eq 0 ]; then
			echo "$OUTFILES_DIFF"
			echo minishell outfiles:
			cat ./mini_outfiles/* 2> /dev/null
			echo bash posix outfiles:
			cat ./bash_outfiles/* 2> /dev/null
		fi
		if [ "$MINI_OUTPUT" != "$BASH_OUTPUT" ] && [ "$ONLY_ICONS" -eq 0 ]; then
			echo minishell out = \($MINI_OUTPUT\)
			echo bash posix out = \($BASH_OUTPUT\)
		fi
		if [ "$MINI_EXIT_CODE" != "$BASH_EXIT_CODE" ] && [ "$ONLY_ICONS" -eq 0 ]; then
			echo minishell exit code = $MINI_EXIT_CODE
			echo bash posix exit code = $BASH_EXIT_CODE
		fi
		if [ "$MINI_ERROR_MSG" != "$BASH_ERROR_MSG" ] && [ "$ONLY_ICONS" -eq 0 ]; then
			echo minishell error = \($MINI_ERROR_MSG\)
			echo bash posix error = \($BASH_ERROR_MSG\)
		fi
		if [[ ("$MINI_OUTPUT" != "$BASH_OUTPUT" || "$MINI_EXIT_CODE" != "$BASH_EXIT_CODE" || -n "$OUTFILES_DIFF") && "$ONLY_ICONS" -eq 0 ]]; then
			echo ""
		fi
	done < $testfile
done

if [[ "$ONLY_ICONS" -eq 0 ]]; then
	printf $PURPLE
	printf $BOLD
	echo   $ok/$i
	printf $END

	if [[ "$ok" == "$i" ]]; then
		echo "🎊 🎊 🎊"
		echo "😎 😎 😎"
		echo "🎉 🎉 🎉"
		exit 0
	else
		echo "😭 😭 😭"
		exit 1
	fi
else
	printf $BYELLOW
	printf $BOLD
	printf "\nResults:\n\n"
	printf $PURPLE
	printf $BOLD
	if [[ "$ok_output" == "$i" ]]; then
		printf 😎
	else
		printf 😭
	fi
	echo   " $ok_output/$i (outputs)"
	if [[ "$ok_code" == "$i" ]]; then
		printf 😎
	else
		printf 😭
	fi
	echo   " $ok_code/$i (exit codes)"
	if [[ "$ok_diff" == "$i" ]]; then
		printf 😎
	else
		printf 😭
	fi
	echo   " $ok_diff/$i (outfiles)"
	if [[ "$ok_error" == "$i" ]]; then
		printf 😎
	else
		printf 😭
	fi
	echo   " $ok_error/$i (errors)"
	if [[ -z "$serious_errors" ]]; then
		printf "${BGREEN}😎 No critical error\n"
	else
		printf "🤯💥💥 $serious_errors (critical errors)\n"
	fi
	printf $END
fi