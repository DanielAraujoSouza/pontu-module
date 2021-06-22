const fs = require("fs");

function randomNumber(min, max) {
  return Math.random() * (max - min) + min;
}

function generateCloud(len, range = 100) {
  cloud = {
    numpts: len,
    points: new Array(len),
  };

  for (let i = 0; i < len; i++) {
    cloud.points[i] = {
      x: randomNumber(-range, range),
      y: randomNumber(-range, range),
      z: randomNumber(-range, range),
    };
  }

  return cloud;
}

function saveCloudPcd(filename, cloud) {
  let buffer = `# .PCD v.5 - Point Cloud Data file format\nVERSION .5\nFIELDS x y z\nSIZE 4 4 4\nTYPE F F F\nCOUNT 1 1 1\nWIDTH ${cloud.numpts}\nHEIGHT 1\nPOINTS ${cloud.numpts}\nDATA ascii\n`;

  cloud.points.forEach((e) => {
    buffer = buffer.concat(`${e.x} ${e.y} ${e.z}\n`);
  });

  fs.writeFileSync(`./test/${filename}`, buffer, "utf-8");
}

function loadCloudJson(filename) {
  const rows = fs.readFileSync(`./test/${filename}`, "utf-8").split("\n");

  const coordRows = rows.filter((e) => {
    const coord = e.split(" ");
    return coord.length === 3 && Number.isFinite(+coord[0]);
  });
  const cloud = {
    numpts: 0,
    points: new Array(coordRows.length),
  };

  for (row of coordRows) {
    const coord = row.split(" ");
    cloud.points[cloud.numpts++] = {
      x: +coord[0],
      y: +coord[1],
      z: +coord[2],
    };
  }
  return cloud;
}

function compareClouds(cloudA, cloudB) {
  if (cloudA.numpts !== cloudB.numpts) {
    return false;
  }
  for (let i = 0; i < cloudA.numpts; i++) {
    if (
      !compareNumber(cloudA.points[i].x, cloudB.points[i].x) ||
      !compareNumber(cloudA.points[i].y, cloudB.points[i].y) ||
      !compareNumber(cloudA.points[i].z, cloudB.points[i].z)
    ) {
      console.log(cloudA.points[i], " != ", cloudB.points[i]);
      return false;
    }
  }
  return true;
}

function compareNumber(x, y) {
  const str1 = x.toString();
  const str2 = y.toString();

  const shortest = str1.length >= str2.length ? str2 : str1;
  const dec = shortest.indexOf(".");
  const numDecimals = shortest.length - dec - 1;

  return (
    parseFloat(str1).toFixed(numDecimals) ===
    parseFloat(str2).toFixed(numDecimals)
  );
}

function deleteFile(filename) {
  fs.unlinkSync(`./test/${filename}`);
}

module.exports = {
  randomNumber,
  generateCloud,
  saveCloudPcd,
  loadCloudJson,
  compareClouds,
  deleteFile,
};
