const pontu = require("../lib/binding.js");

// Configurações do ICP
const th = 0.000001; // Critérios de parada (erro)
const k = 10; // Número máximo de iterações
const max_dist = 4000; // Distância máxima entre pontos
const closestType = "bf"; // Closest points call back

console.log("Carregando nuvens...");
const srcPromise = pontu.cloud_load("./test/clouds/bun01.pcd"); // A que sera corrigida
// .then(cloud => pontu.cloud_save(cloud, "./test/clouds/bun01.csv"))
// .then(cloud => pontu.cloud_save(cloud, "./test/clouds/bun01.ply"))
// .then(cloud => pontu.cloud_save(cloud, "./test/clouds/bun01.xyz"))

const tgtPromise = pontu.cloud_load("./test/clouds/bun0.pcd");
// .then(cloud => pontu.cloud_save(cloud, "./test/clouds/bun02.csv"))
// .then(cloud => pontu.cloud_save(cloud, "./test/clouds/bun02.ply"))
// .then(cloud => pontu.cloud_save(cloud, "./test/clouds/bun02.xyz"))

// const regPromise = Promise.all([srcPromise, tgtPromise])
// .then(([source, target]) => {
//   console.log("Nuvens carregadas. Iniciando ICP...")
//   return pontu.registration_icp(source, target, th, k, max_dist, closestType)
// })
// .then(icpRes => {
//   console.log("ICP Finalizado")
//   return icpRes
// })

// const alignPromise = Promise.all([srcPromise, regPromise])
// .then(([src, icpRes]) => {
//   console.log("Iniciando Alinhamento...")
//   return pontu.cloud_transform(src, icpRes.tm);
// })
// // .then(aligned => {
// //   console.log("Alinhamento Finalizado")

// //   console.log("Salvando nuvem alinhada")
// //   return pontu.cloud_save(aligned, "./test/clouds/bun12.pcd")
// // })
// // .then(salvedCloud => {
// //   console.log("Nuvem Salva")
// //   return salvedCloud;
// // })

// const rmsePromise = Promise.all([tgtPromise, alignPromise])
// .then(([tgt, aligned]) => {
//   console.log("Calculando RMSE...")
//   return pontu.cloud_rmse(aligned, tgt, max_dist, closestType);
// })
// .then(rmse => {
//   console.log("RMSE Calculado")
//   return rmse
// })

// Promise.all([regPromise, rmsePromise, alignPromise])
// .then(([icpRes, rmse, aligned]) => {
//   console.log("Resultados Obtidos")
//   //console.log("- Matriz Transformação: ", icpRes.tm)
//   console.log("- RMSE: ", rmse)
//   //console.log("- Nuvem Alinhada: ", aligned)
// })

// ICP

const regPromise = Promise.all([srcPromise, tgtPromise])
  .then(([source, target]) => {
    console.log("Nuvens carregadas. Iniciando ICP...");
    return pontu.registration_icp(source, target, th, k, max_dist, closestType);
  })
  .then((icpRes) => {
    console.log("Icp Calculado!");
    return icpRes.algnCloud;
  });

// // RMSE;

Promise.all([regPromise, tgtPromise])
  .then(([src, tgt]) => {
    console.log("Calculando RMSE...");
    return pontu.cloud_rmse(src, tgt, max_dist, "bf");
  })
  .then((rmse) => {
    console.log("- RMSE: ", rmse);
  });

// // TRANSFORM
// const tm = [
//   [
//     { re: Math.cos((50 * Math.PI) / 180), im: 0 },
//     { re: 0, im: 0 },
//     { re: Math.sin((50 * Math.PI) / 180), im: 0 },
//     { re: 0, im: 0 },
//   ],
//   [
//     { re: 0, im: 0 },
//     { re: 1, im: 0 },
//     { re: 0, im: 0 },
//     { re: 0, im: 0 },
//   ],
//   [
//     { re: -Math.sin((50 * Math.PI) / 180), im: 0 },
//     { re: 0, im: 0 },
//     { re: Math.cos((50 * Math.PI) / 180), im: 0 },
//     { re: 0, im: 0 },
//   ],
//   [
//     { re: 0, im: 0 },
//     { re: 0, im: 0 },
//     { re: 0, im: 0 },
//     { re: 1, im: 0 },
//   ],
// ];

// srcPromise
//   .then((cloud) => {
//     console.log(cloud);
//     return cloud;
//   })
//   .then((src) => pontu.cloud_transform(src, tm))
//   .then((cloud) => pontu.cloud_save(cloud, "./test/clouds/bun0_d.pcd"))
//   .then((cloud) => {
//     console.log(cloud);
//   });
