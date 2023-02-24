const fs = require('fs');


const fullVersion = process.argv[2];
const versionParts = fullVersion.split(".");
const version = versionParts[0] + "." + versionParts[1];

content = fs.readFileSync("./release/vizzu/README.md", {encoding:'utf8', flag:'r'});

content = content.replaceAll(
    "https://github.com/vizzuhq/vizzu-lib-doc/raw/main/docs/readme/",
    "https://lib.vizzuhq.com/latest/readme/"
);
content = content.replaceAll(
    "https://cdn.jsdelivr.net/npm/vizzu@latest/",
    `https://cdn.jsdelivr.net/npm/vizzu@${version}/`,
);
content = content.replaceAll(
    "https://lib.vizzuhq.com/latest/",
    `https://lib.vizzuhq.com/${version}/`
);

fs.writeFileSync("./release/vizzu/README.md", content);