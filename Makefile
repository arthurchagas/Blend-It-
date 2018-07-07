RM = del
MAIN = Blender.exe
flags = -Wall
TARGET_O = main.o Botao.o AVL.o Util.o Cor.o
LINKER_INCLUDE = -Llib -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
PREPROCESSOR_INCLUDE = -Iinclude

all: $(TARGET_O)
	g++ -o $(MAIN) $(TARGET_O) $(flags) $(LINKER_INCLUDE)
	.\$(MAIN)

main.o:
	g++ -c main.cpp $(flags) $(PREPROCESSOR_INCLUDE)

Botao.o:
	g++ -c src/Botao.cpp $(flags) $(PREPROCESSOR_INCLUDE)

AVL.o:
	g++ -c src/AVL.cpp $(flags) $(PREPROCESSOR_INCLUDE)

Util.o:
	g++ -c src/Util.cpp $(flags) $(PREPROCESSOR_INCLUDE)

Cor.o:
	g++ -c src/Util/Cor.cpp $(flags) $(PREPROCESSOR_INCLUDE)

clean:
	$(RM) $(MAIN) $(TARGET_O)