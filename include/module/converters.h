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
 * @param point Napi Object a ser convertido.
 * @param new_vector3 Ponteiro para a estrutura que armazenará o valor convertido.
 * @return napi_status Status informando a execucação do método. napi_ok se não houver erros e napi_generic_failure caso contrário.
 */

napi_status napi_object_to_pontu_vector3(napi_env env, napi_value point, struct vector3 **new_vector3);

/**
 * @brief Converte um Vector3 em um Napi Object.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param point Vector3 a ser convertido.
 * @param point_obj Ponteiro para o napi_value que armazenará o valor convertido
 * @return napi_status Status informando a execucação do método. napi_ok se não houver erros e napi_generic_failure caso contrário.
 */
napi_status pontu_vector3_to_napi_object(napi_env env, struct vector3 *point, napi_value *point_obj);

/**
 * @brief Converte um Napi Object em uma estrutura Cloud.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param cloud Napi Object a ser convertido.
 * @param new_cloud Ponteiro para a estrutura que armazenará o valor convertido.
 * @return napi_status Status informando a execucação do método. napi_ok se não houver erros e napi_generic_failure caso contrário.
 */
napi_status napi_object_to_pontu_cloud(napi_env env, napi_value cloud, struct cloud **new_cloud);

/**
 * @brief REVER Converte uma estrutura Cloud em um Napi Object.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param cloud Estrutura Cloud a ser convertida.
 * @param cloud_obj Ponteiro para o napi_value que armazenará o valor convertido.
 * @return napi_status Status informando a execucação do método. napi_ok se não houver erros e napi_generic_failure caso contrário.
 */
napi_status pontu_cloud_to_napi_object(napi_env env, struct cloud *cloud, napi_value *cloud_obj);

/**
 * @brief Converte uma estrutura Pointset em um Napi Object.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param point_set Estrutura Pointset a ser convertida.
 * @param points Ponteiro para o napi_value que armazenará o valor convertido.
 * @return napi_status Status informando a execucação do método. napi_ok se não houver erros e napi_generic_failure caso contrário.
 */
napi_status pontu_pointset_to_napi_array(napi_env env, struct pointset *point_set, napi_value *points);

/**
 * @brief Converte um Napi Array em uma estrutura Matrix.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param rt Napi Array a ser convertido
 * @param rt_mat Ponteiro para a estrutura que armazenará o valor convertido.
 * @return napi_status Status informando a execucação do método. napi_ok se não houver erros e napi_generic_failure caso contrário.
 */
napi_status napi_array_to_pontu_matrix(napi_env env, napi_value rt, struct matrix **rt_mat);

/**
 * @brief Converte uma estrutura Matrix em um Napi Object.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param mat Estrutura Matrix a ser convertida.
 * @param m_arr Ponteiro para o napi_value que armazenará o valor convertido.
 * @return napi_status Status informando a execucação do método. napi_ok se não houver erros e napi_generic_failure caso contrário.
 */
napi_status pontu_matrix_to_napi_array(napi_env env, struct matrix *mat, napi_value *m_arr);

/**
 * @brief Converte um Napi Object para um cnum.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param num Objeto que representa um numero complexo {re:double, im:double}
 * @param ncpx Ponteiro para o número complexo que armazenará o valor convertido.
 * @return napi_status Status informando a execucação do método. napi_ok se não houver erros e napi_generic_failure caso contrário.
 */
napi_status napi_object_to_pontu_cnum(napi_env env, napi_value num, cnum *ncpx);

/**
 * @brief Converte um número complexo(cnum) em um Napi Object.
 * 
 * @param env Contexto que a implementação Node-API subjacente pode usar para persistir o estado específico da VM.
 * @param num Número complexo a ser convertido.
 * @param num_obj Ponteiro para o napi_value que armazenará o valor convertido.
 * @return napi_status Status informando a execucação do método. napi_ok se não houver erros e napi_generic_failure caso contrário.
 */
napi_status pontu_cnum_to_napi_object(napi_env env, cnum num, napi_value *num_obj);

#endif // CONVERTERS_H