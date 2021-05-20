const test = require('tape')
const pontu = require("../lib/binding.js");
const utils = require("./utils");

const cloudName = "cloudTeste.pcd"
const cloud = utils.generateCloud(5)

test('Carregar nuvem de pontos', async (t) => {
  t.assert(pontu.cloud_load, "Função cloud_load definida")

  utils.saveCloudPcd(cloudName, cloud)

  const loaded = await pontu.cloud_load(`./test/${cloudName}`)
  utils.deleteFile(cloudName)

  t.deepEqual(loaded, cloud, "Carregou corretamente")
  t.end()  
})

test('Salvar nuvem de pontos', async (t) => {
  t.assert(pontu.cloud_save, "Função cloud_save definida")

  const saved = await pontu.cloud_save(cloud, `./test/${cloudName}`)
  utils.deleteFile(cloudName)

  t.deepEqual(saved, cloud, "Salvou corretamente")
  t.end()  
})

// test('Calcular RMSE entre duas nuvens de pontos', async (t) => {
//   t.assert(pontu.cloud_rmse, "Função cloud_rmse definida")

//   t.end()  
// })

test('Aplicar uma matriz de transformação a uma nuvem', async (t) => {
  t.assert(pontu.cloud_transform, "Função cloud_transform definida")
  t.end()  
})

test('Aplicar ICP a duas nuvens de pontos', async (t) => {
  t.assert(pontu.registration_icp, "Função registration_icp definida")

  t.end()  
})

test('Calcular RMSE entre duas nuvens de pontos', async (t) => {
  cloudA = {
    numpts: 2, 
    points: [
      {x:1.1, y: 1.1, z: 1.1},
      {x:5.2, y: 5.2, z: 5.2}
    ]
  }

  cloudB = {
    numpts: 2, 
    points: [
      {x:1.2, y: 1.2, z: 1.2},
      {x:5.4, y: 5.4, z: 5.4}
    ]
  }
    
  const rmse = await pontu.cloud_rmse(cloudA, cloudB, 18, "bf")

  console.log(rmse)

  t.deepEqual(cloudA, cloudB, "RMSE calculado corretamente")
  t.end()  
})
