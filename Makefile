# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nfordoxc <nfordoxc@42luxembourg.lu>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/16 08:27:57 by nfordoxc          #+#    #+#              #
#    Updated: 2026/08/30 15:55:06 by nfordoxc         ###   Luxembourg.lu      #
#                                                                              #
# **************************************************************************** #

################################################################################
#	Makefile variables														   #
################################################################################

CXX				=	c++
CXXFLAGS		=	-Wall -Werror -Wextra -g -O3 -MMD
CXX_OPT			=	-std=c++20

DEB				=	valgrind
DEB_OPT			=	--tool=memcheck \
					--leak-check=full \
					--show-leak-kinds=all \
					--track-origins=yes \
					--track-fds=yes \
					--trace-children=yes

ARG0			=	

ARG1			=	

ARG2			=	

ARG3			=	

ARG4			=	

RM				=	rm -f

################################################################################
#	Mandatory part															   #
################################################################################

CONTROLLER		=	./controller/TournamentController.cpp

CLASS			=	./class/Match.cpp \
					./class/Player.cpp \
					./class/Phase.cpp \
					./class/Pool.cpp \
					./class/Settings.cpp \
					./class/Team.cpp \
					./class/Tournament.cpp

FACTORY			=	./factory/TeamFactory.cpp

MANAGER			=	./manager/PhaseManager.cpp \
					./manager/PoolManager.cpp \
					./manager/TeamManager.cpp \
					./manager/PlayerManager.cpp

CLI				=	./cli/CLIUtils.cpp \
					./cli/MatchCLI.cpp \
					./cli/PlayerCLI.cpp \
					./cli/PhaseCLI.cpp \
					./cli/PoolCLI.cpp \
					./cli/SettingsCLI.cpp \
					./cli/TeamCLI.cpp \
					./cli/TournamentCLI.cpp

UTILS			=	./utils/CheckerCSV.cpp \
					./utils/Exporter.cpp \
					./utils/Importer.cpp \
					./utils/FormatUtils.cpp \
					./utils/PrintUtils.cpp \
					./utils/SettingsChecker.cpp \
					./utils/TournamentHistory.cpp

VIEWER			=	./viewer/MatchViewer.cpp \
                    ./viewer/PlayerViewer.cpp \
                    ./viewer/PhaseViewer.cpp \
                    ./viewer/PoolViewer.cpp \
                    ./viewer/SettingsViewer.cpp \
                    ./viewer/TeamViewer.cpp \
                    ./viewer/TitleViewer.cpp \
                    ./viewer/TournamentViewer.cpp


SRC				=	./src/main.cpp \
					$(CONTROLLER) \
					$(CLASS) \
					$(FACTORY) \
					$(MANAGER) \
					$(CLI) \
					$(UTILS) \
					$(VIEWER)

OBJ				=	$(SRC:.cpp=.o)

DEP				=	$(SRC:.cpp=.d)

NAME			=	Tournament2000

################################################################################
#	Tester part
################################################################################

SRC_TESTER		=	
BONUS_TESTER	=	

OBJ_TESTER		=	$(SRC_TESTER:.cpp=.o)
OBJ_BONUS_TESTER=	

TESTER			=	
TESTER_BONUS	=	

################################################################################
#	Colors																	   #
################################################################################

RESET			=	\033[0m

# Regular

BLACK			=	\033[0;30m
RED				=	\033[0;31m
GREEN			=	\033[0;92m
YELLOW			=	\033[0;93m
BLUE			=	\033[0;94m
PURPLE			=	\033[0;95m
CYAN			=	\033[0;96m
WHITE			=	\033[0;97m

# Bold

BBLACK			=	\033[1;30m
BRED			=	\033[1;31m
BGREEN			=	\033[1;92m
BYELLOW			=	\033[1;93m
BBLUE			=	\033[1;94m
BPURPLE			=	\033[1;95m
BCYAN			=	\033[1;96m
BWHITE			=	\033[1;97m

# Blink

CBLACK			=	\033[5;30m
CRED			=	\033[5;31m
CGREEN			=	\033[5;32m
CYELLOW			=	\033[5;93m
CBLUE			=	\033[5;34m
CPURPLE			=	\033[5;35m
CCYAN			=	\033[5;36m
CWHITE			=	\033[5;97m

################################################################################
#	Progress bar															   #
################################################################################

CURRENT_FILE	= 	0
NB_SRC			=	$(words $(SRC))
NB_BONUS		=	$(words $(SRC_BONUS))
NB_TESTER		=	$(words $(SRC_TESTER))
NB_TESTER_BONUS	=	$(words $(BONUS_TESTER))
SLEEP_TIME		=	0.001

################################################################################
#	Functions
################################################################################

define compile_c_to_o
	@$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE)+1))))
	@$(CXX) $(CXXFLAGS) $(CXX_OPT) $(CXX_DEF) -c $< -o $@
	@printf "$(BLUE)\r\033[K\033[0KCompiling files => $< ["
	@for i in $$(seq 0 $$(($(CURRENT_FILE)*100/$(NUM_FILES)))); do \
		printf "$(YELLOW)="; \
	done
	@printf "$(BBLUE)] $(CURRENT_FILE)/$(NUM_FILES)$(RESET)"
	@if [ $(CURRENT_FILE) -eq $(NUM_FILES) ]; then \
		echo ; \
	fi
	@$(eval CURRENT_FILE=$(if \
		$(filter $(CURRENT_FILE), $(NUM_FILES)), 0, $(CURRENT_FILE)))
endef

define delete_progress
	@$(RM) $(1) > /dev/null 2>&1
	@for i in $$(seq 100 -1 0); do \
		printf "\r\033[K\033[0K"; \
		if [ $$((i % 4)) -eq 0 ]; then \
			printf "$(BRED)DELETE - ["; \
		elif [ $$((i % 4)) -eq 1 ]; then \
			printf "$(BRED)DELETE / ["; \
		elif [ $$((i % 4)) -eq 2 ]; then \
			printf "$(BRED)DELETE | ["; \
		else \
			printf "$(BRED)DELETE \\ ["; \
		fi; \
		for j in $$(seq 0 $$i); do \
			printf "="; \
		done; \
		printf " %d/100 ]$(RESET)" $$i; \
		sleep $(SLEEP_TIME); \
	done
	@printf "\r\033[K\033[0K$(GREEN)All files $(1) of $(NAME) deleted$(RESET)\n"
endef

define delete_file
	@$(RM) $(1) > /dev/null 2>&1
	@printf "\r\033[K\033[0K$(GREEN)$(1) deleted$(RESET)\n"
endef

################################################################################
#	Rules																	   #
################################################################################

.PHONY : all $(NAME) deb clean fclean re help

all: 		$(NAME)

$(NAME):	NUM_FILES=$(NB_SRC)
$(NAME):	$(OBJ)
	@$(CXX) $(CXXFLAGS) $(CXX_OPT) $(OBJ) $(MYLIBS) -o $(NAME)
	@echo "\n$(CGREEN)The programm $(NAME) successfully compiled$(RESET)"

%.o :		%.cpp
	$(call compile_c_to_o)

deb:			all
	@echo "$(RESET)$(BBLUE)==========	RUN DEBUG MODE	==========$(RESET)"
	$(DEB) $(DEB_OPT) ./$(NAME) $(ARG0) $(DEB_OUT)

clean:
	$(call delete_progress, ./src/*.o)
	$(call delete_progress, ./class/*.o)
	$(call delete_progress, ./controller/*.o)
	$(call delete_progress, ./factory/*.o)
	$(call delete_progress, ./manager/*.o)
	$(call delete_progress, ./cli/*.o)
	$(call delete_progress, ./utils/*.o)
	$(call delete_progress, ./viewer/*.o)
	$(call delete_progress, ./src/*.d)
	$(call delete_progress, ./class/*.d)
	$(call delete_progress, ./controller/*.d)
	$(call delete_progress, ./factory/*.d)
	$(call delete_progress, ./manager/*.d)
	$(call delete_progress, ./cli/*.d)
	$(call delete_progress, ./utils/*.d)
	$(call delete_progress, ./viewer/*.d)

fclean: 	clean
	$(call delete_file, $(NAME))

re : 		fclean all

help:
	@echo "usage: make"
	@echo "usage: make deb for running with valgrind"

-include $(DEP)
