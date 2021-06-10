const test = require("tape");
const pontu = require("../lib/binding.js");
const utils = require("./utils");

const cloudName = "cloudTeste.pcd";
const cloud = utils.generateCloud(5);

test("Load point cloud", async (t) => {
  t.assert(pontu.cloud_load, "cloud_load function defined");

  utils.saveCloudPcd(cloudName, cloud);

  const loaded = await pontu.cloud_load(`./test/${cloudName}`);
  utils.deleteFile(cloudName);

  t.deepEqual(loaded, cloud, "Loaded correctly");
  t.end();
});

test("Salve point cloud", async (t) => {
  t.assert(pontu.cloud_save, "cloud_save function defined");

  await pontu.cloud_save(cloud, `./test/${cloudName}`);
  const cloudSaved = utils.loadCloudJson(cloudName);
  utils.deleteFile(cloudName);
  t.equal(utils.compareClouds(cloudSaved, cloud), true, "Saved correctly");
  t.end();
});

test("Apply a transformation matrix to a point cloud", async (t) => {
  t.assert(pontu.cloud_transform, "cloud_transform function defined");
  t.end();
});

test("Apply ICP to two point cloud", async (t) => {
  t.assert(pontu.registration_icp, "registration_icp function defined");
  t.end();
});

test("Calculate RMSE between two point clouds", async (t) => {
  t.assert(pontu.cloud_rmse, "cloud_rmse function defined");

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

  const rmse = await pontu.cloud_rmse(cloudA, cloudB, 20, "bf");

  t.equal(
    parseFloat(rmse).toFixed(6),
    parseFloat("0.34641").toFixed(6),
    "RMSE calculated correctly"
  );
  t.end();
});
