const fs = require('fs');

function randomNumber(min, max) { 
  return Math.random() * (max - min) + min;
}

function generateCloud(len, range = 100) { 
  cloud = {
    numpts: len, 
    points: new Array(len)
  }

  for (let i = 0; i < len; i++) {
    cloud.points[i] = {
      x: randomNumber(-range, range),
      y: randomNumber(-range, range),
      z: randomNumber(-range, range),
    }
  }

  return cloud;
}

function saveCloudPcd(filename, cloud) {
  let buffer = `# .PCD v.5 - Point Cloud Data file format\nVERSION .5\nFIELDS x y z\nSIZE 4 4 4\nTYPE F F F\nCOUNT 1 1 1\nWIDTH ${cloud.numpts}\nHEIGHT 1\nPOINTS ${cloud.numpts}\nDATA ascii\n`;

  cloud.points.forEach(e => {
    buffer = buffer.concat(`${e.x} ${e.y} ${e.z}\n`)
  });

  fs.writeFileSync(`./test/${filename}`, buffer, 'utf-8');
}

function deleteFile(filename) {
  fs.unlinkSync(`./test/${filename}`)
}

module.exports = {
  randomNumber,
  generateCloud,
  saveCloudPcd,
  deleteFile
}