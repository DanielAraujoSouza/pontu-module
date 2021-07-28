/**
 * @file common.h
 * @author Daniel Araújo Chaves Souza (daniel_araujo@alu.ufc.br)
 * @brief Funções auxiliares para execução das rotinas do módulo.
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
#include <stdio.h>
#include <string.h>

/**
 * @brief Simplifica o procedimento de exportação de um método para o módulo.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param exports 
 * @param name Nome do método.
 * @param method Ponteiro para ao metodo que será chamado pelo módulo.
 */
void export_function(napi_env env, napi_value exports, const char *name, napi_value (*method)(napi_env, napi_callback_info));

/**
 * @brief Converte uma string em minusculo.
 * 
 * @param str String de entrada.
 */
void to_lower(char *str);

/**
 * @brief Retonar a extensão de um arquivo.
 * 
 * @param filename Nome do arquivo.
 * @return const char* Extensão do aquivo (char[3]).
 */
const char *file_ext(const char *filename);

/**
 * @brief Cria uma promessa para processamento assíncrono.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param info Contem informações adicionais sobre o contexto no qual o retorno de chamada foi invocado.
 * @param deferred 
 * @param work É o identificador para o processo assíncrono recém-criado que irá fazer o processamento da promessa.
 * @param addon_data Contexto de dados que será passado de volta para as funções fn_execute e fn_complete.
 * @param fn_execute A função nativa que deve ser chamada para executar a lógica de forma assíncrona.
 * @param fn_complete A função nativa que será chamada quando a lógica assíncrona for concluída ou cancelada.
 * @param promise_name Identificador do tipo de recurso que está sendo fornecido.
 * @return napi_value Identificador para promessa criada.
 */
napi_value create_promise(napi_env env,
                          napi_callback_info info,
                          napi_deferred *deferred,
                          napi_async_work *work,
                          void *addon_data,
                          void (*fn_execute)(napi_env, void *),
                          void (*fn_complete)(napi_env, napi_status, void *),
                          const char *promise_name);

/**
 * @brief Cria um erro que aborta a execução do programa
 * 
 * @param local Local onde o erro ocorreu (método ou arquivo)
 * @param message Messagem de erro
 */
void create_fatal_error(const char *local, const char *message);

/**
 * @brief Insere uma mensagem de erro na estrutura de dados de um método
 * 
 * @param err_info Endereço que armazenará o a mensagem na estrutura de dados
 * @param txt Mensagem que será escrita
 */
void addon_data_write_err(char **err_info, const char *txt);

/**
 * @brief Verifica se uma variavel é de um determinado tipo da N-API
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param value Variavel a ser verificado
 * @param expected_type Tipo de dados da N-API que será comparado
 * @return int 1(um) se value for do tipo esperado e 0(zero) caso contrario
 */
int check_type(napi_env env, napi_value value, napi_valuetype expected_type);

#endif // COMMON_H