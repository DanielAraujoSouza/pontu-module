# Módulo Pontu - Node.Js

Esse módulo nativo adapta a biblioteca **Pontu**, para processamento de nuvens de pontos 3D, desenvolvida em **C**, ao ambiente javascript **Node.JS**. Esse módulo é desenvolvido a partir da API de desenvolvimento de complementos nativos do projeto Node.JS, [node-addon-api](https://github.com/nodejs/node-addon-api). Com o módulo Pontu é possivel realiza a manipulação e processamento de nuvens de pontos 3D de forma fácil sem abrir mão do desempenho proporcionado por uma linguagem de programação de baixo nível.

## Índice

- [Dependências](#dependências)
- [Instalação](#instalação)
- [Testes](#testes)
- [Uso](#uso)
- [Pontu API](#pontu-api)
  - [cloud_load](#cloud_load)
  - [cloud_load_sync](#cloud_load_sync)
  - [cloud_rmse](#cloud_rmse)
  - [cloud_rmse_sync](#cloud_rmse_sync)
  - [cloud_save](#cloud_save)
  - [cloud_save_sync](#cloud_save_sync)
  - [cloud_transform](#cloud_transform)
  - [cloud_transform_sync](#cloud_transform_sync)
  - [registration_icp](#registration_icp)
  - [registration_icp_sync](#registration_icp_sync)
- [Exemplos](#exemplos)
  - [Assíncrono](#assíncrono)
  - [Síncrono](#síncrono)
- [Tipos de Dados](#tipos-de-dados)
  - [Cloud OBJ](#cloud-obj)
  - [Matrix4](#matrix4)

## Dependências

- Uma versão do [Node.js](https://nodejs.org/) instalada;
- Módulo global [node-gyp](https://www.npmjs.com/package/node-gyp);
- Módulo [node-addon-api](https://github.com/nodejs/node-addon-api) (instalado automaticamente no passo seguinte);

## Instalação

> **OBS:** Se ainda não possuir nenhum versão do Node.JS [faça o download e instale](https://nodejs.org/en/download/).

Em seguida faça o download desse projeto, acesse o diretório do projeto a partir do terminal e execute o comando abaixo para instalar o módulo **node-gyp**, responsável por compilar o módulo nativo, de forma global.

```bash
npm install -g node-gyp
```

Em seguida, ainda dentro do diretório do projeto, execute o comando abaixo para instalar as demais dependências e compilar o módulo:

```bash
npm install
```

## Testes

Para executar o conjunto de testes, após ter realizado a instalação, execute `npm test`:

```bash
npm test
```

## Uso

Como atualmente o módulo ainda não está disponivel no NPM, a maneira mais facil de intancia-lo em seu projeto é por meio de uma instalação NPM usando diretório local. Para isso, execute o `npm intall <folder>`, onde `<folder>` é o caminho para o diretório onde econtra-se o módulo Pontu. Por exemplo, se o módulo estiver no mesmo nível de diretório que seu projeto, bastaria executar dentro do diretório do seu projeto:

```bash
npm install ../pontu-module
```

Após incluir o módulo Pontu às dependências de seu projeto, basta utilizar a função `require`.

```js
const pontu = require("pontu-module");

const src = pontu.cloud_load_sync("mycloud.pcd");
```

## Pontu API

As funções assíncronas do módulo **Pontu** são baseadas em [Javascript Promises](https://developer.mozilla.org/pt-BR/docs/Web/JavaScript/Reference/Global_Objects/Promise), podendo ser facilmente encadeadas ou executadas em paralelo. Esse módulo também possui funções síncronas para todos os métodos.

### cloud_load

Método para carregar arquivos de nuvem de pontos (**csv**, **obj**, **pcd**, **ply** ou **xyz**) de forma assíncrona.

```js
pontu.cloud_load(filename)
  .then(cloudObj => {
    ...
  })
```

- **[IN] filename** - Path para o arquivo de entrada ([String](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String)).
- **[OUT] cloudObj** - Objeto contendo o número de pontos e um vetor de coordenadas dos pontos ([Cloud OBJ](#cloud-obj)).

### cloud_load_sync

Método para carregar arquivos de nuvem de pontos (**csv**, **obj**, **pcd**, **ply** ou **xyz**) de forma síncrona.

```js
const cloudObj = pontu.cloud_load(filename);
```

- **[IN] filename** - Path para o arquivo de entrada ([String](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String)).
- **[OUT] cloudObj** - Objeto contendo o número de pontos e um vetor de coordenadas dos pontos ([Cloud OBJ](#cloud-obj)).

### cloud_rmse

Método para calcula o RMSE entre duas nuvens de pontos de forma assíncrona.

```js
pontu.cloud_rmse(srcCloud, tgtCloud, max_dist, closest_type)
  .then(rmse => {
    ...
  })
```

- **[IN] srcCloud** - Objeto correspondente a nuvem _source_ ([Cloud OBJ](#cloud-obj)).
- **[IN] tgtCloud** - Objeto correspondente a nuvem _target_ ([Cloud OBJ](#cloud-obj)).
- **[IN] max_dist** - Distancia máxima entre pontos ([Number](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number)).
- **[IN] closest_type** - Tipo de algoritmo utilizado para o pareamento dos pontos ([String](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String)).
  - `"bf"` - Pareia os pontos utilizando operações algébricas. Mais preciso e lento (_brute force_).
  - `"tree"` - Pareia os pontos utilizando operações em árvore. Menos preciso e mais rápido (usando árvore).
- **[OUT] rmse** - Valor de RMSE calculado ([Number](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number))

### cloud_rmse_sync

Método para calcula o RMSE entre duas nuvens de pontos de forma síncrona.

```js
const rmse = pontu.cloud_rmse(srcCloud, tgtCloud, max_dist, closest_type);
```

- **[IN] srcCloud** - Objeto correspondente a nuvem _source_ ([Cloud OBJ](#cloud-obj)).
- **[IN] tgtCloud** - Objeto correspondente a nuvem _target_ ([Cloud OBJ](#cloud-obj)).
- **[IN] max_dist** - Distancia máxima entre pontos ([Number](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number)).
- **[IN] closest_type** - Tipo de algoritmo utilizado para o pareamento dos pontos ([String](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String)).
  - `"bf"` - Pareia os pontos utilizando operações algébricas. Mais preciso e lento (_brute force_).
  - `"tree"` - Pareia os pontos utilizando operações em árvore. Menos preciso e mais rápido (usando árvore).
- **[OUT] rmse** - Valor de RMSE calculado ([Number](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number))

### cloud_save

Método para salvar uma nuvem de pontos em um arquivo (**csv**, **obj**, **pcd**, **ply** ou **xyz**) de forma assíncrona.

```js
pontu.cloud_save(cloudObj, filename)
  .then(salvedCloud => {
    ...
  })
```

- **[IN] cloudObj** - Objeto correspondente a nuvem de pontos que será salva ([Cloud OBJ](#cloud-obj)).
- **[IN] filename** - Nome do arquivo que será criado ([String](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String)). A extensão do arquivo determinará em qual formato a nuvem será salva (Ex.: `"/home/user/myCloud.pcd"`, `"/home/user/myCloud.ply"`).
- **[OUT] salvedCloud** - Objeto correspondente a nuvem salva ([Cloud OBJ](#cloud-obj)).

### cloud_save_sync

Método para salvar uma nuvem de pontos em um arquivo (**csv**, **obj**, **pcd**, **ply** ou **xyz**) de forma síncrona.

```js
const saveResult = pontu.cloud_save_sync(cloudObj, filename);
```

- **[IN] cloudObj** - Objeto correspondente a nuvem de pontos que será salva ([Cloud OBJ](#cloud-obj)).
- **[IN] filename** - Nome do arquivo que será criado ([String](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String)). A extensão do arquivo determinará em qual formato a nuvem será salva (Ex.: `"/home/user/myCloud.pcd"`, `"/home/user/myCloud.ply"`).
- **[OUT] salvedCloud** - Retorna um boleano (Boolean) informando se operação foi realizada (true: nuvem salva; false: nuvem não foi salva).

### cloud_transform

Método para aplica uma matriz de transformação (4x4 - rotação e translação) em uma nuvem de pontos de forma assíncrona.

```js
pontu.cloud_transform(cloudObj, tm)
  .then(transCloud => {
    ...
  })
```

- **[IN] cloudObj** - Objeto correspondente a nuvem de pontos que será transformada ([Cloud OBJ](#cloud-obj)).
- **[IN] tm** - Vetor correspondente a matriz de transfomação ([Matrix4](#matrix4)).
- **[OUT] transCloud** - Objeto contendo a nuvem transformada [Cloud OBJ](#cloud-obj)).

### cloud_transform_sync

Método para aplica uma matriz de transformação (4x4 - rotação e translação) em uma nuvem de pontos de forma síncrona.

```js
const transCloud = pontu.cloud_transform(cloudObj, tm);
```

- **[IN] cloudObj** - Objeto correspondente a nuvem de pontos que será transformada ([Cloud OBJ](#cloud-obj)).
- **[IN] tm** - Vetor correspondente a matriz de transfomação ([Matrix4](#matrix4)).
- **[OUT] transCloud** - Objeto contendo a nuvem transformada ([Cloud OBJ](#cloud-obj)).

### registration_icp

Método para alinhar uma nuvem de pontos (_source_) à outra (_target_) utilizando o algoritmo de registro [ICP](https://en.wikipedia.org/wiki/Iterative_closest_point) de forma assíncrona.

```js
pontu.registration_icp(srcCloud, tgtCloud, th, k, max_dist, closest_type)
  .then(icpRes => {
    ...
  })
```

- **[IN] srcCloud** - Objeto correspondente a nuvem que será alinhada ([Cloud OBJ](#cloud-obj)).
- **[IN] tgtCloud** - Objeto correspondente a nuvem referência para o alinhamento ([Cloud OBJ](#cloud-obj)).
- **[IN] th** - Critério de parada, limiar de erro ([Number](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number)).
- **[IN] k** - Quantidade máxima de iterações ([Number](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number)).
- **[IN] max_dist** - Distância máxima entre pontos ([Number](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number)). Os pares de pontos com distância superior a esse valor serão ignorados para o calculo do ICP.
- **[IN] closest_type** - Tipo de algoritmo utilizado para o pareamento dos pontos ([String](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String)).
  - `"bf"` - Pareia os pontos utilizando operações algébricas. Mais preciso e lento (_brute force_).
  - `"tree"` - Pareia os pontos utilizando operações em árvore. Menos preciso e mais rápido (usando árvore).
- **[OUT] icpRes** - Resultado do obtido no alinhamento (Object).
  - `icpRes.tm` - Matrix de transformação calculada pelo algoritmo ([Matrix4](#matrix4)).
  - `icpRes.algnCloud` - Objeto a nuvem _source_ alinhada ([Cloud OBJ](#cloud-obj)).

### registration_icp_sync

Método para alinhar uma nuvem de pontos (_source_) à outra (_target_) utilizando o algoritmo de registro [ICP](https://en.wikipedia.org/wiki/Iterative_closest_point) de forma assíncrona.

```js
const icpRes = pontu.registration_icp_sync(
  srcCloud,
  tgtCloud,
  th,
  k,
  max_dist,
  closest_type
);
```

- **[IN] srcCloud** - Objeto correspondente a nuvem que será alinhada ([Cloud OBJ](#cloud-obj)).
- **[IN] tgtCloud** - Objeto correspondente a nuvem referência para o alinhamento ([Cloud OBJ](#cloud-obj)).
- **[IN] th** - Critério de parada, limiar de erro ([Number](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number)).
- **[IN] k** - Quantidade máxima de iterações ([Number](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number)).
- **[IN] max_dist** - Distância máxima entre pontos ([Number](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number)). Os pares de pontos com distância superior a esse valor serão ignorados para o calculo do ICP.
- **[IN] closest_type** - Tipo de algoritmo utilizado para o pareamento dos pontos ([String](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String)).
  - `"bf"` - Pareia os pontos utilizando operações algébricas. Mais preciso e lento (_brute force_).
  - `"tree"` - Pareia os pontos utilizando operações em árvore. Menos preciso e mais rápido (usando árvore).
- **[OUT] icpRes** - Resultado do obtido no alinhamento (Object).
  - `icpRes.tm` - Matrix de transformação calculada pelo algoritmo ([Matrix4](#matrix4)).
  - `icpRes.algnCloud` - Objeto a nuvem _source_ alinhada ([Cloud OBJ](#cloud-obj)).

## Exemplos

### Assíncrono

```js
const pontu = require("pontu-module");

// Configurações do ICP
const th = 0.000001; // Critérios de parada (erro)
const k = 10; // Número máximo de iterações
const max_dist = 4000; // Distância máxima entre pontos
const closestType = "bf"; // Algorimo de pareamento

console.log("Carregando nuvens...");
const srcPromise = pontu.cloud_load("./test/clouds/bun01.pcd"); // Nuvem que será corrigida
const tgtPromise = pontu.cloud_load("./test/clouds/bun0.pcd"); // Nuvem referência

// Executa o ICP
const regPromise = Promise.all([srcPromise, tgtPromise])
  .then(([source, target]) => {
    console.log("Nuvens carregadas. Iniciando ICP...");
    return pontu.registration_icp(source, target, th, k, max_dist, closestType);
  })
  .then((icpRes) => {
    console.log("ICP Finalizado");
    return icpRes;
  });

// Aplica a matrix de transformação obtida no alinhamento
const alignPromise = Promise.all([srcPromise, regPromise])
  .then(([src, icpRes]) => {
    console.log("Iniciando Alinhamento...");
    return pontu.cloud_transform(src, icpRes.tm);
  })
  .then((aligned) => {
    console.log("Alinhamento Finalizado");

    return aligned;
  });

// Salva a nuvem transformada em um arquivo
const savePromise = Promise.all([alignPromise])
  .then(([aligned]) => {
    console.log("Salvando nuvem alinhada");
    return pontu.cloud_save(aligned, "./test/clouds/bun10.pcd");
  })
  .then((salvedCloud) => {
    console.log("Nuvem Salva");
    return salvedCloud;
  });

// Calcula o RMSE entre a nuvem transformada e nuvem referência
const rmsePromise = Promise.all([tgtPromise, alignPromise])
  .then(([tgt, aligned]) => {
    console.log("Calculando RMSE...");
    return pontu.cloud_rmse(aligned, tgt, max_dist, closestType);
  })
  .then((rmse) => {
    console.log("RMSE Calculado");
    return rmse;
  });

// Imprime os resultados obtidos
Promise.all([regPromise, rmsePromise, alignPromise]).then(
  ([icpRes, rmse, aligned]) => {
    console.log("Resultados Obtidos");
    console.log("- Matriz Transformação: ", icpRes.tm);
    console.log("- RMSE: ", rmse);
    console.log("- Nuvem Alinhada: ", aligned);
  }
);
```

### Síncrono

```js
const pontu = require("pontu-module");

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
```

## Tipos de Dados

Formatos de dados utilizados pela API.

### Cloud OBJ

- **numpts** - Contém o total de pontos da nuvem;
- **points** - Vetor de objetos que representam os pontos, as propriedades desse objeto respresentam as cordenadas **x**, **y** e **z** do ponto.

Exemplo:

```js
const cloudObj = {
  numpts: 3,
  points: [
    {
      x: 0.044211581349372864,
      y: -0.02174988202750683,
      z: 0.05041627958416939,
    },
    {
      x: 0.05305223912000656,
      y: -0.02392994426190853,
      z: 0.05167124792933464,
    },
    {
      x: 0.05364399775862694,
      y: -0.023770984262228012,
      z: 0.05163086578249931,
    },
  ],
};
```

### Matrix4

Matriz 4x4 de [número complexos](https://mathjs.org/docs/datatypes/complex_numbers.html)

Exemplo:

```js
const matrix = [
  [
    { re: 1, im: 0 },
    { re: 2, im: 0 },
    { re: 3, im: 0 },
    { re: 1, im: 0 },
  ],
  [
    { re: 4, im: 0 },
    { re: 5, im: 0 },
    { re: 6, im: 0 },
    { re: 2, im: 0 },
  ],
  [
    { re: 7, im: 0 },
    { re: 8, im: 0 },
    { re: 9, im: 0 },
    { re: 3, im: 0 },
  ],
  [
    { re: 0, im: 0 },
    { re: 0, im: 0 },
    { re: 0, im: 0 },
    { re: 1, im: 0 },
  ],
];
```
