.code
	choose_opponent_card proc
	; extern "C" signed char choose_opponent_card(signed char* opponent_cards, signed char* eventual_cards, short count_of_check_opponent);
	; Параметры:
	;	RCX - Адрес массива карт противника
	;	RDX - Адрес массива предполагаемых карт игрока
	; Переменные:
	;	R8 - Текущее количество проверок
	;	R9 - Буфер хранения индекса наибольшего числа карт
	;	R10 - Буфер хранения текущее число карт
	;	R11 - Буфер хранения наибольшего числа карт
	;	R12 - Проверяемое число карт (3, 2, 1)

	push r12

	xor r9, r9
	mov r11, 0
	mov r10, 0

	CheckNinePriorInit:
		xor rax, rax
		xor r12, r12
	CheckNinePrior:	
		cmp al, 13
		je CheckOnePriorInit

		mov r10b, byte ptr [rcx + rax]
		mov r12b, byte ptr [rdx + rax]

		cmp r10b, 0
		jle NextNinePrior

		cmp r12b, 9
		jge ExitLoop

	NextNinePrior:
		inc rax								
		jmp CheckNinePrior

	CheckOnePriorInit:
		xor rax, rax
		xor r12, r12
	CheckOnePrior:	
		cmp al, 13
		je CheckThreeCardsInit

		mov r10b, byte ptr [rcx + rax]
		mov r12b, byte ptr [rdx + rax]

		cmp r10b, 0
		jle NextOnePrior

		cmp r12b, 1
		jge ExitLoop

	NextOnePrior:
		inc rax								
		jmp CheckOnePrior

	; Проверка колоды на тройки
	CheckThreeCardsInit:
		xor rax, rax
	CheckThreeCards:
		cmp al, 13							; Проверяем, проверили ли мы всю колоду на тройки
		je CheckTwoCardsInit				; Если да, переходим к проверке на двойки

		mov r10b, byte ptr [rcx + rax]		; Берём следующий элемент из массива
		
		cmp r10b, 3							; Проверяем, равняется ли элемент трём
		jne NextThreeCard					; Если нет, переходим к следующему
		
		cmp r8, 0							; Проверяем, проверяли ли мы эту тройку
		je ExitLoop							
		dec r8

	NextThreeCard:
		inc rax								
		jmp CheckThreeCards

	; Проверка колоды на двойки
	CheckTwoCardsInit:
		xor rax, rax
	CheckTwoCards:

		cmp al, 13							; Проверяем, проверили ли мы всю колоду на двойки
		je CheckOneCardInit					; Если да, переходим к проверке на одну карту

		mov r10b, byte ptr [rcx + rax]		; Берём следующий элемент из массива

		cmp r10b, 2							; Проверяем, равняется ли элемент двум
		jne NextTwoCard						; Если нет, переходим к следующему

		cmp r8, 0							; Проверяем, проверяли ли мы эту двойку
		je ExitLoop									
		dec r8

	NextTwoCard:
		inc rax
		jmp CheckTwoCards

	; Проверка колоды на одну карту
	CheckOneCardInit:
		xor rax, rax
	CheckOneCard:

		cmp al, 13							; Проверяем, проверили ли мы всю колоду на одну карту
		je CheckMaxCardInit						; Если да, переходим к проверке на одну карту

		mov r10b, byte ptr [rcx + rax]		; Берём следующий элемент из массива

		cmp r10b, 1							; Проверяем, равняется ли элемент единице
		jne NextOneCard						; Если нет, переходим к следующему

		cmp r8, 0							; Проверяем, проверяли ли мы эту единицу
		je ExitLoop
		dec r8

	NextOneCard:
		inc rax
		jmp CheckOneCard

	CheckMaxCardInit:
		xor rax, rax
		xor r11, r11
	CheckMaxCard:

		cmp al, 13
		je ExitLoopMaxCard

		mov r10b, byte ptr [rcx + rax]

		cmp r10b, r11b
		jg ChangeMaxCard

	NextMaxCard:
		inc rax
		jmp CheckMaxCard

	ChangeMaxCard:
		mov r11b, r10b
		mov r9b, al
		jmp NextMaxCard

	ExitLoop:
		pop r12
		ret

	ExitLoopMaxCard:
		mov al, r9b
		pop r12
		ret

	choose_opponent_card endp
end