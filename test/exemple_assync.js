const pontu = require('../lib/binding.js')

// Configurações do ICP
const th = 0.000001 // Critérios de parada (erro)
const k = 10 // Número máximo de iterações
const max_dist = 4000 // Distância máxima entre pontos
const closestType = 'bf' // Algorimo de pareamento

console.log('Carregando nuvens...')
const srcPromise = pontu.cloud_load('./test/clouds/bun01.pcd') // Nuvem que será corrigida
const tgtPromise = pontu.cloud_load('./test/clouds/bun0.pcd') // Nuvem referência

// Executa o ICP
const regPromise = Promise.all([srcPromise, tgtPromise])
  .then(([source, target]) => {
    console.log('Nuvens carregadas. Iniciando ICP...')
    return pontu.registration_icp(source, target, th, k, max_dist, closestType)
  })
  .then((icpRes) => {
    console.log('ICP Finalizado')
    return icpRes
  })

// Aplica a matrix de transformação obtida no alinhamento
const alignPromise = Promise.all([srcPromise, regPromise])
  .then(([src, icpRes]) => {
    console.log('Iniciando Alinhamento...')
    return pontu.cloud_transform(src, icpRes.tm)
  })
  .then((aligned) => {
    console.log('Alinhamento Finalizado')

    return aligned
  })

// Salva a nuvem transformada em um arquivo
const savePromise = Promise.all([alignPromise])
  .then(([aligned]) => {
    console.log('Salvando nuvem alinhada')
    return pontu.cloud_save(aligned, './test/clouds/bun10.pcd')
  })
  .then((salvedCloud) => {
    console.log('Nuvem Salva')
    return salvedCloud
  })

// Calcula o RMSE entre a nuvem transformada e nuvem referência
const rmsePromise = Promise.all([tgtPromise, alignPromise])
  .then(([tgt, aligned]) => {
    console.log('Calculando RMSE...')
    return pontu.cloud_rmse(aligned, tgt, max_dist, closestType)
  })
  .then((rmse) => {
    console.log('RMSE Calculado')
    return rmse
  })

// Imprime os resultados obtidos
Promise.all([regPromise, rmsePromise, alignPromise]).then(
  ([icpRes, rmse, aligned]) => {
    console.log('Resultados Obtidos')
    console.log('- Matriz Transformação: ', icpRes.tm)
    console.log('- RMSE: ', rmse)
    console.log('- Nuvem Alinhada: ', aligned)
  }
)
