const yargs = require("yargs");
const path = require("path");
const fs = require("fs");
const child_process = require("child_process");

const getAllFiles = function (dirPath, arrayOfFiles) {
  files = fs.readdirSync(dirPath);

  arrayOfFiles = arrayOfFiles || [];

  files.forEach(function (file) {
    if (fs.statSync(dirPath + "/" + file).isDirectory()) {
      arrayOfFiles = getAllFiles(dirPath + "/" + file, arrayOfFiles);
    } else {
      arrayOfFiles.push(path.join(dirPath, "/", file));
    }
  });

  return arrayOfFiles;
};

try {
  var argv = yargs
    .usage("Usage: $0 [options]")

    .help("h")
    .alias("h", "help")

    .version(false)

    .alias("s", "size")
    .describe("s", "Change video's height")
    .nargs("s", 1)
    .default("s", 320).argv;

  fs.rmSync(__dirname + "/resized", { force: true, recursive: true });
  let files = getAllFiles(__dirname + "/generated");
  files.forEach((file) => {
    if (!fs.lstatSync(file).isDirectory()) {
      if (path.extname(file) == ".webm") {
        outfile = file.replace("generated", "resized");
        fs.mkdirSync(path.dirname(outfile), { recursive: true });
        child_process.execSync(
          `ffmpeg -i ${file} -vf scale=${argv.size.toString()}:-1 -ss 00:00.01 ${outfile}`
        );
        mp4file = outfile.replace(".webm", ".mp4");
        child_process.execSync(
          `ffmpeg -i ${file} -vf scale=${argv.size.toString()}:-1 -ss 00:00.01 ${mp4file}`
        );
      }
    }
  });
} catch (err) {
  process.exitCode = 1;
  console.error(err);
}
