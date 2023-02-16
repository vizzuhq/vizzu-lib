const yargs = require('yargs');
const path = require('path');
const fs = require('fs');
const child_process = require('child_process');


try {
    var argv = yargs
        .usage('Usage: $0 [options]')
        
        .help('h')
        .alias('h', 'help')

        .version(false)

        .alias('s', 'size')
        .describe('s', 'Change video\'s height')
        .nargs('s', 1)
        .default('s', 320)
        .argv;

        fs.mkdirSync(__dirname + '/resized', { recursive: true });
        let files = fs.readdirSync(__dirname);
        files.forEach(file => {
            if (!fs.lstatSync(__dirname + '/' + file).isDirectory()) {
                if (path.extname(file) == '.webm') {
                    child_process.execSync(
                    `ffmpeg -i ${file} -vf scale=${argv.size.toString()}:-1 -ss 00:00.01 ${__dirname}/resized/${file}`);
                    mp4file = file.replace('.webm', '.mp4');
                    child_process.execSync(
                    `ffmpeg -i ${file} -vf scale=${argv.size.toString()}:-1 -ss 00:00.01 ${__dirname}/resized/${mp4file}`);
                }
            }
        });
} catch (err) {
    process.exitCode = 1;
    console.error(err);
}
