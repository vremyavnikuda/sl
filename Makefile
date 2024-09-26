CXX = g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g -Iglobal
LFLAGS = -lmpg123 -lao -lstdc++fs -lncurses

SRC := main.cpp service/Music.cpp ui/Ui.cpp


MAIN := console_player

all: $(MAIN)
	@echo "Сборка завершена!"

$(MAIN): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(MAIN) $(SRC) $(LFLAGS)

clear:
	rm -f $(MAIN)
	@echo "Очистка завершена!"

run: all
	./$(MAIN)
	@echo "Запуск приложения завершен!"
