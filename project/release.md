# Releasing Vizzu

## Releasing npm package

Note: Build and test the project before release.

Create npm package

```
mkdir -p release/vizzu

cp README.md release/vizzu/README.md
cp CHANGELOG.md release/vizzu/CHANGELOG.md
sed -i  '/\#\# \[Unreleased\]/,/\#\#\ \[/{//!d}' release/vizzu/CHANGELOG.md
sed -i '/\#\# \[Unreleased\]/d' release/vizzu/CHANGELOG.md
cp project/npm/package.json release/vizzu/package.json

mkdir -p release/vizzu/dist

cp example/lib/cvizzu.wasm release/vizzu/dist/cvizzu.wasm
cp example/lib/vizzu.d.ts release/vizzu/dist/vizzu.d.ts
cp example/lib/vizzu.min.js release/vizzu/dist/vizzu.min.js
```

Set version number

```
cd release/vizzu
npm version $(node ../../test/integration/test.js -v --vizzu /release/vizzu/dist/vizzu.min.js|awk -F'-' '{print $1}')
```

Publish npm package

```
npm login
npm publish
```