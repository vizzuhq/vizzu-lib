import { uglify } from "rollup-plugin-uglify";

const path = require('path');
 
export default {
    input: path.resolve(__dirname, '../../example/lib/vizzu.js'),
    output: {
        file: path.resolve(__dirname, '../../example/lib/vizzu.min.js'),
        format: 'es',
        name: 'bundle'
    },
    plugins: [
        uglify()
    ]
}