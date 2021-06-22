/**
 * @file converters.h
 * @author Daniel Araújo Chaves Souza (daniel_araujo@alu.ufc.br)
 * @brief Converte os tipo de dados utilizados pela biblioteca pontu para os valores utilizados
 * pela N-API e vice versa.
 * @version 0.1
 * @date 2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <node_api.h>
#include "./common.h"
#include "../cloud.h"
#include "../vector3.h"

/**
 * @brief Converte um Napi Object em um Vector3.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param point Napi Object a ser convertido.
 * @return struct vector3*
 */
struct vector3 *napi_object_to_pontu_vector3(napi_env env, napi_value point);

/**
 * @brief Converte um Vector3 em um Napi Object.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param point Vector3 a ser convertido.
 * @return napi_value 
 */
napi_value pontu_vector3_to_napi_object(napi_env env, struct vector3 *point);

/**
 * @brief Converte um Napi Object em uma estrutura Cloud.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param cloud Napi Object a ser convertido.
 * @return struct cloud* 
 */
struct cloud *napi_object_to_pontu_cloud(napi_env env, napi_value cloud);

/**
 * @brief Converte uma estrutura Cloud em um Napi Object.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param cloud Estrutura Cloud a ser convertida.
 * @return napi_value 
 */
napi_value pontu_cloud_to_napi_object(napi_env env, struct cloud *cloud);

/**
 * @brief Converte uma estrutura Pointset em um Napi Object.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param point_set Estrutura Pointset a ser convertida.
 * @return napi_value 
 */
napi_value pontu_pointset_to_napi_array(napi_env env, struct pointset *point_set);

/**
 * @brief Converte um Napi Array em uma estrutura Matrix.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param rt Napi Array a ser convertido
 * @return struct matrix* 
 */
struct matrix *napi_array_to_pontu_matrix(napi_env env, napi_value rt);

/**
 * @brief Converte uma estrutura Matrix em um Napi Object.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param mat Estrutura Matrix a ser convertida.
 * @return napi_value 
 */
napi_value pontu_matrix_to_napi_array(napi_env env, struct matrix *mat);

/**
 * @brief Converte um Napi Object para um cnum.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param num Objeto que representa um numero complexo {re:double, im:double}
 * @return cnum 
 */
cnum napi_object_to_pontu_cnum(napi_env env, napi_value num);

/**
 * @brief Converte um número complexo(cnum) em um Napi Object.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param num Número complexo a ser convertido.
 * @return napi_value 
 */
napi_value pontu_cnum_to_napi_object(napi_env env, cnum num);

#endif // CONVERTERS_H