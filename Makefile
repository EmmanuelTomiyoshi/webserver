NAME 		=	webserver

SRC 		=	main.cpp \
				IPResolver.cpp \
				Request.cpp \
				Server.cpp \
				Config.cpp \
				Route.cpp \
				Methods.cpp \
				config_test.cpp \

VPATH		=	./src \
				./src/parsing \
				./src/utils \
				./src/socket \
				./src/config \

CXX 		=	c++
# CXXFLAGS 	=	-Wall -Wextra -Werror -std=c++98 -g
CPPFLAGS 	=	-MMD -MP -I ./src/includes
BUILD_DIR 	=	build

OBJ 		=	$(addprefix $(BUILD_DIR)/, $(SRC:.cpp=.o))
DEP 		=	$(addprefix $(BUILD_DIR)/, $(SRC:.cpp=.d))

VALGRIND 	=	valgrind -q --leak-check=full --show-leak-kinds=all

NUMBER_SRC_FILES	=	$(words $(SRC))
PROGRESS			=	0

all: $(NAME)

$(BUILD_DIR)/%.o: %.cpp
	@echo -n "$(COLOR_YELLOW)Compiling $(NAME) $(COLOR_WHITE)$$(( \
	$(PROGRESS) * 100 / $(NUMBER_SRC_FILES)))%\r"
	$(eval PROGRESS=$(shell echo $$(($(PROGRESS)+1))))
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(NAME): $(BUILD_DIR) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)
	@echo "$(COLOR_GREEN)$(NAME) has compiled succesfully\
	$(COLOR_WHITE)"

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@echo "$(COLOR_BLUE)Removing all objects$(COLOR_WHITE)"
	$(RM) $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d

fclean: clean
	@echo "$(COLOR_BLUE)Removing $(NAME)$(COLOR_WHITE)"
	$(RM) *.out $(NAME)
	$(RM) -rf $(BUILD_DIR)
	$(RM) -rf client

re: fclean all

rr: re
	@./webserver

push:
	git push origin tomatte

leaks: $(NAME)
	$(VALGRIND) ./$(NAME) a

client:
	g++ ./src/client.cpp -o client

-include $(DEP)

.PHONY: all clean fclean leaks