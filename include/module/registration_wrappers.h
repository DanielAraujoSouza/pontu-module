/**
 * @file registration_wrappers.h
 * @author Daniel Araújo Chaves Souza (daniel_araujo@alu.ufc.br)
 * @brief Wrappers e estruturas para execução síncrona e assíncrona das funções de registro da pontu.
 * @version 0.1
 * @date 2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef REGISTRATION_WRAPPERS_H
#define REGISTRATION_WRAPPERS_H

#include <node_api.h>
#include "./converters.h"
#include "../registration.h"

typedef struct
{
  napi_async_work work;
  napi_deferred deferred;
  struct cloud *source;
  struct cloud *target;
  real th;
  uint k;
  real max_dist;
  char *closest;
  struct cloud *aligned;
  struct matrix *tm;
} RegistrationIcpData;

/**
 * @brief Desaloca memoria da esturtura RegistrationIcpData.
 * 
 * @param data Estrutura a ser desalocada.
 */
void FreeRegistrationIcpData(RegistrationIcpData **data);

/**
 * @brief Realiza, de forma assíncrona, o alinhamento de uma nuvem de pontos utilizando o algoritmo de registro ICP.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param info Contem informações adicionais sobre o contexto no qual o retorno de chamada foi invocado.
 * Argumentos esperados em info:
 * arg[0] - Nuvem que será alinhada (Objeto cloud).
 * arg[1] - Nuvem alvo (Objeto cloud).
 * arg[2] - Criterio de parada (Double).
 * arg[3] - Número máximo de iterações (uint).
 * arg[4] - Distância máxima (Double).
 * arg[5] - Tipo de algorimo Closest points utilizado:
 *          "bf" - Nuvem de equivalência com pontos de origem mais próximos no alvo (força bruta).
 *          "tree" - Nuvem de equivalência com pontos de origem mais próximos no destino (usando árvore).
 * @return napi_value Promise que quando resolvida retorna um objeto contendo a nuvem alinhada
 * e a matrix de transformação.
 */
napi_value RegistrationIcpPromise(napi_env env, napi_callback_info info);

/**
 * @brief Realiza, de forma síncrona, o alinhamento de uma nuvem de pontos utilizando o algoritmo de registro ICP.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param info Contem informações adicionais sobre o contexto no qual o retorno de chamada foi invocado.
 * Argumentos esperados em info:
 * arg[0] - Nuvem que será alinhada (Objeto cloud).
 * arg[1] - Nuvem alvo (Objeto cloud).
 * arg[2] - Criterio de parada (Double).
 * arg[3] - Número máximo de iterações (uint).
 * arg[4] - Distância máxima (Double).
 * arg[5] - Tipo de algorimo Closest points utilizado:
 *          "bf" - Nuvem de equivalência com pontos de origem mais próximos no alvo (força bruta).
 *          "tree" - Nuvem de equivalência com pontos de origem mais próximos no destino (usando árvore).
 * @return napi_value Objeto contendo a nuvem alinhada e a matrix de transformação.
 */
napi_value RegistrationIcpSync(napi_env env, napi_callback_info info);

#endif // REGISTRATION_WRAPPERS_H