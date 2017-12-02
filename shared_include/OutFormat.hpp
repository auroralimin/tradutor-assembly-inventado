/**
 * @file
 */
#ifndef OUT_FORMAT_HPP
#define OUT_FORMAT_HPP

/**
 * @def BOLD
 * @brief Macro que defini a sequência para se printar em negrito no cout
 */
#define BOLD      "\e[1m"

/**
 * @def OFF
 * @brief Macro que defini a sequência para se printar normal no out
 */
#define OFF       "\e[0m"

/**
 * @def COLOR(id)
 * @brief Macro que gera a sequência para se printar em negrito e com cor
 */
#define COLOR(id) "\033[1;3" + std::to_string(id) + "m"

/**
 * @def UNUSED_VAR
 * @brief Macro para suprimir avisos de variável não utilizada
 */
#define UNUSED_VAR (void)

/**
 * @def PRINT_ERROR
 * @brief Macro que defini a sequência de characteres para se imprimir\
 *        "Error: " em letras vermelhas
 */
#define ERROR_PRINT COLOR(color::red) << "Error: " << OFF

/**
 * @def ERROR_INDENT
 * @brief Macro para fazer espaços do mesmo tamanho de "Error: "
 */
#define ERROR_SPACE "       "

/**
 * @enum color
 * @brief Enum para os ids das cores no cout
 */
enum color {
    red = 1,     /**< Para vermelho */
    green = 2,   /**< Para verde */
    yellow = 3,  /**< Para amarelo*/
    blue = 4,    /**< Para azul */
    magenta = 5, /**< Para magenta */
    cyan = 6,    /**< Para ciano */
};

#endif
