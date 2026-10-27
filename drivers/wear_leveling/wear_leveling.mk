ifndef WEAR_LEG
	WEAR_LEG := ../drivers/wear_leveling
endif

C_SOURCES +=\
    ${WEAR_LEG}/wear_leveling_legacy.c \
    ${WEAR_LEG}/fnv/hash_32.c \
    ${WEAR_LEG}/fnv/hash_32a.c \
    ${WEAR_LEG}/fnv/hash_64.c \
    ${WEAR_LEG}/fnv/hash_64a.c \
    ${WEAR_LEG}/fnv/test_fnv.c \
    ${WEAR_LEG}/fnv/qmk_fnv_type_validation.c

C_INCLUDES +=\
	-I${WEAR_LEG}	\
    -I${WEAR_LEG}/fnv