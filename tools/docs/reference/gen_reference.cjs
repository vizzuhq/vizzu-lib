const TypeDoc = require('typedoc')
const path = require('node:path')

const repoPath = path.join(__dirname, '..', '..', '..')
const toolsPath = path.join(repoPath, 'tools')
const mkdocsPath = path.join(toolsPath, 'docs')
const genPath = path.join(mkdocsPath, 'reference')
const srcPath = path.join(repoPath, 'src', 'apps', 'weblib', 'ts-api')

async function reference() {
  const app = await TypeDoc.Application.bootstrapWithPlugins(
    {
      plugin: [/*'typedoc-plugin-markdown', */ 'typedoc-plugin-rename-defaults'],
      entryPoints: [
        path.join(srcPath, '*.ts'),
        path.join(srcPath, 'module', '*.ts'),
        path.join(srcPath, 'types', '*.ts')
      ],
      entryPointStrategy: 'expand',
      tsconfig: path.join(srcPath, 'tsconfig.json'),
      name: 'Vizzu',
      //hideInPageTOC: true,
      disableSources: true,
      excludeNotDocumented: true,
      excludePrivate: true,
      readme: path.join(repoPath, 'docs', 'reference', 'index.md')
    },
    [new TypeDoc.TSConfigReader()]
  )

  const project = await app.convert()

  if (project) {
    const outputDir = path.join(genPath, 'tmp')
    return app.generateDocs(project, outputDir)
  }
}

reference()
