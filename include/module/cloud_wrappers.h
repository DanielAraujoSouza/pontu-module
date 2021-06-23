/**
 * @file cloud_wrappers.h
 * @author Daniel Araújo Chaves Souza (daniel_araujo@alu.ufc.br)
 * @brief Wrappers e estruturas para execução síncrona e assíncrona das funções cloud da pontu.
 * @version 0.1
 * @date 2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef CLOUD_WRAPPERS_H
#define CLOUD_WRAPPERS_H

#include <node_api.h>
#include "./converters.h"
#include "../cloud.h"
#include "../matrix.h"

typedef struct
{
  napi_async_work work;
  napi_deferred deferred;
  char *filename;
  struct cloud *cloud;
} CloudSaveData;

/**
 * @brief Desaloca memoria da esturtura CloudSaveData.
 * 
 * @param data Estrutura a ser desalocada.
 */
void FreeCloudSaveData(CloudSaveData **data);

/**
 * @brief Salva, de forma assíncrona, uma nuvem de pontos em um arquivo (csv, pcd, ply ou xyz).
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param info Contem informações adicionais sobre o contexto no qual o retorno de chamada foi invocado.
 * Argumentos esperados em info:
 * arg[0] - Objeto cloud que será salva.
 * arg[1] - Path para o arquivo que sera criado (*.csv, *.pcd, *.ply, *.xyz).
 * @return napi_value Promise que quando resolvida retorna a nuvem salva (Object).
 */
napi_value CloudSavePromise(napi_env env, napi_callback_info info);

/**
 * @brief Salva, de forma síncrona, uma nuvem de pontos em um arquivo (csv, pcd, ply ou xyz).
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param info Contem informações adicionais sobre o contexto no qual o retorno de chamada foi invocado.
 * Argumentos esperados em info:
 * arg[0] - Objeto cloud que será salva.
 * arg[1] - Path para o arquivo que sera criado (*.csv, *.pcd, *.ply, *.xyz).
 * @return napi_value A nuvem salva (Object).
 */
napi_value CloudSaveSync(napi_env env, napi_callback_info info);

typedef struct
{
  napi_async_work work;
  napi_deferred deferred;
  char *filename;
  struct cloud *cloud;
} CloudLoadData;

/**
 * @brief Desaloca memoria da esturtura CloudLoadData.
 * 
 * @param data Estrutura a ser desalocada.
 */
void FreeCloudLoadData(CloudLoadData **data);

/**
 * @brief Carrega, de forma assíncrona, uma nuvem de pontos a partir de um arquivo (csv, obj, pcd, ply ou xyz).
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param info Contem informações adicionais sobre o contexto no qual o retorno de chamada foi invocado.
 * Argumentos esperados em info:
 * arg[0] - Path para o arquivo que sera lido (*.csv, *.obj, *.pcd, *.ply, *.xyz).
 * @return napi_value Promise que quando resolvida retorna a nuvem lida (Object).
 */
napi_value CloudLoadPromise(napi_env env, napi_callback_info info);

/**
 * @brief Carrega, de forma síncrona, uma nuvem de pontos a partir de um arquivo (csv, obj, pcd, ply ou xyz).
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param info Contem informações adicionais sobre o contexto no qual o retorno de chamada foi invocado.
 * Argumentos esperados em info:
 * arg[0] - Path para o arquivo que sera lido (*.csv, *.obj, *.pcd, *.ply, *.xyz).
 * @return napi_value A nuvem lida (Object).
 */
napi_value CloudLoadSync(napi_env env, napi_callback_info info);

typedef struct
{
  napi_async_work work;
  napi_deferred deferred;
  struct cloud *source;
  struct cloud *target;
  real max_dist;
  char *closest;
  real rmse;
} CloudRmseData;

/**
 * @brief Desaloca memoria da esturtura CloudRmseData.
 * 
 * @param data Estrutura a ser desalocada.
 */
void FreeCloudRmseData(CloudRmseData **data);

/**
 * @brief Calcula, de forma assíncrona, o RMSE entre duas nuvens de pontos.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param info Contem informações adicionais sobre o contexto no qual o retorno de chamada foi invocado.
 * Argumentos esperados em info:
 * arg[0] - Objeto cloud source.
 * arg[1] - Objeto cloud target
 * arg[2] - Distância máxima (Double).
 * arg[3] - Tipo de algorimo Closest points utilizado:
 *          "bf" - Nuvem de equivalência com pontos de origem mais próximos no alvo (força bruta).
 *          "tree" - Nuvem de equivalência com pontos de origem mais próximos no destino (usando árvore).
 * @return napi_value Promise que quando resolvida retorna o valor do rmse (Double).
 */
napi_value CloudRmsePromise(napi_env env, napi_callback_info info);

/**
 * @brief Calcula, de forma síncrona, o RMSE entre duas nuvens de pontos.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param info Contem informações adicionais sobre o contexto no qual o retorno de chamada foi invocado.
 * Argumentos esperados em info:
 * arg[0] - Objeto cloud source.
 * arg[1] - Objeto cloud target
 * arg[2] - Distância máxima (Double).
 * arg[3] - Tipo de algorimo Closest points utilizado:
 *          "bf" - Nuvem de equivalência com pontos de origem mais próximos no alvo (força bruta).
 *          "tree" - Nuvem de equivalência com pontos de origem mais próximos no destino (usando árvore).
 * @return napi_value O valor do rmse (Double).
 */
napi_value CloudRmseSync(napi_env env, napi_callback_info info);

typedef struct
{
  napi_async_work work;
  napi_deferred deferred;
  struct cloud *cloud;
  struct matrix *rt;
} CloudTransformData;

/**
 * @brief Desaloca memoria da esturtura CloudTransformData.
 * 
 * @param data Estrutura a ser desalocada.
 */
void FreeCloudTransfoData(CloudTransformData **data);

/**
 * @brief Aplica, de forma assíncrona, uma matriz de transformação(4x4 - rotação e translação) em uma nuvem de pontos
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param info Contem informações adicionais sobre o contexto no qual o retorno de chamada foi invocado.
 * Argumentos esperados em info:
 * arg[0] - Objeto cloud.
 * arg[1] - Array matrix.
 * @return napi_value Promise que quando resolvida retorna a nuvem transformada (Object).
 */
napi_value CloudTransformPromise(napi_env env, napi_callback_info info);

/**
 * @brief Aplica, de forma síncrona, uma matriz de transformação(4x4 - rotação e translação) em uma nuvem de pontos
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param info Contem informações adicionais sobre o contexto no qual o retorno de chamada foi invocado.
 * Argumentos esperados em info:
 * arg[0] - Objeto cloud.
 * arg[1] - Objeto matrix.
 * @return napi_value A nuvem transformada (Object).
 */
napi_value CloudTransformSync(napi_env env, napi_callback_info info);

#endif // CLOUD_WRAPPERS_H