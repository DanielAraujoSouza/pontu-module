const pontu = require("../lib/binding.js");

// Configurações do ICP
const th = 0.000001; // Critérios de parada (erro)
const k = 10; // Número máximo de iterações
const max_dist = 4000; // Distância máxima entre pontos
const closestType = "bf"; // Algorimo de pareamento

console.log("Carregando nuvens...");
const src = pontu.cloud_load_sync("./test/clouds/bun01.pcd"); // Nuvem que será corrigida
const tgt = pontu.cloud_load_sync("./test/clouds/bun0.pcd"); // Nuvem referência

// Executa o ICP
console.log("Nuvens carregadas. Iniciando ICP...");
const icpRes = pontu.registration_icp_sync(
  src,
  tgt,
  th,
  k,
  max_dist,
  closestType
);

// Aplica a matrix de transformação obtida no alinhamento
console.log("ICP Finalizado. Iniciando Alinhamento...");
const aligned = pontu.cloud_transform_sync(src, icpRes.tm);

// Salva a nuvem transformada em um arquivo
console.log("Alinhamento Finalizado. Iniciando Salvamento...");
const salveRes = pontu.cloud_save_sync(aligned, "./test/clouds/bun10.pcd");
console.log(
  `Nuvem de pontos ${salveRes ? "salva corretamente" : "não foi salva"}.`
);

// Calcula o RMSE entre a nuvem transformada e nuvem referência
console.log("Salvamento Finalizado. Calculando RMSE...");
const rmse = pontu.cloud_rmse_sync(aligned, tgt, max_dist, closestType);

// Imprime os resultados obtidos
console.log("Resultados Obtidos");
console.log("- Matriz Transformação: ", icpRes.tm);
console.log("- RMSE: ", rmse);
console.log("- Nuvem Alinhada: ", aligned);
