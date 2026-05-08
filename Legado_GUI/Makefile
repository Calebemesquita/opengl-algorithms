# 1. Detecta o Sistema Operacional
ifeq ($(OS),Windows_NT)
    UNAME_S := Windows
else
    UNAME_S := $(shell uname -s)
endif

# Nome amigável do SO para exibição
OS_LABEL := $(UNAME_S)
ifeq ($(UNAME_S),Darwin)
    OS_LABEL := MacOS
endif

.PHONY: all lite clean help

# 2. Separação dos arquivos fonte (.cpp)
# Arquivos base (usados em todos os projetos)
SRCS_BASE = main.cpp bib/Camera.cpp bib/CameraDistante.cpp bib/CameraJogo.cpp bib/Desenha.cpp bib/Vetor3D.cpp bib/Transform.cpp gui_glut/extra.cpp gui_glut/gui.cpp gui_glut/OpenTextures.cpp

# Nome do executável (pode ser sobrescrito: make EXEC=meu_programa)
EXEC ?= GLUTdoZero20261

# Arquivos pesados (usados apenas quando tem modelo 3D)
SRC_ASSIMP = bib/Model3D.cpp

# 3. Configurações específicas por Sistema Operacional
ifeq ($(UNAME_S),Windows)
    # Flags para Windows (MSYS2/MinGW)
    OUT = run/$(EXEC).exe
    INCLUDES = -I bib -I gui_glut
    LIBS_BASE = -lopengl32 -lglu32 -lfreeglut -lIL
    LIB_ASSIMP = -lassimp
endif

ifeq ($(UNAME_S),Linux)
    # Flags para Ubuntu/Linux
    OUT = run/$(EXEC)
    INCLUDES = -I bib -I gui_glut
    LIBS_BASE = -lGL -lGLU -lglut -lIL
    LIB_ASSIMP = -lassimp
endif

ifeq ($(UNAME_S),Darwin)
    # Flags para macOS (usando o framework GLUT nativo da Apple)
    OUT = run/$(EXEC)
    INCLUDES = -I bib -I gui_glut -I/opt/homebrew/include -I mac -Wno-deprecated-declarations -Wno-macro-redefined
    LIBS_BASE = -L/opt/homebrew/lib -framework OpenGL -framework GLUT -lIL
    LIB_ASSIMP = -lassimp
endif



# 4. Regras de compilação
# Regra padrão: Compila TUDO (Basta digitar 'make')
all:
	@echo ""
	@echo "==========   Compilando para o sistema operacional: $(OS_LABEL)"
	@echo ""
	g++ -o $(OUT) $(SRCS_BASE) $(SRC_ASSIMP) $(INCLUDES) $(LIBS_BASE) $(LIB_ASSIMP)
	@echo ""
	@echo "==========   Sucesso! O executavel foi criado em: $(OUT)"
	@echo "==========   Digite './$(OUT)' para executar o programa"
	@echo ""
	@echo "==========   Digite 'make help' para ver mais opcoes"
	@echo ""

# Regra rápida: Compila SEM Assimp e SEM Model3D.cpp (Digitar 'make lite')
lite:
	@echo ""
	@echo "==========   Compilando versao leve para o sistema operacional: $(OS_LABEL)"
	@echo ""
	g++ -o $(OUT) $(SRCS_BASE) $(INCLUDES) $(LIBS_BASE)
	@echo ""
	@echo "==========   Sucesso! O executavel foi criado em: $(OUT)"
	@echo "==========   Digite './$(OUT)' para executar o programa"
	@echo ""
	@echo "==========   Digite 'make help' para ver mais opcoes"
	@echo ""

# Regra de limpeza: Apaga o executavel antigo (Digitar 'make clean')
clean:
	@echo ""
	@echo "==========   Limpando o executavel: $(OUT)"
	@echo ""
	rm $(OUT)
	@echo ""
	@echo "==========   Executavel limpo: $(OUT)"
	@echo "==========   Digite 'make help' para ver mais opcoes"
	@echo ""

# Ajuda: Mostra os alvos disponíveis e exemplos de uso (Digitar 'make help')
help:
	@echo ""
	@echo "==========   Ajuda para o Makefile do projeto de computacao grafica"
	@echo ""
	@echo "Alvos disponiveis:"
	@echo "  make                 -> Compila tudo (com Assimp)"
	@echo "  make lite            -> Compila versao leve (sem Assimp)"
	@echo "  make clean           -> Remove o executavel gerado"
	@echo "  make help            -> Mostra esta ajuda"
	@echo ""
	@echo "Personalizacao do nome do executavel:"
	@echo "  make EXEC=meu_programa"
	@echo "  make lite EXEC=meu_programa"
	@echo "  make clean EXEC=meu_programa"
	@echo ""
	@echo "==========   Escolha o alvo que melhor se encaixa no seu cenario e boa sorte com o projeto!"
	@echo ""
