/**
 * @file common.h
 * @author Daniel Araújo Chaves Souza (daniel_araujo@alu.ufc.br)
 * @brief Funções auxiliares para execução das rotinas do modulo.
 * @version 0.1
 * @date 2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef COMMON_H
#define COMMON_H

#include <node_api.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Função para checar o napi status e lançar exceção caso haja erro.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param status Código de status integral indicando o sucesso ou falha de uma chamada Node-API. 
 * @param msg Mensagem que será apresetada junto a exceção, caso uma seja lançada.
 */
void check_status(napi_env env, napi_status status, char* msg);

/**
 * @brief Verifica se um napi_value corresponde a um determinado napi_valuetype, e lança uma excesão caso
 * contrario.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param value Variável que será checada.
 * @param expected_type Tipo de valor esperado.
 */
void throw_type_error(napi_env env, napi_value value, napi_valuetype expected_type);

/**
 * @brief Simplifica o procedimento de exportação de um método para o módulo.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param exports 
 * @param name Nome do método.
 * @param method Ponteiro para ao metodo que será chamado pelo módulo.
 */
void export_function(napi_env env, napi_value exports, char *name, void *method);

/**
 * @brief Retorna uma cópia da string de entrada convertida em minúsculo.
 * 
 * @param str String de entrada.
 * @return char* String convertida em minúsculo.
 */
char *to_lower(char *str);

/**
 * @brief Retonar a extensão de um arquivo.
 * 
 * @param filename Nome do arquivo.
 * @return char* Extensão do aquivo (char[3]).
 */
char *file_ext(char *filename);

#endif // COMMON_H