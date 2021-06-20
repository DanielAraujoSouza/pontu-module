const pontu = require("../lib/binding.js");

// Configurações do ICP
const th = 0.000001; // Critérios de parada (erro)
const k = 10; // Número máximo de iterações
const max_dist = 4000; // Distância máxima entre pontos
const closestType = "bf"; // Closest points call back

///////// SYNC
console.log("Carregando as nuvens...");
const src = pontu.cloud_load_sync("./test/clouds/bun0_RY50.pcd");
const tgt = pontu.cloud_load_sync("./test/clouds/bun0.pcd");

console.log(`Src: ${src.numpts} Pts; Tgt: ${tgt.numpts} Pts;`);

console.log("Executando ICP...");
const icpRes = pontu.registration_icp_sync(
  src,
  tgt,
  th,
  k,
  max_dist,
  closestType
);

console.log("Calculando RMSE...");
const rmse = pontu.cloud_rmse_sync(icpRes.algnCloud, tgt, max_dist, "bf");
console.log(`RMSE: ${rmse}`);

console.log("Transformando SRC...");
const aligned = pontu.cloud_transform_sync(src, icpRes.tm);

console.log("Calculando RMSE...");
const rmseTrans = pontu.cloud_rmse_sync(icpRes.algnCloud, tgt, max_dist, "bf");
console.log(`RMSE Trans: ${rmseTrans}`);
