const yargs = require('yargs');
const path = require('path');
const child_process = require('child_process');
const fs = require('fs');

const padLength = 7;


try {

    var argv = yargs
        .usage('Usage: $0 [options]')
        
        .help('h')
        .alias('h', 'help')
        .version('0.0.1')
        .alias('v', 'version')
        .alias('s', 'size')
        .describe('s', 'heigth')
        .nargs('s', 1)
        .default('s', 320)
        .argv;

        fs.mkdirSync(__dirname + '/resized', { recursive: true });
        let files = fs.readdirSync(__dirname);
        files.forEach(file => {
            if (!fs.lstatSync(__dirname + '/' + file).isDirectory()) {
                if (path.extname(file) == '.webm') {
                    child_process.execSync('ffmpeg -i ' + file + ' -vf scale=' 
                        + argv.size.toString() + ':-1 ' + __dirname + '/resized/' + file);
                    mp4file = file.replace('.webm', '.mp4');
                    child_process.execSync('ffmpeg -i ' + file + ' -vf scale=' 
                        + argv.size.toString() + ':-1 ' + __dirname + '/resized/' + mp4file);
                }
            }
        });
} catch (err) {
    process.exitCode = 1;
    let errMsg = err.toString();
    if (err.stack !== undefined) {
        errMsg = err.stack;
    }
    console.error('[ ' + 'ERROR'.padEnd(padLength, ' ') + ' ] ' + errMsg);
}
