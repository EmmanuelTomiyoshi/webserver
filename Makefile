NAME 		=	webserv

SRC 		=	main.cpp			\
				IPResolver.cpp		\
				Server.cpp			\
				Config.cpp			\
				Route.cpp			\
				Methods.cpp			\
				config_test.cpp		\
				Configs.cpp			\
				Utils.cpp			\
				File.cpp			\
				ft.cpp				\
				Response.cpp		\
				CGI.cpp				\
				Request2.cpp		\
				Timeout.cpp			\
				save_request.cpp	\
				test_request.cpp	\
				ErrorPages.cpp		\
				signal_stop.cpp		\
				Memory.cpp			\
				CustomData.cpp		\
				BuffList.cpp		\

VPATH		=	./src				\
				./src/utils			\
				./src/socket		\
				./src/config		\
				./src/includes		\
				./src/tests			\

CXX 		=	c++
CXXFLAGS 	=	-Wall -Wextra -Werror -std=c++98 -g
CPPFLAGS 	=	-MMD -MP -I ./src/includes
BUILD_DIR 	=	build

OBJ 		=	$(addprefix $(BUILD_DIR)/, $(SRC:.cpp=.o))
DEP 		=	$(addprefix $(BUILD_DIR)/, $(SRC:.cpp=.d))

VALGRIND 	=	valgrind -q --leak-check=full --show-leak-kinds=all

all: $(NAME)

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(NAME): $(BUILD_DIR) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)
	@echo "$(NAME) has compiled succesfully"

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@echo "Removing all objects"
	$(RM) $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d

fclean: clean
	@echo "Removing $(NAME)"
	$(RM) *.out $(NAME)
	$(RM) -rf $(BUILD_DIR)
	$(RM) -rf client

re: fclean all

rr: re
	@./$(NAME)

push:
	git push origin tomatte

leaks: $(NAME)
	$(VALGRIND) ./$(NAME) a

client:
	g++ ./src/client.cpp -o client

-include $(DEP)

.PHONY: all clean fclean re rr push leaks client