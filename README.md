# Tradutor

Esse trabalho está sendo implementado como requisito da disciplina Software Básico da Universidade de Brasília (UnB).
Ele consiste no processo de tradução (montador + ligador + carregador) do assembly inventado especificado a seguir.

## Assembly inventado
A linguagem de montagem utilizada será a linguagem simbólica hipotética apresentada em sala de aula.
Esta linguagem e formada por um conjunto de apenas 14 instruções.

<table>
  <tr><td colspan="5" align="center">                      <strong>Instruções</strong>                                 </td></tr>
  <tr>
    <td> <strong>Mneumônico</strong> </td>
    <td> <strong>Operandos</strong>  </td>
    <td> <strong>Código</strong>     </td>
    <td> <strong>Tamanho</strong>    </td>
    <td> <strong>Descrição</strong>  </td>
  </tr>
  <tr><td>     ADD    </td><td>     1     </td><td>    1   </td><td>     2    </td><td>      ACC <- ACC + MEM[OP]      </td></tr>
  <tr><td>     SUB    </td><td>     1     </td><td>    2   </td><td>     2    </td><td>      ACC <- ACC - MEM[OP]      </td></tr>
  <tr><td>     MULT   </td><td>     1     </td><td>    3   </td><td>     2    </td><td>      ACC <- ACC * MEM[OP]      </td></tr>
  <tr><td>     DIV    </td><td>     1     </td><td>    4   </td><td>     2    </td><td>      ACC <- ACC / MEM[OP]      </td></tr>
  <tr><td>     JMP    </td><td>     1     </td><td>    5   </td><td>     2    </td><td>             PC <- OP           </td></tr>
  <tr><td>    JMPN    </td><td>     1     </td><td>    6   </td><td>     2    </td><td>       Se ACC < 0, PC <- OP     </td></tr>
  <tr><td>    JMPP    </td><td>     1     </td><td>    7   </td><td>     2    </td><td>       Se ACC > 0, PC <- OP     </td></tr>
  <tr><td>    JMPZ    </td><td>     1     </td><td>    8   </td><td>     2    </td><td>       Se ACC = 0, PC <- OP     </td></tr>
  <tr><td>    COPY    </td><td>     2     </td><td>    9   </td><td>     3    </td><td>      MEM[OP2] <- MEM[OP1]      </td></tr>
  <tr><td>    LOAD    </td><td>     1     </td><td>    10  </td><td>     2    </td><td>         ACC <- MEM[OP]         </td></tr>
  <tr><td>    STORE   </td><td>     1     </td><td>    11  </td><td>     2    </td><td>         MEM[OP] <- ACC         </td></tr>
  <tr><td>    INPUT   </td><td>     1     </td><td>    12  </td><td>     2    </td><td>         MEM[OP] <- STDIN       </td></tr>
  <tr><td>    OUTPUT  </td><td>     1     </td><td>    13  </td><td>     2    </td><td>        STDOUT <- MEM[OP]       </td></tr>
  <tr><td>     STOP   </td><td>     0     </td><td>    14  </td><td>     1    </td><td>       Encerrar execução        </td></tr>
</table>

<table>
  <tr><td colspan="5" align="center">                      <strong>Diretivas</strong>                 </td></tr>
  <tr>
    <td> <strong>Mneumônico</strong> </td>
    <td> <strong>Operandos</strong>  </td>
    <td> <strong>Tamanho</strong>    </td>
    <td> <strong>Descrição</strong>  </td>
  </tr>
  <tr><td>   SECTION  </td><td>     1     </td><td>     0    </td><td>  Marcar inıcio de seção de
                                                                        codigo (TEXT) ou dados (DATA)  </td></tr>
  <tr><td>    SPACE   </td><td>    0/1    </td><td> Variável </td><td>  Reservar 1 ou mais enderecos
                                                                        de memoria nao-inicializada
                                                                        para armazenamento de uma
                                                                        palavra                        </td></tr>
  <tr><td>    CONST   </td><td>     1     </td><td>     1    </td><td>  Reservar memoria para
                                                                        armazenamento de uma constante
                                                                        inteira de 16 bits em base
                                                                        decimal ou hexadecimal         </td></tr>
  <tr><td>    EQU     </td><td>     1     </td><td>     0    </td><td>  Cria um sinônimo textual
                                                                        para um símbolo                </td></tr>
  <tr><td>    IF      </td><td>     1     </td><td>     0    </td><td>  Instrue  o  montador  a 
                                                                        incluir  a linha seguinte do
                                                                        codigo somente se o valor do
                                                                        operando for 1                 </td></tr>
  <tr><td>    BEGIN   </td><td>     0     </td><td>     0    </td><td>  Início de um módulo            </td></tr>
  <tr><td>    END     </td><td>     0     </td><td>     0    </td><td>  Fim de um módulo               </td></tr>
  <tr><td>    EXTERN  </td><td>     0     </td><td>     0    </td><td>  Indica que o rótulo é definido
                                                                        em um módulo externo           </td></tr>
  <tr><td>    PUBLIC  </td><td>     0     </td><td>     0    </td><td>  Indica que o rótulo é público
                                                                        para outros módulos            </td></tr>
</table>

## Especificação

### Montador

Deve-se implementar um montador do assembly inventado descrito acima (não há tratamento de macros).

Além disso, o mesmo deve receber de 1 a 3 programas de entrada (ex.:  ./montador prog1.asm prog2.asm prog3.asm).
Assim, se um unico programa foi passado como entrada então o mesmo não deve ter as diretivas BEGIN e END.
Já se 2 ou 3 programas foram passados como entrada, as diretivas BEGIN e END são obrigatorias.
Checar se as regras acima aplicam-se é o ́unico teste de erro que o montador deve fazer.

Por fim, o arquivo objeto (.o) gerado deve conter o cabeçalho indicado pela letra H (contendo nome, tamanho e informações de realocação do código), a Tabela de Uso indicada pelas letras TU e a Tabela de Definições indicada pelas letras TD.

### Ligador

Recebe como entrada o(s) arquivo(s) objeto(s) gerador pelo montador.
Deve-se realizar o processo de ligação dada a ordem dos arquivos passados gerando um único arquivo executável.
O arquivo executável não deve ter extensão e deve conter um cabeçalho cobtebdo nome, tamanho e informações de realocação.

### Carregador

Recebe como entrada o arquivo executável gerado pelo ligador e uma sequência de números.
Nessa sequência de números, o primeiro representa a quantidade de chunks, os próximos indicam o tamanho em bytes de cada chunk e os últimos os endereços iniciais de cada chunk.

Assim, o carregador deve simular a execução do programa e depois tentar gerar um arquivo de saída de imagem de memória.
Para gerar esse arquivo leva-se em conta a sequência de números descrita acima.
Cabe ao carregador verificar se um desses chunks é suficiente para suportar o programa inteiro.
Se não for, deve-se verificar se ́e possıvel entao dividir o programa em diferentes chunks.
Caso, não seja possível, o carregador deve indicar uma mensagem de: “OUT OF MEMORY - YOUR PROGRAM WILL NOT BE LOADED”.
Caso contrario, o carregador deve modificar a informação relativa para os enderecos corretos e colocar o codigo final (sem cabecalho) em um arquivo com extensão ".im".

## Como compilar

### Todos os executáveis no modo padrão:

    $ make

### Todos os executáveis no modo de depuração:

    $ make debug
    
### Todos os executáveis no modo de otimização:

    $ make release

## Como limpar os arquivos gerados de compilação

    $ make clean

## Como gerar a documentação
  
    $ make doc

## Como limpar os arquivos gerados de documentação

    $ make clean

## Como executar

### Montador

    $ ./assembler/montador [<programa>.asm]

### Ligador

    $ ./linker/ligador [<programa>.asm]

### Carregador

    $ ./loader/carregador <executável> <quantidade de chunks> [<tamanho de cada chunk>] [<endereço inicial de cada chunk>]

## Dependências

### Bison e Flex

Esse projeto depende do Bison e do Flex. O uso de ambos foi autorizado pelo professor da disciplina e ambos geralmente já vem incluídos nos pacotes de utilitários essenciais (build essentials).

Ressalta-se a necessidade do Bison com versão 3.o ou superior e do Flex com versão 2.5 ou superior. Por serem utilitários GNU padrões, se necessário é possível atualizar ambos de forma padrão (com apt-get em distribuições Ubuntu, por exemplo).

Observa-se que os códigos gerados pelo Bison e pelo Flex não são tão legíveis como os códigos feitos a mão. Desta forma, para melhor entendimento dos arquivos scanner e parser recomenda-se analisar os arquivos do diretório generate (são eles que definem as regras para geração dos códigos Flex e Bison).

### GCC

Esse projeto necessita do compilador GCC na versão 5.x ou maior.

## Diretrizes do projeto

O código será escrito com o padrão "Camel Case". 
Como padrão, as variáveis, abstrações e nomes no código são em inglês, bem como as mensagens de saída. Já toda a documentação foi feita em português. 

A linguagem de programação utilizada é o C++ e o projeto segue diretrizes (*[Guidelines][GUIDELINES]*)
para programar nessa linguagem feitas pelos desenvolvedores da Google.

## Agradecimentos

Em nossos estudos para entender o Bison e o Flex, nos deparamos com dois repositórios interessantes (*[aqui][REPO1]* e *[aqui][REPO2]*). Nesse percurso, ambos nos inspiraram e nos forneceram uma base inicial de partida. Portanto, gostaríamos de deixar aqui o nosso agradecimento aos autores desse repositórios!

[GUIDELINES]: https://google.github.io/styleguide/cppguide.html
[REPO1]: https://github.com/jonathan-beard/simple_wc_example
[REPO2]: https://github.com/ezaquarii/bison-flex-cpp-example

