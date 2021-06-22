const test = require("tape");
const pontu = require("../lib/binding.js");
const utils = require("./utils");

const cloudName = "cloudTeste.pcd";
const cloud = utils.generateCloud(5);

test("Load point cloud", async (t) => {
  utils.saveCloudPcd(cloudName, cloud);

  t.assert(pontu.cloud_load, "cloud_load function defined");
  const loaded = await pontu.cloud_load(`./test/${cloudName}`);
  t.deepEqual(loaded, cloud, "Correctly loaded asynchronously");

  t.assert(pontu.cloud_load_sync, "cloud_load_sync function defined");
  const loadedSync = pontu.cloud_load_sync(`./test/${cloudName}`);
  t.deepEqual(loadedSync, cloud, "Correctly loaded synchronously");

  utils.deleteFile(cloudName);
  t.end();
});

test("Save point cloud", async (t) => {
  t.assert(pontu.cloud_save, "cloud_save function defined");
  await pontu.cloud_save(cloud, `./test/${cloudName}`);
  const cloudSaved = utils.loadCloudJson(cloudName);
  utils.deleteFile(cloudName);
  t.equal(
    utils.compareClouds(cloudSaved, cloud),
    true,
    "Correctly saved asynchronously"
  );

  t.assert(pontu.cloud_save_sync, "cloud_save_sync function defined");
  pontu.cloud_save_sync(cloud, `./test/${cloudName}`);
  const cloudSavedSync = utils.loadCloudJson(cloudName);
  utils.deleteFile(cloudName);
  t.equal(
    utils.compareClouds(cloudSaved, cloud),
    true,
    "Correctly saved synchronously"
  );

  t.end();
});

test("Apply a transformation matrix to a point cloud", async (t) => {
  const tm = [
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
  const cloud = {
    numpts: 2,
    points: [
      { x: 1, y: 4, z: 8 },
      { x: 2, y: 6, z: 9 },
    ],
  };
  const expected = {
    numpts: 2,
    points: [
      { x: 34, y: 74, z: 114 },
      { x: 42, y: 94, z: 146 },
    ],
  };

  t.assert(pontu.cloud_transform, "cloud_transform function defined");
  const cloudTrans = await pontu.cloud_transform(cloud, tm);
  t.deepEqual(cloudTrans, expected, "Correctly transformed asynchronously");

  t.assert(pontu.cloud_transform_sync, "cloud_transform_sync function defined");
  const cloudTransSync = pontu.cloud_transform_sync(cloud, tm);
  t.deepEqual(cloudTransSync, expected, "Correctly transformed synchronously");
  t.end();
});

test("Apply ICP to two point cloud", async (t) => {
  t.assert(pontu.registration_icp, "registration_icp function defined");
  t.assert(
    pontu.registration_icp_sync,
    "registration_icp_sync function defined"
  );
  t.end();
});

test("Calculate RMSE between two point clouds", async (t) => {
  cloudA = {
    numpts: 2,
    points: [
      { x: 1.1, y: 1.1, z: 1.1 },
      { x: 5.2, y: 5.2, z: 5.2 },
    ],
  };

  cloudB = {
    numpts: 2,
    points: [
      { x: 1.2, y: 1.2, z: 1.2 },
      { x: 5.5, y: 5.5, z: 5.5 },
    ],
  };

  t.assert(pontu.cloud_rmse, "cloud_rmse function defined");
  const rmse = await pontu.cloud_rmse(cloudA, cloudB, 20, "bf");
  t.equal(
    parseFloat(rmse).toFixed(6),
    parseFloat("0.34641").toFixed(6),
    "RMSE correctly calculated asynchronously"
  );

  t.assert(pontu.cloud_rmse_sync, "cloud_rmse_sync function defined");
  const rmse_sync = pontu.cloud_rmse_sync(cloudA, cloudB, 20, "bf");
  t.equal(
    parseFloat(rmse_sync).toFixed(6),
    parseFloat("0.34641").toFixed(6),
    "RMSE correctly calculated synchronously"
  );

  t.end();
});
