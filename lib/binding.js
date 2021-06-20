const addon = require("../build/Release/pontuModuleNative");

/**
 * Carrega uma nuvem de pontos a partir de um arquivo (csv, obj, pcd, ply ou xyz).
 * @param {string} filename Path para o arquivo que sera lido (*.csv, *.obj, *.pcd, *.ply, *.xyz).
 * @returns {Promise} Resolve: Objeto contendo a nuvem lida { numpts: int, points: [{x: double, y: double, z: double}] }
 *                    Reject: undefined
 */
exports.cloud_load = addon.cloud_load; /*(filename)*/

/**
 * Calcula o RMSE entre duas nuvens de pontos.
 * @param {object} srcCloud Objeto correspondente a nuvem source.
 * @param {object} tgtCloud Objeto correspondente a nuvem target.
 * @param {number} max_dist Distancia maxima entre pontos.
 * @param {string} closest_type Tipo de algorimo Closest points utilizado:
 *                              "bf" - Nuvem de equivalência com pontos de origem mais próximos no
 *                                     alvo (força bruta).
 *                              "tree" - Nuvem de equivalência com pontos de origem mais próximos no
 *                                       destino (usando árvore).
 * @returns {Promise} Resolve: Valor do RMSE calculado
 *                    Reject: undefined
 */
exports.cloud_rmse =
  addon.cloud_rmse; /*(srcCloud, tgtCloud, max_dist, closest_type)*/

/**
 * Salva a nuvem de pontos em um arquivo (csv, obj, pcd, ply ou xyz).
 * @param {object} cloud Objeto correspondente a nuvem de pontos que será salva
 * @param {string} filename Nome do arquivo onde sera salva (*.csv, *.obj, *.pcd, *.ply, *.xyz).
 *                          Se o arquivo existir será sobrescrito.
 * @returns {Promise} Resolve: Objeto contendo a nuvem salva { numpts: int, points: [{x: double, y: double, z: double}] }
 *                    Reject: undefined
 */
exports.cloud_save = addon.cloud_save; /*(cloud, filename)*/

/**
 * Aplica uma matriz de transformação(4x4 - rotação e translação) em uma nuvem de pontos
 * @param {object} cloud Objeto correspondente a nuvem de pontos que será transformada
 * @param {object} tm Vetor correspondente a matriz de transfomação [[{re: double, im:double}]]
 * @returns {Promise} Resolve: Objeto contendo a nuvem transformada { numpts: int, points: [{x: double, y: double, z: double}] }
 *                    Reject: undefined
 */
exports.cloud_transform = addon.cloud_transform; /*(cloud, tm)*/

/**
 * Realiza o alinhamento de uma nuvem de pontos utilizando o algoritmo de registro ICP.
 * @param {object} srcCloud Objeto correspondente a nuvem source.
 * @param {object} tgtCloud Objeto correspondente a nuvem target.
 * @param {number} th Critério de parada.
 * @param {number} k Número máximo de iteração.
 * @param {number} max_dist Distancia maxima entre pontos.
 * @param {string} closest_type Tipo de algorimo Closest points utilizado:
 *                              "bf" - Nuvem de equivalência com pontos de origem mais próximos no
 *                                     alvo (força bruta).
 *                              "tree" - Nuvem de equivalência com pontos de origem mais próximos no
 *                                       destino (usando árvore).
 * @returns {Promise} Resolve: Um objeto contendo a nuvem source alinhada(algnCloud) e a matriz de
 *                             tranformação { algnCloud: { numpts: int, points: [{x: double, y: double, z: double}] }, tm: [[{re: double, im:double}]] }
 *                    Reject: undefined
 */
exports.registration_icp =
  addon.registration_icp; /*(srcCloud, tgtCloud, th, k, max_dist, closest_type)*/

exports.cloud_save_sync = addon.cloud_save_sync;
exports.cloud_load_sync = addon.cloud_load_sync;
exports.cloud_rmse_sync = addon.cloud_rmse_sync;
exports.cloud_transform_sync = addon.cloud_transform_sync;
exports.registration_icp_sync = addon.registration_icp_sync;
