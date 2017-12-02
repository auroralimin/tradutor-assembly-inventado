# Regras possíveis
TOPTARGETS := all debug clean release generate

# Lista de subdiretórios
SUBDIRS := $(wildcard */)
SUBDIRS := $(filter-out doc/, $(SUBDIRS))
SUBDIRS := $(filter-out shared_include/, $(SUBDIRS))

# Nome do diretório na qual a documentação será gerada
DOC_PATH = doc/

# Regra para executar comandos make toplevel
$(TOPTARGETS): $(SUBDIRS)

# Regra para executar comandos para cada subdiretório
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

# Regra para gerar a documentação com Doxygen
doc:
	doxygen Doxyfile

# Regra limpar a documentação gerada
doc-clean:
	rm -rf  $(DOC_PATH)

# Regra para printar uma variável no terminal. Auxilia no debug do Makefile
print-% : ; @echo $* = $($*)

# Regra que torna as dependências "phony"
.PHONY: $(TOPTARGETS) $(SUBDIRS) doc doc-clean

