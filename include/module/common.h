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
#include <string.h>

/**
 * @brief Função para checar o napi status e lançar exceção caso haja erro.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param status Código de status integral indicando o sucesso ou falha de uma chamada Node-API. 
 * @param msg Mensagem que será apresetada junto a exceção, caso uma seja lançada.
 */
void check_status(napi_env env, napi_status status, const char *msg);

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
void export_function(napi_env env, napi_value exports, const char *name, void *method);

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
                          void *fn_execute,
                          void *fn_complete,
                          const char *promise_name);

#endif // COMMON_H