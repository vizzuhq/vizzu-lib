//export GOOGLE_APPLICATION_CREDENTIALS="~/.gcp/vizzu-ci.json"

exports.getVizzuList = (req, res) => {
    const {Storage} = require('@google-cloud/storage');
    const storage = new Storage();
    const bucketName = 'vizzu-lib-main-sha';
    const bucket = storage.bucket(bucketName);
    bucket.getFiles().then(files => {
        let ans = [];
        files[0].forEach(file => {
            let fileEnd = '/vizzu.js'
            if (file.name.endsWith(fileEnd)) {
                ans.push(file.name.slice(0,-fileEnd.length));
            }
        });
        let message = req.query.message || req.body.message || JSON.stringify(ans);
        res.status(200).send(message);
    })
};
