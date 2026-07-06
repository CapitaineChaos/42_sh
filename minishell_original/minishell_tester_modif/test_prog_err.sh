#test_prog_err.sh

manage_abort_error() {
	if [ -z "$err_t" ]; then
		err_t="${BBLUE}$2${RESET}"
	else
		err_t+=" | ${BBLUE}$2${RESET}"
	fi
}

manage_valgrind_error() {
	if [ -z "$err_t" ]; then
		err_t="${BPURPLE}$2${RESET}"
	else
		err_t+=" | ${BPURPLE}$2${RESET}"
	fi
}

manage_base_error() {
	if [ -z "$err_t" ]; then
		err_t="${BRED}$2${RESET}"
	else
		err_t+=" | ${BRED}$2${RESET}"
	fi
}

test_prog_err() {

	if [[ "$1" -eq 99 ]]; then

		local log_file="$2"
		local nb_valgrind_errors=0

		# Vérifier s'il y a une erreur d'accès mémoire invalide (lecture/écriture hors limites)
		if grep -iq "Invalid read" "$log_file" 2>&- || grep -iq "Invalid write" "$log_file" 2>&-; then
			manage_valgrind_error "INVALID MEMORY ACCESS ERROR" "M"
		fi

		# Vérifier s'il y a des fuites de mémoire définitives
		if grep -iq "definitely lost:" "$log_file" 2>&-; then
			manage_valgrind_error "MEMORY LEAK ERROR" "L"
		fi

		# Vérifier l'utilisation de valeurs non initialisées
		if grep -iq "Conditional jump or move depends on uninitialised value" "$log_file" 2>&-; then
			manage_valgrind_error "UNINITIALIZED VALUE ERROR" "U"
		fi

		# Vérifier les erreurs de libération (double free, invalid free, etc.)
		if grep -iq "Invalid free" "$log_file" 2>&- || grep -iq "mismatched free" "$log_file" 2>&-; then
			manage_valgrind_error "FREE ERROR" "F"
		fi

		# Vérifier les erreurs d'utilisation après libération (use-after-free)
		if grep -iq "use-after-free" "$log_file" 2>&-; then
			manage_valgrind_error "USE AFTER FREE ERROR" "A"
		fi

		# Vérifier les erreurs de chevauchement mémoire (overlap), par exemple lors d'un memcpy avec chevauchement
		if grep -iq "Source and destination overlap" "$log_file" 2>&-; then
			manage_valgrind_error "MEMORY OVERLAP ERROR" "P"
		fi

		# Vérifier les erreurs liées aux paramètres système invalides
		if grep -iq "Syscall param" "$log_file" 2>&-; then
			manage_valgrind_error "SYSCALL PARAMETER ERROR" "C"
		fi

		# Stack overflow detection
		if grep -iq "stack overflow" "$log_file" 2>&-; then
			manage_valgrind_error "STACK OVERFLOW ERROR" "O"
		fi

		# Stack smashing detected
		if grep -iq "stack smashing detected" "$log_file" 2>&-; then
			manage_valgrind_error "STACK SMASHING ERROR" "S"
		fi

		# Heap corruption errors
		if grep -iq "heap corruption" "$log_file" 2>&-; then
			manage_valgrind_error "HEAP CORRUPTION ERROR" "H"
		fi

		# Race condition errors (reporting from Helgrind/DRD, par exemple)
		if grep -iq "Race condition" "$log_file" 2>&-; then
			manage_valgrind_error "RACE CONDITION ERROR" "R"
		fi

		# Invalid jump errors
		if grep -iq "Invalid jump" "$log_file" 2>&-; then
			manage_valgrind_error "INVALID JUMP ERROR" "J"
		fi

	elif [ "$1" -eq 124 ]; then

		manage_base_error "TIMEOUT" "T"
		ret_err=true

	elif [ "$1" -eq 126 ]; then

		manage_base_error "NOT EXECUTABLE" "X"

	elif [ "$1" -eq 127 ]; then

		manage_base_error "FILE NOT EXISTS" "N"

	elif [ "$1" -eq 130 ]; then

		manage_base_error "INTERRUPTED (SIGINT)" "C"

	elif [ "$1" -eq 132 ]; then

		manage_base_error "ILLEGAL INSTRUCTION (SIGILL)" "L"

	elif [ "$1" -eq 134 ]; then

		local log_file="$2"

		# Vérifier les erreurs de pointeur invalide, par exemple lors d'un appel à munmap_chunk()
		if grep -iq "munmap_chunk(): invalid pointer" "$log_file" 2>&-; then
			manage_abort_error "INVALID POINTER ERROR" "P"
		fi

		# Vérifier si le message "IOT instruction" apparaît (cas typique d'un abort via SIGABRT ou SIGIOT)
		if grep -iq "IOT instruction" "$log_file" 2>&-; then
			manage_abort_error "IOT INSTRUCTION ERROR" "I"
		fi

		# Vérifier si le système détecte un "stack smashing" (pile corrompue)
		if grep -iq "stack smashing detected" "$log_file" 2>&-; then
			manage_abort_error "STACK SMASHING ERROR" "S"
		fi

	elif [ "$1" -eq 135 ]; then
		# SIGBUS
		manage_base_error "BUS ERROR (SIGBUS)" "B"

	elif [ "$1" -eq 136 ]; then
		# SIGFPE
		manage_base_error "FLOATING POINT EXCEPTION (SIGFPE)" "F"

	elif [ "$1" -eq 137 ]; then
		# SIGKILL
		manage_base_error "KILLED (SIGKILL)" "K"

	elif [ "$1" -eq 139 ]; then
		# SIGSEGV
		manage_base_error "SEGFAULT (SIGSEGV)" "S"

	elif [ "$1" -eq 143 ]; then
		# SIGTERM
		manage_base_error "TERMINATED (SIGTERM)" "T"

	elif [ "$1" -ge 129 ] && [ "$1" -le 192 ]; then
		# SIG
		signal_num=$(( $1 - 128 ))
		manage_base_error "TERMINATED BY $signal_num SIGNAL" "?"
	fi


}

